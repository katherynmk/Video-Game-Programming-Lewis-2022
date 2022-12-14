/* Copyright (c) 2007-2008 CSIRO
   Copyright (c) 2007-2009 Xiph.Org Foundation
   Written by Jean-Marc Valin */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "quant_bands.h"
#include "laplace.h"
#include <math.h>
#include "os_support.h"
#include "arch.h"
#include "mathops.h"
#include "stack_alloc.h"

#define E_MEANS_SIZE (5)

const celt_word16 eMeans[E_MEANS_SIZE] = {QCONST16(7.5f,DB_SHIFT), -QCONST16(1.33f,DB_SHIFT), -QCONST16(2.f,DB_SHIFT), -QCONST16(0.42f,DB_SHIFT), QCONST16(0.17f,DB_SHIFT)};

/* FIXME: Implement for stereo */
int intra_decision(celt_word16 *eBands, celt_word16 *oldEBands, int len)
{
   int i;
   celt_word32 dist = 0;
   for (i=0;i<len;i++)
   {
      celt_word16 d = SUB16(eBands[i], oldEBands[i]);
      dist = MAC16_16(dist, d,d);
   }
   return SHR32(dist,2*DB_SHIFT) > 2*len;
}

int *quant_prob_alloc(const CELTMode *m)
{
   int i;
   int *prob;
   prob = celt_alloc(4*m->nbEBands*sizeof(int));
   if (prob==NULL)
     return NULL;
   for (i=0;i<m->nbEBands;i++)
   {
      prob[2*i] = 6000-i*200;
      prob[2*i+1] = ec_laplace_get_start_freq(prob[2*i]);
   }
   for (i=0;i<m->nbEBands;i++)
   {
      prob[2*m->nbEBands+2*i] = 9000-i*240;
      prob[2*m->nbEBands+2*i+1] = ec_laplace_get_start_freq(prob[2*m->nbEBands+2*i]);
   }
   return prob;
}

void quant_prob_free(int *freq)
{
   celt_free(freq);
}

unsigned quant_coarse_energy(const CELTMode *m, int start, const celt_word16 *eBands, celt_word16 *oldEBands, int budget, int intra, int *prob, celt_word16 *error, ec_enc *enc, int _C, celt_word16 max_decay)
{
   int i, c;
   unsigned bits_used = 0;
   celt_word32 prev[2] = {0,0};
   celt_word16 coef = m->ePredCoef;
   celt_word16 beta;
   const int C = CHANNELS(_C);

   if (intra)
   {
      coef = 0;
      prob += 2*m->nbEBands;
   }
   /* The .8 is a heuristic */
   beta = MULT16_16_P15(QCONST16(.8f,15),coef);

   /* Encode at a fixed coarse resolution */
   for (i=start;i<m->nbEBands;i++)
   {
      c=0;
      do {
         int qi;
         celt_word16 q;
         celt_word16 x;
         celt_word32 f;
         celt_word32 mean =  (i-start < E_MEANS_SIZE) ? SUB32(SHL32(EXTEND32(eMeans[i-start]),15), MULT16_16(coef,eMeans[i-start])) : 0;
         x = eBands[i+c*m->nbEBands];
#ifdef FIXED_POINT
         f = SHL32(EXTEND32(x),15)-mean -MULT16_16(coef,oldEBands[i+c*m->nbEBands])-prev[c];
         /* Rounding to nearest integer here is really important! */
         qi = (f+QCONST32(.5,DB_SHIFT+15))>>(DB_SHIFT+15);
#else
         f = x-mean-coef*oldEBands[i+c*m->nbEBands]-prev[c];
         /* Rounding to nearest integer here is really important! */
         qi = (int)floor(.5f+f);
#endif
         if (qi < 0 && x < oldEBands[i+c*m->nbEBands]-max_decay)
         {
            qi += SHR16(oldEBands[i+c*m->nbEBands]-max_decay-x, DB_SHIFT);
            if (qi > 0)
               qi = 0;
         }
         /* If we don't have enough bits to encode all the energy, just assume something safe.
            We allow slightly busting the budget here */
         bits_used=ec_enc_tell(enc, 0);
         if (bits_used > budget)
         {
            qi = -1;
            error[i+c*m->nbEBands] = QCONST16(.5f,DB_SHIFT);
         } else {
            ec_laplace_encode_start(enc, &qi, prob[2*i], prob[2*i+1]);
            error[i+c*m->nbEBands] = PSHR32(f,15) - SHL16(qi,DB_SHIFT);
         }
         q = SHL16(qi,DB_SHIFT);
         
         oldEBands[i+c*m->nbEBands] = PSHR32(MULT16_16(coef,oldEBands[i+c*m->nbEBands]) + mean + prev[c] + SHL32(EXTEND32(q),15), 15);
         prev[c] = mean + prev[c] + SHL32(EXTEND32(q),15) - MULT16_16(beta,q);
      } while (++c < C);
   }
   return bits_used;
}

void quant_fine_energy(const CELTMode *m, int start, celt_ener *eBands, celt_word16 *oldEBands, celt_word16 *error, int *fine_quant, ec_enc *enc, int _C)
{
   int i, c;
   const int C = CHANNELS(_C);

   /* Encode finer resolution */
   for (i=start;i<m->nbEBands;i++)
   {
      celt_int16 frac = 1<<fine_quant[i];
      if (fine_quant[i] <= 0)
         continue;
      c=0;
      do {
         int q2;
         celt_word16 offset;
#ifdef FIXED_POINT
         /* Has to be without rounding */
         q2 = (error[i+c*m->nbEBands]+QCONST16(.5f,DB_SHIFT))>>(DB_SHIFT-fine_quant[i]);
#else
         q2 = (int)floor((error[i+c*m->nbEBands]+.5f)*frac);
#endif
         if (q2 > frac-1)
            q2 = frac-1;
         if (q2<0)
            q2 = 0;
         ec_enc_bits(enc, q2, fine_quant[i]);
#ifdef FIXED_POINT
         offset = SUB16(SHR16(SHL16(q2,DB_SHIFT)+QCONST16(.5,DB_SHIFT),fine_quant[i]),QCONST16(.5f,DB_SHIFT));
#else
         offset = (q2+.5f)*(1<<(14-fine_quant[i]))*(1.f/16384) - .5f;
#endif
         oldEBands[i+c*m->nbEBands] += offset;
         error[i+c*m->nbEBands] -= offset;
         /*printf ("%f ", error[i] - offset);*/
      } while (++c < C);
   }
}

void quant_energy_finalise(const CELTMode *m, int start, celt_ener *eBands, celt_word16 *oldEBands, celt_word16 *error, int *fine_quant, int *fine_priority, int bits_left, ec_enc *enc, int _C)
{
   int i, prio, c;
   const int C = CHANNELS(_C);

   /* Use up the remaining bits */
   for (prio=0;prio<2;prio++)
   {
      for (i=start;i<m->nbEBands && bits_left>=C ;i++)
      {
         if (fine_quant[i] >= 7 || fine_priority[i]!=prio)
            continue;
         c=0;
         do {
            int q2;
            celt_word16 offset;
            q2 = error[i+c*m->nbEBands]<0 ? 0 : 1;
            ec_enc_bits(enc, q2, 1);
#ifdef FIXED_POINT
            offset = SHR16(SHL16(q2,DB_SHIFT)-QCONST16(.5,DB_SHIFT),fine_quant[i]+1);
#else
            offset = (q2-.5f)*(1<<(14-fine_quant[i]-1))*(1.f/16384);
#endif
            oldEBands[i+c*m->nbEBands] += offset;
            bits_left--;
         } while (++c < C);
      }
   }
   c=0;
   do {
      for (i=start;i<m->nbEBands;i++)
      {
         eBands[i+c*m->nbEBands] = log2Amp(oldEBands[i+c*m->nbEBands]);
         if (oldEBands[i+c*m->nbEBands] < -QCONST16(7.f,DB_SHIFT))
            oldEBands[i+c*m->nbEBands] = -QCONST16(7.f,DB_SHIFT);
      }
   } while (++c < C);
}

void unquant_coarse_energy(const CELTMode *m, int start, celt_ener *eBands, celt_word16 *oldEBands, int budget, int intra, int *prob, ec_dec *dec, int _C)
{
   int i, c;
   celt_word32 prev[2] = {0, 0};
   celt_word16 coef = m->ePredCoef;
   celt_word16 beta;
   const int C = CHANNELS(_C);

   if (intra)
   {
      coef = 0;
      prob += 2*m->nbEBands;
   }
   /* The .8 is a heuristic */
   beta = MULT16_16_P15(QCONST16(.8f,15),coef);

   /* Decode at a fixed coarse resolution */
   for (i=start;i<m->nbEBands;i++)
   {
      c=0;
      do {
         int qi;
         celt_word16 q;
         celt_word32 mean =  (i-start < E_MEANS_SIZE) ? SUB32(SHL32(EXTEND32(eMeans[i-start]),15), MULT16_16(coef,eMeans[i-start])) : 0;
         /* If we didn't have enough bits to encode all the energy, just assume something safe.
            We allow slightly busting the budget here */
         if (ec_dec_tell(dec, 0) > budget)
            qi = -1;
         else
            qi = ec_laplace_decode_start(dec, prob[2*i], prob[2*i+1]);
         q = SHL16(qi,DB_SHIFT);

         oldEBands[i+c*m->nbEBands] = PSHR32(MULT16_16(coef,oldEBands[i+c*m->nbEBands]) + mean + prev[c] + SHL32(EXTEND32(q),15), 15);
         prev[c] = mean + prev[c] + SHL32(EXTEND32(q),15) - MULT16_16(beta,q);
      } while (++c < C);
   }
}

void unquant_fine_energy(const CELTMode *m, int start, celt_ener *eBands, celt_word16 *oldEBands, int *fine_quant, ec_dec *dec, int _C)
{
   int i, c;
   const int C = CHANNELS(_C);
   /* Decode finer resolution */
   for (i=start;i<m->nbEBands;i++)
   {
      if (fine_quant[i] <= 0)
         continue;
      c=0; 
      do {
         int q2;
         celt_word16 offset;
         q2 = ec_dec_bits(dec, fine_quant[i]);
#ifdef FIXED_POINT
         offset = SUB16(SHR16(SHL16(q2,DB_SHIFT)+QCONST16(.5,DB_SHIFT),fine_quant[i]),QCONST16(.5f,DB_SHIFT));
#else
         offset = (q2+.5f)*(1<<(14-fine_quant[i]))*(1.f/16384) - .5f;
#endif
         oldEBands[i+c*m->nbEBands] += offset;
      } while (++c < C);
   }
}

void unquant_energy_finalise(const CELTMode *m, int start, celt_ener *eBands, celt_word16 *oldEBands, int *fine_quant,  int *fine_priority, int bits_left, ec_dec *dec, int _C)
{
   int i, prio, c;
   const int C = CHANNELS(_C);

   /* Use up the remaining bits */
   for (prio=0;prio<2;prio++)
   {
      for (i=start;i<m->nbEBands && bits_left>=C ;i++)
      {
         if (fine_quant[i] >= 7 || fine_priority[i]!=prio)
            continue;
         c=0;
         do {
            int q2;
            celt_word16 offset;
            q2 = ec_dec_bits(dec, 1);
#ifdef FIXED_POINT
            offset = SHR16(SHL16(q2,DB_SHIFT)-QCONST16(.5,DB_SHIFT),fine_quant[i]+1);
#else
            offset = (q2-.5f)*(1<<(14-fine_quant[i]-1))*(1.f/16384);
#endif
            oldEBands[i+c*m->nbEBands] += offset;
            bits_left--;
         } while (++c < C);
      }
   }
   c=0;
   do {
      for (i=start;i<m->nbEBands;i++)
      {
         eBands[i+c*m->nbEBands] = log2Amp(oldEBands[i+c*m->nbEBands]);
         if (oldEBands[i+c*m->nbEBands] < -QCONST16(7.f,DB_SHIFT))
            oldEBands[i+c*m->nbEBands] = -QCONST16(7.f,DB_SHIFT);
      }
   } while (++c < C);
}

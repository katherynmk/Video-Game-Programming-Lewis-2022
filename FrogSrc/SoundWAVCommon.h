#ifndef __FROG__SOUNDWAVCOMMON_H__
#define __FROG__SOUNDWAVCOMMON_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/******* Wave format definitions *********/

#if FROG_BIG_ENDIAN
   #define WT_SND_WAVE_RIFF_ID               0x52494646 // "RIFF"
   #define WT_SND_WAVE_ID1                   0x57415645 // "WAVE"
   #define WT_SND_WAVE_ID2                   0x666D7420 // "fmt "
   #define WT_SND_WAVE_FACT_ID               0x66616374 // "fact"
   #define WT_SND_WAVE_LIST_ID               0x4c495354 // "LIST"
   #define WT_SND_WAVE_DATA_ID               0x64617461 // "data"
   #define WT_SND_WAVE_MS_PCM_FORMAT         0x0100     // Uncompressed PCM
   #define WT_SND_WAVE_MS_ADPCM_FORMAT       0x0200     // Compressed PCM (MS-ADPCM)
   #define WT_SND_WAVE_MS_ADPCM_COEFFICIENTS 0x0700     // number of coefficients
#else
   #define WT_SND_WAVE_RIFF_ID               0x46464952 // "RIFF"
   #define WT_SND_WAVE_ID1                   0x45564157 // "WAVE"
   #define WT_SND_WAVE_ID2                   0x20746D66 // "fmt "
   #define WT_SND_WAVE_FACT_ID               0x74636166 // "fact"
   #define WT_SND_WAVE_LIST_ID               0x5453494c // "LIST"
   #define WT_SND_WAVE_DATA_ID               0x61746164 // "data"
   #define WT_SND_WAVE_MS_PCM_FORMAT         0x0001     // Uncompressed PCM
   #define WT_SND_WAVE_MS_ADPCM_FORMAT       0x0002     // Compressed PCM (MS-ADPCM)
   #define WT_SND_WAVE_MS_ADPCM_COEFFICIENTS 0x0007     // number of coefficients
#endif

#define WT_SND_WAVE_MS_ADPCM_NUMBER_OF_COEFFICIENTS 7

#define WT_SND_WAVE_MS_ADPCM_NUMBER_OF_COEFFICIENTS 7

#if __PRAGMA_PACK__
#pragma pack (push, 1)
#endif

typedef struct __GCC_PACKED__
        {
        uint32 RIFF_id;
        uint32 RIFF_len;
        uint32 WAVE_id1;
        uint32 WAVE_id2;
        uint32 data_ptr;
        } WT_SND_WAVE_RIFF;

typedef struct __GCC_PACKED__
        {
        int16  FormatTag;
        int16  Channels;
        uint32 SamplesPerSec;
        uint32 AvgBytesPerSec;
        int16  BlockAlign;
        } WT_SND_WAVE_COMMON;

typedef struct __GCC_PACKED__
        {
        int16 BitsPerSample;
        } WT_SND_WAVE_SPECIFIC;


typedef struct __GCC_PACKED__
        {
        uint16 adpcm_info_length;
        uint16 uncompressed_samples_per_block;
        uint16 number_of_coefficients;
        int16 coefficient [WT_SND_WAVE_MS_ADPCM_NUMBER_OF_COEFFICIENTS][2];
        } WT_SND_WAVE_ADPCM;

typedef struct __GCC_PACKED__
        {
        uint32 FACT_id;
        uint32 fact_length;
        uint32 NumberOfSample;
        } WT_SND_WAVE_FACT_CHUNK;

typedef struct __GCC_PACKED__
        {
        uint32 DATA_id;
        uint32 data_length;
        } WT_SND_WAVE_DATA_CHUNK;

#if __PRAGMA_PACK__
#pragma pack (pop)
#endif

typedef struct
        {
        int16 *buffer_pcm;
        uchar *buffer_adpcm;
        uint32 buffer_pcm_length;
        uint32 buffer_adpcm_length;
        uint32 real_pcm_length;
        WT_SND_WAVE_COMMON *wave_common;
        WT_SND_WAVE_ADPCM  *wave_adpcm;
        } WT_SND_WAVE_MS_ADPCM_CONTEXT;

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDWAVCOMMON_H__

#ifndef __FROG__PROGRESSION_H__
#define __FROG__PROGRESSION_H__

#include "FrogMemory.h"
#include "FrogMath.h"

namespace Webfoot {

//==============================================================================

/// Transform the value so that it will change according to a certain pattern
/// as the input value moves between 0 and 1.
class Progression
{
public:
   virtual ~Progression() {}

   /// Transform the value so that it will change according to a certain pattern
   /// as the input value moves between 0 and 1.  The returned value will be between 0 and 1.
   virtual float ValueGet(float t) = 0;
};

//==============================================================================

/// Simple transition in which progress is completely even.
class ProgressionLinear : public Progression
{
public:
   virtual ~ProgressionLinear() {}
   
   virtual float ValueGet(float t)
   {
      if(t < 0.0f)
         return 0.0f;
      else if(t > 1.0f)
         return 1.0f;
      else
         return t;
   }
   
   static ProgressionLinear instance;
};

ProgressionLinear* const theProgressionLinear = &ProgressionLinear::instance; 

//==============================================================================

/// Smoothly go from stopped to moving to stopped.
class ProgressionSmooth : public Progression
{
public:
   virtual ~ProgressionSmooth() {}
   
   virtual float ValueGet(float t)
   {
      if(t < 0.0f)
         t = 0.0f;
      if(t > 1.0f)
         t = 1.0f;
         
      // Use the second half of the cosine curve to smoothly start and stop.
      return 0.5f + (0.5f * FrogMath::Cos((t * 180.0f) + 180.0f));  
   }
   
   static ProgressionSmooth instance;
};

ProgressionSmooth* const theProgressionSmooth = &ProgressionSmooth::instance;

//==============================================================================

/// Smoothly go from stopped to moving
class ProgressionSmoothStart : public Progression
{
public:
   virtual ~ProgressionSmoothStart() {}
   
   virtual float ValueGet(float t)
   {
      if(t < 0.0f)
         t = 0.0f;
      if(t > 1.0f)
         t = 1.0f;
         
      // Use the third quarter of the cosine curve to smoothly start but not smoothly stop.
      return 1.0f + FrogMath::Cos((t * 90.0f) + 180.0f);  
   }
   
   static ProgressionSmoothStart instance;
};

ProgressionSmoothStart* const theProgressionSmoothStart = &ProgressionSmoothStart::instance;

//==============================================================================

/// Smoothly go from moving to stopped
class ProgressionSmoothStop : public Progression
{
public:
   virtual ~ProgressionSmoothStop() {}
   
   virtual float ValueGet(float t)
   {
      if(t < 0.0f)
         t = 0.0f;
      if(t > 1.0f)
         t = 1.0f;
         
      // Use the fourth quarter of the cosine curve to smoothly stop but not smoothly start.
      return FrogMath::Cos((t * 90.0f) + 270.0f);  
   }
   
   static ProgressionSmoothStop instance;
};

ProgressionSmoothStop* const theProgressionSmoothStop = &ProgressionSmoothStop::instance;

//==============================================================================

/// Quickly overshoot the end value, elasticly settles to the requested end value
class ProgressionElasticStop : public Progression
{
public:
   virtual ~ProgressionElasticStop() {}
   
   virtual float ValueGet(float t)
   {
      if(t < 0.0f)
         t = 0.0f;
      if(t > 1.0f)
         t = 1.0f;

      float tSquared = t * t;
      float tCubed = tSquared * t;

      return (33.0f*tCubed*tSquared + -106.0f*tSquared*tSquared + 126.0f*tCubed + -67.0f*tSquared + 15.0f*t);
   }
   
   static ProgressionElasticStop instance;
};

ProgressionElasticStop* const theProgressionElasticStop = &ProgressionElasticStop::instance;

//==============================================================================
/// Quickly overshoot the end value, settles to the requested end value with a 
/// few bounces

class ProgressionBounceStop : public Progression
{
public:
   virtual ~ProgressionBounceStop() {}
   
   virtual float ValueGet(float t)
   {
      if(t < 0.0f)
         t = 0.0f;
      if(t > 1.0f)
         t = 1.0f;

      if (t < (1.0f/2.75f))
         return (7.5625f*t*t);
      else if (t < (2.0f / 2.75f))
      {
         t -= (1.5f / 2.75f);
         return (7.5625f * t * t + 0.75f);
      }
      else if (t < (2.5f / 2.75f))
      {
         t -= (2.25f / 2.75f);
         return (7.5625f * t * t + 0.9375f);
      }
      else
      {
         t -= (2.625f / 2.75f);
         return (7.5625f * t * t + 0.984375f);
      }
   }
   
   static ProgressionBounceStop instance;
};

ProgressionBounceStop* const theProgressionBounceStop = &ProgressionBounceStop::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PROGRESSION_H__

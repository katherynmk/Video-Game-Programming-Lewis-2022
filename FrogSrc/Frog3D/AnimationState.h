#ifndef __FROG__FROG3D__ANIMATIONSTATE_H__
#define __FROG__FROG3D__ANIMATIONSTATE_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// AnimationState specifies and manages how an animation should be sampled and
/// applied over time. 
class AnimationState
{
public:
   AnimationState();
   
   /// Increment time by the given number of seconds.
   void Update(float dtSeconds);
   
   /// Set the time at which sampling should be performed for the associated
   /// animation.
   void TimeSet(float _time);
   /// Return the time at which sampling should be performed for the associated
   /// animation.
   float TimeGet() const { return time; }
      
   /// Set the influence of the associated animation.
   void WeightSet(float _weight) { weight = _weight; };
   /// Return the influence of the associated animation.
   float WeightGet() const { return weight; }
   
   /// Set the coefficient for the passage of time.
   void TimeScaleSet(float _timeScale) { timeScale = _timeScale; }
   /// Return the coefficient for the passage of time.
   float TimeScaleGet() const { return timeScale; }
   
   /// Set the duration of the animation in seconds.
   void DurationSet(float _duration) { duration = _duration; }
   /// Return the duration of the animation in seconds
   float DurationGet() const { return duration; }
   
   /// Set whether the animation should loop or stop at the end.
   void LoopSet(bool _loop) { loop = _loop; }
   /// Return whether the animation should loop or stop at the end.
   bool LoopCheck() const { return loop; }
   
   /// Return true if the animation is complete.
   bool FinishedCheck() const { return (TimeGet() >= DurationGet()) && !LoopCheck(); }

   /// Pause the progress of time.  This uses a counter internally.
   void Pause() { pauseCounter++; }
   /// Unpause the progress of time.  This uses a counter internally.
   void Unpause() { pauseCounter--; }
   /// Return true if the animation state is currently paused.
   bool PausedCheck() const { return pauseCounter > 0; }
   
protected:
   /// Time at which sampling should be performed for the associated animation.
   float time;
   /// Influence of the associated animation.
   float weight;
   /// Coefficient for the passage of time.    
   float timeScale;
   /// Duration of the animation in seconds.
   float duration;
   /// Whether the animation should loop or stop at the end.
   bool loop;
   /// Used to track whether this should be paused.
   int pauseCounter;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FROG3D__ANIMATIONSTATE_H__

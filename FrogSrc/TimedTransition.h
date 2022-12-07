#ifndef __FROG__TIMEDTRANSITION_H__
#define __FROG__TIMEDTRANSITION_H__

#include "FrogMemory.h"
#include "Progression.h"
#include "Point2.h"
#include "Color.h"
#include "SplineLinear.h"
#include "SplineHermiteCubic.h"

namespace Webfoot {

class JSONValue;

/// Key for the number of milliseconds before interpolation begins.
#define TIMED_TRANSITION_START_DELAY_KEY "StartDelay"

//==============================================================================

/// TimedTransition encapsulates the details of a transition that follows
/// a given progression over a specific time.
class TimedTransition
{
public:
   TimedTransition()
   {
      Reset();
   }
   
   /// Initialize the transition to start '_startTime' milliseconds from now,
   /// end 'duration' milliseconds later, and follow the given progression.
   void Init(unsigned int _startDelay, unsigned int duration, Progression* _progression = theProgressionLinear)
   {
      Reset();
      started = true;
      startTime = _startDelay;
      endTime = startTime + duration;
      progression = _progression;

      if(time >= endTime)
         finished = true;
   }
   
   /// Reset the transition to inactive, so that ProgressGet() acts as though
   /// the transition hasn't started yet.
   void Reset()
   {
      time = 0;
      started = false;
      finished = false;
      startTime = 0;
      endTime = 0;
      progression = theProgressionLinear;
   }

   /// Start the transition again using the same settings as before.
   void Restart()
   {
      time = 0;
      started = true;
      finished = false;
   }
   
   /// Move time forward by the given number of milliseconds.
   void Update(unsigned int frameDuration)
   {
      if(started && !finished)
      {
         time += frameDuration;

         if(time >= endTime)
            finished = true;
      }
   }
   
   /// Based on the current time, return the current position in the transition.
   /// The returned value will be in the range 0-1. 
   float ProgressGet()
   {
      if(!started || (time < startTime))
         return progression->ValueGet(0.0f);
         
      if(finished || !(endTime - startTime))
         return progression->ValueGet(1.0f);
      else
         return progression->ValueGet((float)(time - startTime) / (float)(endTime - startTime)); 
   }

   /// Return the duration of the transition.
   unsigned int DurationGet() { return endTime - startTime; }

   /// Return true if Init or Restart have been called to start the transition.
   bool StartedCheck() { return started; }

   /// Check if the transition is in progress
   bool TransitioningCheck() { return (!FinishedCheck() && (time >= startTime) && StartedCheck()); }
   
   /// Force the transition to be completed.
   void FinishedSet() { finished = true; time = endTime; }
   /// Return true if the transition is finished.
   bool FinishedCheck() { return finished; }

   /// Set the delay before the transition begins.
   void StartDelaySet(unsigned int _startDelay)
   {
      int change = (int)_startDelay - (int)startTime;
      startTime = _startDelay;
      endTime = (unsigned int)((int)endTime + change);
   }
   
protected:
   /// True if the transition is active.
   bool started;
   /// True if the transition should always be treated as completed.
   bool finished;
   /// Delay in milliseconds before the transition begins.
   unsigned int startTime;
   /// Time in milliseconds at which the transition ends.
   unsigned int endTime;
   /// Number of milliseconds since the 
   unsigned int time;
   /// Pointer to a Transition object used to shape the transition over its active time.
   Progression* progression;
};

//==============================================================================

/// TimedTransitionLinear is a TimedTransition that uses linear interpolation
/// to blend between two values.
template <typename T>
class TimedTransitionLinear : public TimedTransition
{
public:
   /// Initialize the transition to start '_startTime' milliseconds from now,
   /// end 'duration' milliseconds later, and follow the given progression.
   void Init(unsigned int _startDelay, unsigned int duration, const T& _startValue, const T& _endValue, Progression* _progression = theProgressionLinear)
   {
      Inherited::Init(_startDelay, duration, _progression);
      startValue = _startValue;
      endValue = _endValue;
   }

   /// Based on the current time and progress through the transition, return
   /// the value for the current point in the transition using linear interpolation. 
   T ValueGet()
   {
      float progress = ProgressGet();
      return Lerp(startValue, endValue, progress, false);
   }

   /// Change the start value.  Beware that this can cause discontinuities if
   /// called during the transition.
   void StartValueSet(const T& _startValue) { startValue = _startValue; }
   /// Return the current start value.
   T StartValueGet() { return startValue; }

   /// Change the end value.  Beware that this can cause discontinuities if
   /// called during the transition.
   void EndValueSet(const T& _endValue) { endValue = _endValue; }
   /// Return the current end value.
   T EndValueGet() { return endValue; }

   typedef TimedTransition Inherited;

protected:
   T startValue;
   T endValue;
};

//==============================================================================

/// TimedTransitionLinear2D facilitates linearly-interpolated transitions
/// in 2D.  Its parameters are gathered from JSONValues to avoid clutter in the
/// code and give control to artists and scripters.  It also allows you to
/// easily apply the resulting transform.  Progressions specified by name in
/// JSON are retrieved from the ProgressionManager.
class TimedTransitionLinear2D
{
public:
   void Init(JSONValue* specifications);
   
   /// Call with the number of milliseconds since the last update.
   void Update(unsigned int dt);
   
   /// Reset the transition to inactive, so that ProgressGet() acts as though
   /// the transition hasn't started yet.
   void Reset();
   /// Start the transition again using the same settings as before.
   void Restart();
   /// Force the transition to be completed.
   void FinishedSet();
   /// Return true if the transition is finished.
   bool FinishedCheck();
   
#ifndef FROGCLI
   /// This can sometimes be used as an alternative to manually passing the
   /// scale, rotation, and position from this object to the drawing commands.
   /// Push a matrix onto the Screen's matrix stack based on the current scale,
   /// rotation, and position of this transition, applied in that order.  Once
   /// this is called, just draw whatever is necessary with the origin for 
   /// rotation and scaling at (0,0).  When the draw calls have been made, pop
   /// the Screen's matrix stack once.  Color must be applied separately.
   void TransformPush();
#endif //#ifndef FROGCLI
   
   /// Return the current position.
   Point2F PositionGet() { return positionTransition.ValueGet(); }
   /// Return the current scale.
   Point2F ScaleGet() { return scaleTransition.ValueGet(); }
   /// Return the current rotation.
   float RotationGet() { return rotationTransition.ValueGet(); }
   /// Return the current color.
   ColorRGBA8 ColorGet() { return colorTransition.ValueGet(); }

   /// Set the delay before the transition begins.
   void StartDelaySet(unsigned int _startDelay)
   {
      positionTransition.StartDelaySet(_startDelay);
      scaleTransition.StartDelaySet(_startDelay);
      rotationTransition.StartDelaySet(_startDelay);
      colorTransition.StartDelaySet(_startDelay);
   }

   /// Manages the translation aspect of the transition.
   TimedTransitionLinear<Point2F> positionTransition;
   /// Manages the scale aspect of the transition.
   TimedTransitionLinear<Point2F> scaleTransition;
   /// Manages the rotation aspect of the transition.
   TimedTransitionLinear<float> rotationTransition;
   /// Manages the color aspect of the transition.
   TimedTransitionLinear<ColorRGBA8> colorTransition;
};

//==============================================================================

/// TimedTransitionBezierQuadratic is a TimedTransition that uses quadratic bezier
/// interpolation to blend between three values.            
template <typename T>
class TimedTransitionBezierQuadratic : public TimedTransition
{
public:
   /// Initialize the transition to start '_startTime' milliseconds from now,
   /// end 'duration' milliseconds later, and follow the given progression.
   void Init(unsigned int _startDelay, unsigned int duration, const T& _startValue, const T& _middleValue, const T& _endValue, Progression* _progression = theProgressionLinear)
   {
      Inherited::Init(_startDelay, duration, _progression);
      startValue = _startValue;
      middleValue = _middleValue;
      endValue = _endValue;
   }
   
   /// Based on the current time and progress through the transition, return
   /// the value for the current point in the transition using linear interpolation. 
   T ValueGet()
   {
      float progress = ProgressGet();
      return BezierQuadraticInterpolate(startValue, middleValue, endValue, progress);
   }
   
   typedef TimedTransition Inherited;
   
protected:
   T startValue;
   T middleValue;
   T endValue;
};

//==============================================================================

/// TimedTransitionSplineLinear is a TimedTransition that uses a
/// SplineLinear to control the value over time.
template <typename T>
class TimedTransitionSplineLinear : public TimedTransition
{
public:
   TimedTransitionSplineLinear()
   {
      spline = NULL;
   }

   /// Initialize the transition to begin the spline '_startTime' milliseconds
   /// from now and follow the given progression.
   void Init(unsigned int _startDelay, SplineLinear<T>* _spline, Progression* _progression = theProgressionLinear)
   {
      Inherited::Init(_startDelay, _spline->DurationGet(), _progression);
      spline = _spline;
   }
   
   /// Based on the current time and progress through the transition, return
   /// the value for the current point in the transition using linear interpolation. 
   T ValueGet()
   {
      float progress = ProgressGet();
      if(spline)
         return spline->ValueGet((unsigned int)(progress * (float)spline->DurationGet()));
      else
         return defaultValue;
   }
   
   typedef TimedTransition Inherited;
   
protected:
   T defaultValue;
   SplineLinear<T>* spline;
};

//==============================================================================

/// TimedTransitionSplineHermiteCubic is a TimedTransition that uses a
/// SplineHermiteCubic to control the value over time.
template <typename T>
class TimedTransitionSplineHermiteCubic : public TimedTransition
{
public:
   TimedTransitionSplineHermiteCubic()
   {
      spline = NULL;
   }

   /// Initialize the transition to begin the spline '_startTime' milliseconds
   /// from now and follow the given progression.
   void Init(unsigned int _startDelay, SplineHermiteCubic<T>* _spline, Progression* _progression = theProgressionLinear)
   {
      Inherited::Init(_startDelay, _spline->DurationGet(), _progression);
      spline = _spline;
   }
   
   /// Based on the current time and progress through the transition, return
   /// the value for the current point in the transition using linear interpolation. 
   T ValueGet()
   {
      float progress = ProgressGet();
      if(spline)
         return spline->ValueGet((unsigned int)(progress * (float)spline->DurationGet()));
      else
         return defaultValue;
   }
   
   typedef TimedTransition Inherited;
   
protected:
   T defaultValue;
   SplineHermiteCubic<T>* spline;
};

//==============================================================================

/// TimedTransitionLinearSpline2D facilitates linearly-interpolated transitions
/// in 2D that may use more than a beginning and ending value.  Its parameters
/// are gathered from JSONValues to avoid clutter in the code and give control
/// to artists and scripters.  It also allows you to easily apply the resulting
/// transform.  Progressions specified by name in JSON are retrieved from the
/// ProgressionManager.
class TimedTransitionSplineLinear2D
{
public:
   void Init(JSONValue* specifications);
   void Deinit();
   
   /// Call with the number of milliseconds since the last update.
   void Update(unsigned int dt);
   
   /// Reset the transition to inactive, so that ProgressGet() acts as though
   /// the transition hasn't started yet.
   void Reset();
   /// Start the transition again using the same settings as before.
   void Restart();
   /// Force the transition to be completed.
   void FinishedSet();
   /// Return true if the transition is finished.
   bool FinishedCheck();
   
#ifndef FROGCLI
   /// This can sometimes be used as an alternative to manually passing the
   /// scale, rotation, and position from this object to the drawing commands.
   /// Push a matrix onto the Screen's matrix stack based on the current scale,
   /// rotation, and position of this transition, applied in that order.  Once
   /// this is called, just draw whatever is necessary with the origin for 
   /// rotation and scaling at (0,0).  When the draw calls have been made, pop
   /// the Screen's matrix stack once.  Color must be applied separately.
   void TransformPush();
#endif //#ifndef FROGCLI
   
   /// Return the current position.
   Point2F PositionGet() { return positionTransition.ValueGet(); }
   /// Return the current scale.
   Point2F ScaleGet() { return scaleTransition.ValueGet(); }
   /// Return the current rotation.
   float RotationGet() { return rotationTransition.ValueGet(); }
   /// Return the current color.
   ColorRGBA8 ColorGet() { return colorTransition.ValueGet(); }

   /// Set the delay before the transition begins.
   void StartDelaySet(unsigned int _startDelay)
   {
      positionTransition.StartDelaySet(_startDelay);
      scaleTransition.StartDelaySet(_startDelay);
      rotationTransition.StartDelaySet(_startDelay);
      colorTransition.StartDelaySet(_startDelay);
   }
   
   /// Manages the translation aspect of the transition.
   TimedTransitionSplineLinear<Point2F> positionTransition;
   /// Underlying spline for position.
   SplineLinear<Point2F> positionSpline;
   /// Manages the scale aspect of the transition.
   TimedTransitionSplineLinear<Point2F> scaleTransition;
   /// Underlying spline for scale.
   SplineLinear<Point2F> scaleSpline;
   /// Manages the rotation aspect of the transition.
   TimedTransitionSplineLinear<float> rotationTransition;
   /// Underlying spline for rotation.
   SplineLinear<float> rotationSpline;
   /// Manages the color aspect of the transition.
   TimedTransitionSplineLinear<ColorRGBA8> colorTransition;
   /// Underlying spline for color.
   SplineLinear<ColorRGBA8> colorSpline;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TIMEDTRANSITION_H__

#ifndef __FROG__GUI__WIDGETTRANSITION_H__
#define __FROG__GUI__WIDGETTRANSITION_H__

#include "FrogMemory.h"
#include "TimedTransition.h"

namespace Webfoot {

class Widget;
class JSONValue;

//==============================================================================

/// An effect that affects a widget over a period of time.
class WidgetTransition
{
public:
   WidgetTransition() { widget = NULL; }
   virtual ~WidgetTransition() {}
   
   void Init(Widget* _widget, bool _remainAfterFinished)
   {
      widget = _widget;
      remainAfterFinished = _remainAfterFinished;
   }
   virtual void Deinit() { widget = NULL; }
   
   /// Begin the transition.
   virtual void Begin() = 0;
   /// Called on every frame to update the transition.
   virtual void Update(unsigned int dt) = 0;
   /// Return true if the transition is complete.
   virtual bool FinishedCheck() = 0;
   /// Skip to the end of the transition.
   virtual void Finish() = 0;

   /// Return any temporary offset to the position caused by the transition.
   virtual Point2F PositionOffsetGet() { return Point2F::Create(0.0f, 0.0f); }
   /// Return any temporary additional rotation caused by the transition.
   virtual float RotationGet() { return 0; }
   /// Return any temporary further scaling caused by the transition.
   virtual Point2F ScaleGet() { return Point2F::Create(1.0f, 1.0f); }
   /// Return any temporary further color modulation caused by the transition.
   virtual ColorRGBA8 ColorGet() { return COLOR_RGBA8_WHITE; }

   /// Return true if the transition should remain in-effect even after it is finished.
   virtual bool RemainAfterFinishedCheck() { return remainAfterFinished; }

   /// Called with the base clip area for the widgets is changed.
   virtual void OnClipAreaBaseChange() {}

   /// Set the start delay for this transition.  This may not be supported by
   /// all implementations, and it may not work as expected if the transition
   /// is already in progress.
   virtual void StartDelaySet(unsigned int _startDelay) { (void)_startDelay; }
   
protected:
   /// The widget to be manipulated by this transition.
   Widget* widget;
   /// True if the transition should remain in-effect even after it is finished.
   bool remainAfterFinished;
};

//==============================================================================

/// A WidgetTransition that moves a widget in x and y.
class WidgetTransitionTranslate : public WidgetTransition
{
public:
   typedef WidgetTransition Inherited;
   
   /// Ways in which the widget can be moved.
   enum Movement
   {
      /// Move from offscreen left to current position.
      FROM_LEFT,
      /// Move from offscreen right to current position.
      FROM_RIGHT,
      /// Move from offscreen above to current position.
      FROM_TOP,
      /// Move from offscreen below to current position.
      FROM_BOTTOM,
      /// Move from the current position to offscreen left.
      TO_LEFT,
      /// Move from the current position to offscreen right.
      TO_RIGHT,
      /// Move from the current position to offscreen above.
      TO_TOP,
      /// Move from the current position to offscreen below.
      TO_BOTTOM,
      MOVEMENT_COUNT
   };
   
   WidgetTransitionTranslate() { progression = NULL; }
   virtual ~WidgetTransitionTranslate() {}
   void Init(Widget* _widget, Movement _movement, int _startDelay,
      int _duration, Progression* _progression, bool _remainAfterFinished);
   
   virtual void Begin();
   virtual void Update(unsigned int dt) { positionTransition.Update(dt); }
   virtual bool FinishedCheck() { return positionTransition.FinishedCheck(); }
   /// Skip to the end of the transition.
   virtual void Finish() { positionTransition.FinishedSet(); }
   
   /// Return the movement value for the given name.
   static Movement MovementGet(const char* name);
   /// Return the name for the given Movement.
   static const char* MovementNameGet(Movement movement);
   
   /// Factory method for making WidgetTransitionTranslate objects.
   static WidgetTransition* Create(Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);

   /// Return any offset to the position caused by the transition.
   virtual Point2F PositionOffsetGet() { return positionTransition.ValueGet(); }

   /// Called with the base clip area for the widgets is changed.
   virtual void OnClipAreaBaseChange();

   virtual void StartDelaySet(unsigned int _startDelay) { startDelay = _startDelay; }

protected:
   /// Set 'beginPosition' and 'endPosition' to the values to use for the
   /// transition.
   void BeginEndPositionsGet(Point2F* beginPosition, Point2F* endPosition);

   /// Way in which the widget is to be moved.
   Movement movement;
   /// Number of milliseconds after the transition officially begins before
   /// actually changing anything.
   int startDelay;
   /// Number of milliseconds after changes actually begin to happen until
   /// the transition is complete.
   int duration;
   /// Pattern of how the transition progresses from beginning to end.
   Progression* progression;
   /// True if 'Begin' has been called.
   bool started;
   /// Absolute position of the associated widget's parent the last time the
   /// transition began.
   Point2F parentNormalAbsolutePosition;
   /// Relative position of the associated widget the last time the transition
   /// began.
   Point2F normalRelativePosition;
   /// Manages the transition
   TimedTransitionLinear<Point2F> positionTransition;
};

//==============================================================================

/// A WidgetTransition that linearly interpolates position, rotation, scale, and
/// color.
class WidgetTransitionLinear : public WidgetTransition
{
public:
   typedef WidgetTransition Inherited;
   
   virtual ~WidgetTransitionLinear() {}
   void Init(Widget* _widget, bool _remainAfterFinished, JSONValue* specifications);
   
   virtual void Begin() { transition.Restart(); }
   virtual void Update(unsigned int dt) { transition.Update(dt); }
   virtual bool FinishedCheck() { return transition.FinishedCheck(); }
   virtual void Finish() { transition.FinishedSet(); }
   
   /// Factory method for making WidgetTransitionLinear objects.
   static WidgetTransition* Create(Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);

   /// Return any offset to the position caused by the transition.
   virtual Point2F PositionOffsetGet() { return transition.PositionGet(); }
   /// Return any additional rotation caused by the transition.
   virtual float RotationGet() { return transition.RotationGet(); }
   /// Return any additional scaling caused by the transition.
   virtual Point2F ScaleGet() { return transition.ScaleGet(); }
   /// Return any additional color modulation caused by the transition.
   virtual ColorRGBA8 ColorGet() { return transition.ColorGet(); }

   virtual void StartDelaySet(unsigned int _startDelay) { transition.StartDelaySet(_startDelay); }

protected:
   TimedTransitionLinear2D transition;
};

//==============================================================================

/// A WidgetTransition that linearly interpolates position, rotation, scale, and
/// color using splines.
class WidgetTransitionSplineLinear : public WidgetTransition
{
public:
   typedef WidgetTransition Inherited;
   
   virtual ~WidgetTransitionSplineLinear() {}
   void Init(Widget* _widget, bool _remainAfterFinished, JSONValue* specifications);
   virtual void Deinit();
   
   virtual void Begin() { transition.Restart(); }
   virtual void Update(unsigned int dt) { transition.Update(dt); }
   virtual bool FinishedCheck() { return transition.FinishedCheck(); }
   virtual void Finish() { transition.FinishedSet(); }
   
   /// Factory method for making WidgetTransitionSplineLinear objects.
   static WidgetTransition* Create(Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);

   /// Return any offset to the position caused by the transition.
   virtual Point2F PositionOffsetGet() { return transition.PositionGet(); }
   /// Return any additional rotation caused by the transition.
   virtual float RotationGet() { return transition.RotationGet(); }
   /// Return any additional scaling caused by the transition.
   virtual Point2F ScaleGet() { return transition.ScaleGet(); }
   /// Return any additional color modulation caused by the transition.
   virtual ColorRGBA8 ColorGet() { return transition.ColorGet(); }

   virtual void StartDelaySet(unsigned int _startDelay);

protected:
   TimedTransitionSplineLinear2D transition;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__WIDGETTRANSITION_H__

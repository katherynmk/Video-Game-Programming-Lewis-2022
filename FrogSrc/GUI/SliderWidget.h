#ifndef __FROG__GUI__SLIDERWIDGET_H__
#define __FROG__GUI__SLIDERWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"
#include "FloatRange.h"

namespace Webfoot {

class WidgetChildSprite;
class SliderThumbWidget;

//=============================================================================

/// Widget that allows the user to adjust a value by dragging a thumb button.
/// By default the maximum is 100, the minimum is 0, the minimum step size is
/// 1, and the initial value is 0.
/// Call Init immediately after construction.
class SliderWidget : public ContainerWidget, public FloatRange
{
public:
   SliderWidget();
   virtual ~SliderWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new SliderWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its children.
   virtual void Update(unsigned int dt);
   /// Draw the widget and its children.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return the "small" amount by which to increment the value of the widget.
   virtual float StepSmallGet() { return smallStep; }
   /// Set the "small" amount by which to increment the value of the widget.
   virtual void StepSmallSet(float _smallStep) { smallStep = _smallStep; }
   /// Return the "large" amount by which to increment the value of the widget.
   virtual float StepLargeGet() { return largeStep; }
   /// Set the "large" amount by which to increment the value of the widget.
   virtual void StepLargeSet(float _largeStep) { largeStep = _largeStep; }

   /// If true, when the user is pressing within the bounds of the slider, the
   /// slider goes directly to the value corresponding to the point rather than
   /// iterating toward it.
   virtual bool ValueSetByPointDirectCheck() { return valueSetByPointDirect; }
   /// See 'ValueSetByPointDirectCheck'.
   virtual void ValueSetByPointDirectSet(bool _valueSetByPointDirect) { valueSetByPointDirect = _valueSetByPointDirect; }

   /// Return the keypad direction that corresponds to an increase in value.
   Direction KeypadDirectionIncreaseGet() { return keypadDirectionIncrease; }
   /// Set the keypad direction that corresponds to an increase in value.
   void KeypadDirectionIncreaseSet(Direction _direction) { keypadDirectionIncrease = _direction; }
   /// Return the keypad direction that corresponds to a decrease in value.
   Direction KeypadDirectionDecreaseGet() { return keypadDirectionDecrease; }
   /// Set the keypad direction that corresponds to a decrease in value.
   void KeypadDirectionDecreaseSet(Direction _direction) { keypadDirectionDecrease = _direction; }

   /// Set the current value within the range.  The final value
   /// will be rounded to a multiple of stepMinimum from the current minimum.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void ValueSet(float _value, bool userMadeChange);
   /// Same as the other 'ValueSet' only it assumes the change did not come from
   /// the user using the widget.
   virtual void ValueSet(float _value) { ValueSet(_value, false); }

   /// Called when the left mouse button is pressed with the cursor over the widget.
   /// When the slider is clicked, increment or decrement the value by the large step
   /// based on which side of the thumb the cursor is on.
   virtual void OnPress();
   /// Called when the left mouse button is held over the widget.
   /// While the button remains held over the widget, the first call
   /// will be made HoldStartDelay milliseconds after
   /// the press and every HoldIntervalDuration milliseconds after that.
   /// 'consecutiveCalls' is the number of updates in a row before this update
   /// where OnHold was called.
   /// When the slider is held, increment or decrement the value by the large step
   /// based on which side of the thumb the cursor is on.
   virtual void OnHold(int consecutiveCalls);
   /// Called when a keypad focus begins to affirm this widget.
   virtual void OnKeypadPress();

   /// Called when the value of the widget changes.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void OnValueChange(float newValue, float oldValue, bool userMadeChange);

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return true; }

   /// Type of the function that can receive the OnValueChange event.
   /// The first float is the new value, the second float is the old value,
   /// the bool is true if the change was caused
   /// directly by the user interacting with the widget, the SliderWidget* is the
   /// slider that changed, and the void* is the userData pointer.
   ///
   /// static void OnValueChangeCallback(float newValue, float oldValue, bool userMadeChange,
   ///    SliderWidget* sliderWidget, void* userData);
   typedef void (*OnValueChangeCallback)(float, float, bool, SliderWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the ValueChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnValueChangeRegister(OnValueChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnValueChange event occurs.
   void OnValueChangeUnregister(OnValueChangeCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnValueChangeRegister(const char* path, OnValueChangeCallback callback, void* userData = NULL);

   virtual bool OpaqueCheck(const Point2F& testPosition);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) SliderWidget(); }

private:
   /// Sprite displayed behind the other child widgets.
   WidgetChildSprite* backgroundSprite;

   /// The widget that can be dragged to adjust the value of the widget.
   SliderThumbWidget* thumb;

   /// The "small" amount by which to increment the value of the widget.
   float smallStep;
   /// The "large" amount by which to increment the value of the widget.
   float largeStep;

   /// Used to manage the behavior of the thumb when the large step size
   /// is longer than the thumb.
   /// If this is -1.0f, allow overshooting by decreasing.  If it's 1.0f,
   /// allow overshooting by increasing.  If it's 0.0f, allow either one.
   float allowLargeStepOvershootDirection;
   /// If true, when the user is pressing within the bounds of the slider, the
   /// slider goes directly to the value corresponding to the point rather than
   /// iterating toward it.
   bool valueSetByPointDirect;

   /// Function to be called for the OnValueChange event.
   OnValueChangeCallback onValueChangeCallback;
   /// Pointer to be passed to the OnValueChange callback.
   void* onValueChangeUserData;
   /// If true, trigger the OnValueChange event when the thumb is pressed, even
   /// if this does not change the value.
   bool callOnValueChangeOnThumbPress;
   /// If true, trigger the OnValueChange event when is widget receives a
   /// keypad press, even though this does not change the value.
   bool callOnValueChangeOnKeypadPress;

   /// Keypad direction that corresponds to an increase in value.
   Direction keypadDirectionIncrease;
   /// Keypad direction that corresponds to a decrease in value.
   Direction keypadDirectionDecrease;

protected:
   typedef ContainerWidget Inherited;

   /// Increment or decrement the value by the large step based on
   /// which side of the thumb the cursor is on.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   void ValueSetByPoint(const Point2F& point, bool userMadeChange);

   friend class SliderThumbWidget;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__SLIDERWIDGET_H__

#ifndef __FROG__INPUTDELEGATE_H__
#define __FROG__INPUTDELEGATE_H__

#include "FrogMemory.h"
#include "KeyboardManager.h"
#include "MouseManager.h"
#include "GamepadGenericManager.h"
#include "XInputControllerManager.h"

namespace Webfoot {

class JSONValue;

/// Specifies a type of InputDelegate implementation.
#define INPUT_DELEGATE_TYPE_KEY "Type"
/// Type string for InputDelegateButtonKeyboardKey.
#define INPUT_DELEGATE_BUTTON_KEYBOARD_KEY_TYPE "KeyboardKey"
/// Type string for InputDelegateButtonMouseButton.
#define INPUT_DELEGATE_BUTTON_MOUSE_BUTTON_TYPE "MouseButton"
/// Type string for InputDelegateButtonGamepadGenericButton.
#define INPUT_DELEGATE_BUTTON_GAMEPAD_GENERIC_BUTTON_TYPE "GamepadGenericButton"
/// Type string for InputDelegateButtonGamepadGenericHat.
#define INPUT_DELEGATE_BUTTON_GAMEPAD_GENERIC_HAT_TYPE "GamepadGenericHat"
/// Type string for InputDelegateButtonGamepadGenericAxis.
#define INPUT_DELEGATE_BUTTON_GAMEPAD_GENERIC_AXIS_TYPE "GamepadGenericAxis"
/// Type string for InputDelegateAxisGamepadGenericAxis.
#define INPUT_DELEGATE_AXIS_GAMEPAD_GENERIC_AXIS_TYPE "GamepadGenericAxis"
/// Type string for InputDelegateConnectedXInputGamepadConnected
#define INPUT_DELEGATE_CONNECTED_XINPUT_GAMEPAD_CONNECTED_TYPE "XInputGamepadConnected"
/// Type string for InputDelegateButtonXInputGamepadButton.
#define INPUT_DELEGATE_BUTTON_XINPUT_GAMEPAD_BUTTON_TYPE "XInputGamepadButton"
/// Type string for InputDelegateButtonXInputGamepadAxis.
#define INPUT_DELEGATE_BUTTON_XINPUT_GAMEPAD_AXIS_TYPE "XInputGamepadAxis"
/// Type string for InputDelegateAxisXInputGamepadAxis.
#define INPUT_DELEGATE_AXIS_XINPUT_GAMEPAD_AXIS_TYPE "XInputGamepadAxis"
/// Type string for InputDelegateJoystickCompound.
#define INPUT_DELEGATE_JOYSTICK_COMPOUND_TYPE "Compound"

//==============================================================================

/// Interface for getting whether a device is connected.
class InputDelegateConnected
{
public:
   virtual ~InputDelegateConnected() {}

   /// Initialize the delegate using the given specifications.
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT) = 0;
   virtual void Deinit() {}

   /// Call on every frame.
   virtual void Update() {}

   /// Return true if the device should appear to be connected.
   virtual bool ConnectedCheck() = 0;

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT) = 0;
};

//==============================================================================

/// Interface for treating something as a button input.
class InputDelegateButton
{
public:
   virtual ~InputDelegateButton() {}
   
   /// Initialize the delegate using the given specifications.
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT) = 0;
   virtual void Deinit() {}

   /// Call on every frame.
   virtual void Update() {}
   
   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed() = 0;

   /// Return a value in the range [0, 1] for when the button is analog.
   virtual float AnalogValueGet() { return ButtonPressed() ? 1.0f : 0.0f; }
   /// Return true if the delegate is natively analog.
   virtual bool AnalogCheck() { return false; }
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT) = 0;

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT) = 0;
};

//==============================================================================

/// Interface for treating something as a gamepad axis.  [0, 1]
class InputDelegateAxis
{
public:
   virtual ~InputDelegateAxis() {}
   
   /// Initialize the delegate using the given specifications.
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT) = 0;
   virtual void Deinit() {}

   /// Call on every frame.
   virtual void Update() {}
   
   /// Return the current value for the axis.  [0, 1]
   virtual float ValueGet() = 0;
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT) = 0;
};

//==============================================================================

/// Interface for getting a value for a joystick.
class InputDelegateJoystick
{
public:
   virtual ~InputDelegateJoystick() {}
   
   /// Initialize the delegate using the given specifications.
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT) = 0;
   virtual void Deinit() {}

   /// Call on every frame.
   virtual void Update() {}
   
   /// Return the value for the joystick's position.  The maximum length of the
   /// vector is 1.0.  The range of the axes is [-1, 1].  Positive X is right
   /// and positive Y is up/forward.
   virtual Point2F ValueGet() = 0;
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT) = 0;
};

//==============================================================================

/// InputDelegateConnected that always returns the same value.
class InputDelegateConnectedConstant : public InputDelegateConnected
{
public:
   InputDelegateConnectedConstant() { connected = false; }
   virtual ~InputDelegateConnectedConstant() {}

   /// Initialize this delegate to return the given value for connected.
   void Init(bool _connected) { connected = _connected; }
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit() { connected = false; }

   /// Return true if the device should appear to be connected.
   virtual bool ConnectedCheck() { return connected; }

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateConnected* Clone(HeapID heapID = HEAP_DEFAULT);

protected:
   /// True if the device should appear connected.
   bool connected;
};

//==============================================================================

/// InputDelegateButton that listens for the press of a specific keyboard button.
class InputDelegateButtonKeyboardKey : public InputDelegateButton
{
public:
   InputDelegateButtonKeyboardKey() { keyCode = -1; }
   virtual ~InputDelegateButtonKeyboardKey() {}
   
   /// Initialize this delegate to listen for the given key to be pressed.
   void Init(int _keyCode) { keyCode = _keyCode; }
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit() { keyCode = -1; }
   
   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed() { return (keyCode != -1) && theKeyboard->KeyPressed(keyCode); }
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);
   
protected:
   /// Key on the keyboard for which this delegate is listening.
   int keyCode;
};

//==============================================================================

/// InputDelegateButton that listens for the press of a specific button of a
/// specific mouse object.
class InputDelegateButtonMouseButton : public InputDelegateButton
{
public:
   InputDelegateButtonMouseButton() { mouse = NULL; }
   virtual ~InputDelegateButtonMouseButton() {}
   
   /// Initialize this delegate to listen for the given button of the given
   /// mouse to be pressed. 
   void Init(Mouse* _mouse, Mouse::Button _mouseButton) { mouse = _mouse; mouseButton = _mouseButton; }
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit() { mouse = NULL; }
   
   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed() { return mouse && mouse->ConnectedCheck() && (mouse->ButtonsGet() & mouseButton); }
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);
      
protected:
   /// Mouse to which this delegate listens.
   Mouse* mouse;
   /// Button to which this delegate listens.
   Mouse::Button mouseButton;
};

//==============================================================================

/// InputDelegateButton that listens for the press of a specific button of a
/// specific GamepadGeneric.
class InputDelegateButtonGamepadGenericButton : public InputDelegateButton
{
public:
   InputDelegateButtonGamepadGenericButton() { gamepad = NULL; }
   virtual ~InputDelegateButtonGamepadGenericButton() {}
   
   void Init(GamepadGeneric* _gamepad, int _buttonIndex);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }
   
   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed();
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);
   
protected:
   /// Gamepad to check
   GamepadGeneric* gamepad;
   /// Index of the button to which this should listen.
   int buttonIndex;
};

//==============================================================================

/// InputDelegate that treats moving a GamepadGeneric POV hat switch in a
/// specific direction as a button press.
class InputDelegateButtonGamepadGenericHat : public InputDelegateButton
{
public:
   InputDelegateButtonGamepadGenericHat() { gamepad = NULL; }
   virtual ~InputDelegateButtonGamepadGenericHat() {}
   
   void Init(GamepadGeneric* _gamepad, int _hatIndex, int _hatDirection);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }
   
   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed();
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);
   
protected:
   /// Gamepad to check
   GamepadGeneric* gamepad;
   /// Index of the axis to treat as a button.
   int hatIndex;
   /// Direction of the POV hat switch to test.
   int hatDirection;
};

//==============================================================================

/// InputDelegate that treats moving a GamepadGeneric axis away from its
/// initial value in a particular direction as a button press.
class InputDelegateButtonGamepadGenericAxis : public InputDelegateButton
{
public:
   InputDelegateButtonGamepadGenericAxis() { gamepad = NULL; }
   virtual ~InputDelegateButtonGamepadGenericAxis() {}
   
   /// Set 'activeLow' to true if the button should be triggered when the axis
   /// value is low, and false if it should be triggered when the value is
   /// high.
   void Init(GamepadGeneric* _gamepad, int _axisIndex, bool _activeLow);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }
   
   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed();

   /// Return a value in the range [0, 1] for when the button is analog.
   virtual float AnalogValueGet();
   /// Return true if the delegate is natively analog.
   virtual bool AnalogCheck() { return true; }
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);
   
protected:
   /// Gamepad to check
   GamepadGeneric* gamepad;
   /// Index of the axis to treat as a button.
   int axisIndex;
   /// True if the button should be triggered when the axis value is low, and
   /// false if it should be triggered when the value is high.
   bool activeLow;
};

//==============================================================================

/// InputDelegateAxis that gets its value from a GamepadGeneric axis.
class InputDelegateAxisGamepadGenericAxis : public InputDelegateAxis
{
public:
   InputDelegateAxisGamepadGenericAxis() { gamepad = NULL; }
   virtual ~InputDelegateAxisGamepadGenericAxis() {}
   
   /// Set '_invert' to true to invert the value coming from the gamepad.
   void Init(GamepadGeneric* _gamepad, int _axisIndex, bool _invert);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }
   
   /// Return the current value for the axis.  [0, 1]
   virtual float ValueGet();
   
   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);
   
protected:
   /// Gamepad to check
   GamepadGeneric* gamepad;
   /// Index of the axis to treat as a button.
   int axisIndex;
   /// True if the range of values coming from the gamepad should be inverted.
   bool invert;
};

//==============================================================================

/// InputDelegateConnected which appears connected when a particular
/// XInputGamepad is connected.
class InputDelegateConnectedXInputGamepadConnected : public InputDelegateConnected
{
public:
   InputDelegateConnectedXInputGamepadConnected() { gamepad = NULL; }
   virtual ~InputDelegateConnectedXInputGamepadConnected() {}

   void Init(XInputGamepad* _gamepad) { gamepad = _gamepad; }
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit() { gamepad = NULL; }

   /// Return true if the device should appear to be connected.
   virtual bool ConnectedCheck();

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateConnected* Clone(HeapID heapID = HEAP_DEFAULT);

protected:
   /// Gamepad to check
   XInputGamepad* gamepad;
};

//==============================================================================

/// InputDelegateButton that listens for the press of a specific button of a
/// specific XInputGamepad.
class InputDelegateButtonXInputGamepadButton : public InputDelegateButton
{
public:
   InputDelegateButtonXInputGamepadButton() { gamepad = NULL; }
   virtual ~InputDelegateButtonXInputGamepadButton() {}

   void Init(XInputGamepad* _gamepad, int _button);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }

   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed();

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);

protected:
   /// Gamepad to check
   XInputGamepad* gamepad;
   /// Button to which this should listen.
   int button;
};

//==============================================================================

/// InputDelegate that treats moving an XInputGamepad axis away from its
/// initial value in a particular direction as a button press.
class InputDelegateButtonXInputGamepadAxis : public InputDelegateButton
{
public:
   InputDelegateButtonXInputGamepadAxis() { gamepad = NULL; }
   virtual ~InputDelegateButtonXInputGamepadAxis() {}

   /// Set 'activeLow' to true if the button should be triggered when the axis
   /// value is low, and false if it should be triggered when the value is
   /// high.
   void Init(XInputGamepad* _gamepad, int _axis, bool _activeLow);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }

   /// Return true if the button should be treated as pressed.
   virtual bool ButtonPressed();

   /// Return a value in the range [0, 1] for when the button is analog.
   virtual float AnalogValueGet();
   /// Return true if the delegate is natively analog.
   virtual bool AnalogCheck() { return true; }

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return a duplicate of this delegate.
   virtual InputDelegateButton* Clone(HeapID heapID = HEAP_DEFAULT);

protected:
   /// Gamepad to check
   XInputGamepad* gamepad;
   /// Axis to treat as a button.
   int axis;
   /// True if the button should be triggered when the axis value is low, and
   /// false if it should be triggered when the value is high.
   bool activeLow;
};

//==============================================================================

/// InputDelegateAxis that gets its value from an XInputGamepad axis.
class InputDelegateAxisXInputGamepadAxis : public InputDelegateAxis
{
public:
   InputDelegateAxisXInputGamepadAxis() { gamepad = NULL; }
   virtual ~InputDelegateAxisXInputGamepadAxis() {}

   /// Set '_invert' to true to invert the value coming from the gamepad.
   void Init(XInputGamepad* _gamepad, int _axis, bool _invert);
   virtual void Init(JSONValue* specifications, HeapID heapID); 
   virtual void Deinit() { gamepad = NULL; }

   /// Return the current value for the axis.  [0, 1]
   virtual float ValueGet();

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

protected:
   /// Gamepad to check
   XInputGamepad* gamepad;
   /// Axis to treat as a button.
   int axis;
   /// True if the range of values coming from the gamepad should be inverted.
   bool invert;
};

//==============================================================================

/// InputDelegateJoystick that builds its values based on other InputDelegates.
class InputDelegateJoystickCompound : public InputDelegateJoystick
{
public:
   InputDelegateJoystickCompound();
   virtual ~InputDelegateJoystickCompound() {}
   
   /// Initialize the delegate without any initial child delegates.
   void Init();
   virtual void Init(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit();
   
   /// Return the value for the joystick's position.  The maximum length of the
   /// vector is 1.0.  The range of the axes is [-1, 1].  Positive X is right
   /// and positive Y is up/forward.
   virtual Point2F ValueGet();

   /// Delegate for the x axis [0, 1] will be mapped to [-1, 1].  This is
   /// mutually exclusive with separate left and right delegates.  The
   /// specified delegate will be deinited and deleted automatically by this
   /// object.
   void XDelegateSet(InputDelegateAxis* _xDelegate);
   /// Delegate for the x axis [0, 1] will be mapped to [-1, 1].  This is
   /// mutually exclusive with separate up and down delegates.  The
   /// specified delegate will be deinited and deleted automatically by this
   /// object.
   void YDelegateSet(InputDelegateAxis* _yDelegate);
   /// Delegate for up (y-axis).  This is mutually exclusive with having a
   /// single delegate for the y axis.  The specified delegate will be deinited
   /// and deleted automatically by this object.
   void UpDelegateSet(InputDelegateButton* _upDelegate);
   /// Delegate for down (y-axis).  This is mutually exclusive with having a
   /// single delegate for the y axis.  The specified delegate will be deinited
   /// and deleted automatically by this object.
   void DownDelegateSet(InputDelegateButton* _downDelegate);
   /// Delegate for left (x-axis).  This is mutually exclusive with having a
   /// single delegate for the x axis.  The specified delegate will be deinited
   /// and deleted automatically by this object.
   void LeftDelegateSet(InputDelegateButton* _leftDelegate);
   /// Delegate for right (x-axis).  This is mutually exclusive with having a
   /// single delegate for the x axis.  The specified delegate will be deinited
   /// and deleted automatically by this object.
   void RightDelegateSet(InputDelegateButton* _rightDelegate);

   /// Return a JSON representation of this object.  Deinit and delete it when
   /// finished with it.
   virtual JSONValue* JSONSpecificationsCreate(HeapID heapID = HEAP_DEFAULT);

protected:
   /// Delegate for the x axis [0, 1] will be mapped to [-1, 1].  This is
   /// mutually exclusive with separate left and right delegates.
   InputDelegateAxis* xDelegate;
   /// Delegate for the x axis [0, 1] will be mapped to [-1, 1].  This is
   /// mutually exclusive with separate up and down delegates.
   InputDelegateAxis* yDelegate;
   /// Delegate for up (y-axis).  This is mutually exclusive with having a
   /// single delegate for the y axis.
   InputDelegateButton* upDelegate;
   /// Delegate for down (y-axis).  This is mutually exclusive with having a
   /// single delegate for the y axis.
   InputDelegateButton* downDelegate;
   /// Delegate for left (x-axis).  This is mutually exclusive with having a
   /// single delegate for the x axis.
   InputDelegateButton* leftDelegate;
   /// Delegate for right (x-axis).  This is mutually exclusive with having a
   /// single delegate for the x axis.
   InputDelegateButton* rightDelegate;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__INPUTDELEGATE_H__

#ifndef __FROG__XINPUTCONTROLLERMANAGER_H__
#define __FROG__XINPUTCONTROLLERMANAGER_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// Enumeration of button flags for the XInput Gamepad
enum XInputGamepadButton
{
   XINPUT_GAMEPAD_BUTTON_DPAD_UP = 0x00000001,
   XINPUT_GAMEPAD_BUTTON_DPAD_DOWN = 0x00000002,
   XINPUT_GAMEPAD_BUTTON_DPAD_LEFT = 0x00000004,
   XINPUT_GAMEPAD_BUTTON_DPAD_RIGHT = 0x00000008,
   XINPUT_GAMEPAD_BUTTON_A = 0x00000010,
   XINPUT_GAMEPAD_BUTTON_B = 0x00000020,
   XINPUT_GAMEPAD_BUTTON_X = 0x00000040,
   XINPUT_GAMEPAD_BUTTON_Y = 0x00000080,
   XINPUT_GAMEPAD_BUTTON_START = 0x00000200,
   XINPUT_GAMEPAD_BUTTON_BACK = 0x00000400,
   XINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER = 0x00000800,
   XINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER = 0x00001000,
   XINPUT_GAMEPAD_BUTTON_LEFT_TRIGGER = 0x00002000,
   XINPUT_GAMEPAD_BUTTON_RIGHT_TRIGGER = 0x00004000,
   /// Pressing the left stick inward, as a button
   XINPUT_GAMEPAD_BUTTON_LEFT_STICK = 0x00008000,
   /// Pressing the right stick inward, as a button
   XINPUT_GAMEPAD_BUTTON_RIGHT_STICK = 0x00010000,
   /// Convienience feature that treats pressing up on the left stick as though
   /// it were a button press.
   XINPUT_GAMEPAD_BUTTON_LEFT_STICK_UP = 0x00020000,
   XINPUT_GAMEPAD_BUTTON_LEFT_STICK_DOWN = 0x00040000,
   XINPUT_GAMEPAD_BUTTON_LEFT_STICK_LEFT = 0x00080000,
   XINPUT_GAMEPAD_BUTTON_LEFT_STICK_RIGHT = 0x00100000,
   XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_UP = 0x00200000,
   XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_DOWN = 0x00400000,
   XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_LEFT = 0x00800000,
   XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_RIGHT = 0x01000000,
   
   XINPUT_GAMEPAD_BUTTON_ANY = XINPUT_GAMEPAD_BUTTON_DPAD_UP | XINPUT_GAMEPAD_BUTTON_DPAD_DOWN |
      XINPUT_GAMEPAD_BUTTON_DPAD_LEFT | XINPUT_GAMEPAD_BUTTON_DPAD_RIGHT | XINPUT_GAMEPAD_BUTTON_A |
      XINPUT_GAMEPAD_BUTTON_B | XINPUT_GAMEPAD_BUTTON_X | XINPUT_GAMEPAD_BUTTON_Y |
      XINPUT_GAMEPAD_BUTTON_START | XINPUT_GAMEPAD_BUTTON_BACK |
      XINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER | XINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER | XINPUT_GAMEPAD_BUTTON_LEFT_TRIGGER |
      XINPUT_GAMEPAD_BUTTON_RIGHT_TRIGGER | XINPUT_GAMEPAD_BUTTON_LEFT_STICK | XINPUT_GAMEPAD_BUTTON_RIGHT_STICK |
      XINPUT_GAMEPAD_BUTTON_LEFT_STICK_UP |XINPUT_GAMEPAD_BUTTON_LEFT_STICK_DOWN |
      XINPUT_GAMEPAD_BUTTON_LEFT_STICK_LEFT | XINPUT_GAMEPAD_BUTTON_LEFT_STICK_RIGHT |
      XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_UP | XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_DOWN |
      XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_LEFT | XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_RIGHT,

   XINPUT_GAMEPAD_DPAD_DIRECTION_BUTTONS = XINPUT_GAMEPAD_BUTTON_DPAD_UP |
      XINPUT_GAMEPAD_BUTTON_DPAD_DOWN | XINPUT_GAMEPAD_BUTTON_DPAD_LEFT |
      XINPUT_GAMEPAD_BUTTON_DPAD_RIGHT,

   XINPUT_GAMEPAD_LEFT_STICK_DIRECTION_BUTTONS = XINPUT_GAMEPAD_BUTTON_LEFT_STICK_UP |
      XINPUT_GAMEPAD_BUTTON_LEFT_STICK_DOWN | XINPUT_GAMEPAD_BUTTON_LEFT_STICK_LEFT |
      XINPUT_GAMEPAD_BUTTON_LEFT_STICK_RIGHT,

   XINPUT_GAMEPAD_RIGHT_STICK_DIRECTION_BUTTONS = XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_UP |
      XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_DOWN | XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_LEFT |
      XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_RIGHT
};

/// Enumeration of analog axes for the XInput Gamepad
enum XInputGamepadAxis
{
   XINPUT_GAMEPAD_AXIS_LEFT_STICK_X,
   XINPUT_GAMEPAD_AXIS_LEFT_STICK_Y,
   XINPUT_GAMEPAD_AXIS_RIGHT_STICK_X,
   XINPUT_GAMEPAD_AXIS_RIGHT_STICK_Y,
   XINPUT_GAMEPAD_AXIS_LEFT_TRIGGER,
   XINPUT_GAMEPAD_AXIS_RIGHT_TRIGGER,
   XINPUT_GAMEPAD_AXIS_COUNT
};

} //namespace Webfoot {

#include "XInputGamepadCommon.h"

namespace Webfoot {

//==============================================================================

/// PC implementation of XInputGamepad.
class XInputGamepad : public XInputGamepadCommon
{
public:
   void Update();

   typedef XInputGamepadCommon Inherited;

protected:
   /// Helper function for converting a native trigger input value to the Frog
   /// representation.
   float TriggerHelper(int inputValue);
   /// Helper function for converting native joystick input values to the Frog
   /// representation.
   Point2F JoystickHelper(int x, int y, int deadZone);
   /// Helper function for setting the 'buttonStatesNext' flags for a joystick
   /// or DPad based on the provided 'value'.
   void JoystickButtonHelper(const Point2F& value, int upButtonCode,
      int downButtonCode, int leftButtonCode, int rightButtonCode);

   friend class XInputControllerManager;

   /// Value of 'buttonStates' for the next frame.
   uint32 buttonStatesNext;
   /// Value of 'leftStick' for the next frame.
   Point2F leftStickNext;
   /// Value of 'rightStick' for the next frame.
   Point2F rightStickNext;
   /// Value of 'leftTrigger' for the next frame.
   float leftTriggerNext;
   /// Value of 'rightTrigger' for the next frame.
   float rightTriggerNext;
};

//==============================================================================

/// 'XInputControllerManager' handles input from XInput controllers in PC builds.
class XInputControllerManager
{
public:
   XInputControllerManager();
   
   void Init(bool initGamepadTypical = true);
   void Deinit();

   void Update(bool hasFocus);

   /// Return true if this is initialized.
   bool InitializedCheck() { return initialized; }

   enum
   {
      /// Maximium number of separate XInputGamepads to track.
      XINPUT_GAMEPAD_COUNT_MAX = 4
   };

   /// Return the maximum number of separate XInputGamepads to track.
   int XInputGamepadCountMaxGet() { return XINPUT_GAMEPAD_COUNT_MAX; }

   static XInputControllerManager instance;

   /// XInputGamepad object that aggregates input from the others.
   static XInputGamepad xInputGamepad;
   /// Separate objects for the individual XInputGamepads.
   static XInputGamepad xInputGamepads[XINPUT_GAMEPAD_COUNT_MAX];
   /// Pointers to the individual elements of 'xInputGamepads' so that
   /// 'theXInputGamepads' can be an array of pointers.
   static XInputGamepad* xInputGamepadPointers[XINPUT_GAMEPAD_COUNT_MAX];

protected:
   /// True if this is initialized.
   bool initialized;
   /// True if Init initialized the GamepadTypical objects.
   bool initializedGamepadTypical;
};

XInputControllerManager* const theXInputControllerManager = &XInputControllerManager::instance;

/// Pointer to the XInputGamepad object that aggregates input from the others.
XInputGamepad* const theXInputGamepad = &XInputControllerManager::xInputGamepad;
/// Separate objects for the individual XInputGamepads.
XInputGamepad** const theXInputGamepads = (XInputGamepad**)(XInputControllerManager::xInputGamepadPointers);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__XINPUTCONTROLLERMANAGER_H__

#ifndef __FROG__XINPUTGAMEPADCOMMON_H__
#define __FROG__XINPUTGAMEPADCOMMON_H__

// This should only be included from XInputControllerManager.h.

#include "FrogMemory.h"
#include "Point2.h"

namespace Webfoot {

class GamepadTypical;

//==============================================================================

/// Platform-independent base class for an XInput Gamepad.
class XInputGamepadCommon
{
public:
   XInputGamepadCommon();

   void Init();
   void Deinit();

   /// Return true if the gamepad is currently connected.
   bool ConnectedCheck() const { return connected; }
   /// Return true if the gamepad just connected as of this frame.
   bool JustConnectedCheck() const { return connected && !connectedPrev; }
   /// Return true if the gamepad just disconnected as of this frame.
   bool JustDisconnectedCheck() const { return !connected && connectedPrev; }

   /// Return true if the given button is currently pressed.
   /// If no button is specified, return true if any "button" is currently pressed.
   bool ButtonPressed(XInputGamepadButton button = XINPUT_GAMEPAD_BUTTON_ANY) const;
   /// Return true if the given button was just pressed within the last frame.
   /// If no button is specified, return true if any "button" was just pressed within the last frame.
   bool ButtonJustPressed(XInputGamepadButton button = XINPUT_GAMEPAD_BUTTON_ANY) const;
   /// Return true if the given button was just released within the last frame.
   /// If no button is specified, return true if any "button" was just released within the last frame.
   bool ButtonJustReleased(XInputGamepadButton button = XINPUT_GAMEPAD_BUTTON_ANY) const;

   /// Return how the left analog stick is currently being pressed.
   /// X is right and Y is forward.  The maximum length is clamped to 1.0.
   Point2F LeftStickGet() const { return connected ? leftStick : Point2F::Create(0.0f, 0.0f); }
   /// Return how the right analog stick is currently being pressed.
   /// X is right and Y is forward.  The maximum length is clamped to 1.0.
   Point2F RightStickGet() const { return connected ? rightStick : Point2F::Create(0.0f, 0.0f); }

   /// Return how much the left trigger is currently being pressed.  Values
   /// should be in the range [0, 1].
   float LeftTriggerGet() { return connected ? leftTrigger : 0.0f; }
   /// Return how much the right trigger is currently being pressed.  Values
   /// should be in the range [0, 1].
   float RightTriggerGet() { return connected ? rightTrigger : 0.0f; }

   bool DPadUpPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_DPAD_UP); }
   bool DPadUpJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_DPAD_UP); }
   bool DPadUpJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_DPAD_UP); }

   bool DPadDownPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_DPAD_DOWN); }
   bool DPadDownJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_DPAD_DOWN); }
   bool DPadDownJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_DPAD_DOWN); }

   bool DPadLeftPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_DPAD_LEFT); }
   bool DPadLeftJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_DPAD_LEFT); }
   bool DPadLeftJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_DPAD_LEFT); }

   bool DPadRightPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_DPAD_RIGHT); }
   bool DPadRightJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_DPAD_RIGHT); }
   bool DPadRightJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_DPAD_RIGHT); }

   bool APressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_A); }
   bool AJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_A); }
   bool AJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_A); }

   bool BPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_B); }
   bool BJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_B); }
   bool BJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_B); }

   bool XPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_X); }
   bool XJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_X); }
   bool XJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_X); }

   bool YPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_Y); }
   bool YJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_Y); }
   bool YJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_Y); }

   bool StartPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_START); }
   bool StartJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_START); }
   bool StartJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_START); }

   bool BackPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_BACK); }
   bool BackJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_BACK); }
   bool BackJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_BACK); }

   bool LeftShoulderPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER); }
   bool LeftShoulderJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER); }
   bool LeftShoulderJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER); }

   bool RightShoulderPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER); }
   bool RightShoulderJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER); }
   bool RightShoulderJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER); }

   /// Convenience function that treats pressing the left trigger as though it
   /// was a button press.
   bool LeftTriggerPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_TRIGGER); }
   bool LeftTriggerJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_TRIGGER); }
   bool LeftTriggerJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_TRIGGER); }

   bool RightTriggerPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_TRIGGER); }
   bool RightTriggerJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_TRIGGER); }
   bool RightTriggerJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_TRIGGER); }

   /// Return true if the left stick is pressed inward, as a button.
   bool LeftStickPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK); }
   bool LeftStickJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK); }
   bool LeftStickJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_STICK); }

   /// Return true if the right stick is pressed inward, as a button.
   bool RightStickPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK); }
   bool RightStickJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK); }
   bool RightStickJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK); }

   /// Convienience function that treats pressing up on the left stick as though
   /// it were a button press.  Different combinations of these "buttons" can
   /// appear "pressed" to denote diagonals.
   bool LeftStickUpPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_UP); }
   bool LeftStickUpJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_UP); }
   bool LeftStickUpJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_UP); }

   bool LeftStickDownPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_DOWN); }
   bool LeftStickDownJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_DOWN); }
   bool LeftStickDownJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_DOWN); }

   bool LeftStickLeftPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_LEFT); }
   bool LeftStickLeftJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_LEFT); }
   bool LeftStickLeftJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_LEFT); }

   bool LeftStickRightPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_RIGHT); }
   bool LeftStickRightJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_RIGHT); }
   bool LeftStickRightJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_LEFT_STICK_RIGHT); }

   bool RightStickUpPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_UP); }
   bool RightStickUpJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_UP); }
   bool RightStickUpJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_UP); }

   bool RightStickDownPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_DOWN); }
   bool RightStickDownJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_DOWN); }
   bool RightStickDownJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_DOWN); }

   bool RightStickLeftPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_LEFT); }
   bool RightStickLeftJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_LEFT); }
   bool RightStickLeftJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_LEFT); }

   bool RightStickRightPressed() const { return ButtonPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_RIGHT); }
   bool RightStickRightJustPressed() const { return ButtonJustPressed(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_RIGHT); }
   bool RightStickRightJustReleased() const { return ButtonJustReleased(XINPUT_GAMEPAD_BUTTON_RIGHT_STICK_RIGHT); }

   /// Return the current value for the given axis.  Values should be in the
   /// range [-1, 1], but it may be closer to [0, 1] for some axes.
   float AxisValueGet(XInputGamepadAxis axis);

   /// Return a platform-independent string representation of the given
   /// button code.  Return NULL if unsuccessful.
   static const char* ButtonCodeToString(int buttonCode);
   /// Return the button code that correspends to the given platform-independent
   /// string.  Return -1 if unsuccessful.
   static int ButtonStringToCode(const char* buttonString);

   /// Return a platform-independent string representation of the given
   /// axis code.  Return NULL if unsuccessful.
   static const char* AxisCodeToString(int axisCode);
   /// Return the axis code that correspends to the given platform-independent
   /// string.  Return -1 if unsuccessful.
   static int AxisStringToCode(const char* axisString);

   /// GamepadTypical object which corresponds to this object.
   GamepadTypical* gamepadTypical;

protected:
   /// Update the given GamepadTypical based on the current state of this one.
   void GamepadTypicalUpdate(GamepadTypical* gamepadTypical);

   /// Value of 'connected' from the next frame.
   bool connectedNext;
   /// True if the gamepad is currently connected.
   bool connected;
   /// Value of 'connected' from the previous frame.
   bool connectedPrev;
   /// Buttons that were pressed on the previous frame.
   uint32 buttonStatesPrev;
   /// Buttons that are currently pressed.
   uint32 buttonStates;
   /// Current value for the left stick.
   Point2F leftStick;
   /// Current value for the right stick.
   Point2F rightStick;
   /// Current value for the left trigger.
   float leftTrigger;
   /// Current value for the right trigger.
   float rightTrigger;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__XINPUTGAMEPADCOMMON_H__

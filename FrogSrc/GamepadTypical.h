#ifndef __FROG__GAMEPADTYPICAL_H__
#define __FROG__GAMEPADTYPICAL_H__

#include "FrogMemory.h"
#include "Point2.h"

namespace Webfoot {

#if PLATFORM_IS_WINDOWS || PLATFORM_IS_ANDROID
   class AndroidGamepadCommon;
   class AndroidGamepad;
#endif

#if PLATFORM_IS_LINUX || PLATFORM_IS_EMSCRIPTEN
   class SDL2GameController;
#endif

#if PLATFORM_IS_WINDOWS || PLATFORM_IS_XBOX360
   class Xbox360GamepadCommon;
   class Xbox360Gamepad;
#endif

#if PLATFORM_IS_WINDOWS
   class XInputGamepadCommon;
   class XInputGamepad;
#endif

//==============================================================================

/// Enumeration of button flags for an Android gamepad
enum GamepadTypicalButton
{
   GAMEPAD_TYPICAL_BUTTON_DPAD_UP = 0x00000001,
   GAMEPAD_TYPICAL_BUTTON_DPAD_DOWN = 0x00000002,
   GAMEPAD_TYPICAL_BUTTON_DPAD_LEFT = 0x00000004,
   GAMEPAD_TYPICAL_BUTTON_DPAD_RIGHT = 0x00000008,
   GAMEPAD_TYPICAL_BUTTON_A = 0x00000010,
   GAMEPAD_TYPICAL_BUTTON_B = 0x00000020,
   GAMEPAD_TYPICAL_BUTTON_X = 0x00000040,
   GAMEPAD_TYPICAL_BUTTON_Y = 0x00000080,
   GAMEPAD_TYPICAL_BUTTON_START = 0x00000200,
   GAMEPAD_TYPICAL_BUTTON_SELECT = 0x00000400,
   GAMEPAD_TYPICAL_BUTTON_LEFT_SHOULDER = 0x00000800,
   GAMEPAD_TYPICAL_BUTTON_RIGHT_SHOULDER = 0x00001000,
   GAMEPAD_TYPICAL_BUTTON_LEFT_TRIGGER = 0x00002000,
   GAMEPAD_TYPICAL_BUTTON_RIGHT_TRIGGER = 0x00004000,
   /// Pressing the left stick inward, as a button
   GAMEPAD_TYPICAL_BUTTON_LEFT_STICK = 0x00008000,
   /// Pressing the right stick inward, as a button
   GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK = 0x00010000,
   /// Convienience feature that treats pressing up on the left stick as though
   /// it were a button press.
   GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_UP = 0x00020000,
   GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_DOWN = 0x00040000,
   GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_LEFT = 0x00080000,
   GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_RIGHT = 0x00100000,
   GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_UP = 0x00200000,
   GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_DOWN = 0x00400000,
   GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_LEFT = 0x00800000,
   GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_RIGHT = 0x01000000,

   /// These are not necessarily dedicated buttons, but they can be used to
   /// convey whether something positive or negative is being pressed.
   GAMEPAD_TYPICAL_BUTTON_AFFIRMATIVE = 0x02000000,
   GAMEPAD_TYPICAL_BUTTON_NEGATIVE = 0x04000000,
   
   GAMEPAD_TYPICAL_BUTTON_ANY = GAMEPAD_TYPICAL_BUTTON_DPAD_UP | GAMEPAD_TYPICAL_BUTTON_DPAD_DOWN |
      GAMEPAD_TYPICAL_BUTTON_DPAD_LEFT | GAMEPAD_TYPICAL_BUTTON_DPAD_RIGHT | GAMEPAD_TYPICAL_BUTTON_A |
      GAMEPAD_TYPICAL_BUTTON_B | GAMEPAD_TYPICAL_BUTTON_X | GAMEPAD_TYPICAL_BUTTON_Y |
      GAMEPAD_TYPICAL_BUTTON_START | GAMEPAD_TYPICAL_BUTTON_SELECT |
      GAMEPAD_TYPICAL_BUTTON_LEFT_SHOULDER | GAMEPAD_TYPICAL_BUTTON_RIGHT_SHOULDER | GAMEPAD_TYPICAL_BUTTON_LEFT_TRIGGER |
      GAMEPAD_TYPICAL_BUTTON_RIGHT_TRIGGER | GAMEPAD_TYPICAL_BUTTON_LEFT_STICK | GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK |
      GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_UP |GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_DOWN |
      GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_LEFT | GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_RIGHT |
      GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_UP | GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_DOWN |
      GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_LEFT | GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_RIGHT |
      GAMEPAD_TYPICAL_BUTTON_AFFIRMATIVE | GAMEPAD_TYPICAL_BUTTON_NEGATIVE
};

//==============================================================================

/// While gamepads for different platforms often have unique features, a certain
/// amount of functionality is typical.  This provides an interface for roughly
/// that level of functionality to provide easier portability between adequately
/// similar platforms.  If there aren't separate Start and Select buttons on the
/// underlying controller, Start should be the button for this role.  Bear in
/// mind that some gamepads may have neither.  There may be some controllers
/// that support only a small subset of this functionality, but still have a
/// corresponding GamepadTypical.
class GamepadTypical
{
public:
   GamepadTypical();

   void Init();
   void Deinit();

   /// Return true if the gamepad is currently connected.
   bool ConnectedCheck() const { return connected; }
   /// Return true if the gamepad just connected as of this frame.
   bool JustConnectedCheck() const { return connected && !connectedPrev; }
   /// Return true if the gamepad just disconnected as of this frame.
   bool JustDisconnectedCheck() const { return !connected && connectedPrev; }

   /// Return true if the given button is currently pressed.  If no button is
   /// specified, return true if any "button" is currently pressed.
   bool ButtonPressed(GamepadTypicalButton button = GAMEPAD_TYPICAL_BUTTON_ANY) const;
   /// Return true if the given button was just pressed within the last frame.
   /// If no button is specified, return true if any "button" was just pressed
   /// within the last frame.
   bool ButtonJustPressed(GamepadTypicalButton button = GAMEPAD_TYPICAL_BUTTON_ANY) const;
   /// Return true if the given button was just released within the last frame.
   /// If no button is specified, return true if any "button" was just released
   /// within the last frame.
   bool ButtonJustReleased(GamepadTypicalButton button = GAMEPAD_TYPICAL_BUTTON_ANY) const;

   /// Return how the left analog stick is currently being pressed.
   /// X is right and Y is forward.  The maximum length is clamped to 1.0.
   Point2F LeftStickGet() const { return connected ? leftStick : Point2F::Create(0.0f, 0.0f); }
   /// Return how the right analog stick is currently being pressed.
   /// X is right and Y is forward.  The maximum length is clamped to 1.0.
   Point2F RightStickGet() const { return connected ? rightStick : Point2F::Create(0.0f, 0.0f); }

   /// Return how much the left trigger is currently being pressed.  Values
   /// should be in the range [0, 1].  This is sometimes called L2.
   float LeftTriggerGet() const { return connected ? leftTrigger : 0.0f; }
   /// Return how much the right trigger is currently being pressed.  Values
   /// should be in the range [0, 1].  This is sometimes called R2.
   float RightTriggerGet() const { return connected ? rightTrigger : 0.0f; }

   /// This is not typically a distinct button, but it is listed separately to
   /// allow for variation between controllers.
   bool AffirmativePressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_AFFIRMATIVE); }
   bool AffirmativeJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_AFFIRMATIVE); }
   bool AffirmativeJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_AFFIRMATIVE); }

   /// This is not typically a distinct button, but it is listed separately to
   /// allow for variation between controllers.
   bool NegativePressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_NEGATIVE); }
   bool NegativeJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_NEGATIVE); }
   bool NegativeJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_NEGATIVE); }

   bool DPadUpPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_UP); }
   bool DPadUpJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_UP); }
   bool DPadUpJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_DPAD_UP); }

   bool DPadDownPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_DOWN); }
   bool DPadDownJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_DOWN); }
   bool DPadDownJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_DPAD_DOWN); }

   bool DPadLeftPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_LEFT); }
   bool DPadLeftJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_LEFT); }
   bool DPadLeftJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_DPAD_LEFT); }

   bool DPadRightPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_RIGHT); }
   bool DPadRightJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_DPAD_RIGHT); }
   bool DPadRightJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_DPAD_RIGHT); }

   bool APressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_A); }
   bool AJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_A); }
   bool AJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_A); }

   bool BPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_B); }
   bool BJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_B); }
   bool BJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_B); }

   bool XPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_X); }
   bool XJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_X); }
   bool XJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_X); }

   bool YPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_Y); }
   bool YJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_Y); }
   bool YJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_Y); }

   bool StartPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_START); }
   bool StartJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_START); }
   bool StartJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_START); }

   bool SelectPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_SELECT); }
   bool SelectJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_SELECT); }
   bool SelectJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_SELECT); }

   // This is sometimes called L1.
   bool LeftShoulderPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_SHOULDER); }
   bool LeftShoulderJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_SHOULDER); }
   bool LeftShoulderJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_SHOULDER); }

   // This is sometimes called R1.
   bool RightShoulderPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_SHOULDER); }
   bool RightShoulderJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_SHOULDER); }
   bool RightShoulderJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_SHOULDER); }

   /// Convenience function that treats pressing the left trigger as though it
   /// was a button press.  This is sometimes called L2.
   bool LeftTriggerPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_TRIGGER); }
   bool LeftTriggerJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_TRIGGER); }
   bool LeftTriggerJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_TRIGGER); }

   /// This is sometimes called R2.
   bool RightTriggerPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_TRIGGER); }
   bool RightTriggerJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_TRIGGER); }
   bool RightTriggerJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_TRIGGER); }

   /// Return true if the left stick is pressed inward, as a button.
   bool LeftStickPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK); }
   bool LeftStickJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK); }
   bool LeftStickJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK); }

   /// Return true if the right stick is pressed inward, as a button.
   bool RightStickPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK); }
   bool RightStickJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK); }
   bool RightStickJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK); }

   /// Convienience function that treats pressing up on the left stick as though
   /// it were a button press.  Different combinations of these "buttons" can
   /// appear "pressed" to denote diagonals.
   bool LeftStickUpPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_UP); }
   bool LeftStickUpJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_UP); }
   bool LeftStickUpJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_UP); }

   bool LeftStickDownPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_DOWN); }
   bool LeftStickDownJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_DOWN); }
   bool LeftStickDownJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_DOWN); }

   bool LeftStickLeftPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_LEFT); }
   bool LeftStickLeftJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_LEFT); }
   bool LeftStickLeftJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_LEFT); }

   bool LeftStickRightPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_RIGHT); }
   bool LeftStickRightJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_RIGHT); }
   bool LeftStickRightJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_LEFT_STICK_RIGHT); }

   bool RightStickUpPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_UP); }
   bool RightStickUpJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_UP); }
   bool RightStickUpJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_UP); }

   bool RightStickDownPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_DOWN); }
   bool RightStickDownJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_DOWN); }
   bool RightStickDownJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_DOWN); }

   bool RightStickLeftPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_LEFT); }
   bool RightStickLeftJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_LEFT); }
   bool RightStickLeftJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_LEFT); }

   bool RightStickRightPressed() const { return ButtonPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_RIGHT); }
   bool RightStickRightJustPressed() const { return ButtonJustPressed(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_RIGHT); }
   bool RightStickRightJustReleased() const { return ButtonJustReleased(GAMEPAD_TYPICAL_BUTTON_RIGHT_STICK_RIGHT); }

   /// True if pressing up on either the d-pad or the left stick.  If you press
   /// up on one and down on the other, it should return false.
   bool UpPressed() const;
   bool UpJustPressed() const;
   bool UpJustReleased() const;

   bool DownPressed() const;
   bool DownJustPressed() const;
   bool DownJustReleased() const;

   bool LeftPressed() const;
   bool LeftJustPressed() const;
   bool LeftJustReleased() const;

   bool RightPressed() const;
   bool RightJustPressed() const;
   bool RightJustReleased() const;

   /// Return a platform-independent string representation of the given
   /// button code.  Return NULL if unsuccessful.
   static const char* ButtonCodeToString(int buttonCode);
   /// Return the button code that correspends to the given platform-independent
   /// string.  Return -1 if unsuccessful.
   static int ButtonStringToCode(const char* buttonString);

   #if PLATFORM_IS_WINDOWS || PLATFORM_IS_ANDROID
      friend class AndroidGamepadCommon;

      /// If this GamepadTypical represents an AndroidGamepad, this points to it.
      /// Otherwise, it should be NULL.
      AndroidGamepad* androidGamepad;
   #endif

   #if PLATFORM_IS_LINUX || PLATFORM_IS_EMSCRIPTEN
      friend class SDL2GameController;

      /// If this GamepadTypical represents an SDL2GameController, this points to it.
      /// Otherwise, it should be NULL.
      SDL2GameController* sdl2GameController;
   #endif

   #if PLATFORM_IS_WINDOWS || PLATFORM_IS_XBOX360
      friend class Xbox360GamepadCommon;

      /// If this GamepadTypical represents an XBox360Gamepad, this points to it.
      /// Otherwise, it should be NULL.
      Xbox360Gamepad* xbox360Gamepad;
   #endif

   #if PLATFORM_IS_WINDOWS
      friend class XInputGamepadCommon;

      /// If this GamepadTypical represents an XInputGamepad, this points to it.
      /// Otherwise, it should be NULL.
      XInputGamepad* xInputGamepad;
   #endif

protected:
   /// True if the gamepad is currently connected.
   bool connected;
   /// Value of 'connectedPrev' from the previous frame.
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

enum
{
   /// Maximum number of GamepadTypical objects in theGamepadTypicals.
   GAMEPAD_TYPICAL_COUNT_MAX = 8
};

/// Pointer to the main GamepadTypical.
extern GamepadTypical* const theGamepadTypical;
/// Pointers to the GamepadTypical objects which represent individual physical
/// gamepads.
extern GamepadTypical* const theGamepadTypicals[GAMEPAD_TYPICAL_COUNT_MAX];

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GAMEPADTYPICAL_H__

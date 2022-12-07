#ifndef __FROG__GAMEPADGENERIC_H__
#define __FROG__GAMEPADGENERIC_H__

#include "FrogMemory.h"
#include "Table.h"
#include <windows.h>

namespace Webfoot {

class GamepadGenericManager;

//==============================================================================

/// Directions for the POV hat switches of a GamepadGeneric.
enum GamepadGenericHatDirection
{
   GAMEPAD_GENERIC_HAT_UP = 1,
   GAMEPAD_GENERIC_HAT_DOWN = 2,
   GAMEPAD_GENERIC_HAT_LEFT = 4,
   GAMEPAD_GENERIC_HAT_RIGHT = 8
};

//==============================================================================

/// Interface for gamepads.  This is intended for debugging in builds like
/// 3DSPC, WiiUPC, etc.
class GamepadGeneric
{
public:
   GamepadGeneric();

   /// Initialize the object for a gamepad with the given features.
   void Init(int _axisCount, int _buttonCount, int _hatCount, int _nativeDeviceIndex,
      const void* _joycaps, HeapID _heapID = HEAP_DEFAULT);
   void Deinit();

   /// Return the number of axes on this gamepad. 
   int AxisCountGet() { return connected ? axisValues.SizeGet() : 0; }
   /// Return the current value for the given axis. [0, 1]
   float AxisValueGet(int axisIndex) { return connected ? axisValues[axisIndex] : 0.0f; }
   /// Return the value for the given axis when the object was initialized.
   float AxisValueInitialGet(int axisIndex) { return axisValuesInitial[axisIndex]; }

   /// Return the number of buttons on this gamepad.
   int ButtonCountGet() { return connected ? buttonStates.SizeGet() : 0; }
   /// Return true if the given button is currently pressed.
   bool ButtonPressed(int buttonIndex) { return connected && buttonStates[buttonIndex]; }
   /// Return true if the given button was just pressed on this update.
   bool ButtonJustPressed(int buttonIndex) { return connected && buttonStates[buttonIndex] && !buttonStatesPrev[buttonIndex]; }
   /// Return true if the given button was just released on this update.
   bool ButtonJustReleased(int buttonIndex) { return connected && !buttonStates[buttonIndex] && buttonStatesPrev[buttonIndex]; }
   
   /// Return the number of POV hat switches on this gamepad.
   int HatCountGet() { return hatStates.SizeGet(); }
   /// Return true if the given POV hat switch is currently pressed in the given
   /// direction. 
   bool HatPressed(int hatIndex, int hatDirection) { return connected && (hatStates[hatIndex] & hatDirection) != 0; }
   /// Return true if the given POV hat switch just started pointing in the
   /// given direction on this update. 
   bool HatJustPressed(int hatIndex, int hatDirection) { return connected && ((hatStates[hatIndex] & hatDirection) != 0) && ((hatStatesPrev[hatIndex] & hatDirection) == 0); } 
   /// Return true if the given POV hat switch just stopped pointing in the
   /// given direction on this update. 
   bool HatJustReleased(int hatIndex, int hatDirection) { return connected && ((hatStates[hatIndex] & hatDirection) == 0) && ((hatStatesPrev[hatIndex] & hatDirection) != 0); }

   /// Return a platform-independent string representation of the given
   /// POV hat switch direction.  Return NULL if unsuccessful.
   static const char* HatCodeToString(int hatCode);
   /// Return the POV hat switch direction that correspends to the given
   /// platform-independent string.  Return -1 if unsuccessful.
   static int HatStringToCode(const char* hatString);
   
   /// Return true if the gamepad is connected.
   bool ConnectedCheck() { return connected; }
   
protected:
   /// True if the gamepad is connected.
   bool connected;
   
   /// States of the buttons on the previous frame.
   Table<bool> buttonStatesPrev;
   /// States of the buttons on the current frame.
   Table<bool> buttonStates;
   
   /// States of the POV hat switches on the previous frame.
   Table<int> hatStatesPrev;
   /// States of the POV hat switches on the current frame.
   Table<int> hatStates;
   
   /// Current values of the individual axes. 
   Table<float> axisValues;
   /// Values of the individual axes when the object was initialized.
   Table<float> axisValuesInitial;
   
   /// Native object for the capabilities of the gamepad.
   void* joycaps;
   /// Index to use when querying the system about this gamepad.
   int nativeDeviceIndex;
   
   friend class GamepadGenericManager;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GAMEPADGENERIC_H__

#ifndef __FROG__INPUTDELEGATESELECTOR_H__
#define __FROG__INPUTDELEGATESELECTOR_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

class InputDelegateButton;
class InputDelegateAxis;
struct InputDelegateSelectorGamepadGenericAxisHelper;
struct InputDelegateSelectorXInputGamepadAxisHelper;
class GamepadGeneric;
class XInputGamepad;

//==============================================================================

/// Utility for creating InputDelegates based on user input.  When setting up
/// a series of controls, begin by calling Init.  Call Update on every frame. To
/// begin testing for an input for something specific, call TestInit.  Call
/// one or more of the InputDelegate creation methods each frame to see if the
/// user is trying to select something.  When done testing for something
/// specific call TestDeinit.  Call Deinit when done setting up controls.
class InputDelegateSelector
{
public:
   InputDelegateSelector();

   /// Begin a session of input selection.
   void Init();
   /// End a session of input selection.
   void Deinit();

   /// Call this once per frame before calling the InputDelegate creation
   /// methods.
   void Update();

   /// Begin testing for the selection of something specific.
   void TestInit();
   /// Finish testing for something specific.  This should be called regardless
   /// of whether an InputDelegate was created.
   void TestDeinit();

   /// If trying to get an InputDelegateButton, try calling this on every
   /// frame. Set 'allowAxes' to false to prevent the usage of a GamepadGeneric
   /// axis as a button.  A returned delegate must be deinited and deleted.
   InputDelegateButton* ButtonCreate(bool allowMouseButtons = true,
      bool allowAxes = true, HeapID heapID = HEAP_DEFAULT);

   /// If trying to get an InputDelegateAxis, try calling this on every
   /// frame.  A returned delegate must be deinited and deleted.
   /// By default, this will be treat the input as if intended to be positive.
   /// Set 'invert' to true if the intended direction is negative.
   InputDelegateAxis* AxisCreate(bool invert = false, HeapID heapID = HEAP_DEFAULT);

protected:
   /// Sometimes used to remember the last button delegate to be created until
   /// it's no longer pressed.
   InputDelegateButton* lastButtonDelegate;

   /// Collection for tracking the axes of GamepadGenerics.
   Table<InputDelegateSelectorGamepadGenericAxisHelper*> gamepadGenericAxisHelpers;
   /// Collection for tracking the axes of XInputGamepads.
   Table<InputDelegateSelectorXInputGamepadAxisHelper*> xInputGamepadAxisHelpers;
};

//==============================================================================

/// Helper object to InputDelegateSelector for testing whether an axis of a
/// GamepadGeneric should be chosen for something specific.
struct InputDelegateSelectorGamepadGenericAxisHelper
{
   /// Gamepad to which the axis belongs.
   GamepadGeneric* gamepad;
   /// Axis to track.
   int axisIndex;
   // To avoid false-positives from axes with no springs, like throttles,
   // ignore axes that haven't been close to their initial position since
   // the test began.
   bool ignoringUntilNearInitialPosition;
};

//==============================================================================

/// Helper object to InputDelegateSelector for testing whether an axis of a
/// XInputGamepad should be chosen for something specific.
struct InputDelegateSelectorXInputGamepadAxisHelper
{
   /// Gamepad to which the axis belongs.
   XInputGamepad* gamepad;
   /// Axis to track.
   int axis;
   // To avoid false-positives from axes with no springs, like throttles,
   // ignore axes that haven't been close to their initial position since
   // the test began.
   bool ignoringUntilNearInitialPosition;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__INPUTDELEGATESELECTOR_H__

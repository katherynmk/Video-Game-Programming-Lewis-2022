#ifndef __FROG__GAMEPADGENERICMANAGER_H__
#define __FROG__GAMEPADGENERICMANAGER_H__

#include "FrogMemory.h"
#include "GamepadGeneric.h"

namespace Webfoot {

//==============================================================================

/// Handles a set of gamepads for Windows.
class GamepadGenericManager
{
public:
   GamepadGenericManager();
   
   void Init(HeapID heapID = HEAP_DEFAULT);
   void Deinit();
   
   /// Update the associated gamepad objects.
   void Update(bool hasFocus);
   
   enum
   {
      /// Maximium number of separate gamepads to track.
      GAMEPAD_COUNT_MAX = 8
   };
   
   /// Return the maximium number of separate gamepads to track.
   int GamepadCountMaxGet() { return GAMEPAD_COUNT_MAX; }
   
   /// Singleton instance
   static GamepadGenericManager instance;
   /// Separate gamepad objects for the individual physical gamepads.
   static GamepadGeneric gamepads[GAMEPAD_COUNT_MAX];
   /// Pointers to the individual elements of 'gamepads' so that
   /// 'theGamepadGenerics' can be an array of pointers.
   static GamepadGeneric* gamepadPointers[GAMEPAD_COUNT_MAX];
   
protected:
   /// Helper function for updating the gamepads.  Set 'axesOnly' to true if
   /// only the axes should be updated.
   void UpdateHelper(bool axesOnly);

   /// Helper function for getting the value for an axis.
   float AxisHelper(DWORD nativeValue, UINT nativeMin, UINT nativeMax);
};

/// Pointer the the singleton instance.
GamepadGenericManager* const theGamepadGenericManager = &GamepadGenericManager::instance;
/// Separate GamepadGeneric objects for the individual physical gamepads.
GamepadGeneric** const theGamepadGenerics = (GamepadGeneric**)(GamepadGenericManager::gamepadPointers);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GAMEPADGENERICMANAGER_H__

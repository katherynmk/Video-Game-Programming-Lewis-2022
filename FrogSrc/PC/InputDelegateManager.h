#ifndef __FROG__INPUTDELEGATEMANAGER_H__
#define __FROG__INPUTDELEGATEMANAGER_H__

#include "FrogMemory.h"

namespace Webfoot {

class InputDelegateConnected;
class InputDelegateButton;
class InputDelegateAxis;
class InputDelegateJoystick;
class JSONValue;

//==============================================================================

/// Handles tasks involving InputDelegates.
class InputDelegateManager
{
public:
   void Init();
   void Deinit();

   /// Use the given JSON specifications to create a new InputDelegateConnected.
   /// Deinit and delete the object when you're done with it.
   InputDelegateConnected* InputDelegateConnectedCreate(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);

   /// Use the given JSON specifications to create a new InputDelegateButton.
   /// Deinit and delete the object when you're done with it.
   InputDelegateButton* InputDelegateButtonCreate(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);

   /// Use the given JSON specifications to create a new InputDelegateAxis.
   /// Deinit and delete the object when you're done with it.
   InputDelegateAxis* InputDelegateAxisCreate(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);

   /// Use the given JSON specifications to create a new InputDelegateJoystick.
   /// Deinit and delete the object when you're done with it.
   InputDelegateJoystick* InputDelegateJoystickCreate(JSONValue* specifications, HeapID heapID = HEAP_DEFAULT);

   static InputDelegateManager instance;
};

InputDelegateManager* const theInputDelegateManager = &InputDelegateManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__INPUTDELEGATEMANAGER_H__

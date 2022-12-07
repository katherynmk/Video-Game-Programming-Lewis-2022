#ifndef __FROG__GUIKEYPADGAMEPADTYPICAL_H__
#define __FROG__GUIKEYPADGAMEPADTYPICAL_H__

#include "FrogMemory.h"
#include "GUIManager.h"
#include "GamepadTypical.h"

namespace Webfoot {

//==============================================================================

/// Implementation of GUIKeypad for abstracting the details of GamepadTypical.
class GUIKeypadGamepadTypical : public GUIKeypad
{
public:
   typedef GUIKeypad Inherited;

   /// Convenience function for setting up a default configuration of
   /// GUIKeypadGamepadTypical in GUIManager.
   static void GUIManagerSetup()
   {
      for(int gamepadIndex = 0; gamepadIndex < GAMEPAD_TYPICAL_COUNT_MAX; gamepadIndex++)
      {
         GUIKeypadGamepadTypical* guiKeypad = frog_new GUIKeypadGamepadTypical();
         guiKeypad->Init(theGamepadTypicals[gamepadIndex]);
         theGUI->GUIKeypadAdd(guiKeypad);
      }
   }

   GUIKeypadGamepadTypical() { gamepad = NULL; }
   virtual ~GUIKeypadGamepadTypical() {}

   /// Specifiy the gamepad which this object should check for input.
   void Init(GamepadTypical* _gamepad) { gamepad = _gamepad; Inherited::Init(); }
   virtual void Deinit() { gamepad = NULL; Inherited::Deinit(); }

   virtual bool ConnectedCheck() { return gamepad && gamepad->ConnectedCheck(); }

   virtual bool AffirmativePressed() { return gamepad && gamepad->AffirmativePressed(); }
   virtual bool NegativePressed() { return gamepad && gamepad->NegativePressed(); }
   virtual bool UpPressed() { return gamepad && gamepad->UpPressed(); }
   virtual bool DownPressed() { return gamepad && gamepad->DownPressed(); }
   virtual bool LeftPressed() { return gamepad && gamepad->LeftPressed(); }
   virtual bool RightPressed() { return gamepad && gamepad->RightPressed(); }

   /// Return the gamepad which this object should check for input.
   GamepadTypical* GamepadGet() { return gamepad; }

protected:
   /// Gamepad which this object should check for input.
   GamepadTypical* gamepad;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUIKEYPADGAMEPADTYPICAL_H__

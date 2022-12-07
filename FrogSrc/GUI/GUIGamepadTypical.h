#ifndef __FROG__GUIGAMEPADTYPICAL_H__
#define __FROG__GUIGAMEPADTYPICAL_H__

#include "FrogMemory.h"
#include "GUIManager.h"
#include "GamepadTypical.h"

namespace Webfoot {

//==============================================================================

/// Implementation of GUIGamepad for abstracting the details of GamepadTypical.
class GUIGamepadTypical : public GUIGamepad
{
public:
   typedef GUIGamepad Inherited;

   /// Convenience function for setting up a default configuration of
   /// GUIGamepadTypical in GUIManager.
   static void GUIManagerSetup()
   {
      for(int gamepadIndex = 0; gamepadIndex < GAMEPAD_TYPICAL_COUNT_MAX; gamepadIndex++)
      {
         GUIGamepadTypical* guiGamepad = frog_new GUIGamepadTypical();
         guiGamepad->Init(theGamepadTypicals[gamepadIndex]);
         theGUI->GUIGamepadAdd(guiGamepad);
      }
   }

   GUIGamepadTypical() { gamepad = NULL; }
   virtual ~GUIGamepadTypical() {}

   /// Specifiy the gamepad which this object should check for input.
   void Init(GamepadTypical* _gamepad) { gamepad = _gamepad; Inherited::Init(); }
   virtual void Deinit() { gamepad = NULL; Inherited::Deinit(); }

   /// Return true if the gamepad's button for affirming a selection is
   /// currently pressed.
   virtual bool AffirmativePressed() { return gamepad && gamepad->AffirmativePressed(); }
   /// Return true if the gamepad's button for indicating negative is currently
   /// pressed.
   virtual bool NegativePressed() { return gamepad && gamepad->NegativePressed(); }
   /// Return true if the gamepad is indicating a direction of up.
   virtual bool UpPressed() { return gamepad && gamepad->UpPressed(); }
   /// Return true if the gamepad is indicating a direction of down.
   virtual bool DownPressed() { return gamepad && gamepad->DownPressed(); }
   /// Return true if the gamepad is indicating a direction of left.
   virtual bool LeftPressed() { return gamepad && gamepad->LeftPressed(); }
   /// Return true if the gamepad is indicating a direction of right.
   virtual bool RightPressed() { return gamepad && gamepad->RightPressed(); }

   /// Return the gamepad which this object should check for input.
   GamepadTypical* GamepadGet() { return gamepad; }

protected:
   /// Gamepad which this object should check for input.
   GamepadTypical* gamepad;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUIGAMEPADTYPICAL_H__

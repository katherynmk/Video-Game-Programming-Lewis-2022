#ifndef __FROG__GUIKEYPADKEYBOARD_H__
#define __FROG__GUIKEYPADKEYBOARD_H__

#include "FrogMemory.h"
#include "GUIManager.h"
#include "KeyboardManager.h"

namespace Webfoot {

//==============================================================================

/// GUIKeypadKeyboard is a type of GUIKeypad for abstracting a Keyboard.
class GUIKeypadKeyboard : public GUIKeypad
{
public:
   typedef GUIKeypad Inherited;

   static void GUIManagerSetup()
   {
      GUIKeypadKeyboard* guiKeypadKeyboard = frog_new GUIKeypadKeyboard();
      guiKeypadKeyboard->Init(theKeyboard);
      theGUI->GUIKeypadAdd(guiKeypadKeyboard);
   }

   GUIKeypadKeyboard() { keyboard = NULL; }
   virtual ~GUIKeypadKeyboard() {}

   void Init(Keyboard* _keyboard) { keyboard = _keyboard; Inherited::Init(); }
   virtual void Deinit() { keyboard = NULL; Inherited::Deinit(); }

   virtual bool ConnectedCheck() { return keyboard && keyboard->ConnectedCheck(); }
   virtual bool AffirmativePressed() { return keyboard && keyboard->KeyPressed(KEY_ENTER) && !keyboard->KeyPressed(KEY_LALT) && !keyboard->KeyPressed(KEY_RALT); }
   virtual bool NegativePressed() { return keyboard && keyboard->KeyPressed(KEY_ESCAPE); }
   virtual bool UpPressed() { return keyboard && keyboard->KeyPressed(KEY_UP); }
   virtual bool DownPressed() { return keyboard && keyboard->KeyPressed(KEY_DOWN); }
   virtual bool LeftPressed() { return keyboard && keyboard->KeyPressed(KEY_LEFT); }
   virtual bool RightPressed() { return keyboard && keyboard->KeyPressed(KEY_RIGHT); }

   /// Return the keyboard which this object should check for input.
   Keyboard* KeyboardGet() { return keyboard; }

protected:
   /// Keyboard which this object should check for input.
   Keyboard* keyboard;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUIKEYPADKEYBOARD_H__

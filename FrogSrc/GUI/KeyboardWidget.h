#ifndef __FROG__GUI__KEYBOARDWIDGET_H__
#define __FROG__GUI__KEYBOARDWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"
#include "GUI/PressButtonWidget.h"
#include "GUI/TextEntryWidget.h"
#include "GUI/StateContainerWidget.h"

namespace Webfoot {

class LayerWidget;

//=============================================================================

/// KeyboardWidget displays a keyboard on screen that a user can use to
/// simualte a real keyboard.
class KeyboardWidget : public ContainerWidget
{
public:
   KeyboardWidget();
   virtual ~KeyboardWidget() {}

   /// Call this or the other Init immediately after construction.
   /// Initialize a new ParticleEffectWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   /// Clean up the widget.
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();
   /// Return the text to be displayed by this widget.
   const char* TextGet();
   /// Seet the text to be displayed by this widget.
   void TextSet(const char* _text);

   /// Set the maximum number of characters that can be typed.
   virtual void TextLengthMaxSet(int _maxTextLength);

   /// Type of the function that can receive the OnEnterClick event.  The
   /// KeyboardWidget* is the keyboard widget to which the enter key belongs,
   /// the PressButtonWidget* is the button that was clicked, and the void* is
   /// the userData pointer.
   typedef void (*OnClickCallback)(KeyboardWidget*, PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterClick event occurs.
   /// For now, only one function can be registered at a time.
   void OnEnterClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnEnterClick event occurs.
   void OnEnterClickUnregister(OnClickCallback callback);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnCloseClick event occurs.
   /// For now, only one function can be registered at a time.
   void OnCloseClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnCloseClick event occurs.
   void OnCloseClickUnregister(OnClickCallback callback);

   /// Type of the function that can receive OnPress events for physical
   /// keyboard key presses.  The KeyboardWidget* is the widget with the
   /// TextEntryWidget that received the press of a key on a physical keyboard.
   typedef void (*OnPhysicalKeyPressCallback)(KeyboardWidget*, void*);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterPhysicalKeyPress event occurs.
   /// For now, only one function can be registered at a time.
   void OnEnterPhysicalKeyPressRegister(OnPhysicalKeyPressCallback callback, void* userData);
   /// Stop calling the given function when the OnEnterPhysicalKeyPress event occurs.
   void OnEnterPhysicalKeyPressUnregister(OnPhysicalKeyPressCallback callback);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnTabPhysicalKeyPress event occurs.
   /// For now, only one function can be registered at a time.
   void OnTabPhysicalKeyPressRegister(OnPhysicalKeyPressCallback callback, void* userData);
   /// Stop calling the given function when the OnTabPhysicalKeyPress event occurs.
   void OnTabPhysicalKeyPressUnregister(OnPhysicalKeyPressCallback callback);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) KeyboardWidget(); }

protected:
   /// Sets of keyboard buttons.
   enum KeySet
   {
      // Uppercase Latin letters.
      KEY_SET_UPPERCASE,
      /// Lowercase Latin letters.
      KEY_SET_LOWERCASE,
      /// Digits, punctuation, etc.
      KEY_SET_SPECIAL_CHARACTERS
   };

   /// Set which key set is showing.
   void KeySetSet(KeySet _keySet);
   /// Return which key set is showing.
   KeySet KeySetGet() { return keySet; }

   /// Loop through a given container widget and set up callbacks.
   void SetCallbacks(ContainerWidget* container);
   /// Return true if the button adds text to the string.
   bool IsATextButton(PressButtonWidget* button);

   /// Text addition button callbacks.
   static void OnKeyClick(PressButtonWidget*, void*);
   /// Space key callback.
   static void OnSpaceClick(PressButtonWidget*, void*);
   /// Backspace key callback.
   static void OnBackspaceClick(PressButtonWidget*, void*);
   /// Clear key callback.
   static void OnClearClick(PressButtonWidget*, void*);
   /// CapsLock key callback.
   static void OnCapsLockClick(PressButtonWidget*, void*);
   /// Special Characters Show key callback.
   static void OnSpecialCharactersShowClick(PressButtonWidget*, void*);
   /// Letters Show key callback.
   static void OnLettersShowClick(PressButtonWidget*, void*);
   /// Enter key callback.
   static void OnEnterClick(PressButtonWidget* button, void* userData);
   /// Close button callback.
   static void OnCloseClick(PressButtonWidget* button, void* userData);
   /// Called when an enter key on a physical keyboard is pressed.
   static void OnEnterPhysicalKeyPress(TextEntryWidget*, void* userData);
   /// Called when a tab key on a physical keyboard is pressed.
   static void OnTabPhysicalKeyPress(TextEntryWidget*, void* userData);

   /// Current key set.
   KeySet keySet;
   /// Which was used more recently, uppercase or lowercase.
   KeySet mostRecentUppercaseLowercase;
   /// Key a hold on this widgets text box.
   TextEntryWidget* textBox;
   /// The state container for upper and lower case.
   StateContainerWidget* caseStateContainer;
   /// Function to be called for the OnEnterClick event.
   OnClickCallback onEnterClickCallback;
   /// Pointer to be passed to the OnEnterClick callback.
   void* onEnterClickUserData;
   /// Function to be called for the OnCloseClick event.
   OnClickCallback onCloseClickCallback;
   /// Pointer to be passed to the OnCloseClick callback.
   void* onCloseClickUserData;
   /// Function to be called for the OnEnterPhysicalKeyPress event.
   OnPhysicalKeyPressCallback onEnterPhysicalKeyPressCallback;
   /// Pointer to be passed to the OnEnterPhysicalKeyPress callback.
   void* onEnterPhysicalKeyPressUserData;
   /// Function to be called for the OnTabPhysicalKeyPress event.
   OnPhysicalKeyPressCallback onTabPhysicalKeyPressCallback;
   /// Pointer to be passed to the OnTabPhysicalKeyPress callback.
   void* onTabPhysicalKeyPressUserData;

   typedef ContainerWidget Inherited;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__KEYBOARDWIDGET_H__

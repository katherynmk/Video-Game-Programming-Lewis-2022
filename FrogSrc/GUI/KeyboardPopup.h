#ifndef __FROG__GUI__KEYBOARDPOPUP_H__
#define __FROG__GUI__KEYBOARDPOPUP_H__

#include "FrogMemory.h"
#include "GUI/Popup.h"
#include "Utility.h"

namespace Webfoot {

class PressButtonWidget;
class KeyboardWidget;

/// Default name for the GUI layer used to display a KeyboardPopup.
#define KEYBOARD_POPUP_GUI_LAYER_NAME_DEFAULT "Keyboard"
/// Expected path of the KeyboardWidget relative to the GUI layer widget of the
/// keyboard popup.
#define KEYBOARD_POPUP_KEYBOARD_PATH_DEFAULT "MenuPanel.Keyboard"
/// Expected path of the title widget relative to the GUI layer widget of the
/// keyboard popup.
#define KEYBOARD_POPUP_TITLE_PATH_DEFAULT "MenuPanel.Title"

//==============================================================================

/// KeyboardPopup is a helper Popup for displaying a KeyboardWidget.  It is set
/// up to handle the clicking of buttons, which are assumed to be using specific
/// paths relative to the LayerWidget.
class KeyboardPopup : public Popup
{
public:
   /// Convenience function for showing a message box using typical settings.
   static KeyboardPopup* Push(const char* _initialText = NULL,
      const char* _guiLayerName = KEYBOARD_POPUP_GUI_LAYER_NAME_DEFAULT,
      const char* _initTransitionName = POPUP_INIT_TRANSITION_NAME_DEFAULT,
      const char* _deinitTransitionName = POPUP_DEINIT_TRANSITION_NAME_DEFAULT);

   KeyboardPopup();
   virtual void Init();
   virtual void OnGUILayerInit(LayerWidget* _layerWidget);
   virtual void Deinit();

   /// Type of the function that can receive the OnClick events.
   /// The KeyboardPopup* is the popup that contains the button, the
   /// PressButtonWidget* is the button that was clicked, and the void* is the
   /// userData pointer.
   typedef void (*OnClickCallback)(KeyboardPopup*, PressButtonWidget*, void*);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnEnterClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnCloseClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnCloseClickRegister(OnClickCallback callback, void* userData = NULL);

   /// Type of the function that can receive OnPress events for physical
   /// keyboard key presses.  The KeyboardPopup* is the popup with the
   /// TextEntryWidget that received the press of a key on a physical keyboard.
   typedef void (*OnPhysicalKeyPressCallback)(KeyboardPopup*, void*);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterPhysicalKeyPress event occurs.
   /// For now, only one function can be registered at a time.
   void OnEnterPhysicalKeyPressRegister(OnPhysicalKeyPressCallback callback, void* userData = NULL);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnTabPhysicalKeyPress event occurs.
   /// For now, only one function can be registered at a time.
   void OnTabPhysicalKeyPressRegister(OnPhysicalKeyPressCallback callback, void* userData = NULL);

   /// Return the currently entered text.
   const char* TextGet();
   /// Set the currently entered text.
   void TextSet(const char* _text);

   /// Set the title text to be displayed.
   void TitleTextSet(const char* _titleText, TextType _titleTextType);

   /// Set the maximum number of characters that can be typed.
   void TextLengthMaxSet(int _textLengthMax);

   typedef Popup Inherited;

protected:
   /// Update the widget used to display the title text.
   void TitleTextWidgetUpdate();
   
   /// Called when an enter button is clicked.
   static void OnEnterClick(KeyboardWidget*, PressButtonWidget* button, void* userData);
   /// Called when a close button is clicked.
   static void OnCloseClick(KeyboardWidget*, PressButtonWidget* button, void* userData);
   /// Called when an enter key is pressed on a physical keyboard.
   static void OnEnterPhysicalKeyPress(KeyboardWidget*, void* userData);
   /// Called when a tab key is pressed on a physical keyboard.
   static void OnTabPhysicalKeyPress(KeyboardWidget*, void* userData);

   /// String that controls the title text to be displayed.
   const char* titleText;
   /// Determines how 'titleText' is interpreted.
   TextType titleTextType;
   /// See 'textLengthMax'.
   bool useTextLengthMax;
   /// If 'useTextLengthMax' is true, apply this to the KeyboardWidget's
   /// TextEntryWidget.
   int textLengthMax;
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
   /// Pointer to be passed to the OnEnterPhysicalKeyPressCallback callback.
   void* onEnterPhysicalKeyPressUserData;
   /// Function to be called for the OnTabPhysicalKeyPress event.
   OnPhysicalKeyPressCallback onTabPhysicalKeyPressCallback;
   /// Pointer to be passed to the OnTabPhysicalKeyPressCallback callback.
   void* onTabPhysicalKeyPressUserData;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__KEYBOARDPOPUP_H__

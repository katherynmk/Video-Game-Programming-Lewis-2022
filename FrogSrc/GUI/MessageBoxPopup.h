#ifndef __FROG__GUI__MESSAGEBOXPOPUP_H__
#define __FROG__GUI__MESSAGEBOXPOPUP_H__

#include "FrogMemory.h"
#include "GUI/Popup.h"
#include "Utility.h"

namespace Webfoot {

class PressButtonWidget;

/// Default name for the GUI layer used to display a MessageBoxPopup.
#define MESSAGE_BOX_POPUP_GUI_LAYER_NAME_DEFAULT "MessageBoxOK"
/// Expected path of the main TextDocumentWidget relative to the GUI layer
/// widget of the message box.
#define MESSAGE_BOX_POPUP_TEXT_PATH_DEFAULT "Panel.Text"
/// Expected path of OK buttons relative to the GUI layer widget of the message
/// box.
#define MESSAGE_BOX_POPUP_OK_BUTTON_PATH_DEFAULT "Panel.OK"
/// Expected path of Cancel buttons relative to the GUI layer widget of the
/// message box.
#define MESSAGE_BOX_POPUP_CANCEL_BUTTON_PATH_DEFAULT "Panel.Cancel"
/// Expected path of Yes buttons relative to the GUI layer widget of the message
/// box.
#define MESSAGE_BOX_POPUP_YES_BUTTON_PATH_DEFAULT "Panel.Yes"
/// Expected path of No buttons relative to the GUI layer widget of the message
/// box.
#define MESSAGE_BOX_POPUP_NO_BUTTON_PATH_DEFAULT "Panel.No"

//==============================================================================

/// MessageBoxPopup is a type of Popup for displaying a simple message.  It
/// assumes the message will be displayed by a TextDocumentWidget with a
/// specific path relative to the LayerWidget.  It is also set up to handle
/// the clicking of buttons, which are also assumed to be using specific paths
/// relative to the LayerWidget.
class MessageBoxPopup : public Popup
{
public:
   /// Convenience function for showing a message box using typical settings.
   static MessageBoxPopup* Push(const char* _text, TextType _textType = TEXT_KEY,
      const char* _guiLayerName = MESSAGE_BOX_POPUP_GUI_LAYER_NAME_DEFAULT,
      const char* _initTransitionName = POPUP_INIT_TRANSITION_NAME_DEFAULT,
      const char* _deinitTransitionName = POPUP_DEINIT_TRANSITION_NAME_DEFAULT);

   MessageBoxPopup();
   virtual void OnGUILayerInit(LayerWidget* _layerWidget);
   virtual void Deinit();

   /// Set the main text to be displayed in the message box.
   void TextSet(const char* _text, TextType _textType);

   /// Type of the function that can receive the OnClick events.
   /// The MessageBoxPopup* is the popup that contains the button, the
   /// PressButtonWidget* is the button that was clicked, and the void* is the
   /// userData pointer.
   typedef void (*OnClickCallback)(MessageBoxPopup*, PressButtonWidget*, void*);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnOKClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnOKClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnOKClick event occurs.
   void OnOKClickUnregister(OnClickCallback callback);
   /// Return the number of callbacks registered for the OnOKClick event.
   int OnOKClickRegistrationCountGet() { return onOKClickCallback ? 1 : 0; }

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnCancelClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnCancelClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnCancelClick event occurs.
   void OnCancelClickUnregister(OnClickCallback callback);
   /// Return the number of callbacks registered for the OnCancelClick event.
   int OnCancelClickRegistrationCountGet() { return onCancelClickCallback ? 1 : 0; }

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnYesClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnYesClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnYesClick event occurs.
   void OnYesClickUnregister(OnClickCallback callback);
   /// Return the number of callbacks registered for the OnYesClick event.
   int OnYesClickRegistrationCountGet() { return onYesClickCallback ? 1 : 0; }

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnNoClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnNoClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnNoClick event occurs.
   void OnNoClickUnregister(OnClickCallback callback);
   /// Return the number of callbacks registered for the OnNoClick event.
   int OnNoClickRegistrationCountGet() { return onNoClickCallback ? 1 : 0; }

   static void OnOKClick(PressButtonWidget* button, void* userData);
   static void OnCancelClick(PressButtonWidget* button, void* userData);
   static void OnYesClick(PressButtonWidget* button, void* userData);
   static void OnNoClick(PressButtonWidget* button, void* userData);

   typedef Popup Inherited;

protected:
   /// Update the widget used to display the main text of the popup.
   void TextWidgetUpdate();

   /// String that controls the text to be displayed.
   const char* text;
   /// Determines how 'text' is interpreted.
   TextType textType;

   /// Function to be called for the OnOKClick event.
   OnClickCallback onOKClickCallback;
   /// Pointer to be passed to the OnOKClick callback.
   void* onOKClickUserData;
   /// Function to be called for the OnCancelClick event.
   OnClickCallback onCancelClickCallback;
   /// Pointer to be passed to the OnCancelClick callback.
   void* onCancelClickUserData;
   /// Function to be called for the OnYesClick event.
   OnClickCallback onYesClickCallback;
   /// Pointer to be passed to the OnYesClick callback.
   void* onYesClickUserData;
   /// Function to be called for the OnNoClick event.
   OnClickCallback onNoClickCallback;
   /// Pointer to be passed to the OnNoClick callback.
   void* onNoClickUserData;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__MESSAGEBOXPOPUP_H__

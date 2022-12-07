#ifndef __FROG__GUI__FILESELECTORPOPUP_H__
#define __FROG__GUI__FILESELECTORPOPUP_H__

#include "FrogMemory.h"
#include "GUI/Popup.h"
#include "GUI/FileSelectorWidget.h"
#include "Utility.h"

namespace Webfoot {

class PressButtonWidget;
class FileSelectorWidget;

/// Default name for the GUI layer used to display a FileSelectorPopup.
#define FILE_SELECTOR_POPUP_GUI_LAYER_NAME_DEFAULT "MessageBoxOK"
/// Expected path of the FileSelectorWidget relative to the GUI layer widget of
/// the popup.
#define FILE_SELECTOR_POPUP_FILE_SELECTOR_WIDGET_PATH_DEFAULT "Panel.FileSelector"
/// Expected path of the title LabelWidget relative to the GUI layer widget of
/// the popup.
#define FILE_SELECTOR_POPUP_TITLE_PATH_DEFAULT "Panel.Title"
/// Expected path of OK buttons relative to the GUI layer widget of the popup.
#define FILE_SELECTOR_POPUP_OK_BUTTON_PATH_DEFAULT "Panel.OK"
/// Expected path of Cancel buttons relative to the GUI layer widget of the
/// popup.
#define FILE_SELECTOR_POPUP_CANCEL_BUTTON_PATH_DEFAULT "Panel.Cancel"

//==============================================================================

/// FileSelectorPopup is a type of Popup for selecting a file of folder.  It
/// assumes the widgets will have specific types and paths relative to the
/// LayerWidget.
class FileSelectorPopup : public Popup
{
public:
   /// Convenience function for showing a popup using typical settings.
   static FileSelectorPopup* Push(FileSelectorWidget::Scope _scope,
      const char* _scopeSubfolder = NULL,
      const char* _rootFolderText = NULL, TextType _rootFolderTextType = TEXT_KEY,
      JSONValue* _fileTypesSpecs = NULL, FileManager* _fileManager = NULL,
      const char* _guiLayerName = FILE_SELECTOR_POPUP_GUI_LAYER_NAME_DEFAULT,
      const char* _initTransitionName = POPUP_INIT_TRANSITION_NAME_DEFAULT,
      const char* _deinitTransitionName = POPUP_DEINIT_TRANSITION_NAME_DEFAULT);

   FileSelectorPopup();
   virtual void OnGUILayerInit(LayerWidget* _layerWidget);
   virtual void Deinit();

   /// Where to look for files and folders.
   void ScopeSet(FileSelectorWidget::Scope _scope);
   /// Set the subfolder within the scope in which to look in for files and
   /// folders.  Paths will implicitly begin with this and it should not be
   /// included when getting or setting the selection.
   void ScopeSubfolderSet(const char* _scopeSubfolder);
   /// FileManager to use when looking for files and folders.
   void FileManagerSet(FileManager* _fileManager);
   /// Specifications for the different types of files which can be displayed.
   void FileTypesSet(JSONValue* _fileTypesSpecs);
   /// Set the title text to be displayed in the popup.
   void TitleTextSet(const char* _titleText, TextType _tileTextType);
   /// Set the text to label the root of the folders.
   void RootFolderTextSet(const char* _rootFolderText, TextType _rootFolderTextType);
   /// True if the selected file or folder must exist to finish the selection.
   void SelectionMustExistSet(bool _selectionMustExist) { selectionMustExist = _selectionMustExist; }
   /// Set the text and GUI layer name for the MessageBoxPopup to show if the
   /// selected file or folder doesn't exist.
   void SelectionNotFoundTextSet(const char* _selectionNotFoundText,
      TextType _selectionNotFoundTextType, const char* _selectionNotFoundGUILayerName);
   /// Select the given path.
   void SelectionSet(const char* path, const char* implicitExtension = NULL);
   /// Refresh the displayed set of files and folders.
   void Refresh();

   /// Return the widget for choosing a file or folder.
   FileSelectorWidget* FileSelectorWidgetGet() { return fileSelectorWidget; }

   /// Type of the function that can receive the OnClick events.
   /// The FileSelectorPopup* is the popup that contains the button, the
   /// PressButtonWidget* is the button that was clicked, and the void* is the
   /// userData pointer.
   typedef void (*OnClickCallback)(FileSelectorPopup*, PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnCancelClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnCancelClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnCancelClick event occurs.
   void OnCancelClickUnregister(OnClickCallback callback);

   /// Type of the function that can receive the OnSelectionFinish event.
   /// The FileSelectorPopup* is the popup, the FileSelectorWidget* is the
   /// source of the event, and the void* is the userData context pointer.
   /// Return true if the popup should close.
   typedef bool (*OnSelectionFinishCallback)(FileSelectorPopup*, FileSelectorWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelectionFinish event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionFinishRegister(OnSelectionFinishCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelectionFinish event occurs.
   void OnSelectionFinishUnregister(OnSelectionFinishCallback callback);

   typedef Popup Inherited;

protected:
   /// Called when the user has indicated that the current selection is final.
   /// Return true if the popup should close.
   virtual bool OnSelectionFinish();

   /// Update the widget used to display the title text of the popup.
   void TitleWidgetUpdate();

   static void OnOKClick(PressButtonWidget* button, void* userData);
   static void OnCancelClick(PressButtonWidget* button, void* userData);
   static void OnSelectionFinish(FileSelectorWidget* fileSelectorWidget, void* userData);

   /// Widget for choosing a file or folder.
   FileSelectorWidget* fileSelectorWidget;
   /// Where to look for files and folders.
   FileSelectorWidget::Scope scope;
   /// Folder within the scope in which to look for files and folders.
   const char* scopeSubfolder;
   /// FileManager to use when looking for files and folders.
   FileManager* fileManager;
   /// Specifications for the different types of files which can be displayed.
   JSONValue* fileTypesSpecs;
   /// String that controls the text to be displayed for the title.
   const char* titleText;
   /// Determines how 'titleText' is interpreted.
   TextType titleTextType;
   /// String that controls the text to label the root of the folders.
   const char* rootFolderText;
   /// Determines how 'rootFolderText' is interpreted.
   TextType rootFolderTextType;
   /// True if the selected file or folder must exist to finish the selection.
   bool selectionMustExist;
   /// Text to display in a MessageBoxPopup if the selected file or folder
   /// doesn't exist.
   const char* selectionNotFoundText;
   /// Determines how 'selectionNotFoundText' is interpreted.
   TextType selectionNotFoundTextType;
   /// GUI layer name for the MessageBoxPopup to display if the selected file
   /// or folder doesn't exist.
   const char* selectionNotFoundGUILayerName;
   /// Function to be called for the OnSelectionFinish event.
   OnSelectionFinishCallback onSelectionFinishCallback;
   /// Pointer to be passed to the OnSelectionFinish callback.
   void* onSelectionFinishUserData;
   /// Function to be called for the OnCancelClick event.
   OnClickCallback onCancelClickCallback;
   /// Pointer to be passed to the OnCancelClick callback.
   void* onCancelClickUserData;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__FILESELECTORPOPUP_H__

#ifndef __FROG__GUI__FILESELECTORWIDGET_H__
#define __FROG__GUI__FILESELECTORWIDGET_H__

#include "FrogMemory.h"
#include "ContainerWidget.h"

namespace Webfoot {

class DropDownListWidget;
class DropDownListItemButtonWidget;
class TableWidget;
class TableRowWidget;
class TextEntryWidget;
class TreeWidget;
class TreeItemWidget;

//==============================================================================

/// Widget for helping the user specify a file or folder.
class FileSelectorWidget : public ContainerWidget
{
public:
   FileSelectorWidget();
   virtual ~FileSelectorWidget() {}

   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();

   virtual const char* WidgetTypeNameGet();

   virtual void TextRefresh();

   /// Set which FileManager to use when looking for files and folders.
   virtual void FileManagerSet(FileManager* _fileManager) { fileManager = _fileManager; }
   
   /// Where to look for files and folders.
   enum Scope
   {
      /// The main file manager for the game.
      SCOPE_GAME,
      /// The graphics paths for the game.
      SCOPE_GAME_GRAPHICS
   };
   
   /// Set where to look for files and folders.
   virtual void ScopeSet(Scope _scope);
   /// Return where to look for files and folders.
   virtual Scope ScopeGet() { return scope; }

   /// Set the subfolder within the scope in which to look for files and
   /// folders.  Paths will implicitly begin with this and it should not be
   /// included when getting or setting the selection.
   virtual void ScopeSubfolderSet(const char* _scopeSubfolder);
   /// Return the subfolder within the scope in which to look for files and
   /// folders.
   virtual const char* ScopeSubfolderGet() { return scopeSubfolder; }

   /// Set the text to label the root of the folders.
   virtual void RootFolderTextSet(const char* _rootFolderText, TextType _rootFolderTextType);

   /// Set the options for the types of files which can be selected.  This
   /// should be a JSON array containing a JSON object for each type of file.
   /// Each of these objects should include a name and a JSON array of file
   /// extensions.  If 'Extensions' is omitted, all extensions are included.
   /// For example...
   /// [
   ///    {
   ///       "TextKey": "FileSelectorFileTypeJPEG",
   ///       "Extensions": [ "jpeg", "jpg" ]
   ///    },
   ///    {
   ///       "TextKey": "FileSelectorFileTypePNG",
   ///       "Extensions": [ "png" ]
   ///    },
   ///    {
   ///       "TextKey": "FileSelectorFileTypeAll"
   ///    }
   /// ]
   virtual void FileTypesSet(JSONValue* _fileTypesSpecs);
   
   /// Get the path of the current folder.  Return true if successful.
   virtual bool FolderPathGet(char* _folderPath, size_t bufferSize);
   
   /// Select the given path.
   virtual void SelectionSet(const char* _path, const char* implicitExtension = NULL);
   /// Get the path of the selected file or folder.  Return true if successful.
   virtual bool SelectionGet(char* _path, size_t bufferSize, bool includeExtension = true);
   /// Return true if the selection exists.  If the folder contents table is
   /// defined, treat the selection as a file.  Otherwise, treat it as a
   /// folder.
   virtual bool SelectionExistsCheck();
   
   /// Refresh the displayed set of files and folders.
   virtual void Refresh();

   /// Called when the user has indicated that the current selection is final.
   virtual void OnSelectionFinish();

   /// Type of the function that can receive the OnSelectionFinish event.
   /// The FileSelectorWidget* is the source of the event, and the void* is the
   /// userData context pointer.
   typedef void (*OnSelectionFinishCallback)(FileSelectorWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelectionFinish event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionFinishRegister(OnSelectionFinishCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelectionFinish event occurs.
   void OnSelectionFinishUnregister(OnSelectionFinishCallback callback);
   /// Convenience function for finding a TableWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionFinishRegister(const char* path, OnSelectionFinishCallback callback, void* userData = NULL);

   /// Type of the function that can receive the ShouldIncludeCheck event.  The
   /// string is the path to include or not, the boolean is whether the item is
   /// a folder, the FileSelectorWidget* is the source of the event, and the
   /// void* is the userData context pointer.  Return true if the item should
   /// be included.
   typedef bool (*ShouldIncludeCheckCallback)(const char*, bool, FileSelectorWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the ShouldIncludeCheck event occurs.
   /// For now, only one function can be registered per instance.
   void ShouldIncludeCheckRegister(ShouldIncludeCheckCallback callback, void* userData = NULL);
   /// Stop calling the given function when the ShouldIncludeCheck event
   /// occurs.
   void ShouldIncludeCheckUnregister(ShouldIncludeCheckCallback callback);

   typedef ContainerWidget Inherited;

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) FileSelectorWidget(); }

protected:
   /// Return true if the given file exists.
   bool FileExistsCheck(const char* _path);
   /// Return true if the given folder exists.
   bool FolderExistsCheck(const char* _path);
   /// If the selected item in the folder contents table is a folder, open it.
   /// Otherwise, indicate that the selection is final.
   void FolderContentsFolderSetOrSelectionFinish();
   /// Set '_folderPath' to the path for the given folder tree item.
   bool FolderTreeItemPathGet(char* _folderPath, size_t bufferSize, TreeItemWidget* folderTreeItem);
   /// Helper function to FolderTreeItemPathGet.
   bool FolderTreeItemPathGetHelper(char* _folderPath, size_t bufferSize, TreeItemWidget* folderTreeItem);
   /// Helper function for loading the children of a folder tree item.
   void FolderTreeItemChildrenLoad(TreeItemWidget* folderItem);
   /// Helper function for loading the necessary parts of the subtree after a
   /// folder item is expanded.
   void FoldersSubtreeExpandHelper(TreeItemWidget* item);
   /// Refresh the table with the contents of the current folder.
   void FolderContentsTableRefresh();
   /// Called when a selection is made in the folder contents table.
   void OnFolderContentsSelection();
   /// Recreate the items in the DropDownList for file type.
   void FileTypeDropDownListRefresh();
   /// Return the specifications for the selected file type.
   JSONValue* FileTypeExtensionsGet();

   /// Called when the selection is changed in the folders tree widget.
   static void OnFolderTreeSelectionChange(TreeItemWidget*,
      TreeItemWidget*, bool userMadeChange, TreeWidget*, void* userData);
   /// Called when a folder tree item is double-clicked.
   static void OnFolderTreeItemDoubleClick(TreeItemWidget* item, TreeWidget*, void*);
   /// Called when a folder tree item is expanded or collapsed.
   static void OnFolderTreeItemExpandedChange(TreeItemWidget* item, bool expanded,
      bool, TreeWidget*, void* userData);
   /// Called when a selection is made in the folder contents table.
   static void _OnFolderContentsSelection(TableRowWidget* selectedRowWidget, TableRowWidget*,
      bool userMadeChange, TableWidget*, void* userData);
   /// Called when a row of the folder contents table is double-clicked.
   static void OnFolderContentsTableRowDoubleClick(TableRowWidget*,
      TableWidget*, void* userData);
   /// Called when the enter key is pressed for the table of folder contents.
   static void OnFolderContentsEnterPress(TableWidget*, void* userData);
   /// Called when the enter key is pressed for the filename TextEntryWidget.
   static void OnFilenameTextEntryEnterPress(TextEntryWidget*, void* userData);
   /// Called when the selection changes in the drop down list of file types.
   static void OnFileTypeDropDownListSelectionChange(DropDownListItemButtonWidget*,
      DropDownListItemButtonWidget*, bool, DropDownListWidget*, void* userData);

   /// Where to look for files and folders.
   Scope scope;
   /// Folder within the scope in which to look for files and folders.
   const char* scopeSubfolder;
   /// FileManager to use when looking for files.
   FileManager* fileManager;
   /// Widget for displaying the tree of folders.
   TreeWidget* folderTree;
   /// Item within the folderTree for the root of the folders.
   TreeItemWidget* folderTreeRootItem;
   /// Widget for displaying the files and folders within the current folder.
   TableWidget* folderContentsTable;
   /// Widget for entering and displaying the filename of the selected file,
   /// excluding the folder path.
   TextEntryWidget* filenameTextEntry;
   /// Widget for displaying and selecting which file types to display for
   /// selection.
   DropDownListWidget* fileTypeDropDownList;
   /// Specifications for the different types of files which can be displayed.
   JSONValue* fileTypesSpecs;
   /// Animation name to use for folder icons.
   const char* folderIconAnimation;
   /// String that controls the text to label the root of the folders.
   const char* rootFolderText;
   /// Determines how 'rootFolderText' is interpreted.
   TextType rootFolderTextType;
   /// Function to be called for the OnSelectionFinish event.
   OnSelectionFinishCallback onSelectionFinishCallback;
   /// Pointer to be passed to the OnSelectionFinish callback.
   void* onSelectionFinishUserData;
   /// Function to be called for the ShouldIncludeCheck event.
   ShouldIncludeCheckCallback shouldIncludeCheckCallback;
   /// Pointer to be passed to the ShouldIncludeCheck callback.
   void* shouldIncludeCheckUserData;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__FILESELECTORWIDGET_H__

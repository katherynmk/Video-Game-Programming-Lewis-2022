#ifndef __FROG__TEXTMANAGER_H__
#define __FROG__TEXTMANAGER_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

class JSONValue;

//==============================================================================

/// Helper structure for TextManager for a given source of text.
struct TextManagerSourceEntry
{
   const char* filename;

   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// File modification time from when the file was last loaded.
      int64 fileModificationTime;
   #endif //#if _DEBUG && PLATFORM_IS_WINDOWS
};

//==============================================================================

/// TextManager is a key/value system for localized text.  Text handled by this
/// system is loaded from language-specific "Text.json" files.  For example,
/// "Text/English/Text.json".  Additional sources can be specified with
/// SourceAdd.
///
/// The text for the current language is loaded when the manager is initialized.
/// It will be reloaded automatically if the language changes.  For now, be sure
/// to initialize this only after the EventManager, ScriptManager, and FileManager
/// are initialized.  It is preferable to initialize the manager after the
/// language is set.
///
/// This is only intended for key/value pairs, so feel free to use ScriptManager,
/// text files, or something else instead, if appropriate.
class TextManager
{
public:
   TextManager();
   void Init();
   void Deinit();
   
   /// Return the string for the given key in the current language.
   const char* Get(const char* key);

   /// Set the aliases for text keys.
   void AliasesSet(JSONValue* _aliases);

   /// Specify an additional JSON file which should be used as a source of
   /// text.  The contents of the file will be merged into the same key-value
   /// set as the contents of Text.json.  'filename' must be relative to the
   /// "Text/[LANGUAGE_NAME]/" folders, and the file must be within those
   /// folders.  Do not include the extension in 'filename'.
   void SourceAdd(const char* filename);

   /// Return the collection of text keys and values for the current language.
   JSONValue* TextCollectionGet() { return textObject; }

   static TextManager instance;
   
protected:
   /// Load the text for the current language.  This is called automatically
   /// when the language changes.
   void OnLanguageChange();
   /// Helper function for using aliases to get the underlying text key given
   /// a requested text key.
   const char* AliasProcess(const char* requestedKey);

   /// Collection of text.
   JSONValue* textObject;
   /// Collection of aliases for text keys.
   JSONValue* aliases;
   /// Collection of sources from which text should be gathered.
   Table<TextManagerSourceEntry> textSources;

   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// Called when the program gains the focus of the OS.
      void OnOSFocusGain();
      /// File modification time from when the main text file was last loaded.
      int64 textFileModificationTime;
   #endif //#if _DEBUG && PLATFORM_IS_WINDOWS
};

TextManager* const theText = &TextManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TEXTMANAGER_H__

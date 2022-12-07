#ifndef __FROG__FONTMANAGER_H__
#define __FROG__FONTMANAGER_H__

#include "FrogMemory.h"
#include "Map.h"
#include "Font.h"
#include "FileManager.h"
#include "FileReferenceCounter.h"

namespace Webfoot {

class JSONValue;

//==============================================================================

/// FontManager is a system for loading and unloading fonts from files.
/// All fonts are assumed to be in the Fonts folder.
class FontManager
{
public:
   FontManager();
   void Init(HeapID _heapID = HEAP_DEFAULT);
   void Deinit();
   
   /// Load and return the given font.  Exclude the extension and the "fonts\\" prefix.
   Font* Load(const char* filename, FileManager* fileManager = theFiles);
   /// Unload the given font.
   void Unload(const char* filename, FileManager* fileManager = theFiles);
   /// Unload the given font.
   void Unload(Font* font);
        
   /// Return the given font if it is already loaded. 
   Font* Get(const char* filename, FileManager* fileManager = theFiles);

   /// Set the aliases for fonts.
   void AliasesSet(JSONValue* _aliases);

   /// Set whether to track requests for characters that are not defined.
   void UndefinedCharactersTrackSet(bool _undefinedCharactersTrack) { undefinedCharactersTrack = _undefinedCharactersTrack; }
   /// Return true if requests for undefined characters should be tracked.
   bool UndefinedCharactersTrackCheck() { return undefinedCharactersTrack; }
   /// Print the list of characters that are undefined that were recorded by
   /// fonts that are currently loaded.  Characters may not appear correctly in
   /// the debug output window, so check the log using a text editor that
   /// supports UTF-8.  Tracking of undefined characters is disabled by
   /// default.
   void UndefinedCharactersDebugPrint();

   static FontManager instance;
   
protected:
   /// Helper function for using aliases to get the underlying font name given
   /// a requested name.
   const char* AliasProcess(const char* requestedName);

   /// Heap to use for this manager's allocations.
   HeapID heapID;
   /// Collection of fonts
   FileReferenceCounter fonts;
   /// True if requests for undefined characters should be tracked.
   bool undefinedCharactersTrack;
   /// Collection of aliases for fonts.
   JSONValue* aliases;
};

/// Singleton instance
static FontManager* const theFonts = &FontManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FONTMANAGER_H__

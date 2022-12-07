#ifndef __FROG__CLIPBOARDMANAGERCOMMON_H__
#define __FROG__CLIPBOARDMANAGERCOMMON_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// Clipboard interface common to all platforms.
class ClipboardManagerCommon
{
public:
   ClipboardManagerCommon();
   
   void Init();
   void Deinit();

   /// Clear the clipboard.
   void Clear();

   /// Return the current text from the clipboard or NULL if there is no text
   /// currently.
   const char* TextGet();
   /// Set the given text to the clipboard.
   void TextSet(const char* _text);

   /// Return true if this is between calls to Init and Deinit.
   bool InitializedCheck() { return initialized; }
   
protected:
   /// True if between calls to Init and Deinit.
   bool initialized;
   /// Text currently in the clipboard.
   const char* text;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CLIPBOARDMANAGERCOMMON_H__

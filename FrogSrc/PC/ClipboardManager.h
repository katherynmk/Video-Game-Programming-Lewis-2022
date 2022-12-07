#ifndef __FROG__CLIPBOARDMANAGER_H__
#define __FROG__CLIPBOARDMANAGER_H__

#include "FrogMemory.h"
#include "ClipboardManagerCommon.h"

namespace Webfoot {

//==============================================================================

/// Native impementation of ClipboardManager.
class ClipboardManager : public ClipboardManagerCommon
{
public:
   /// Clear the clipboard.
   void Clear();

   /// Return the current text from the clipboard or NULL if there is no text
   /// currently.
   const char* TextGet();
   /// Set the given text to the clipboard.
   void TextSet(const char* _text);

   typedef ClipboardManagerCommon Inherited;

   static ClipboardManager instance;
};

ClipboardManager* const theClipboard = &ClipboardManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CLIPBOARDMANAGER_H__

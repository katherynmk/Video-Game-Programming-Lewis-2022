#ifndef __FROG__CLIPBOARDMANAGERDEFAULT_H__
#define __FROG__CLIPBOARDMANAGERDEFAULT_H__

#include "FrogMemory.h"
#include "ClipboardManagerCommon.h"

namespace Webfoot {

//==============================================================================

/// Dummy impementation of ClipboardManager.
class ClipboardManagerDefault : public ClipboardManagerCommon
{
public:
   typedef ClipboardManagerCommon Inherited;

   static ClipboardManagerDefault instance;
};

ClipboardManagerDefault* const theClipboard = &ClipboardManagerDefault::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CLIPBOARDMANAGERDEFAULT_H__

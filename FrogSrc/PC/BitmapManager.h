#ifndef __FROG__BITMAPMANAGER_H__
#define __FROG__BITMAPMANAGER_H__

#include "FrogMemory.h"
#include "Allocator.h"

namespace Webfoot {

/// Select the implementation of BitmapManager to use here.
class BitmapManagerDefault;
typedef BitmapManagerDefault BitmapManager;

} //namespace Webfoot {

#include "BitmapManagerDefault.h" 

#endif //#ifndef __FROG__BITMAPMANAGER_H__

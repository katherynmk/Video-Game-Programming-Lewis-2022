#ifndef __FROG__IMAGEMANAGER_H__
#define __FROG__IMAGEMANAGER_H__

#include "FrogMemory.h"

namespace Webfoot {

/// Select the implementation of ImageManager to use here.
class ImageManagerOpenGL;
typedef ImageManagerOpenGL ImageManager;

} //namespace Webfoot {

#include "ImageManagerOpenGL.h" 

#endif //#ifndef __FROG__IMAGEMANAGER_H__

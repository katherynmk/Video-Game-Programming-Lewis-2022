#ifndef __FROG__SOUNDMANAGER_H__
#define __FROG__SOUNDMANAGER_H__

#include "FrogMemory.h"
#include "SoundManagerOpenAL.h"

namespace Webfoot {

/// Select the implementation of SoundManager to use here.
typedef SoundManagerOpenAL SoundManager;

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDMANAGER_H__

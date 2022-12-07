#ifndef __FROG__SOUNDSOURCEOPENAL_H__
#define __FROG__SOUNDSOURCEOPENAL_H__

#include "FrogMemory.h"

#if PLATFORM_IS_WINDOWS
   #include "OpenAL/al.h"
   #include "OpenAL/alc.h"
#else
   #include <OpenAL/al.h>
   #include <OpenAL/alc.h>
#endif

#include "SoundSourceCommon.h"

namespace Webfoot {

class SoundManagerOpenAL;

//==============================================================================

/// A SoundSource object is used to track and control internal audio 
/// channels to play specific sounds.  Don't use the same SoundSource
/// object for more than one instance of a sound at once.
/// All SoundSources should ultimately be created by theSounds->SoundSourceCreate.
class SoundSourceOpenAL : public SoundSourceCommon
{
public:
   virtual ~SoundSourceOpenAL() {}

protected:
   SoundSourceOpenAL() { internalUse = false; }

   /// Called from the sound thread to pause this source.
   void PauseHelper();
   /// Called from the sound thread to unpause this source.
   void UnpauseHelper();

   /// Called from the sound thread to recalculate and set the true volume for this source.
   virtual void VolumeRefresh();

   /// OpenAL source ID number.
   ALuint sourceID;
   /// True if the object was created for internal use.
   bool internalUse;

   friend class SoundManagerOpenAL;
   friend class SoundOpenAL;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDSOURCEOPENAL_H__

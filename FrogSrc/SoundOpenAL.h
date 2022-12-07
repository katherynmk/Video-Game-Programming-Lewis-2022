#ifndef __FROG__SOUNDOPENAL_H__
#define __FROG__SOUNDOPENAL_H__

#include "FrogMemory.h"
#include "SoundManager.h"

namespace Webfoot {

//==============================================================================

/// Basic sound object for use with OpenAL.
/// It is not safe to pass a Sound by value.
class SoundOpenAL : public Sound
{
public:
   typedef Sound Inherited;
   
   SoundOpenAL();
   virtual ~SoundOpenAL() {}
   
   /// Clean up the object.
   virtual void Deinit();
      
protected:
   /// Called from the sound thread to start the sound.
   /// Return true if successful.
   virtual bool PlayHelper(SoundSource* source, bool loop, Sound::Usage usage, int volume,
      const Point3F& position, const Point3F& velocity);
   
   /// Called from the sound thread to stop the sound.
   virtual void StopHelper(SoundSource* source);

   /// True if the sound is loaded.
   bool loaded;
   /// Number of channels (1 for mono, 2 for stereo)
   int channelCount;
   /// 8 for 8-bit, 16 for 16-bit
   int bitsPerSample;
   /// Sample rate in Hz
   int sampleRate;
   /// OpenAL constant for the data format.
   ALenum format;
   /// Size of the buffer in bytes
   ALsizei bufferSize;
   /// OpenAL buffer ID number.
   ALuint bufferID;

   friend class SoundManagerOpenAL;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDOPENAL_H__

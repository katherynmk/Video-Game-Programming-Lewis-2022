#ifndef __FROG__SOUND_H__
#define __FROG__SOUND_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "ThreadUtilities.h"
#include "SoundSource.h"

namespace Webfoot {

//==============================================================================

/// Return the sound usage for the given name.  Return Sound::USAGE_INVALID if
/// unsuccessful.
int SoundUsageStringToCode(const char* usageName);
/// Return a string representation of the given sound usage.  Return NULL if
/// unsuccessful.
const char* SoundUsageCodeToString(int usage);

//==============================================================================

/// A Sound object abstracts different ways of representing an audio sample.
/// It is not safe to pass a Sound by value.
class Sound
{
public:
   /// Sound sources can be controlled to some extent according to their usage.
   enum Usage
   {
      USAGE_INVALID = 1,
      SFX,
      MUSIC,
      SPEECH,
      BACKGROUND,
      USAGE_COUNT,
      USAGE_DEFAULT = SFX
   };

   enum
   {
      VOLUME_DEFAULT = 100,
      VOLUME_MAX = 100,
      VOLUME_MIN = 0
   };
   static const Point3F POSITION_DEFAULT;
   static const Point3F VELOCITY_DEFAULT; 
   
   virtual ~Sound() {}
   
   virtual void Init();
   /// Clean up the object.
   virtual void Deinit();
   
   /// Request that this sound be played.  If 'source' is provided, use that object for
   /// continuing to control the audio after it has been started.  If 'loop' is true,
   /// the sound will loop indefinitely.  'volume' can be any value between 0 and 100 (inclusive).
   /// 'position' and 'velocity' are only applicable to monaural sounds.
   void Play(SoundSource* source = NULL, bool loop = false, Usage usage = USAGE_DEFAULT, int volume = VOLUME_DEFAULT,
      const Point3F& position = POSITION_DEFAULT, const Point3F& velocity = VELOCITY_DEFAULT);
      
   /// Request that all sources playing this sound be stopped.
   void Stop();
   
   /// Return the number of sources currently using this sound.
   int PlayingCountGet() { return playingCount.ValueGet(); }
      
protected:
   /// Called from the sound thread to start the sound.
   /// Return true if successful.
   virtual bool PlayHelper(SoundSource* source, bool loop, Sound::Usage usage, int volume,
      const Point3F& position, const Point3F& velocity);
   
   /// Called from the sound thread to stop the sound.
   virtual void StopHelper(SoundSource* source) = 0;
   
   /// Notify this sound object a SoundSource has stopped playing it.
   void NotifyStopped() { playingCount.Decrement(); }
   
   /// Count of all the number of SoundSources that are currently using this Sound.
   AtomicCount playingCount;

   friend class SoundManagerCommon;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUND_H__

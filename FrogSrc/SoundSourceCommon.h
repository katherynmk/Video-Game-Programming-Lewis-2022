#ifndef __FROG__SOUNDSOURCECOMMON_H__
#define __FROG__SOUNDSOURCECOMMON_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Thread.h"

namespace Webfoot {

class Sound;

//==============================================================================

/// A SoundSource can be used by application programmers to control how a sound
/// is played after it has started.  These objects are not safe to copy by value.
/// Be sure to call Init before using one and Deinit when you're done.
/// All SoundSources should ultimately be created by theSounds->SoundSourceCreate.
class SoundSourceCommon
{
protected:
   enum State
   {
      STOPPED,
      STARTING,
      PLAYING
   };

public:
   SoundSourceCommon() { state = STOPPED; }
   virtual ~SoundSourceCommon() {}

   virtual void Init();
   virtual void Deinit();
   
   /// Request that this sound source stop playing audio.
   void Stop();

   /// Request that the sound be paused.
   /// The pause count is set up so that a source can be double-paused, but not double-unpaused.
   /// So Pause-Pause-Unpause would remain paused, Pause-Pause-Unpause-Unpause would be unpaused,
   /// and Unpause-Unpause-Pause would be paused.
   void Pause();
   /// Request that the sound be unpaused.
   /// The pause count is set up so that a source can be double-paused, but not double-unpaused.
   /// So Pause-Pause-Unpause would remain paused, Pause-Pause-Unpause-Unpause would be unpaused,
   /// and Unpause-Unpause-Pause would be paused.
   void Unpause();
   
   /// Request a change in the volume of this source.  (0-100)
   void VolumeSet(int _volume);
   /// Return the most recently requested volume for this source.  (0-100)
   int VolumeGet() { return requestedVolume; }
   
   /// Request a change in the position of this source.
   void PositionSet(const Point3F& _position);
   /// Return the most recently requested position for this source.
   const Point3F& PositionGet() { return requestedPosition; }
   
   /// Request a change in the velocity of this source.
   /// This is only applicable to monaural sounds.
   void VelocitySet(const Point3F& _velocity);
   /// Return the most recently requested velocity for this source.
   /// This is only applicable to monaural sounds.
   const Point3F& VelocityGet() { return requestedVelocity; }

   /// True if this source was set for looping the last time it was used in a Play call.
   /// The returned value is not affected by whether the sound has stopped.
   bool LoopCheck() { return requestedLoop; }
   
   /// Return true if this source is done playing audio.
   /// If it is just paused, this will return true.
   bool StoppedCheck();
   
protected:
   /// Return true if the source is paused.
   /// Called from the sound thread.
   bool PausedCheck() { return pauseCount > 0; }

   /// Set the state of the source
   void StateSet(State _state);

   /// Called from the sound thread to set the volume.
   void VolumeSetHelper(int volume);

   /// Called from the sound thread to recalculate and set the true volume for this source.
   virtual void VolumeRefresh() = 0;
   
   /// Called from the sound thread to see if this is currently a looping source.
   bool InternalLoopCheck() { return loop; }

   /// Used to track whether the source is done yet.
   State state;
   /// Used to protect 'state'.
   Mutex stateMutex;
   /// Most recently requested volume
   int requestedVolume;
   /// Most recently requested position
   Point3F requestedPosition;
   /// Most recently requested velocity
   Point3F requestedVelocity;
   /// True if this source was set for looping the last time it was used in a Play call.
   bool requestedLoop;
   /// Current associated sound, if any.
   Sound* sound;
   /// Current volume.
   int volume;
   /// How the source is currently being used. (music, sfx, speech, etc.)
   int usage;
   /// Used to keep track of whether the source should be paused.
   int pauseCount;
   /// True if currently a looping source (for sound thread)
   bool loop;
   
   friend class Sound;
   friend class SoundManagerCommon;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDSOURCECOMMON_H__

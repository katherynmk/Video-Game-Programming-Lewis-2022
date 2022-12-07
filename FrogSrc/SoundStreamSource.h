#ifndef __FROG__SOUNDSTREAMSOURCE_H__
#define __FROG__SOUNDSTREAMSOURCE_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Thread.h"
#include "SoundStream.h"

namespace Webfoot {

class Sound;

//==============================================================================

/// A SoundStreamSource can be used by application programmers to control how a streaming sound
/// is played after it has started.  These objects are not safe to copy by value.
/// Be sure to call Init before using one and Deinit when you're done.
class SoundStreamSource
{
protected:
   enum State
   {
      STOPPED,
      STARTING,
      PLAYING
   };

public:
   SoundStreamSource() { state = STOPPED; }
   virtual ~SoundStreamSource() {}

   void Init();
   void Deinit();
   
   /// Request that this sound source stop playing audio.
   void Stop();

   /// Request that the stream be paused.
   void Pause();
   /// Request that the stream be unpaused.
   void Unpause();
   
   /// Request a change in the volume of this source.  (0-100)
   void VolumeSet(int _volume);
   /// Return the most recently requested volume for this source.  (0-100)
   int VolumeGet() { return requestedVolume; }
   
   /// Request a change in the position of this source.
   /// This is only applicable to monaural sounds.
   void PositionSet(const Point3F& _position);
   /// Return the most recently requested position for this source.
   const Point3F& PositionGet() { return requestedPosition; }
   
   /// Request a change in the velocity of this source.
   /// This is only applicable to monaural sounds.
   void VelocitySet(const Point3F& _velocity);
   /// Return the most recently requested velocity for this source.
   const Point3F& VelocityGet() { return requestedVelocity; }

   /// True if this source was set for looping the last time it was used in a Play call.
   /// The returned value is not affected by whether the sound has stopped.
   bool LoopCheck() { return requestedLoop; }
   
   /// Return true if this source is done playing audio.
   /// If it is just paused, this will return true.
   bool StoppedCheck();

   /// Return the current time of the stream in milliseconds.
   /// This is only supported for non-looping streams, and it is not
   /// necessarily supported on all platforms.
   unsigned int TimeGet();
   
protected:

   /// Set the state of the source
   void StateSet(State _state);

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
   SoundStream* stream;
   
   friend class SoundManagerCommon;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDSTREAMSOURCE_H__

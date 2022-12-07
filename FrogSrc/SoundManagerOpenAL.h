#ifndef __FROG__SOUNDMANAGEROPENAL_H__
#define __FROG__SOUNDMANAGEROPENAL_H__

#include "FrogMemory.h"

#if PLATFORM_IS_WINDOWS
   #include "OpenAL/al.h"
   #include "OpenAL/alc.h"
#else
   #include <OpenAL/al.h>
   #include <OpenAL/alc.h>
#endif

#include "SoundManagerCommon.h"
#include "SoundSourceCommon.h"
#include "Sound.h"
#include "FileManager.h"
#include "Map.h"
#include "TableStatic.h"
#include "ListStatic.h"
#include "Thread.h"
#include "SoundStream.h"

namespace Webfoot {

class SoundSourceOpenAL;

//==============================================================================

/// SoundManager is the central interface to the audio API.
class SoundManagerOpenAL : public SoundManagerCommon
{
public:
   typedef SoundManagerCommon Inherited;

   SoundManagerOpenAL();
   virtual ~SoundManagerOpenAL() {}

   /// Initialize the manager and use the given heaps for allocations on
   /// the main thread and sound thread.
   void Init(bool _multithreaded = true, HeapID _heapID = HEAP_DEFAULT,
      HeapID _soundThreadHeapID = HEAP_SOUND_THREAD, 
      Allocator* _soundDataAllocator = theAllocatorSoundData);
   /// Stop and clean up.
   void Deinit();

   /// Load the given sound file.  Do not include the extension.
   Sound* Load(const char* filename, FileManager* fileManager = theFiles);
   /// Return true if the given sound file exists.  Do not include the extension.
   bool ExistsCheck(const char* filename, FileManager* fileManager = theFiles);

   /// Create and return a new SoundSource instance.
   SoundSourceOpenAL* SoundSourceCreate(HeapID heapID = HEAP_DEFAULT);

   /// Return true if the given sound stream exists.  Do not include the file
   /// extension.
   bool StreamExistsCheck(const char* filename, FileManager* fileManager = theFiles);
   
   static SoundManagerOpenAL instance;
   
protected:  
   enum
   {
      /// The manager will not try to create sources for more than this many channels.
      /// Depending on the hardware, there may be fewer.
      MAX_CHANNEL_COUNT = 32,
      /// Wake the sound thread at intervals of this many milliseconds.
      UPDATE_PERIOD = 16,
   };

   /// Load the the given wav file.  'filename' is provided for debug output.
   Sound* WAVLoad(File* file, const char* filename);

   /// Called frequently from the sound thread for misc update tasks.
   virtual void SourcesUpdate();
   /// Update all the streams.
   virtual void StreamsUpdate();

   /// Called when a message arrives on the sound thread to be processed.
   virtual void MessageHandle(Message* message);

   /// Called from the sound thread to stop all the sources.
   void AllStopHelper();
   /// Called from the sound thread to pause all the sources.
   void AllPauseHelper();
   /// Called from the sound thread to unpause all the sources.
   void AllUnpauseHelper();
   /// Called from the sound thread to stop all non-looping, non-streaming sounds.
   void AllNonStreamingOneShotsStopHelper();

   /// Called on the sound thread to handle the OpenAL side of loading a sound.
   void SoundLoadHelper(Sound* sound, void* sampleData, bool* success);
   /// Send a message to the sound thread to handle the OpenAL side of unloading a sound.
   void SoundUnloadHelperRequest(Sound* sound);
   /// Called on the sound thread to handle the OpenAL side of unloading a sound.
   void SoundUnloadHelper(Sound* sound);
   /// Called on the sound thread to start playing a sound.
   void PlayHelper(Sound* sound, SoundSource* source, bool loop, Sound::Usage usage, int volume,
      const Point3F& position, const Point3F& velocity);
   /// Stop all active sources playing the given sound.
   void SoundStopHelper(Sound* sound);

   /// Set the position of the listener.
   void ListenerPositionSetHelper(const Point3F& position);
   /// Set the velocity of the listener.
   void ListenerVelocitySetHelper(const Point3F& velocity);
   /// Set the orientation of the listener.
   void ListenerOrientationSetHelper(const Point3F& forward, const Point3F& up);

   /// Called from the sound thread to stop the given source.
   void SourceStopHelper(SoundSource* source);
   /// Called from the sound thread to pause the given source.
   void SourcePauseHelper(SoundSource* source);
   /// Called from the sound thread to unpause the given source.
   void SourceUnpauseHelper(SoundSource* source);
   /// Set the position of the given source.
   void SourcePositionSetHelper(SoundSource* source, const Point3F& position);
   /// Set the velocity of the given source.
   void SourceVelocitySetHelper(SoundSource* source, const Point3F& velocity);

   /// Called from the sound thread to start a new stream.
   void StreamPlayHelper(const char* filename, FileManager* fileManager, 
      SoundStreamSource* source, bool loop, Sound::Usage usage, int volume,
      const Point3F& position, const Point3F& velocity);
   /// Called from the sound thread to stop a stream.
   void StreamSourceStopHelper(SoundStreamSource* streamSource);

   /// Called from the sound thread to change the volume for a given usage type.
   void UsageVolumeSetHelper(Sound::Usage usage, int volume);

   /// Set 'sourceID' to an available sound source ID and return true if successful.
   bool AvailableSourceIDGet(ALuint* sourceID);
   /// Put the given sourceID back in the collection of available IDs.
   void AvailableSourceIDPush(ALuint sourceID);

   /// Call this when a SoundSource either finished or failed to start.
   void SourceRecycle(SoundSource* source);

   /// Thread function for periodically waking the main sound thread.
   static void TimerThreadFunction(void* userData);
   /// Called from TimerThreadFunction
   void TimerThreadMethod();

   /// Thread used to periodically wake the main sound thread.
   Thread timerThread;
   /// Used to determine whether the timer thread should continue to loop.
   bool continueTimerThread;
   /// Mutex used to protect continueTimerThread.
   Mutex continueTimerThreadMutex;

   /// OpenAL sound device
   ALCdevice* soundDevice;
   /// OpenAL context
   ALCcontext* soundContext;
   
   /// The IDs of all the OpenAL audio sources that are not available to play a new sound.
   TableStatic<ALuint, MAX_CHANNEL_COUNT> activeSourceIDs;
   /// The IDs of all the OpenAL audio sources that are available to play a new sound.
   TableStatic<ALuint, MAX_CHANNEL_COUNT> availableSourceIDs;

   /// All currently running SoundSource objects.
   TableStatic<SoundSource*, MAX_CHANNEL_COUNT> activeSources;
   /// Collection of pre-allocated SoundSource objects for internal use that are
   /// not currently active.
   TableStatic<SoundSource*, MAX_CHANNEL_COUNT> availableSources;
   /// Collection of SoundSource objects for internal use.
   SoundSource sourcePool[MAX_CHANNEL_COUNT];

   friend class SoundOpenAL;
   friend class SoundStreamOpenAL;
   friend class SoundSourceOpenAL;
};

SoundManagerOpenAL* const theSounds = &SoundManagerOpenAL::instance;

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDMANAGEROPENAL_H__

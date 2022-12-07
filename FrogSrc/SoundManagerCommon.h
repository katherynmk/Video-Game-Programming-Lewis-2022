#ifndef __FROG__SOUNDMANAGERCOMMON_H__
#define __FROG__SOUNDMANAGERCOMMON_H__

#include "FrogMemory.h"
#include "SoundSourceCommon.h"
#include "Sound.h"
#include "SoundStream.h"
#include "SoundStreamSource.h"
#include "FileManager.h"
#include "Map.h"
#include "TableStatic.h"
#include "ListStatic.h"
#include "Thread.h"
#include "Utility.h"
#include "FileReferenceCounter.h"

namespace Webfoot {

//==============================================================================

/// SoundManager is the central interface to the audio API.
class SoundManagerCommon
{
public:
   /// Default position of the listener.
   static const Point3F LISTENER_POSITION_DEFAULT;
   /// Default velocity of the listnener.
   static const Point3F LISTENER_VELOCITY_DEFAULT;
   /// Default forward vector for the listener.
   static const Point3F LISTENER_FORWARD_DEFAULT;
   /// Default up vector for the listener.
   static const Point3F LISTENER_UP_DEFAULT;

   enum Codec
   {
      CODEC_SPEEX = 0,
      CODEC_CELT = 1,
      CODEC_WII = 2
   };

   SoundManagerCommon();
   virtual ~SoundManagerCommon() {}

   /// Initialize the manager and use the given heaps for allocations on
   /// the main thread and sound thread.
   void Init(bool _multithreaded = true, HeapID _heapID = HEAP_DEFAULT, HeapID _soundThreadHeapID = HEAP_SOUND_THREAD, 
      Allocator* _soundDataAllocator = theAllocatorSoundData, size_t soundThreadStackSize = 0,
      Thread::Priority soundThreadPriority = Thread::PRIORITY_PLUS_1);
   /// Stop and clean up.
   void Deinit();
   /// Call to update the sounds when multithreading is not available.
   void Update();

   /// Clean up the given sound
   void Unload(Sound* sound);
   /// Clean up the given sound
   void Unload(const char* filename, FileManager* fileManager = theFiles);
   
   /// Assuming the given sound is loaded, return a pointer to it.  Return NULL otherwise.
   /// This will call Flush internally before attempting to retrieve the object.
   /// Do not include the extension in the filename.
   Sound* Get(const char* filename, FileManager* fileManager = theFiles);
   
   /// Stop all sounds. (both streaming and non-streaming)
   void AllStop();
   /// Pause all currently non-stopped sounds. (both streaming and non-streaming)
   /// The pause counts are set up so that a source can be double-paused, but not double-unpaused.
   /// So Pause-Pause-Unpause would remain paused, Pause-Pause-Unpause-Unpause would be unpaused,
   /// and Unpause-Unpause-Pause would be paused.
   void AllPause();
   /// Unpause all currently non-stopped sounds. (both streaming and non-streaming)
   /// The pause counts are set up so that a source can be double-paused, but not double-unpaused.
   /// So Pause-Pause-Unpause would remain paused, Pause-Pause-Unpause-Unpause would be unpaused,
   /// and Unpause-Unpause-Pause would be paused.
   void AllUnpause();
   /// Sounds starting after this point should initially be paused (both streaming and non-streaming)
   /// This uses a counter system like AllPause and AllUnpause so double-unpausing is not allowed.
   void AllStartingPause();
   /// Undo a call to AllStartingPause.
   void AllStartingUnpause();
   /// Stop all non-looping, non-streaming sounds.
   void AllNonStreamingOneShotsStop();

   /// Begin streaming the given sound file.  Do not include the extension.  Volume should be 0-100.
   void StreamPlay(const char* filename, SoundStreamSource* source = NULL, bool loop = false,
      Sound::Usage usage = Sound::MUSIC, int volume = Sound::VOLUME_DEFAULT,
      const Point3F& position = Sound::POSITION_DEFAULT,
      const Point3F& velocity = Sound::VELOCITY_DEFAULT, FileManager* fileManager = theFiles);

   /// Request a change in the volume for the given usage type.  (0-100)
   void UsageVolumeSet(Sound::Usage usage, int volume);
   /// Return the most recently requested volume setting for the given usage type. (0-100)
   int UsageVolumeGet(Sound::Usage usage) { if(initialized) return requestedUsageVolumes[usage]; else return 0; }

   /// Convenience function for streaming the specified file as music.
   /// If the last stream started with this method is not finished, and the
   /// same song is being requested again, do nothing.  Volume should be 0-100.
   /// Do not include the extension in the filename.
   void MusicPlay(const char* filename, bool loop = true,
      int volume = Sound::VOLUME_DEFAULT, const Point3F& position = Sound::POSITION_DEFAULT,
      const Point3F& velocity = Sound::VELOCITY_DEFAULT, FileManager* fileManager = theFiles);
   /// Return the most recent filename to be played with MusicPlay.
   const char* MusicFilenameGet();
   /// Return true if the last stream started with MusicPlay has finished.
   /// If MusicPlay hasn't been called yet, return false.
   bool MusicStoppedCheck() { if(initialized) return musicSource.StoppedCheck(); else return true; }
   /// If the last stream started with MusicPlay has not finished, stop it now.
   void MusicStop() { if(initialized) musicSource.Stop(); }
   /// If the last stream started with MusicPlay has not finished, pause it now.
   void MusicPause() { if(initialized) musicSource.Pause(); }
   /// If the last stream started with MusicPlay has not finished, unpause it now.
   void MusicUnpause() { if(initialized) musicSource.Unpause(); }

   /// Convenience function for setting the usage type volume for music.  (0-100)
   void MusicVolumeSet(int volume) { UsageVolumeSet(Sound::MUSIC, volume); }
   /// Convenience function for returning the usage type volume for music.  (0-100)
   int MusicVolumeGet() { return UsageVolumeGet(Sound::MUSIC); }

   /// Convenience function for streaming the specified file as background sound.
   /// If the last stream started with this method is not finished, and the
   /// same sound is being requested again, do nothing.  Volume should be 0-100.
   /// Do not include the extension in the filename.
   void BackgroundPlay(const char* filename, bool loop = true,
      int volume = Sound::VOLUME_DEFAULT, const Point3F& position = Sound::POSITION_DEFAULT,
      const Point3F& velocity = Sound::VELOCITY_DEFAULT, FileManager* fileManager = theFiles);
   /// Return the most recent filename to be played with BackgroundPlay.
   const char* BackgroundFilenameGet();
   /// Return true if the last stream started with BackgroundPlay has finished.
   /// If BackgroundPlay hasn't been called yet, return false.
   bool BackgroundStoppedCheck() { if(initialized) return backgroundSource.StoppedCheck(); else return true; }
   /// If the last stream started with BackgroundPlay has not finished, stop it now.
   void BackgroundStop() { if(initialized) backgroundSource.Stop(); }
   /// If the last stream started with BackgroundPlay has not finished, pause it now.
   void BackgroundPause() { if(initialized) backgroundSource.Pause(); }
   /// If the last stream started with BackgroundPlay has not finished, unpause it now.
   void BackgroundUnpause() { if(initialized) backgroundSource.Unpause(); }

   /// Block until the message queue is empty.
   void Flush();

   /// Set the position of the listener.
   void ListenerPositionSet(const Point3F& position);
   /// Return the most recently requested position of the listener.
   Point3F ListenerPositionGet() { if(initialized) return requestedListenerPosition; else return Point3F::Create(0.0f, 0.0f, 0.0f); }
   /// Set the velocity of the listener.
   void ListenerVelocitySet(const Point3F& velocity);
   /// Return the most recently requested velocity of the listener.
   Point3F ListenerVelocityGet() { if(initialized) return requestedListenerVelocity; else return Point3F::Create(0.0f, 0.0f, 0.0f); }
   /// Set the orientation of the listener.
   void ListenerOrientationSet(const Point3F& forward, const Point3F& up);
   /// Return the most recently requested forward vector of the listener.
   Point3F ListenerForwardGet() { if(initialized) return requestedListenerForward; else return Point3F::Create(0.0f, 0.0f, -1.0f); }
   /// Return the most recently requested up vector of the listener.
   Point3F ListenerUpGet() { if(initialized) return requestedListenerUp; else return Point3F::Create(0.0f, 1.0f, 0.0f); }
   
   /// Return the allocator for sound data.
   Allocator* SoundDataAllocatorGet() { if(initialized) return soundDataAllocator; else return NULL; }

   /// Callback for the sound thread.
   typedef void (*SoundThreadCallback)(void* userData);
   /// Register the given function to receive callbacks from the sound thread.
   void SoundThreadCallbackRegister(SoundThreadCallback soundThreadCallback, void* userData = NULL);
   /// Unregister the given combination of function and userData from receiving
   /// callbacks from the sound thread.
   void SoundThreadCallbackUnregister(SoundThreadCallback soundThreadCallback, void* userData = NULL);

   /// Return true if this is currently initialized.
   bool InitializedCheck() { return initialized; }
   /// Return true if multiple threads should be used.
   bool MultithreadedCheck() { return multithreaded; }

protected:
   enum
   {
      /// Maximum number of messages allowed in the queue at once.
      MESSAGE_COUNT_MAX = 256,
      /// Maximum allowed number of stream objects.
      STREAM_COUNT_MAX = 64,
      /// Maximum number of registrations for callbacks from the sound thread.
      SOUND_THREAD_CALLBACK_COUNT_MAX = 64
   };
   
   /// MessageType lists the different messages that can be sent to the sound thread. 
   enum MessageType
   {
      DEINIT,
      
      ALL_STOP,
      ALL_PAUSE,
      ALL_UNPAUSE,
      ALL_STARTING_PAUSE,
      ALL_STARTING_UNPAUSE,
      ALL_NON_STREAMING_ONE_SHOTS_STOP,

      SOUND_LOAD,
      SOUND_UNLOAD,
      SOUND_PLAY,
      SOUND_STOP,
      
      SOURCE_STOP,
      SOURCE_PAUSE,
      SOURCE_UNPAUSE,
      SOURCE_VELOCITY_SET,
      SOURCE_POSITION_SET,
      SOURCE_VOLUME_SET,

      LISTENER_POSITION_SET,
      LISTENER_VELOCITY_SET,
      LISTENER_ORIENTATION_SET,

      STREAM_PLAY,
      STREAM_STOP,
      STREAM_PAUSE,
      STREAM_UNPAUSE,
      STREAM_VELOCITY_SET,
      STREAM_POSITION_SET,
      STREAM_VOLUME_SET,
      STREAM_TIME_GET,
      STREAMS_ACTIVE_CHECK,

      USAGE_VOLUME_SET,

      ON_OS_FOCUS_GAIN,
      ON_OS_FOCUS_LOSE,

      SOUND_THREAD_CALLBACK_REGISTER,
      SOUND_THREAD_CALLBACK_UNREGISTER
   };

   /// Message is used to communicate asynchronously with the sound thread.
   struct Message
   {
      MessageType type;
      bool loop;
      Sound::Usage usage;
      int volume;
      Sound* sound;
      SoundSource* source;
      SoundStreamSource* streamSource;
      FileManager* fileManager;
      Point3F position;
      Point3F velocity;
      Point3F forward;
      Point3F up;
      void* data;
      void* data2;
      bool* success;
      void* result;
      char filename[FROG_PATH_MAX+1];
   };

   /// Data for a single callback from the sound thread.
   struct SoundThreadCallbackRegistration
   {
      /// Function to call.
      SoundThreadCallback callback;
      /// Context information.
      void* userData;
   };

   /// Add the given sound to the collection.  Be sure it is not already present.
   void SoundAdd(const char* filename, FileManager* fileManager, Sound* sound);

   /// Set the volume of the given source.
   void SourceVolumeSetHelper(SoundSource* source, int volume);

   /// This allows platform-specific SoundManager implementations to call NotifyStopped
   /// on a sound object.  It's protected, but this class is a friend.
   void SoundProtectedNotifyStopped(Sound* sound) { sound->NotifyStopped(); }
   /// This allows platform-specific SoundManager implementations to call PlayHelper
   /// on a sound object.  It's protected, but this class is a friend.
   bool SoundProtectedPlayHelper(Sound* sound, SoundSource* source, bool loop, Sound::Usage usage, int volume,
      const Point3F& position, const Point3F& velocity)
   {
      return sound->PlayHelper(source, loop, usage, volume, position, velocity);
   }
   /// This allows platform-specific SoundManager implementations to call StopHelper
   /// on a sound object.  It's protected, but this class is a friend.
   void SoundProtectedStopHelper(Sound* sound, SoundSource* source) { sound->StopHelper(source); }

   /// This allows platform-specific SoundManager implementations to call
   /// StateSet(SoundSourceCommon::STOPPED) on a sound object.
   /// It's protected, but this class is a friend.
   void SourceProtectedNotifyStopped(SoundSource* source) { source->StateSet(SoundSourceCommon::STOPPED); }

   /// Called from the sound thread to pause a stream.
   void StreamSourcePauseHelper(SoundStreamSource* streamSource);
   /// Called from the sound thread to unpause a stream.
   void StreamSourceUnpauseHelper(SoundStreamSource* streamSource);
   /// Called from the sound thread to change the position.
   void StreamSourcePositionSetHelper(SoundStreamSource* streamSource, const Point3F& position);
   /// Called from the sound thread to change the velocity.
   void StreamSourceVelocitySetHelper(SoundStreamSource* streamSource, const Point3F& velocity);
   /// Called from the sound thread to change the volume.
   void StreamSourceVolumeSetHelper(SoundStreamSource* streamSource, int volume);
   /// Called from the sound thread to get the time from the stream.
   unsigned int StreamSourceTimeGetHelper(SoundStreamSource* streamSource);
   /// Called from the sound thread to check the number of active streams.
   void StreamsActiveCheckHelper(bool* result);
   /// This allows platform-specific SoundManager implementations to call
   /// StateSet(SoundStreamSource::STOPPED) on a sound stream source object.
   /// It's protected, but this class is a friend.
   void StreamSourceProtectedNotifyStopped(SoundStreamSource* streamSource) { streamSource->StateSet(SoundStreamSource::STOPPED); }
   /// This allows platform-specific SoundManager implementations to call
   /// StateSet(SoundStreamSource::PLAYING) on a sound stream source object.
   /// It's protected, but this class is a friend.
   void StreamSourceProtectedNotifyPlaying(SoundStreamSource* streamSource) { streamSource->StateSet(SoundStreamSource::PLAYING); }
   /// This allows platform-specific SoundManager implementations to
   /// associate a given stream with a source.
   /// It's protected, but this class is a friend.
   void StreamSourceProtectedStreamSet(SoundStreamSource* streamSource, SoundStream* stream) { streamSource->stream = stream; }
   /// This allows platform-specific SoundManager implementations to 
   /// get the SoundStream associated with a SoundStreamSource.
   /// It's protected, but this class is a friend.
   SoundStream* StreamSourceProtectedStreamGet(SoundStreamSource* source) { return source->stream; }

   /// Set the position of the listener.
   void ListenerPositionSetHelper(const Point3F& position) { listenerPosition = position; }
   /// Set the velocity of the listener.
   void ListenerVelocitySetHelper(const Point3F& velocity) { listenerVelocity = velocity; }
   /// Set the orientation of the listener.
   void ListenerOrientationSetHelper(const Point3F& forward, const Point3F& up) { listenerForward = forward; listenerUp = up; }

   /// Return the current volume setting for the given usage.
   int UsageVolumeCurrentGet(Sound::Usage usage) { if(initialized) return usageVolumes[usage]; else return 0; }

   /// Called from the sound thread to change the volume for a given usage type.
   void UsageVolumeSetHelper(Sound::Usage usage, int volume);
   
   /// A sound (streaming or non-streaming) that is starting right now
   /// should begin with this value for its pause count.
   virtual int StartingPauseCountGet() { return startingPauseCount; }
   /// Handle the sound thread side of AllStartingPause.
   void AllStartingPauseHelper();
   /// Handle the sound thread side of AllStartingUnpause.
   void AllStartingUnpauseHelper();

   /// Called from the sound thread to register a callback.
   void SoundThreadCallbackRegisterHelper(SoundThreadCallback soundThreadCallback, void* userData);
      /// Called from the sound thread to unregister a callback.
   void SoundThreadCallbackUnregisterHelper(SoundThreadCallback soundThreadCallback, void* userData);

   /// Return an available message object.
   /// If 'block' is true, this will block and wait for message objects to
   /// become available if needed.  If 'block' is false and no message objects
   /// are available, this will return NULL.
   Message* AvailableMessageGet(bool block = true);
   /// Put a message back in the availableMessage list.
   void AvailableMessagePush(Message* message);
   /// Enqueue the given message object to be executed.
   void MessageEnqueue(Message* message);

   /// Called when the program gains the focus of the OS.
   void OnOSFocusGain();
   /// Called when the program loses the focus of the OS.
   void OnOSFocusLose();

   /// Helper function for updating the sounds when multithreading is not
   /// available.
   void UpdateHelper();

   /// Wake the SoundManager's worker thread.
   void ThreadWake();
   /// The sound thread runs from this function.
   static void ThreadFunction(void* userData);
   /// Called by ThreadFunction.
   void ThreadMethod();

   /// Called when a message arrives on the sound thread to be processed.
   virtual void MessageHandle(Message* message) = 0;
   /// Called frequently from the sound thread for misc update tasks.
   virtual void SourcesUpdate() = 0;
   /// Update all the streams.
   virtual void StreamsUpdate() = 0;

   /// True if the system is currently initialized.
   bool initialized;
   /// True if multiple threads should be used.
   bool multithreaded;

   /// Used to keep track of whether a sound that is started now should be paused.
   int startingPauseCount;

   /// Current position for the listener.
   Point3F listenerPosition;
   /// Current velocity for the listener.
   Point3F listenerVelocity;
   /// Current forward vector for the listener.
   Point3F listenerForward;
   /// Current up vector for the listener.
   Point3F listenerUp;

   /// Most recently requested position for the listener.
   Point3F requestedListenerPosition;
   /// Most recently requested velocity for the listener.
   Point3F requestedListenerVelocity;
   /// Most recently requested forward vector for the listener.
   Point3F requestedListenerForward;
   /// Most recently requested up vector for the listener.
   Point3F requestedListenerUp;

   /// Current volume for each usage type.
   int usageVolumes[Sound::USAGE_COUNT];
   /// Most recently requested volume for each usage type.
   int requestedUsageVolumes[Sound::USAGE_COUNT];

   /// Stream source used by MusicPlay.
   SoundStreamSource musicSource;
   /// Last filename used with MusicPlay.
   char musicFilename[FROG_PATH_MAX+1];
   /// Last FileManager used with MusicPlay.
   FileManager* musicFileManager;

   /// Stream source used by BackgroundPlay.
   SoundStreamSource backgroundSource;
   /// Last filename used with BackgroundPlay.
   char backgroundFilename[FROG_PATH_MAX+1];
   /// Last FileManager used with BackgroundPlay.
   FileManager* backgroundFileManager;
   
   /// Heap for allocations on the main thread.
   HeapID heapID;
   /// Heap to use for allocations on the sound thread
   HeapID soundThreadHeapID;
   /// Allocator to use for waveform data.
   Allocator* soundDataAllocator;

   /// Thread on which interaction with the underlying API takes place.
   Thread thread;

   /// Collection of all loaded sounds.
   FileReferenceCounter sounds;

   /// Mutex to protect the message queue. 
   Mutex messageQueueMutex;
   /// ConditionVariable used to wait for messages.
   ConditionVariable messageQueueConditionVariable;
   /// Queue of messages from the non-audio thread.
   ListStatic<Message*, MESSAGE_COUNT_MAX> messageQueue;
   /// Mutex to protect availableMessages.
   Mutex availableMessagesMutex;
   /// ConditionVariable used to wait for 'availableMessages'.
   ConditionVariable availableMessagesConditionVariable;
   /// Collection of pre-allocated message objects for the message queue.
   TableStatic<Message*, MESSAGE_COUNT_MAX> availableMessages;
   /// Static collection of Message objects.
   Message messagePool[MESSAGE_COUNT_MAX];

   /// Collection of all current stream objects, regardless of whether they are currently playing.
   TableStatic<SoundStream*, STREAM_COUNT_MAX> activeStreams;
   /// Collection of registrations for callbacks from the sound thread.
   TableStatic<SoundThreadCallbackRegistration, SOUND_THREAD_CALLBACK_COUNT_MAX> soundThreadCallbacks;

   friend class Sound;
   friend class SoundSourceCommon;
   friend class SoundStreamSource;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDMANAGERCOMMON_H__

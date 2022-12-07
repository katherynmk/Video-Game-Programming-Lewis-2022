#ifndef __FROG__SOUNDSTREAMOPENAL_H__
#define __FROG__SOUNDSTREAMOPENAL_H__

#include "FrogMemory.h"
#include "SoundStream.h"
#include "Point3.h"

#if PLATFORM_IS_WINDOWS
   #include "OpenAL/al.h"
   #include "OpenAL/alc.h"
#else
   #include <OpenAL/al.h>
   #include <OpenAL/alc.h>
#endif

namespace Webfoot {

class File;
class FileManager;
class SoundStreamSource;

//==============================================================================

/// Base class for all sound stream implementations that use OpenAL.
class SoundStreamOpenAL
{
public:
   SoundStreamOpenAL() { allocatedSourceID = false; source = NULL; }
   virtual ~SoundStreamOpenAL() {}

   /// Begin streaming the given file.
   virtual bool Init(File* _file, FileManager* _fileManager, HeapID _heapID, 
      SoundStreamSource* _source, bool _loop, int usage, int volume,
      const Point3F& position, const Point3F& velocity) = 0;
   /// Stop the stream and clean up.
   virtual void Deinit() = 0;
   
   virtual void Update() = 0;
   
   virtual void Stop() = 0;
   
   /// Pause the stream.  This uses a counter internally.
   /// The pause count is set up so that a source can be double-paused, but not double-unpaused.
   /// So Pause-Pause-Unpause would remain paused, Pause-Pause-Unpause-Unpause would be unpaused,
   /// and Unpause-Unpause-Pause would be paused.
   virtual void Pause() = 0;
   /// Unpause the stream.  This uses a counter internally.
   /// The pause count is set up so that a source can be double-paused, but not double-unpaused.
   /// So Pause-Pause-Unpause would remain paused, Pause-Pause-Unpause-Unpause would be unpaused,
   /// and Unpause-Unpause-Pause would be paused.
   virtual void Unpause() = 0;

   /// Return true when the sound is done playing.
   virtual bool FinishedCheck() = 0;
   
   /// Set the volume of the stream. (0-100)
   void VolumeSet(int volume);
   /// Set the position of the stream.  This only applies to monaural sounds.
   void PositionSet(const Point3F& position);
   /// Set the velocity of the stream.  This only applies to monaural sounds.
   void VelocitySet(const Point3F& velocity);

   /// Return the current time in the stream in milliseconds.
   virtual unsigned int TimeGet() = 0;

   /// Called from the sound thread to recalculate and set the true volume for this source.
   void VolumeRefresh();

   /// Set 'sourceID' to an available sound source ID and return true if successful.
   bool AvailableSourceIDGet(ALuint* sourceID);
   /// Put the given sourceID back in the collection of available IDs.
   void AvailableSourceIDPush(ALuint sourceID);

   /// Return a pointer to the object, if any, that can be used to control this specific stream.
   SoundStreamSource* StreamSourceGet() { return source; }

   /// Return true if the stream is currently paused.
   bool PausedCheck() { return (pauseCount > 0); }
   
protected:

   /// Forward the request to SoundManager::StartingPauseCountGet();
   int StartingPauseCountGet();

   /// Notify the associated stream source, if any, that the stream is done playing.
   void StreamSourceProtectedNotifyStopped();

   /// How the stream is currently being used. (music, sfx, speech, etc.)
   int usage;
   /// Current volume setting for this stream.
   int volume;
   /// True if 'sourceID' was allocated and needs to be returned when finished.
   bool allocatedSourceID;
   /// OpenAL source ID number.
   ALuint sourceID;
   /// The object, if any, that can be used to control this specific stream.
   SoundStreamSource* source;
   /// Used to keep track of whether the source should be paused.
   int pauseCount;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SOUNDSTREAMOPENAL_H__

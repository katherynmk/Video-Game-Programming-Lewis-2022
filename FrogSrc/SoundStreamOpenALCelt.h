#ifndef __FROG__SOUNDSTREAMOPENALCELT_H__
#define __FROG__SOUNDSTREAMOPENALCELT_H__

#include "FrogMemory.h"
#include "SoundStream.h"
#include "Table.h"

namespace Webfoot {

class File;
class FileManager;
class SoundStreamSource;

//==============================================================================

/// Beginning of the file.
struct CeltFileHeader
{
   /// 0 for Speex, 1 for Celt
   uint32 encodedFormat;
   /// In Hz
   uint32 sampleRate;
   /// Total number of Celt frames in the file.
   uint32 frameCount;
   /// Size of a compressed frame in bytes.
   uint32 compressedFrameSize;
   /// Number of samples in a frame.
   uint32 samplesPerFrame;
   /// 1 for mono and 2 for stereo.
   uint32 channelCount;
};

//==============================================================================

/// Manages a single stream of Celt-encoded audio.  The object should
/// not be directly accessed by the application programmer.
/// They should instead interact with the streams through a SoundManager
/// or a SoundStreamSource. 
class SoundStreamOpenALCelt : public SoundStream
{
public:
   SoundStreamOpenALCelt();
   virtual ~SoundStreamOpenALCelt() {}
   
   /// Begin streaming the given file.
   virtual bool Init(File* _file, FileManager* _fileManager, HeapID _heapID, 
      SoundStreamSource* _source, bool _loop, int usage, int volume,
      const Point3F& position, const Point3F& velocity);
   /// Stop the stream and clean up.
   virtual void Deinit();
   
   virtual void Update();
   
   virtual void Stop();
   
   /// Pause the stream.  This uses a counter internally.
   virtual void Pause();
   /// Unpause the stream.  This uses a counter internally.
   virtual void Unpause();

   /// Return true when the sound is done playing.
   virtual bool FinishedCheck() { return done; }

   /// Return the current time in the stream in milliseconds.
   virtual unsigned int TimeGet();
   
protected:
   enum
   {
      /// Minimum duration of a single page in milliseconds
      MIN_PAGE_DURATION = 250
   };

   /// The stream is played by cycling through a set of Pages
   /// where each page contains the audio data for a short amount of time.
   struct Page
   {
      Page() { allocatedBuffer = false; }

      /// True if the bufferID is valid
      bool allocatedBuffer;
      /// OpenAL buffer ID number.
      ALuint bufferID;
   };

   /// Prepare the given page to be played.
   void PagePrepare(Page* page);

   /// File from which data will be streamed.
   File* file;
   /// FileManager that made 'file'.
   FileManager* fileManager;
   /// Heap from which any data should be allocated.
   HeapID heapID;
   /// True if the stream should loop indefinitely.
   bool loop;
   /// Used to decode Celt data
   void* celtMode;
   /// Used to decode Celt data
   void* celtDecoder;
   /// Data loaded from the beginning of the sound file.
   CeltFileHeader header;
   /// Uncompressed size of a single Celt frame in bytes.
   size_t uncompressedFrameSize;
   /// Buffer into which compressed data is read.
   unsigned char* compressedFrame;
   /// Buffer into which data is decompressed.
   short* pageSoundData;
   /// Collection of segments for the currently loaded audio data for the stream.
   Table<Page*> pages;
   /// Number of Celt frames to use for each page.
   int framesPerPage;
   /// Number of frames left in the stream to be decoded.
   int framesRemaining;
   /// Number of times a page has been dequeued since initialization.
   unsigned int pagesDequeued;
   /// True if the stream is done playing.
   bool done;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDSTREAMOPENALCELT_H__

#ifndef __FROG__VIDEOSTREAMDEFAULT_H__
#define __FROG__VIDEOSTREAMDEFAULT_H__

#include "FrogMemory.h"
#include "FileRAM.h"
#include "Image.h"
#include "Table.h"
#include "BitmapSplitter.h"

// This header should only be included from a platform's VideoStream.h.

namespace Webfoot {

class Bitmap;
class SoundStreamSource;
class FileManager;

//==============================================================================

/// VideoStreamDefault allows you to stream video from a file.  This implementation is
/// not suitable for all platforms.
class VideoStreamDefault
{
public:
   VideoStreamDefault();

   /// Initialize the stream to load from the given filename.  The specified
   /// FileManager may not be used, depending on the implementation.
   void Init(const char* filename, FileManager* fileManager = theFiles);
   void Deinit();

   void Update(unsigned int dt);

   /// Return an Image that can be used to display the current frame.
   Image* FrameImageGet() { return frameImage; }

   /// Return true if the end of the video has been reached.
   bool FinishedCheck() { return finished; }

protected:
   enum
   {
      PAGE_COUNT = 2,
   };

   enum VideoFormat
   {
      STREAM_FORMAT_THEORA
   };

   /// Data provided at the beginning of a stream.
   struct StreamHeader
   {
      /// Format of the video.
      uint32 videoFormat;
      /// Number of frames in the entire video.
      uint32 videoFrameCount;
      /// Number of pages in the stream.
      uint32 pageCount;
      /// Maximum size of a container page, in bytes.
      uint32 pageSizeMax;
      /// Combined size of the Theora header packets, in bytes.
      uint32 theoraHeadersSize;
      /// Number of bytes to read for the first page.
      uint32 firstPageSize;
   };

   /// Data provided at the beginning of a single page of a stream.
   struct PageHeader
   {
      /// Number of bytes to read for the next page.
      uint32 nextPageSize;
      /// Number of video frames in this page.
      uint32 videoFrameCount;
   };

   struct Page
   {
      /// Header data for this page.
      PageHeader header;
      /// Buffer of page data.
      void* data;
      /// How much of the data buffer contains page data.
      size_t dataSizeUsed;
      /// Provides an easier interface for parsing data.
      FileRAM pageDataInterface;
      /// Number of video frames left to be decoded in this page.
      uint32 videoFramesRemaining;
   };

   /// Decode the next frame and make it the offical bitmap for the present time.
   void FramePrepare() { FramePrepareHelper(false); }
   /// Skip one frame without affecting the official bitmap for the present time.
   void FrameSkip() { FramePrepareHelper(true); }
   /// Decode the next frame.  If 'skipFrame' is false, make it the offica
   /// bitmap for the present time.
   void FramePrepareHelper(bool skipFrame);
   /// Load the next page from the file.  Return true if successful.
   bool PageRead();
   /// Load an ogg packet from the given File.  'buffer' is the underlying
   /// buffer to which 'bufferInterface' has been assigned.
   void OggPacketLoad(File* bufferInterface, void* buffer, void* _oggPacket);
   /// Convert data from the input buffer to the output RGB8 bitmap.  This assumes
   /// both buffers have already been properly initialized.  Return true if
   /// successful.
   bool YCbCrToRGB(Bitmap* output, void* _input);

   /// File from which data is streamed.
   File* streamFile;
   /// FileManager used to get the stream file.
   FileManager* fileManager;
   /// Number of pages left to read.
   uint32 pageReadsRemaining;
   /// Number of video frames left to decode.
   uint32 videoFramesRemaining;
   /// Size of the next page to be read, in bytes.
   uint32 nextPageSize;
   /// Bitmap with the decoded video data for the current frame.
   Bitmap* frameBitmap;
   /// Image that can be used to display the current frame.
   Image* frameImage;
   /// Segments into which the decoded video frames are split.  This is NULL
   /// when the frame bitmap is already an appropriate size.
   Table<BitmapSplitterSegment>* rgb8Segments;
   /// Header data from the stream.
   StreamHeader streamHeader;
   /// Index of the current page in the collection of pages.
   int currentPageIndex;
   /// Next page index into which to read.
   int nextPageReadIndex;
   /// Buffers for loading blocks of data from the file.
   Page pages[PAGE_COUNT];
   /// The frame rate is given as a fraction in frames per second.
   int frameRateNumerator;
   /// The frame rate is given as a fraction in frames per second.
   int frameRateDenominator;
   /// True if the end of the video has been reached.
   bool finished;
   /// Used to track time when there is no sound stream to follow.
   unsigned int timer;
   /// Used the play the streaming sound that accompanies the video.
   SoundStreamSource* soundStreamSource;

   /// Theora decoder context.
   void* _thDecCtx;
   /// Theora uncompressed framebuffer.
   void* _thYCbCrBuffer;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__VIDEOSTREAMDEFAULT_H__

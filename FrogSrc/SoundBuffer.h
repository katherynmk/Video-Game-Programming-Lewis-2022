#ifndef __FROG__SOUNDBUFFER_H__
#define __FROG__SOUNDBUFFER_H__

#include "FrogMemory.h"
#include "Allocator.h"

namespace Webfoot {

//==============================================================================

/// A buffer of sound data, including metadata like format and length.
class SoundBuffer
{
public:
   SoundBuffer();

   /// Ways of encoding the sound data.
   enum Format
   {
      FORMAT_INVALID = -1,
      /// Unsigned 8-bit PCM.  128 is neutral.  Data for multiple channels is
      /// interleaved.  For example, the pattern for stereo is one sample for
      /// the left channel followed by a sample for the right channel.
      FORMAT_PCM_UINT8,
      /// Signed 16-bit PCM.  0 is neutral.  Data for multiple channels is
      /// interleaved.  For example, the pattern for stereo is one sample for
      /// the left channel followed by a sample for the right channel.
      FORMAT_PCM_INT16,
      /// Number of formats.
      FORMAT_COUNT
   };

   /// Clean up anything that was allocated by this class.
   void Deinit();

   /// Allocate space to give the buffer the specified length and format.
   /// Return true if successful.
   bool Allocate(Format _format, int _channelCount, int _length, int _sampleRate, Allocator* _allocator = theAllocatorDefault);
   /// Without forgetting the format or length, clean up anything that was
   /// allocated by this object.
   void Deallocate();
   /// Use the given point in memory as data for a sound buffer of the given
   /// format and length.  This would be useful for sounds stored in program
   /// memory or allocated externally.
   void DataSet(void* _data, Format _format, int _channelCount, int _length, int _sampleRate);

   /// Return a pointer to the sound data.
   void* DataGet() { return data; }
   /// Return the format of this sound buffer.
   Format FormatGet() { return format; }
   /// Return the number of channels (1 = mono, 2 = stereo)
   int ChannelCountGet() { return channelCount; }
   /// Return the number of samples per channel of the recording.
   int LengthGet() { return length; }
   /// Return the rate at which the buffer should be played in samples per
   /// channel per second.
   int SampleRateGet() { return sampleRate; }

   /// Try to make the whole buffer silent.  This may not be supported by all
   /// formats.
   void Clear();

   /// Given the format, length, and number of channels, return the size of the
   /// buffer data in bytes.
   size_t DataSizeBytesGet();

protected:
   /// Sound data.  (like the waveform)
   void* data;
   /// If the Allocate or Clone methods were used, this is the allocator that
   /// was used.  Otherwise, it is NULL.
   Allocator* allocator;
   /// Encoding of the buffer data.
   Format format;
   /// Number of channels (1 = mono, 2 = stereo)
   int channelCount;
   /// Number of samples per channel of the buffer.
   int length;
   /// Rate at which the buffer should be played in samples per channel per
   /// second.
   int sampleRate;
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__SOUNDBUFFER_H__

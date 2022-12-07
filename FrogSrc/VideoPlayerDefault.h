#ifndef __FROG__VIDEOPLAYERDEFAULT_H__
#define __FROG__VIDEOPLAYERDEFAULT_H__

// This header should only be included from a platform's VideoPlayer.h.

#include "FrogMemory.h"
#include "GameState.h"
#include "FileManager.h"
#include "VideoStream.h"

namespace Webfoot {

//==============================================================================

/// Helper struct for a single entry in the video queue.
struct VideoPlayerDefaultQueueEntry
{
   /// Path of the file to be played.
   char filename[FROG_PATH_MAX];
   /// FileManager to be used to load the file.  Depending on the
   /// implementation, this may not be used.
   FileManager* fileManager;
};

//==============================================================================

/// This class is a GameState that plays a video and pops itself from the stack
/// of GameStates when the video is complete.  Which video it should play must
/// be configured before the Init function is called.  The player will attempt
/// to fill the screen without altering the aspect ratio on its own.  On some
/// platforms, this will use the extended area, even if the rest of the game
/// does not.
///
/// This implementation may not be suitable for all platforms.  Not all
/// platforms necessarily support the same formats.  On some platforms, this
/// may cover all other drawing, including fading and the cursor.  Do not
/// attempt to draw over it.  Depending on the file and the platform, reading
/// of the file may nor may not go through Frog.  This will determine whether
/// the video should be stored in resource.dat.
class VideoPlayerDefault : public GameState
{
public:
   VideoPlayerDefault();
   virtual ~VideoPlayerDefault() {}

   virtual void Init();
   virtual void Deinit();
   virtual void Update();
   virtual void Draw();

   /// Call this before Init is called to enqueue a video to be shown.  Return
   /// true if successful.  Do not include the extension.  Depending on the
   /// implementation, the specified FileManager may not be used.
   bool Enqueue(const char* _filename, FileManager* _fileManager = theFiles);
   
   /// Set whether the video should end early if certain input is received.
   /// For example, pressing a left mouse button would end it.
   void SkipOnInputSet(bool _skipOnInput) { skipOnInput = _skipOnInput; }
   /// Set whether the GameState should persist for an extra frame before
   /// exiting, without drawing.  On some platforms and situations, the screen
   /// may be blank at the end of the state regardless of this setting.
   void BlankFrameBeforeExitSet(bool _blankFrameBeforeExit) { blankFrameBeforeExit = _blankFrameBeforeExit; }

   enum
   {
      /// Maximum number of videos that can be in the queue at once.
      QUEUE_LENGTH_MAX = 16
   };

protected:
   /// Return the filename at the front of the queue.
   const char* FilenameGet();
   /// Return the FileManager to use to load the video.  Depending on the
   /// implementation, this may not be used.
   FileManager* FileManagerGet();

   /// Initialize the player, optionally attempting to use the default video format.
   void InitHelper(bool useDefaultFormat);
   /// Return true if the video should be ended early.
   virtual bool ExitEarlyCheck();
   
   /// Object that streams the video to be displayed.
   VideoStream* videoStream;
   /// True if the video should end early if certain input is received.
   bool skipOnInput;
   /// True if the state should persist for an extra frame before exiting
   bool blankFrameBeforeExit;
   /// True if waiting one more frame before exiting the state.
   bool waitingOneFrameBeforeExit;
   /// Queue of videos to be played.
   VideoPlayerDefaultQueueEntry videoQueue[QUEUE_LENGTH_MAX];
   /// Number of videos in the queue.
   int videoQueueLength;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__VIDEOPLAYERDEFAULT_H__

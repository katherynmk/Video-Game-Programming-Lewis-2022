#ifndef __FROG__ANIMATEDBACKGROUND_H__
#define __FROG__ANIMATEDBACKGROUND_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

class AnimatedBackgroundItem;
class AnimatedBackgroundManager;

/// Transparency value for fully clear.
#define ANIMATED_BACKGROUND_TRANSPARENCY_CLEAR 0
/// Transparency value for fully opaque.
#define ANIMATED_BACKGROUND_TRANSPARENCY_OPAQUE 255

//=============================================================================

class AnimatedBackground
{
public:
   AnimatedBackground() { spriteResourceFilename = NULL; }
   virtual ~AnimatedBackground() {}

   void Init(const char* filename);
   void Deinit();

   /// Reset the time and remove all items.
   void Reset();

   /// Update the position in the animation.
   void Update(unsigned int dt);
   /// Draw all elements of the animation.
   void Draw();

   /// Return the filename for this AnimatedBackground's SpriteResourceFile.
   const char* SpriteResourceFilenameGet() { return (const char*)spriteResourceFilename; }

   /// Return the duration of the animation in milliseconds.
   unsigned int DurationGet() { return duration; }
   /// Return the current position in the animation in milliseconds.
   unsigned int TimeGet() { return currentTime; }

   /// Return the first item with the given name.
   AnimatedBackgroundItem* ItemGetByName(const char* itemName);

private:
   /// Name of the sprite resource file for the background's sprites.
   char* spriteResourceFilename;
   /// The current position in the looping animation.
   unsigned int currentTime;
   /// The length of the animation loop in milliseconds.
   unsigned int duration;
   /// Begin loops at this time, including when wrapping around.
   unsigned int startTime;
   /// Collection of items to be drawn.
   Table<AnimatedBackgroundItem*> items;

   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// Time at which the file containing the specifications was last modified.
      /// This is only used by AnimatedBackgroundManager.
      int64 fileModificationTime;
   #endif //#if _DEBUG && PLATFORM_IS_WINDOWS

   friend class AnimatedBackgroundManager;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__ANIMATEDBACKGROUND_H__

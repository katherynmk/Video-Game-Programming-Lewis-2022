#ifndef __FROG__ANIMATEDBACKGROUNDMANAGER_H__
#define __FROG__ANIMATEDBACKGROUNDMANAGER_H__

#include "FrogMemory.h"
#include "Color.h"

namespace Webfoot {

class AnimatedBackground;

//=============================================================================

/// Manages the loading, updating, displaying, and cleanup of AnimatedBackgrounds.
class AnimatedBackgroundManager
{
public:
   AnimatedBackgroundManager();
   virtual ~AnimatedBackgroundManager() {}

   void Init();
   void Deinit();
   void Update(unsigned int dt);
   void Draw();
   
   /// Load the background from the given file.
   /// Call with NULL to remove the current background.
   void BackgroundSet(const char* filename);
   /// Remove the current background.
   void BackgroundClear() { BackgroundSet(NULL); }
   /// Return the current background, if any.
   AnimatedBackground* BackgroundGet() { return background; }
   /// Return true if a file exists for the given background.
   bool BackgroundExistsCheck(const char* filename);

   /// Return true if the animated background is paused.
   bool PausedCheck() { return paused; }
   /// Set whether the animated background is paused.
   void PausedSet(bool _paused) { paused = _paused; }

   /// Set the color with which all parts of the background should be
   /// modulated.
   void ColorSet(const ColorRGBA8& _color) { color = _color; }
   /// Return the color with which all parts of the background should be
   /// modulated.
   ColorRGBA8 ColorGet() { return color; }

   /// Set the main parallax offset.
   void ParallaxOffsetSet(const Point2F& _parallaxOffset) { parallaxOffset = _parallaxOffset; }
   /// Return the main parallax offset.
   Point2F ParallaxOffsetGet() { return parallaxOffset; }

   static AnimatedBackgroundManager instance;

private:
   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// Called when the program gains the focus of the OS.
      void OnOSFocusGain();

      /// Return the time at which the background specification file of the
      /// current background was last modified.
      int64 BackgroundFileModificationTimeGet();
   #endif //#if _DEBUG && PLATFORM_IS_WINDOWS

   /// True if the system is between Init and Deinit calls.
   bool initialized;
   /// Whether the background is currently animating.
   bool paused;
   /// Current background
   AnimatedBackground* background;
   /// Filename of the currently loaded background
   char* currentFilename;
   /// Color with which all parts of the background should be modulated.
   ColorRGBA8 color;
   /// Main parallax offset applied when drawing items.
   Point2F parallaxOffset;
};

//=============================================================================

AnimatedBackgroundManager* const theAnimatedBackgrounds = &AnimatedBackgroundManager::instance;

} //namespace Webfoot

#endif //#ifndef __FROG__ANIMATEDBACKGROUNDMANAGER_H__

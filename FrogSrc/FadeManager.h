#ifndef __FROG__FADEMANAGER_H__
#define __FROG__FADEMANAGER_H__

#include "FrogMemory.h"
#include "Color.h"

namespace Webfoot {

//=============================================================================

/// FadeManager draws a full-screen filled rectangle for fading in and out.
class FadeManager
{
public:
   enum
   {
      /// Default duration of a fade in milliseconds.
      FADE_DURATION_DEFAULT = 500
   };

   FadeManager();

   void Init();
   void Deinit();

   void Update(unsigned int dt);
   /// Draw a full-screen rectangle of the appropriate color, if needed.
   void Draw();

   /// Stop all fading and jump to the given color.
   void ColorSet(const ColorRGBA8& color);
   /// Return the color that should be displayed on this frame.
   ColorRGBA8 ColorCurrentGet();
   
   /// Fade to the given color over the given number of milliseconds.
   void FadeToColor(const ColorRGBA8& colorEnd, unsigned int _fadeDuration = FADE_DURATION_DEFAULT);
   /// Fade to fully clear over the given number of milliseconds.
   void FadeIn(unsigned int _fadeDuration = FADE_DURATION_DEFAULT) { FadeToColor(COLOR_RGBA8_TRANSPARENT, _fadeDuration); }
   /// Fade to black over the given number of milliseconds.
   void FadeToBlack(unsigned int _fadeDuration = FADE_DURATION_DEFAULT) { FadeToColor(COLOR_RGBA8_BLACK, _fadeDuration); }

   /// Return true if currently fading.
   bool FadeActiveCheck() { return fadeActive; }
   /// Return true if this was fading, as of the previous update.
   bool FadeWasActiveCheck() { return fadeWasActive; }
   /// Return true if the screen fading is not completely transparent right now.
   bool FadedCheck() { return ColorCurrentGet().alpha != 0; }
   /// Finish any fading now.
   void FadeFinish() { fadeActive = false; }

   /// Set the region of the screen over which the rectangle will be drawn.
   void DrawRegionSet(const Box2F& _drawRegion) { drawRegion = _drawRegion; drawRegionExplicit = true; }
   /// Use the default for the draw region.
   void DrawRegionRestoreDefault();

   static FadeManager instance;

private:
   /// Called when the "OnWindowChange" event is triggered.
   void OnWindowChange();

   /// Area in which to draw the rectangle
   Box2F drawRegion;
   /// True if the draw region has been explicitly set.
   bool drawRegionExplicit;
   /// True if currently fading.
   bool fadeActive;
   /// Value of 'fadeActive' from the previous update.
   bool fadeWasActive;
   /// Full length of the current fade in milliseconds.
   unsigned int fadeDuration;
   /// Position in the current fade in milliseconds.
   unsigned int fadeTimer;
   /// Starting color of the fade.
   ColorRGBA8 colorBegin;
   /// Ending color of the fade.
   ColorRGBA8 colorEnd;
};

//=============================================================================

FadeManager* const theFades = &FadeManager::instance;

} //namespace Webfoot

#endif //#ifndef __FROG__FADEMANAGER_H__

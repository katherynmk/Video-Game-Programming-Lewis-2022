#ifndef __FROG__SCREENOPENGLSDL2_H__
#define __FROG__SCREENOPENGLSDL2_H__

#include "FrogMemory.h"
#include "ScreenOpenGL.h"

#include <SDL2/SDL.h>

// Determine whether an X11 dummy window will be needed.
#if RASPBERRYPI
   #define FROG_X_DUMMY_WINDOW 1
#else
   #define FROG_X_DUMMY_WINDOW 0
#endif

namespace Webfoot {

class ScreenParameters;

#if FROG_OPENGL_ES
   /// GLee is not available for OpenGL ES.
   extern bool GLEE_EXT_texture_filter_anisotropic;
   const bool GLEE_ARB_geometry_shader4 = false;
   #define GL_GEOMETRY_SHADER_ARB 0x8DD9
   #define GL_TRIANGLES_ADJACENCY_ARB 0xC
#endif //#if FROG_OPENGL_ES

#define SCREEN_TITLE_DEFAULT "Webfoot"

//==============================================================================

/// Abstracts the details of setting up, using, and cleaning up the ability
/// to draw to the screen.
class ScreenOpenGLSDL2 : public ScreenOpenGL
{
public:
   ScreenOpenGLSDL2();

   /// Initialize the screen using the given parameters.
   /// Return true if successful and false otherwise.
   bool Init(ScreenParameters* screenParameters);
   /// Destroy the window.
   void Deinit();
   /// Call on every frame.
   void Update(bool hasFocus);

   /// Call this when finished making display calls for a particular frame.
   void PostDraw();

   /// Call this with incoming SDL2 messages.
   /// Update must only be called on the same thread as EventNotify.
   void EventNotify(SDL_Event* event);

   static Screen instance;

   /// Return the normal desktop resolution.
   virtual Point2I DesktopSizeGet();

   /// Set the string to be displayed in the window titlebar when running in
   /// windowed mode.  This is not applicable to all platforms.
   void WindowTitleSet(const char* _windowTitle);

   /// Return true if an X11 dummy window is currently in use.
   bool XDummyWindowExistsCheck();

protected:
   typedef ScreenOpenGL Inherited;

   /// Hide or show the cursor, as appropriate.
   void SystemCursorVisibleRefresh();
   /// Return true if between calls to Init and Deinit.
   bool InitializedCheck() { return initialized; }

   #if FROG_OPENGL_ES
      /// Prepare important OpenGL extensions.
      void OpenGLExtensionsInit();
   #endif

   /// True if between calls to Init and Deinit.
   bool initialized;
   /// True if the normal system cursor should be hidden while over the window.
   bool systemCursorHide;
   /// True if the system cursor is currently visible.
   bool systemCursorVisible;
   /// Cached value for the normal desktop resolution in pixels.
   Point2I desktopSize;
   /// True if the desktop size has already been retrieved from the OS.
   bool desktopSizeCached;
   /// True if anything drawn since PreDraw should be erased before swapping
   /// the frames.
   bool drawingErase;
   /// SDL handle for the window.
   SDL_Window* window;
   /// SDL handle for the renderer.
   SDL_Renderer* renderer;
};

static ScreenOpenGLSDL2 * const theScreen = &ScreenOpenGLSDL2::instance;

//===============================================================================

/// Collection of parameters used to initialize the Screen.
/// This is platform-dependent.
class ScreenParameters : public ScreenParametersOpenGL
{
public:
   ScreenParameters()
   {
      graphicsResolutionsProvided = NULL;
      screenSize = Point2I::Create(SCREEN_IOS_GRAPHICS_WIDTH_DEFAULT, SCREEN_IOS_GRAPHICS_HEIGHT_DEFAULT);
      windowTitle = SCREEN_TITLE_DEFAULT;
      outputScaling = ScreenOpenGL::STRETCH_KEEP_ASPECT_RATIO;
      extendedAreaAllowed = false;
      vSync = true;
      systemCursorHide = true;
      preparedArea.Set(0, 0, 0, 0);
      imageScales = NULL;
      graphicsPath = NULL;
   }

   /// When 'graphicsPath" is NULL, set this to a JSON array of Point2Is for the
   /// resolutions that have graphics folders in this game.  The game will choose
   /// one automatically based on the native screen size.  All provided
   /// resolutions must be in the correct orientation for the game.
   JSONValue* graphicsResolutionsProvided;
   /// Dimensions of the area of the screen available to the application
   /// programmer, in pixels.  This will only be used if
   /// 'graphicsResolutionsProvided' fails to specify resolutions.
   Point2I screenSize;
   /// String to be displayed in the window titlebar when running in windowed mode.
   const char* windowTitle;
   /// Specifies how the Screen class should handle differences between the
   /// the actual framebuffer resolution and the virtual screen resolution
   /// requested by the application programmer.
   ScreenOpenGL::OutputScaling outputScaling;
   /// False if content should generally be restricted to the area of the
   /// screen that remains after applying letterbox or pillarbox.
   bool extendedAreaAllowed;
   /// Set to true to synchronize the framebuffer updates with the display device.
   /// This can reduce tearing, but it can also make the game feel less responsive.
   bool vSync;
   /// True if the normal system cursor should be hidden while over the window.
   bool systemCursorHide;
   /// Maximum region for which the game is generally prepared.  This must be
   /// at least as large as the requested size of the screen from the game's
   /// perspective, and it must be centered on that requested area.  Clipping
   /// may be extended out to this area even if the full extended area is not
   /// allowed.
   Box2I preparedArea;
   /// JSON array of JSON objects to specify the provided ImageScales and their
   /// filename suffixes.
   JSONValue* imageScales;
   /// Path that is implicitly prefixed to all graphics filenames in the
   /// main file system.  If this is NULL, a default will be chosen based
   /// on the screen's size.
   const char* graphicsPath;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SCREENOPENGLSDL2_H__

#ifndef __FROG__SCREEN_H__
#define __FROG__SCREEN_H__

#include "FrogMemory.h"
#include "ScreenOpenGL.h"
#include "FileManager.h"
#include "Table.h"
#include "KeyboardManager.h"

namespace Webfoot {

#define SCREEN_WIDTH_DEFAULT 1024
#define SCREEN_HEIGHT_DEFAULT 768
#define SCREEN_TITLE_DEFAULT "Webfoot"
/// By default, if this key was pressed, save a screenshot.
#define SCREEN_SCREENSHOT_KEY_DEFAULT KEY_F9
/// By default, if this key is pressed in a debug build zoom to 1:1.
#define SCREEN_DEBUG_ONE_TO_ONE_KEY_DEFAULT KEY_F8

/// Types of anti-aliasing, if any.
enum AntiAliasingMode
{
   /// No anti-aliasing
   AA_DISABLED,
   /// 2x multisample anti-aliasing
   AA_MSAA_2X,
   /// 4x multisample anti-aliasing
   AA_MSAA_4X,
   /// 8x multisample anti-aliasing
   AA_MSAA_8X,
   /// 16x multisample anti-aliasing
   AA_MSAA_16X,
   /// 32x multisample anti-aliasing
   AA_MSAA_32X
};
   
/// How to restrict the position of the system cursor on applicable platforms.
enum SystemCursorPositionLock
{
   /// Do not restrict the position of the system cursor.
   SYSTEM_CURSOR_POSITION_LOCK_NONE,
   /// Restrict the position of the system cursor to a single point in the center
   /// of the client area of the window.
   SYSTEM_CURSOR_POSITION_LOCK_CENTER,
   /// Restrict the position of the system cursor to the client area of the window.
   SYSTEM_CURSOR_POSITION_LOCK_CLIENT_AREA,
   /// Use the behavior of SYSTEM_CURSOR_POSITION_LOCK_CENTER in fullscreen and
   /// the behavior of SYSTEM_CURSOR_POSITION_LOCK_NONE in windowed mode.
   SYSTEM_CURSOR_POSITION_LOCK_CENTER_IN_FULLSCREEN,
   /// Use the behavior of SYSTEM_CURSOR_POSITION_LOCK_CLIENT_AREA in fullscreen
   /// and the behavior of SYSTEM_CURSOR_POSITION_LOCK_NONE in windowed mode.
   SYSTEM_CURSOR_POSITION_LOCK_CLIENT_AREA_IN_FULLSCREEN
};

class WindowMenu;

//===============================================================================

/// Collection of parameters used to initialize the Screen.
/// This is platform-dependent.
class ScreenParameters : public ScreenParametersOpenGL
{
public:
   ScreenParameters()
   {
   #ifdef _DEBUG
      fullscreen = false;
   #else
      fullscreen = true;
   #endif
      outputScaling = ScreenOpenGL::STRETCH_KEEP_ASPECT_RATIO;
      extendedAreaAllowed = false;
      screenSize.x = SCREEN_WIDTH_DEFAULT;
      screenSize.y = SCREEN_HEIGHT_DEFAULT;
      windowTitle = SCREEN_TITLE_DEFAULT;
      vSync = true;
      antiAliasingMode = AA_DISABLED;
      screenshotKey = SCREEN_SCREENSHOT_KEY_DEFAULT;
      debugOneToOneKey = SCREEN_DEBUG_ONE_TO_ONE_KEY_DEFAULT;
      systemCursorHide = true;
      systemCursorPositionLock = SYSTEM_CURSOR_POSITION_LOCK_CLIENT_AREA_IN_FULLSCREEN;
      preparedArea.Set(0, 0, 0, 0);
      windowSizePreferred.Set(0, 0);
      fullscreenSizePreferred.Set(0, 0);
      fullscreenDisplayModeChangeAllowed = false;
      imageScales = NULL;

      simulateDisplaySize = false;
      simulatedDisplaySize = screenSize;
      intermediateFramebufferEnabled = false;
      intermediateFramebufferSize.Set(0, 0);
      intermediateFramebufferNearestSampling = false;

      iconResource = NULL;
      cursorResource = NULL;
      hInstance = NULL;
      wndProc = NULL;
      nCmdShow = 0;
      doubleBufferPixelFormat = true;
      fullscreenBorder = false;
      prepareWindowMenu = false;
      heapID = HEAP_DEFAULT;
      graphicsPath = SCREEN_GRAPHICS_PATH_DEFAULT;
      graphicsPathSecondary = NULL;
   }

   /// True if the game should begin in fullscreen.
   bool fullscreen;
   /// Specifies how the Screen class should handle differences between the
   /// the actual framebuffer resolution and the virtual screen resolution
   /// requested by the application programmer.
   ScreenOpenGL::OutputScaling outputScaling;
   /// False if content should generally be restricted to the area of the
   /// screen that remains after applying letterbox or pillarbox.
   bool extendedAreaAllowed;
   /// Dimensions of the area of the screen available to the application programmer, in pixels.
   Point2I screenSize;
   /// String to be displayed in the window titlebar when running in windowed mode.
   const char* windowTitle;
   /// Set to true to synchronize the framebuffer updates with the display device.
   /// This can reduce tearing, but it can also make the game feel less responsive.
   bool vSync;
   /// Specifies how to anti-alias the output, if at all.
   AntiAliasingMode antiAliasingMode;
   /// When this key is pressed, take a screenshot.
   int screenshotKey;
   /// While this key is pressed in a debug build, zoom to 1:1.
   int debugOneToOneKey;
   /// True if the normal system cursor should be hidden while over the window.
   bool systemCursorHide;
   /// How to restrict the position of the system cursor.
   SystemCursorPositionLock systemCursorPositionLock;
   /// Maximum region for which the game is generally prepared.  This must be
   /// at least as large as the requested size of the screen from the game's
   /// perspective, and it must be centered on that requested area.  Clipping
   /// may be extended out to this area even if the full  extended area is not
   /// allowed.
   Box2I preparedArea;
   /// Preferred size of the framebuffer for windowed mode.
   Point2I windowSizePreferred;
   /// Preferred size of the framebuffer for fullscreen mode.
   Point2I fullscreenSizePreferred;
   /// True if the display mode should be allowed to change for fullscreen
   /// mode.
   bool fullscreenDisplayModeChangeAllowed;
   /// JSON array of JSON objects to specify the provided ImageScales and their
   /// filename suffixes.
   JSONValue* imageScales;

   /// If true, use a window to show an estimate of how the game would look
   /// running at the 'simulatedDisplaySize' resolution as if it was native.
   bool simulateDisplaySize;
   /// Values to use for the desktop size instead of the true desktop size.
   Point2I simulatedDisplaySize;
   /// True if the game should draw to the intermediate framebuffer, rather than
   /// the main one for the window.
   bool intermediateFramebufferEnabled;
   /// Size to be used when drawing to the intermediate framebuffer instead of
   /// the main one.  By default, the preferred window size is used.
   Point2I intermediateFramebufferSize;
   /// True if nearest-heighbor sampling should be used when drawing the
   /// intermediate framebuffer to the main framebuffer, rather than bilinear
   /// filtering.  In other words, if this is true, don't blend between pixels
   /// when zooming in.  This can help simulate low resolution displays more
   /// accurately.
   bool intermediateFramebufferNearestSampling;

   /// Name of the icon resource, if any, to associate with the window.
   WCHAR* iconResource;
   /// Name of the cursor resource, if any, to use by default.
   WCHAR* cursorResource;
   /// Primary path that is implicitly prefixed to all graphics filenames in the
   /// main file system.
   const char* graphicsPath;
   /// If this is not NULL and a path is not found when prefixing with 'graphicsPath',
   /// this can be tried as a secondary graphics path prefix.
   const char* graphicsPathSecondary;
   /// Windows application instance handle.
   HINSTANCE hInstance;
   /// Windows message processesing function.
   WNDPROC wndProc;
   /// Variable passed to WinMain which specifies how the window should be initialized.
   int nCmdShow;
   /// True if a double buffer pixel format should be requested when setting up
   /// the window.  Setting this to false may help when you have a child
   /// "window" in the main window.  However, it caused the main window to be
   /// black in fullscreen on the Windows 8 build machine.
   bool doubleBufferPixelFormat;
   /// True if a border should be requested for the main window when in
   /// fullscreen.  For some reason, this seems to help when you have a child
   /// "window" in the main window in fullscreen.
   bool fullscreenBorder;
   /// True if the Screen should make room for a window menu.
   bool prepareWindowMenu;
   /// Heap used for internal allocations.
   HeapID heapID;
};

//===============================================================================

/// Abstracts the details of setting up, using, and cleaning up the ability
/// to draw to the screen.
class Screen : public ScreenOpenGL
{
public:
   Screen();

   /// Initialize the screen using the given parameters.
   /// Return true if successful and false otherwise.
   bool Init(ScreenParameters* screenParameters);
   /// Destroy the window.
   void Deinit();
   /// Call on every frame.
   void Update();

   /// Call this before beginning the display calls for a particular frame.
   /// On some platforms, this will wait until a vertical interrupt or equivalent. 
   void PreDraw();
   /// Call this when finished making display calls for a particular frame.
   void PostDraw();

   /// Return the area last set by SafeFrameSet.
   /// There is no actual safe frame on the PC.  This is simply used for providing
   /// a preview of the safe frame for an arbitrary platform.
   Box2I SafeFrameGet() { return safeFrame; }
   /// Set the region of the screen to be treated as the safe frame.
   /// There is no actual safe frame on the PC.  This is simply used for providing
   /// a preview of the safe frame for an arbitrary platform.
   void SafeFrameSet(const Box2I& _safeFrame) { safeFrame = _safeFrame; }

   /// Take an RGB8 screenshot and save it to the given file.
   /// If no filename is provided, one will be generated.
   /// If a filename is provided, exclude the extension.
   /// This is not available on all platforms.
   void ScreenshotSave(const char* _filename = NULL, FileManager* fileManager = theFilesPersistence);

   /// Call this with incoming Windows messages.  If this returns true, the
   /// caller should return the value referenced by 'returnValue' rather than
   /// calling DefWindowProc.
   bool MessageNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, long* returnValue);

   /// Windows-only.  Return the HINSTANCE handle for the program.
   HINSTANCE GetHInstance() { return hInstance; }
   /// Windows-only.  Return the HWND handle for the window.
   HWND GetHWnd() { return hWnd; }

   /// Return the coefficient used to compensate for differences in the size
   /// and aspect ratio of the desktop and the game.
   Point2F MouseMovementCoefficientGet() const { return mouseMovementCoefficient; }
   /// Return true if the normal system cursor should be hidden while over the
   /// window.
   bool SystemCursorHideCheck() { return systemCursorHide; }
   /// Called by the MouseManager to notify the Screen whether the cursor is
   /// within the client area of the window.
   void SystemCursorWithinClientAreaSet(bool _systemCursorWithinClientArea);
   /// Return the current setting for locking the system cursor.
   SystemCursorPositionLock SystemCursorPositionLockGet() { return systemCursorPositionLock; }
   /// Set how and when the position of the system cursor should be locked.
   void SystemCursorPositionLockSet(SystemCursorPositionLock _systemCursorPositionLock);

   /// Set the string to be displayed in the window titlebar when running in
   /// windowed mode.  This is not applicable to all platforms.
   void WindowTitleSet(const char* _windowTitle);

   /// Return the native handle of the root menu for window menus.
   HMENU RootMenuGet() { return rootMenu; }
   /// Add the given WindowMenu to the collection.  This will call the object's
   /// 'SubmenusAdd' method.
   void WindowMenuAdd(WindowMenu* windowMenu);
   /// Call this after the structure of the window menus has been changed to
   /// refresh their appearance.
   void WindowMenuRefresh();

   /// Return true if the game is in fullscreen mode, as opposed to windowed
   /// mode.
   virtual bool FullscreenCheck() { return useFullscreen; }
   /// Used to toggle fullscreen mid-game.
   void FullscreenSet(bool _fullscreen);

   /// Return the currently requested type of anti-aliasing, if any.
   AntiAliasingMode AntiAliasingModeGet() { return antiAliasingMode; }
   /// Return true if the screen is currently using MSAA anti-aliasing.
   virtual bool AntiAliasingModeMSAACheck() { return antiAliasingMode != AA_DISABLED; }

   /// Return the size of what could be causing an aspect ratio discrepancy.
   virtual Point2I AspectRatioDiscrepancyCauseSizeGet();

   /// Return the normal desktop resolution.
   virtual Point2I DesktopSizeGet();

   /// Set the requested window resolution.
   void WindowSizePreferredSet(const Point2I& _windowSizePreferred);
   /// Return the requested window resolution.
   Point2I WindowSizePreferredGet() { return windowSizePreferred; }
   /// Return the requested fullscreen resolution.
   Point2I FullscreenSizePreferredGet() { return fullscreenSizePreferred; }

   /// Set to true if the game should draw to the intermediate framebuffer,
   /// rather than the main one for the window.
   void IntermediateFramebufferEnabledSet(bool _intermediateFramebufferEnabled);
   /// Return true if the game should draw to the intermediate framebuffer,
   /// rather than the main one for the window.
   bool IntermediateFramebufferEnabledCheck() { return intermediateFramebufferEnabled; }
   /// Set the size to be used when drawing to the intermediate framebuffer
   /// instead of the main one.
   void IntermediateFramebufferSizeSet(const Point2I& _intermediateFramebufferSize);
   /// Restore the defaults for the intermediate framebuffer size.
   void IntermediateFramebufferSizeClear() { IntermediateFramebufferSizeSet(Point2I::Create(0, 0)); }
   /// Return the scale to compensate for differences in aspect ratio between
   /// the intermediate framebuffer size and the final output.
   Point2F IntermediateFramebufferAspectRatioCompensationGet();

   /// Return the size of the framebuffer fro drawing to the window.
   /// Application programmers should typically ignore this.
   Point2I MainFramebufferSizeGet() { return mainFramebufferSize; }
   /// Return the size to be used for the intermediate framebuffer.
   Point2I IntermediateFramebufferSizeGet();

   /// Copy the contents of the front framebuffer to the given Bitmap.
   /// Be sure to provide an RGBA8 or RGB8 Bitmap with enough space allocated.
   void ScreenshotCopy(Bitmap* screenCopy);

   #if _DEBUG
      virtual Matrix43 DebugOneToOneMatrixGet();
      /// While this key is pressed in a debug build, zoom to 1:1.
      int DebugOneToOneKeyGet() { return debugOneToOneKey; }
   #endif
   
   static Screen instance;

protected:
   typedef ScreenOpenGL Inherited;

   /// Hide or show the cursor, as appropriate.
   void SystemCursorVisibleRefresh();
   /// Lock or rlease the cursor, as appropriate.
   void SystemCursorPositionLockRefresh();
   /// Helper function for creating the main window.  If there is already
   /// a main window, it is deleted first.  Return true if successful.
   bool WindowCreate(ScreenParameters* screenParameters);
   /// Set the size of the window and compensate for the frame size if needed.
   void MainFramebufferSizeSet(const Point2I& _mainFramebufferSize, const Box2I* forcedWin32WindowBox = NULL);
   /// Initialize the pixel format for the screen.
   bool PixelFormatInit(bool doubleBufferPixelFormat);
   /// Without changing the window, set the display settings for fullscreen.
   /// Return true if successful.
   bool FullscreenResolutionBegin();
   /// Without changing the window, restore the desktop display settings.
   void FullscreenResolutionEnd();

   /// Fill the given table with eligible modes for this screen.
   virtual void DisplayModesGet(Table<DisplayMode>* displayModes);

   /// Return the Windows API information for the appropriate window style settings.
   LONG_PTR WindowStyleGet();

   /// Create the intermediate framebuffer based on the current settings.
   void IntermediateFramebufferInit();
   /// Clean up any existing intermediate framebuffer.
   void IntermediateFramebufferDeinit();
   /// If appropriate, create or recreate the intermediate framebuffer based on
   /// the current settings.
   void IntermediateFramebufferRefresh();

   /// Update the position of the debug console window
   void ConsoleWindowPositionRefresh();

   /// The region of the screen to be treated as the safe frame.
   /// There is no actual safe frame on the PC.  This is simply used for providing
   /// a preview of the safe frame for an arbitrary platform.
   Box2I safeFrame;

   /// Windows application instance handle
   HINSTANCE hInstance;
   /// Window handle
   HWND hWnd;
   /// Root menu for the window.
   HMENU rootMenu;
   /// Brush for drawing the default background of a window.
   HBRUSH backgroundBrush;
   /// Used to specify properties of the window.
   ATOM windowsClassRegistration;
   /// True if the game should use fullscreen when it has the focus.
   bool useFullscreen;
   /// Currently requested type of anti-aliasing, if any.
   AntiAliasingMode antiAliasingMode;
   /// True if the screen resolution needs to be set back to normal.
   bool restoreDisplaySettings;
   /// Number of the display mode to use for fullscreen.
   PlatformDisplayMode fullscreenDisplayModeNumber;
   /// Resoution of the display mode indicated by 'fullscreenDisplayModeNumber'.
   Point2I fullscreenDisplayModeSize;
   /// True if 'fullscreenDisplayModeNumber' and 'fullscreenDisplayModeSize' are defined.
   bool fullscreenDisplayModeDefined;
   /// Preferred size of the framebuffer for windowed mode.
   Point2I windowSizePreferred;
   /// Preferred size of the framebuffer for fullscreen mode.
   Point2I fullscreenSizePreferred;
   /// True if the display mode should be allowed to change for fullscreen
   /// mode.
   bool fullscreenDisplayModeChangeAllowed;
   /// True if a border should be requested for the main window when in
   /// fullscreen.  For some reason, this seems to help when you have a child
   /// "window" in the main window in fullscreen.
   bool fullscreenBorder;
   /// Size of the framebuffer for the output window.
   Point2I mainFramebufferSize;
   /// Cached value for the normal desktop resolution in pixels.
   Point2I desktopSize;
   /// True if the desktop size has already been retrieved from the OS.
   bool desktopSizeCached;
   /// If true, use a window to show an estimate of how the game would look
   /// running at the framebuffer resolution as if it was native.
   bool simulateDisplaySize;
   /// When this key is pressed, take a screenshot.
   int screenshotKey;
   /// While this key is pressed in a debug build, zoom to 1:1.
   int debugOneToOneKey;
   /// Used to compensate for differences in the size and aspect ratio of the
   /// desktop and the game.
   Point2F mouseMovementCoefficient;
   /// True if the normal system cursor should be hidden while over the window.
   bool systemCursorHide;
   /// True if the system cursor is currently visible.
   bool systemCursorVisible;
   /// How to restrict the position of the system cursor.
   SystemCursorPositionLock systemCursorPositionLock;
   /// True if the system cursor is believed to be within the client area of
   /// the window.
   bool systemCursorWithinClientArea;
   /// True if the position of the system cursor is restricted.
   bool systemCursorPositionLocked;
   /// True if the Screen should process messages about gaining or losing focus.
   bool focusMessagesAllowed;
   
   /// OpenGL Device Context
   HDC gl_ghDC;
   /// OpenGL Context
   HGLRC gl_ghRC;
   /// Specification of the pixel format
   PIXELFORMATDESCRIPTOR gl_pfd;

   /// True if the game should draw to the intermediate framebuffer, rather than
   /// the main one for the window.
   bool intermediateFramebufferEnabled;
   /// Ssize to be used when drawing to the intermediate framebuffer instead of
   /// the main one.
   Point2I intermediateFramebufferSize;
   /// True if nearest-heighbor sampling should be used when drawing the
   /// intermediate framebuffer to the main framebuffer, rather than bilinear
   /// filtering.  In other words, if this is true, don't blend between pixels
   /// when zooming in.  This can help simulate low resolution displays more
   /// accurately.
   bool intermediateFramebufferNearestSampling;
   /// Native object for the render-to-texture framebuffer.
   GLuint intermediateFramebuffer;
   /// Native object for the color aspect of render-to-texture.
   GLuint intermediateColorRenderTexture;
   /// Native object for the depth aspect of render-to-texture.
   GLuint intermediateDepthRenderbuffer;
   /// Native object for multisample rendering.
   GLuint intermediateMultisampleFramebuffer;
   /// Native object for the color buffer for multisample rendering.
   GLuint intermediateMultisampleColorRenderbuffer;
   /// Native object for the depth buffer for multisample rendering.
   GLuint intermediateMultisampleDepthRenderbuffer;

   /// Values to be used with 'SetDisplayAutoRotationPreferences'
   /// from user32.dll.
   enum FROG_WIN32_ORIENTATION_PREFERENCE
   {
      FROG_WIN32_ORIENTATION_PREFERENCE_NONE = 0x0,
      FROG_WIN32_ORIENTATION_PREFERENCE_LANDSCAPE = 0x1,
      FROG_WIN32_ORIENTATION_PREFERENCE_PORTRAIT = 0x2,
      FROG_WIN32_ORIENTATION_PREFERENCE_LANDSCAPE_FLIPPED = 0x4,
      FROG_WIN32_ORIENTATION_PREFERENCE_PORTRAIT_FLIPPED = 0x8,
   };

   /// Function pointer for SetDisplayAutoRotationPreferences from user32.dll.
   typedef BOOL (WINAPI *FrogSetDisplayAutoRotationPreferences)(FROG_WIN32_ORIENTATION_PREFERENCE);
   FrogSetDisplayAutoRotationPreferences frogSetDisplayAutoRotationPreferences;

   /// Values to be used with GetDpiForMonitor from shcore.dll.
   enum FROG_WIN32_MONITOR_DPI_TYPE
   {
      FROG_WIN32_MDT_EFFECTIVE_DPI,
      FROG_WIN32_MDT_ANGULAR_DPI,
      FROG_WIN32_MDT_RAW_DPI,
      FROG_WIN32_MDT_DEFAULT
   };

   /// Function pointer for GetDpiForWindow from user32.dll.
   typedef UINT (WINAPI *FrogGetDpiForWindow)(HWND);
   FrogGetDpiForWindow frogGetDpiForWindow;
   /// Function pointer for GetDpiForMonitor from shcore.dll.
   typedef HRESULT (WINAPI *FrogGetDpiForMonitor)(HMONITOR, FROG_WIN32_MONITOR_DPI_TYPE, UINT*, UINT*);
   FrogGetDpiForMonitor frogGetDpiForMonitor;

   /// Collection of active WindowMenu objects.
   Table<WindowMenu*> windowMenus;

   #if _DEBUG
      /// Magnification to apply beyond 1:1 when using 1:1 mode.
      float debugOneToOneMagnification;
      /// Timer used to handle multiple taps of the 1:1 hotkey to increase
      /// magnification.
      unsigned int debugOnToOneMagnificationTimer;
   #endif
};

static Screen * const theScreen = &Screen::instance;

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SCREEN_H__

#ifndef __FROG__MOUSEMANAGER_H__
#define __FROG__MOUSEMANAGER_H__

#include "FrogMemory.h"
#include "MouseCommon.h"
#include <windows.h>

namespace Webfoot {

class MouseManager;

/// Specifies how the MouseManager should determine the positions of the mice.
enum MouseTrackMode
{
   /// Follow the position of the system cursor rather than accumulating
   /// movements or tracking multiple mice.
   MOUSE_TRACK_SYSTEM_CURSOR,
   /// Use accumulated mouse movements to determine a single mouse positions.
   MOUSE_TRACK_ACCUMULATED_MOVEMENTS,
   /// Track the accumulated movements of connected mice individually.
   MOUSE_TRACK_MULTIPLE_MICE,
   /// Use the behavior of MOUSE_TRACK_ACCUMULATED_MOVEMENTS when the system
   /// cursor is locked to the center, and use the behavior of
   /// MOUSE_TRACK_SYSTEM_CURSOR when it is not locked the the center.
   MOUSE_TRACK_SYSTEM_CURSOR_WHEN_NOT_LOCKED_TO_CENTER
};

//==============================================================================

/// A mouse class that gives MouseManager full access.
class Mouse : public MouseCommon
{
public:
   void Init(const Point2I& _position, const Box2I& _region);
   
   void PreUpdate();
   void PostUpdate(bool hasFocus);

   typedef MouseCommon Inherited;
   friend class MouseManager;

protected:
   /// Helper function for clearing accumulated data when the way the mouse
   /// is tracked changes.
   void TrackingChangeHelper();

   /// Form of movementUnclipped that allows mouse sensitivity to be applied
   /// before casting to integers.
   Point2F movementUnclippedFloat;
   /// Value of 'movementUnclippedFloat' for the next frame.
   Point2F movementUnclippedFloatNext;
   /// Accumulator for 'scroll'.  This is especially important for the Windows
   /// API.
   float scrollFloatNext;
   /// Value for 'lastInputTouch' for the next frame.
   bool nextLastInputTouch;
};

//==============================================================================

/// Handles a set of mice using the ManyMouse library and the Windows API.
class MouseManager
{
public:
   MouseManager();

   /// Initialize the manager, the mice, and restrict the mice to the given region.
   /// If no region is specified, the bounds of the screen will be used instead.
   void Init(const Box2I* _region = NULL, MouseTrackMode _trackMode = MOUSE_TRACK_SYSTEM_CURSOR_WHEN_NOT_LOCKED_TO_CENTER);
   void Deinit();
   
   /// Update the associated mouse objects.
   /// Set 'hasFocus' to false if the game's window does not currently have the focus.
   /// Certain messages will be ignored accordingly.  This should be called even
   /// when the window does not have focus so that certain messages are still handled.
   void Update(bool hasFocus);

   /// Call this with incoming Windows messages.  If this returns true, the
   /// caller should return the value referenced by 'returnValue' rather than
   /// calling DefWindowProc.  Update must only be called on the same thread as
   /// MessageNotify.
   bool MessageNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, long* returnValue);

   enum
   {
      /// Maximium number of separate mice to track.
      MOUSE_COUNT_MAX = 8
   };
   
   /// Return the maximium number of separate mice to track.
   int MouseCountMaxGet() { return MOUSE_COUNT_MAX; }

   /// Return true if the system cursor is within the client area.
   static bool SystemCursorWithinClientAreaCheck();
   /// Helper function for getting the in-game position based on the
   /// position within the client area.
   static Point2F ClientAreaToGamePosition(const Point2F& positionInClientArea);
   /// Helper function for getting the position within the client area based on
   /// the in-game position.
   static Point2F GameToClientAreaPosition(const Point2F& gamePosition);

   /// True if 'hasFocus' was true in the last call to Update.
   bool lastFocus;
   /// True if the manager has been notified of a device change, but has not yet handled it.
   bool deviceChanged;
   /// Singleton instance
   static MouseManager instance;
   /// A mouse that treats all the mice as one.
   static Mouse mouse;
   /// Separate mouse objects for the individual physical mice.
   static Mouse mice[MOUSE_COUNT_MAX];
   /// Pointers to the individual elements of 'mice' so that 'theMice' can be an array of pointers.
   static Mouse* micePointers[MOUSE_COUNT_MAX];

protected:
   /// Use ManyMouse to update the mice.
   void ManyMouseUpdate(bool hasFocus);
   /// Use the Windows API to update the mice.
   void WindowsMouseUpdate(bool hasFocus);
   /// Based on the current settings, return the mouse track mode which should
   /// effectively be used.  This helps abstract modes which depend on other
   /// settings, like MOUSE_TRACK_SYSTEM_CURSOR_WHEN_NOT_LOCKED_TO_CENTER.
   MouseTrackMode NewTrackModeEffectiveGet();
   /// Helper function for setting up the appropriate underlying APIs for
   /// tracking mice based on the current 'trackModeEffective'.  Return the
   /// number of mice.
   int TrackInitHelper();
   /// Helper function for cleaning up the appropriate underlying API for
   /// tracking mice based on the current 'trackModeEffective'.
   void TrackDeinitHelper();
   /// Convert the given raw absolute input position to game coordinates.
   Point2F RawAbsoluteToGamePosition(Point2I rawAbsolutePosition);

   /// Called when the "OnOSFocusGain" event is triggered.
   void OnOSFocusGain();
   /// Called when the "OnWindowChange" event is triggered.
   void OnWindowChange();

   /// Assuming the Windows API is being used to track the mice, refresh the
   /// button states now.
   void WindowsButtonStatesRefresh();

   /// How should the positions be determined.
   MouseTrackMode trackMode;
   /// Which mousetrack mode is currently effectively used. This helps abstract
   /// modes which depend on other settings, like
   /// MOUSE_TRACK_SYSTEM_CURSOR_WHEN_NOT_LOCKED_TO_CENTER.
   MouseTrackMode trackModeEffective;
   /// True if a region was explicitly specified in the call to Init.
   bool regionExplicitlySpecified;
   /// True if the system was within the client area on the last update.
   bool lastSystemCursorWithinClientArea;

   /// True if the operating system supports calls to touch-related functions.
   bool osSupportsTouch;
   /// Function pointer for RegisterTouchWindow from user32.dll.
   typedef BOOL (WINAPI *FrogRegisterTouchWindowType)(HWND, ULONG);
   FrogRegisterTouchWindowType frogRegisterTouchWindow;
   /// Function pointer for UnregisterTouchWindow from user32.dll.
   typedef BOOL (WINAPI *FrogUnregisterTouchWindowType)(HWND);
   FrogUnregisterTouchWindowType frogUnregisterTouchWindow;
   /// Function pointer for GetTouchInputInfo from user32.dll.
   typedef BOOL (WINAPI *FrogGetTouchInputInfoType)(HTOUCHINPUT, UINT, PTOUCHINPUT, int);
   FrogGetTouchInputInfoType frogGetTouchInputInfo;
   /// Function pointer for CloseTouchInputHandle from user32.dll.
   typedef BOOL (WINAPI *FrogCloseTouchInputHandleType)(HTOUCHINPUT);
   FrogCloseTouchInputHandleType frogCloseTouchInputHandle;

   friend class Mouse;
};

/// Pointer the the singleton instance.
MouseManager* const theMouseManager = &MouseManager::instance;
/// A mouse object that takes input from all connected physical mice.
Mouse* const theMouse = &MouseManager::mouse;
/// Separate mouse objects for the individual physical mice.
Mouse** const theMice = (Mouse**)(MouseManager::micePointers);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__MOUSEMANAGER_H__

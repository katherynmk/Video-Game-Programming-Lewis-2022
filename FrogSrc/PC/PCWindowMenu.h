#ifndef __FROG__PCWINDOWMENU_H__
#define __FROG__PCWINDOWMENU_H__

#include "FrogMemory.h"
#include "WindowMenu.h"
#include "Table.h"
#include "Point2.h"

namespace Webfoot {

class JSONValue;

//===============================================================================

/// PCWindowMenu is intended for use in PC-only debug builds.  It provides a
/// way to request changes to things like the window resolution even if the game
/// has no user-facing setting for this in release builds.
class PCWindowMenu : public WindowMenu
{
public:
   PCWindowMenu();

   void Init(JSONValue* _gameSettings);
   void Deinit();

   /// Add the given windowSize to the list of sizes that should be available in
   /// the window size menu.  This must only be called before 'SubmenusAdd'.
   void WindowSizeAdd(const Point2I& _windowSize);

   /// Called to allow the WindowMenu to add its content to the root window menu.
   virtual void SubmenusAdd();

   /// This is called with incoming Windows messages.  If this returns true, the
   /// caller should return the value referenced by 'returnValue'.
   virtual bool MessageNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, long* returnValue);

   static PCWindowMenu instance;

protected:
   /// Return the handle for the options menu, and create it if needed.
   HMENU OptionsMenuGet();
   /// Refresh which device has a checkmark next to it in the window size menu.
   void WindowSizeCheckedRefresh();
   /// Comparator used to sort the window sizes.
   static bool WindowSizeComparator(const Point2I& a, const Point2I& b);

   /// True if between Init and Deinit calls.
   bool initialized;
   /// Submenu that lists window sizes.
   HMENU windowSizeMenu;
   /// Submenu for misc options.
   HMENU optionsMenu;
   /// Main collection of game-specific settings.
   JSONValue* gameSettings;
   /// Window size to use as the default setting.
   Point2I windowSizeDefault;
   /// Window sizes to be included in the menu.
   Table<Point2I> windowSizes;
};

PCWindowMenu* const thePCWindowMenu = &PCWindowMenu::instance;

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PCWINDOWMENU_H__

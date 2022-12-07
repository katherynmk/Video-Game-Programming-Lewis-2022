#ifndef __FROG__GUI__POPUPMANAGER_H__
#define __FROG__GUI__POPUPMANAGER_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

class Popup;

//==============================================================================

/// PopupManager handles the collection of active Popups.  It provides a single
/// location from which they can be added, removed, and updated.
class PopupManager
{
public:
   void Init();
   void Deinit();
   void Update(unsigned int dt);

   /// Show the given popup.
   void Push(Popup* popup);
   /// Remove the given popup.
   void Remove(Popup* popup);
   /// Remove all current popups.
   void Clear();

   /// Return the current number of popups.
   int PopupCountGet() { return popups.SizeGet(); }
   /// Return the popup with the given index.
   Popup* PopupGet(int index);
   /// Return the index of the given popup or -1 if it isn't found.
   int PopupIndexGet(Popup* popup);

   /// Refresh the text for all the popups.  This is called automatically
   /// when the language changes.
   void TextRefresh();

   static PopupManager instance;

protected:
   /// Called after a state has been deinitialized.
   void GameStatePostDeinit();

   /// Collection of popups currently being managed.
   Table<Popup*> popups;
   /// Copy of 'popups' that is used internally to more safely iterate over
   /// popups.
   Table<Popup*> popupsCopy;
   /// True if the manager is in the middle of an update.
   bool updating;
};

PopupManager* const thePopups = &PopupManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__POPUPMANAGER_H__

#ifndef __FROG__PROGRESSIONMANAGER_H__
#define __FROG__PROGRESSIONMANAGER_H__

#include "FrogMemory.h"
#include "Map.h"

namespace Webfoot {

class Progression;

//==============================================================================

/// ProgressionManager is a registry for associating names with 
/// Progression objects.  Games can register their own project-specific
/// progressions, but some progressions are automatically registered and
/// available to all projects: Linear, Smooth, SmoothStart, and SmoothStop.
///
/// This has no dependencies on the GUIManager, but if it is not already
/// initialized when theGUI is initialized, the theGUI will
/// automatically initialize theProgressions and deinitalize it when theGUI
/// receives its Deinit call.  
class ProgressionManager
{
public:
   ProgressionManager();

   void Init();
   void Deinit();

   /// Associate the given progression with the given name.
   void Register(const char* name, Progression* progression);
   /// Return the progression with the given name.
   Progression* Get(const char* name);
   
   /// Return true if this is currently initialized.
   bool InitializedCheck() { return initialized; }

   static ProgressionManager instance;
   
protected:
   typedef Map<const char*, Progression*> ProgressionMap;
   
   /// Map of names to Progressions.
   ProgressionMap progressions;
   /// True if the manager is currently initialized.
   bool initialized;
};

ProgressionManager* const theProgressions = &ProgressionManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PROGRESSIONMANAGER_H__

#ifndef __FROG__GRAPHICSPATHMANAGER_H__
#define __FROG__GRAPHICSPATHMANAGER_H__

#include "FrogMemory.h"
#include "FileManager.h"
#include "TableStatic.h"

namespace Webfoot {

//==============================================================================

/// Helper object for keeping track of a given graphics path.
struct GraphicsPathEntry
{
   GraphicsPathEntry() { path[0] = '\0'; }

   char path[FROG_PATH_MAX];
};

//==============================================================================

/// Manages the paths that are implicitly prefixed to all graphics filenames in the
/// main file system.  Path indices that are numerically lower are given higher
/// priority.
class GraphicsPathManager
{
public:
   enum
   {
      /// Maximum number of graphics paths that can be active at once.
      GRAPHICS_PATHS_COUNT_MAX = 3
   };
   
   /// Return the number of active graphics paths.
   int CountGet() { return paths.SizeGet(); }
   /// Return the graphics path at the given index.
   const char* Get(int pathIndex);
   /// Set the graphics path with the given index.
   void Set(int pathIndex, const char* path);

   static GraphicsPathManager instance;

protected:
   /// Collection of active graphics paths of decreasing priority. 
   TableStatic<GraphicsPathEntry, GRAPHICS_PATHS_COUNT_MAX> paths;
};

GraphicsPathManager* const theGraphicsPaths = &GraphicsPathManager::instance;

//==============================================================================

} // namespace Webfoot {

#endif //#ifdef __FROG__GRAPHICSPATHMANAGER_H__

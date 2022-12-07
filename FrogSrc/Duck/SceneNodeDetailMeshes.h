#ifndef __FROG__DUCK__SCENENODEDETAILMESHES_H__
#define __FROG__DUCK__SCENENODEDETAILMESHES_H__

#include "FrogMemory.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Base class for drawing groups of detail meshes.
class SceneNodeDetailMeshes : public SceneNode
{
public:
   /// Given the current position of the camera, refresh the appearance of all
   /// detail meshes, even when this would ordinarily be broken up over
   /// multiple frames.
   virtual void RefreshFull() = 0;

   typedef SceneNode Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEDETAILMESHES_H__

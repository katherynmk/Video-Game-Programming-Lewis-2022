#ifndef __FROG__DUCK__SCENENODECAMERA_H__
#define __FROG__DUCK__SCENENODECAMERA_H__

#include "FrogMemory.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Scene node for controlling the camera.
class SceneNodeCamera : public SceneNode
{
public:
   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeCamera(); }

   typedef SceneNode Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODECAMERA_H__

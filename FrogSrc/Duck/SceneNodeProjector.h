#ifndef __FROG__DUCK__SCENENODEPROJECTOR_H__
#define __FROG__DUCK__SCENENODEPROJECTOR_H__

#include "FrogMemory.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

/// Default priority number for projectors.
#define DUCK_SCENE_NODE_PROJECTOR_PROJECTOR_PRIORITY_DEFAULT 1.0f

//==============================================================================

/// Base class for nodes that project something.
class SceneNodeProjector : public SceneNode
{
public:
   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);

   /// Apply the projector to the scene.
   virtual void ApplyToScene() = 0;

   /// Set the priority of the projector.  Projections with numerically higher
   /// priority will be drawn after those of lower priority.  The default
   /// priority is 0.0.
   void ProjectorPrioritySet(float _projectorPriority) { projectorPriority = _projectorPriority; }
   /// Return the priority of the projector.
   float ProjectorPriorityGet() { return projectorPriority; }

   static bool ProjectorPriorityComparator(const SceneNodeProjector* valueA, const SceneNodeProjector* valueB)
   {
      return valueA->projectorPriority < valueB->projectorPriority;
   }

   typedef SceneNode Inherited;

protected:
   /// Priority number for this projector.
   float projectorPriority;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEPROJECTOR_H__

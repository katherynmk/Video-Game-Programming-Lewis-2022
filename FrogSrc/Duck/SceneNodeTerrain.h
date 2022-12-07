#ifndef __FROG__DUCK__SCENENODETERRAIN_H__
#define __FROG__DUCK__SCENENODETERRAIN_H__

#include "FrogMemory.h"
#include "Duck/SceneNode.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

//==============================================================================

/// Base class for terrain scene nodes.
class SceneNodeTerrain : public SceneNode
{
public:
   typedef SceneNode Inherited;

   /// Return the height of the terrain at the given horizontal position in
   /// local coordinates.
   virtual float TerrainHeightGet(const Point2F& position) = 0;
   /// Return the normal of the terrain at the given horizontal position in
   /// local coordanates.
   virtual Point3F TerrainNormalGet(const Point2F& position) = 0;
   /// Return the horizontal bounds of the terrain in local coordinates.
   virtual Box2F TerrainBoundsHorizontalGet() = 0;

protected:
   /// Type of a helper function to IntersectGridHelper for testing a single
   /// quad.  Return true if an intersection was found.
   typedef bool (*IntersectQuadHelper)(SceneNodeTerrain* sceneNodeTerrain, const Ray3& rayLocalSpace,
      SceneNodeRayIntersectResult* intersectResult, float& intersectDistanceScaled, const Point2I& southwestQuadIndices);

   /// Helper function used for ray-terrain intersection when the terrain is
   /// grid-based.
   void IntersectGridHelper(const Ray3& rayLocalSpace, float rayLengthScale, SceneNodeRayIntersectResult* intersectResult,
      float& intersectDistanceScaled, float _terrainScaleHorizontal, float terrainHeightMin, float terrainHeightMax,
      const Point2I& _heightmapResolution, IntersectQuadHelper intersectQuadHelper);
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODETERRAIN_H__

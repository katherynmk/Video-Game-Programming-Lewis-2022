#ifndef __FROG__FROG3D__RENDERDATA_H__
#define __FROG__FROG3D__RENDERDATA_H__

#include "FrogMemory.h"
#include "Matrix43.h"
#include "Matrix44.h"
#include "Color.h"
#include "Table.h"
#include "Frustum.h"
#include "Quaternion.h"

namespace Webfoot {

class RenderSceneNode;
class LightSceneNode;
class ProjectorSceneNode;
class MaterialSceneNode;

//=============================================================================

struct RenderData
{
   RenderData()
   {
      stereo = false;
      hmdOrientation = Quaternion::Create();
      nextSortedNode = NULL;
      forcedMaterial = NULL;
      lights = NULL;
      projectors = NULL;
      time = 0.0f;
      fogEnabled = false;
      fogDistanceBegin = 0.0f;
      fogDistanceEnd = 0.0f;
      fogColor.red = 0;
      fogColor.green = 0;
      fogColor.blue = 0;
   }
   
   /// Number of seconds by which to advance time.
   float dtSeconds;
   
   /// This is not a global time.  This is used as a parameter to control the
   /// time at which controllers should perform sampling.
   float time;
   
   int cacheId;

   Matrix44 projectionMatrix;
   Matrix43 cameraMatrix;
   Matrix43 modelMatrix;

   /// If doing stereo rendering, this provides the camera matrix excluding any
   /// offset added for stereoscopy.
   Matrix43 cameraMatrixMiddle;
   /// True if doing stereo rendering.
   bool stereo;
   /// Orientation of the head-mounted display.
   Quaternion hmdOrientation;

   /// Return the camera matrix excluding any offsets for stereo rendering.
   inline Matrix43& CameraMatrixMiddleGet() { return stereo ? cameraMatrixMiddle : cameraMatrix; }

   ColorRGB8 ambientLight;

   /// True if fog should be enabled.
   bool fogEnabled;
   /// Distance at which fog should begin to be visible.
   float fogDistanceBegin;
   /// Distance beyond which only fog should be visible.
   float fogDistanceEnd;
   /// Color to use for the fog.
   ColorRGB8 fogColor;

   /// True if currently drawing only fully opaque objects.
   bool renderingOpaqueNodes;
   /// True if currently building a list of nodes to sort.
   bool buildingSortedNodeList;
   /// Helper for counting the number of nodes to sort.
   int numberOfSortedNodes;
   /// Helper for the list of nodes to sort.
   RenderSceneNode* nextSortedNode;

   /// Used to render with an alternate material.  This may not work on all
   /// platforms.
   MaterialSceneNode* forcedMaterial;

   /// Collection of lights.  This is only available while rendering.
   Table<LightSceneNode*>* lights;
   /// Collection of projectors.  This is only available while rendering.
   Table<ProjectorSceneNode*>* projectors;

   /// Used for view frustum culling.
   Frustum viewFrustum;
   
   static RenderData dummyInstance;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__RENDERDATA_H__

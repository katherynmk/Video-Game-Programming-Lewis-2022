#ifndef __FROG__FROG3D__SCENECOMMON_H__
#define __FROG__FROG3D__SCENECOMMON_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

struct RenderData;
class SceneNode;
struct IntersectRecord;
struct Ray3;
class RootSceneNode;
class RenderSceneNode;
class LightSceneNode;
class ProjectorSceneNode;
class CameraController;

//=============================================================================

/// Base class for Scene implementations.
class SceneCommon
{
public:
   SceneCommon();

   void Init();
   void Deinit();

   void NodeAdd(SceneNode* node);
   void NodeRemove(SceneNode* node);

   /// Add a node to the sky.
   void SkyNodeAdd(SceneNode* node);
   /// Remove a node from the sky.
   void SkyNodeRemove(SceneNode* node);

   /// Early update method that mainly increments time and invalidates cached values.
   void UpdateEarly(RenderData* renderData);
   /// Update function to be called after UpdateEarly.
   void Update(RenderData* renderData);

   void Intersect(RenderData* renderData, const Ray3& ray, IntersectRecord& intersectRecord);

   void DebugPrint();

   SceneNode* NodeFirstGet();
   SceneNode* NodeGetByName(const char* name);

   /// Refresh collections of special nodes that are not likely to change every
   /// frame.  For example, this will build a list of lights and projectors.
   void SpecialNodesGather();
   /// Clear the collections of special nodes.
   void SpecialNodesClear();

   /// Restore the default camera controller.
   void CameraControllerReset();
   /// Set the active camera controller.
   void CameraControllerSet(CameraController* newCameraController);
   /// Return the current camera controller.
   CameraController* CameraControllerGet() { return cameraController; }
   /// Update the current CameraController.
   void CameraControllerUpdate(RenderData* renderData);

   /// Return the main root scene node.
   RootSceneNode* RootSceneNodeGet() { return rootSceneNode; }
   /// Return the sky root scene node.
   RootSceneNode* SkyRootSceneNodeGet() { return skyRootSceneNode; }

protected:
   /// Helper function for building collections of lights and projectors.
   void SpecialNodesGatherHelper(SceneNode* sceneRoot, Table<LightSceneNode*>* lights,
      Table<ProjectorSceneNode*>* projectors);
   /// Helper function for building collections of lights and projectors.
   void SpecialNodesGatherHelper(SceneNode* sceneNode, int* lightCount, LightSceneNode** nextLight,
      int* projectorCount, ProjectorSceneNode** nextProjector);
   /// Prepare the collections of lights and projectors for rendering.
   void SpecialNodesPreRender(RenderData* renderData, SceneNode* sceneRoot);
   /// Clean up the collections of lights and projectors after rendering.
   void SpecialNodesPostRender(RenderData* renderData);

   /// Comparator for sorting RenderSceneNodes by depth.
   static bool RenderSceneNodeDepthComparator(const RenderSceneNode* first, const RenderSceneNode* second);

   RootSceneNode* rootSceneNode;
   RootSceneNode* skyRootSceneNode;
   CameraController* cameraController;

   /// Cache of lights in 'rootSceneNode'.
   Table<LightSceneNode*> sceneLights;
   /// Cache of lights in 'skyRootSceneNode'.
   Table<LightSceneNode*> skyLights;
   /// Cache of projectors in 'rootSceneNode'.
   Table<ProjectorSceneNode*> sceneProjectors;
   /// Cache of projectors in 'skyRootSceneNode'.
   Table<ProjectorSceneNode*> skyProjectors;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENECOMMON_H__

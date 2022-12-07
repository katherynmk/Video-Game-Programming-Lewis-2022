#ifndef __FROG__DUCK__ENTITY_H__
#define __FROG__DUCK__ENTITY_H__

#include "FrogMemory.h"
#include "Color.h"
#include "Point3.h"
#include "Quaternion.h"
#include "Table.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

class SceneNode;
class SceneNodeParticleEffect;

/// Name which determines which entity class should be instantiated.
#define DUCK_ENTITY_TYPE_KEY "Type"
/// Name of an entity.
#define DUCK_ENTITY_NAME_KEY "Name"
/// Scene file to display for the entity.
#define DUCK_ENTITY_SCENE_FILENAME_KEY "SceneFilename"
/// Alternate scene file to display for the entity in the editor.
#define DUCK_ENTITY_EDITOR_SCENE_FILENAME_KEY "EditorSceneFilename"
/// Scene node to display for the entity.
#define DUCK_ENTITY_SCENE_NODE_KEY "SceneNode"
/// Alternate scene node to display for the entity in the editor.
#define DUCK_ENTITY_EDITOR_SCENE_NODE_KEY "EditorSceneNode"
/// Position of an entity.
#define DUCK_ENTITY_POSITION_KEY "Position"
extern const Point3F DUCK_ENTITY_POSITION_DEFAULT;
/// Position of an entity relative to the surface of the terrain at that
/// horizontal point.
#define DUCK_ENTITY_POSITION_OVER_TERRAIN_KEY "PositionOverTerrain"
/// Rotation of an entity
#define DUCK_ENTITY_ROTATION_KEY "Rotation"
extern const Quaternion DUCK_ENTITY_ROTATION_DEFAULT;
/// Scale of an entity.
#define DUCK_ENTITY_SCALE_KEY "Scale"
extern const Point3F DUCK_ENTITY_SCALE_DEFAULT;
/// Specifications for the AnimationPlayer to be associated with the entity.
#define DUCK_ENTITY_ANIMATION_KEY "Animation"
/// True if a debug sphere should be shown for an entity.
#define DUCK_ENTITY_DEBUG_SPHERE_VISIBLE_KEY "DebugSphereVisible"
/// Radius of a debug sphere to be drawn around an entity.
#define DUCK_ENTITY_DEBUG_SPHERE_RADIUS_KEY "DebugSphereRadius"
extern const float DUCK_ENTITY_DEBUG_SPHERE_RADIUS_DEFAULT;
/// Color of a debug sphere to be drawn around an entity.
#define DUCK_ENTITY_DEBUG_SPHERE_COLOR_KEY "DebugSphereColor"
extern const ColorRGBA8 DUCK_ENTITY_DEBUG_SPHERE_COLOR_DEFAULT;
/// If defined, local position should be restricted to a multiple of this
/// number by default.
#define DUCK_ENTITY_EDITOR_POSITION_INCREMENT_KEY "EditorPositionIncrement"
/// If defined, local rotation should be restricted to a multiple of this
/// number by default.
#define DUCK_ENTITY_EDITOR_ROTATION_INCREMENT_KEY "EditorRotationIncrement"
/// Set of particle effects to be created along with an entity.
#define DUCK_ENTITY_PARTICLE_EFFECTS_KEY "ParticleEffects"
/// Name to be used for the particle effect scene node.
#define DUCK_ENTITY_PARTICLE_EFFECT_NAME_KEY "Name"
/// Filename with the specifications for a given particle effect.
#define DUCK_ENTITY_PARTICLE_EFFECT_PARTICLE_EFFECT_KEY "ParticleEffect"
/// Name of the scene node within the entity to which the particle effect
/// should be attached.
#define DUCK_ENTITY_PARTICLE_EFFECT_SCENE_NODE_PARENT "SceneNodeParent"
/// True if the particle effect should initially be active.
#define DUCK_ENTITY_PARTICLE_EFFECT_PARTICLE_EMISSION_ACTIVE_KEY "ParticleEmissionActive"
#define DUCK_ENTITY_PARTICLE_EFFECT_PARTICLE_EMISSION_ACTIVE_DEFAULT true

/// Helper function for setting up particle effects for an entity.
void EntityParticleEffectsInit(JSONValue* entitySpecs, SceneNode* entitySceneNode,
   Table<SceneNodeParticleEffect*>* entityParticleEffects);

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__ENTITY_H__

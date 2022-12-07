#ifndef __FROG__FROG3D__PARTICLEEFFECT3D_H__
#define __FROG__FROG3D__PARTICLEEFFECT3D_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Table.h"
#include "FileManager.h"
#include "Particle3D.h"
#include "ParticleEmitter3D.h"
#include "SceneNodes.h"

namespace Webfoot {

class JSONValue;
class ParticleModifier3D;
class ParticleEmitterModifier3D;

//==============================================================================

/// A ParticleEffectSceneNode is a collection of emitters and particles that follow
/// an external specification rather than primarily hard-coded behavior.
class ParticleEffectSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   ParticleEffectSceneNode();

   /// Initialize the effect to be empty, but reserve space for this many
   /// emitters.
   void Init(int emitterCount = 1, HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   /// Initialize the effect based on the settings in the given file.
   /// The path automatically includes the graphics path.  Do not include the
   /// extension.
   void Init(RenderData* renderData, const char* filename, FileManager* fileManager = theFiles,
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   /// Initialize the effect based on the given settings.  'filename' can be
   /// specified for debug output purposes.
   void Init(RenderData* renderData, JSONValue* specifications, HeapID _heapID = HEAP_DEFAULT,
      HeapID _tempHeapID = HEAP_TEMP, const char* filename = NULL);
   
   virtual void Deinit();
   
   /// Add the given emitter to the manager.
   void EmitterAdd(ParticleEmitterSceneNode* emitter);
   /// Without deiniting or deleting the given emitter, remove it from the collection.
   /// Return true if successful.
   bool EmitterRemove(ParticleEmitterSceneNode* emitter);
   
   /// Deinit, delete, and remove all the emitters in the effect, including their particles.
   void EmittersClear();

   /// Start automatically emitting particles for as long as the emission
   /// active durations of the individual emitters permit.  If a countdown is
   /// already in progress, this will reset it.  Newly initialized
   /// emitters emit indefinitely.
   void EmissionActiveBegin(RenderData* renderData);
   /// Stop automatically releasing particles.
   void EmissionActiveEnd();
   /// Return true if any emitter is currently emitting particles automatically over time.
   bool EmissionActiveCheck();

   /// Perform any pre-simulation now.
   void PreSimulate(RenderData* renderData);

   /// Return the total number of active particles across all emitters in the effect.
   int ParticlesActiveCountGet();

   /// Expire all currently active particles.  Newly expired particles will
   /// not necessarily be available again until the emitter is updated.
   void AllExpire();

   /// Return the number of emitters in the effect.
   int EmitterCountGet() { return emitters.SizeGet(); }
   /// Return the given emitter.
   ParticleEmitterSceneNode* EmitterGet(int emitterIndex);

   virtual void DebugPrint(int depth);
   virtual void Intersect(RenderData*, const Ray3&, IntersectRecord&) { }

   typedef TransformSceneNode<GroupSceneNode> Inherited;
   
protected:
   /// Initialize the effect based on the settings in the given file.
   /// The path automatically includes the graphics path.  Do not include the
   /// extension.
   void InitHelper(RenderData* renderData, const char* filename, FileManager* fileManager,
      HeapID _heapID, HeapID _tempHeapID);
   void InitHelper(RenderData* renderData, JSONValue* specifications, HeapID _heapID,
      HeapID _tempHeapID, const char* filename);
   void DeinitHelper();

#if defined _DEBUG && PLATFORM_IS_WINDOWS
   /// Called when the program gains the focus of the OS.
   void OnOSFocusGain();
   /// Return the last time the specifications file was modified.
   int64 SpecsFileModificationTimeGet();
   /// Filename provided to load the specs file.
   const char* specsGivenFilename;
   /// Path to check for changes in the specifications file.
   const char* specsTrueFilename;
   /// FileManager used to load the specifications file.
   /// Used to keep track of whether the specifications file has changed.
   FileManager* specsFileManager;
   /// Time that the specifications file was last modified.
   /// Used to keep track of whether the specifications file has changed.
   int64 specsFileModificationTime;
#endif //#if defined _DEBUG && PLATFORM_IS_WINDOWS

   /// Long-term allocations
   HeapID heapID;
   /// Short-term allocations.
   HeapID tempHeapID;
   
   /// Collection of emitters that must be updated regularly.
   Table<ParticleEmitterSceneNode*> emitters;
};

//==============================================================================

/// ParticleEffectEmitterSceneNode is a ParticleEmitterSceneNode that applies the settings of
/// a ParticleEffectSceneNode.  It is the base class of all ParticleEmitterSceneNode's that
/// are created automatically by ParticleEffectSceneNode.  Do not directly use the Init
/// methods inherited from ParticleEmitterSceneNode.
class ParticleEffectEmitterSceneNode : public ParticleEmitterSceneNode
{
public:
   ParticleEffectEmitterSceneNode();
   
   /// Create the emitter and its particles according to the given specifications.  
   /// 'filename' may be provided for debug output purposes.
   void Init(JSONValue* specs, ParticleEffectSceneNode* _particleEffect,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP, const char* filename = NULL);
   
   /// Clean up.
   virtual void Deinit();

   /// Called by the ParticleEffectSceneNode on every frame.
   virtual void Update(RenderData* renderData);

   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle3D* Emit(RenderData* renderData);
   
   /// Add the given modifier to this emitter.  When the emitter is
   /// deinitialized, the modifier will be deinitialized and deleted.
   void ModifierAdd(ParticleEmitterModifier3D* modifier) { modifiers.Add(modifier); }

   /// Set the position of the emission relative to the emitter.
   void EmissionPointSet(const Point3F& point) { emissionPoint = point; }
   /// Set the A position of the emission line relative to the emitter.
   void EmissionLinePointASet(const Point3F& point) { emissionLinePointA = point; }
   /// Set the B position of the emission line relative to the emitter.
   void EmissionLinePointBSet(const Point3F& point) { emissionLinePointB = point; }
   /// Set the region of the emission box relative to the emitter.
   void EmissionBoxSet(const Box3F& box) { emissionBox = box; }

   typedef ParticleEmitterSceneNode Inherited;

protected:
   /// Shape of the area from which particles are emitted.
   enum EmissionShape
   {
      /// Particles are emitted at any one time come from a single point.
      EMISSION_SHAPE_POINT,
      /// Particles are emitted from a random point along a line.
      EMISSION_SHAPE_LINE,
      /// Particles are emitted from a random point in a rectangle. 
      EMISSION_SHAPE_BOX_FILLED,
      /// Number of emitter shape types.
      EMISSION_SHAPE_COUNT
   };
   
   /// Effect of which this emitter is a part.
   ParticleEffectSceneNode* particleEffect;
   /// Shape of the area from which particles are emitted. 
   EmissionShape emissionShape;
   /// If using EMISSION_SHAPE_POINT, particles are emitted from this point.
   Point3F emissionPoint;
   /// If using EMISSION_SHAPE_LINE, particles are emitted along a line using
   /// this endpoint.
   Point3F emissionLinePointA;
   /// If using EMISSION_SHAPE_LINE, particles are emitted along a line using
   /// this endpoint.
   Point3F emissionLinePointB;
   /// If using EMISSION_SHAPE_BOX_FILLED, particles are emitted from a random point in
   /// this rectangle.
   Box3F emissionBox;
   
   /// A quaternion that rotates an up vector (0, 0, 1) to point in the base
   /// direction in which particles should be emitted.
   Quaternion particleLinearVelocityOrientation;
   /// Cached value related to the minimum amount of deviation from the
   /// requested direction.
   float particleLinearVelocityDirectionDeviationMin;
   /// Cached value related to the maximum amount of deviation from the
   /// requested direction.
   float particleLinearVelocityDirectionDeviationMax;
   /// Minimum azimuth angle at which particles are emitted, in degrees.
   float particleLinearVelocityAzimuthMin;
   /// Maximum azimuth angle at which particles are emitted, in degrees.
   float particleLinearVelocityAzimuthMax;
   /// Minimum linear speed of a particle that has just been emitted, in
   /// world units/second.
   float particleLinearSpeedMin;
   /// Maximum linear speed of a particle that has just been emitted, in
   /// world units/second.
   float particleLinearSpeedMax;

   /// Initial acceleration for particles when they are emitted, in
   /// world units/second squared. 
   Point3F particleLinearAcceleration;
   
   /// Minimum linear damping for a particle when emitted.
   float particleLinearDampingMin;
   /// Maximum linear damping for a particle when emitted.
   float particleLinearDampingMax;

   /// Minimum angle of rotation for a particle when emitted, in degrees.
   float particleRotationMin;
   /// Maximum angle of rotation for a particle when emitted, in degrees.
   float particleRotationMax;

   /// Minimum angular velocity for a particle when emitted
   /// in degrees/second.
   float particleAngularVelocityMin;
   /// Maximum angular velocity for a particle when emitted
   /// in degrees/second.
   float particleAngularVelocityMax;
   
   /// Minimum angular acceleration for a particle when emitted
   /// in degrees/second squared.
   float particleAngularAccelerationMin;
   /// Maximum angular acceleration for a particle when emitted
   /// in degrees/second squared.
   float particleAngularAccelerationMax;
   
   /// Minimum angular damping for a particle when emitted.
   float particleAngularDampingMin;
   /// Maximum angular damping for a particle when emitted.
   float particleAngularDampingMax;

   /// True if particleLifeArea should be used.
   bool particleUseLifeArea;
   /// If the origin of the particle leaves this area, in screen coordinates, its 
   /// life ends.  This is not affected by the position of the emitter or the
   /// effect as a whole.
   Box3F particleLifeArea;
   /// Minimum duration of the life of a particle in milliseconds.  If this is
   /// -1, it will not expire due to time alone. 
   int particleLifeDurationMin;
   /// Maximum duration of the life of a particle in milliseconds.
   int particleLifeDurationMax;
   
   /// Minimum size of a particle at the beginning of its lifetime.
   Point2F particleSizeMin;
   /// Maximum size of a particle at the beginning of its lifetime.
   Point2F particleSizeMax;

   /// Initial color of a particle.
   ColorRGBA8 particleColor;
   
   /// Initial additive blending of a particle.
   float particleAdditiveBlending;
   
   /// Objects that influence the emitter
   Table<ParticleEmitterModifier3D*> modifiers;

   /// True if the emitter should perform pre-simulation when initialized.
   bool preSimulateOnInit;
   
   friend class ParticleEffectSceneNode;
};

//==============================================================================

/// ParticleEffectParticle3D is the base class of all particles created
/// automatically by a ParticleEffectEmitterSceneNode.  Do not directly use the Init
/// methods inherited from Particle3D.
class ParticleEffectParticle3D : public Particle3D
{
public:
   virtual ~ParticleEffectParticle3D() {}
   
   void Init(HeapID heapID);
   virtual void Deinit();

   /// Start a new life for the particle.
   virtual void Activate(RenderData* renderData);
   /// Called every frame by the ParticleEmitterSceneNode.
   virtual void Update(RenderData* renderData);
   
   /// Add the given modifier to this particle.  When the particle is
   /// deinitialized, the modifier will be deinitialized and deleted.
   void ModifierAdd(ParticleModifier3D* modifier) { modifiers.Add(modifier); }

   typedef Particle3D Inherited;

protected:
   /// Objects that influence the particle
   Table<ParticleModifier3D*> modifiers;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FROG3D__PARTICLEEFFECT3D_H__

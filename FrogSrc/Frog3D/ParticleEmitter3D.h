#ifndef __FROG__FROG3D__PARTICLEEMITTER3D_H__
#define __FROG__FROG3D__PARTICLEEMITTER3D_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Box2.h"
#include "List.h"
#include "SceneNodes.h"

namespace Webfoot {

class Particle3D;
struct RenderData;
struct IntersectRecord;
class ParticleEffectSceneNode;

/// By default, don't pre-simulate.
const unsigned int PARTICLE_EMITTER_3D_PRE_SIMULATE_DURATION_DEFAULT = 0;
/// By default, use approximately 30fps to pre-simulate.
const unsigned int PARTICLE_EMITTER_3D_PRE_SIMULATE_STEP_SIZE_DEFAULT = 33;

//==============================================================================

/// Base class for all objects that emit 3D particles.  Rather than generating
/// particles, the emitter uses a finite pool of particles that have been added
/// with ParticleAdd().  When creating an emitter, call Init() with the number
/// of particles you intend to add to the pool.  You can optionally add it to a
/// ParticleEffectSceneNode.  When an emitter is deleted, it deletes all the particles
/// that have been added to it.  Emitters can activate particles intermittently
/// based on their emissionFrequency or explicitly by calling Emit().
///
/// You can set up a particle emitter to use the particle collection of another
/// emitter.  Most of the methods of these dependent emitters will do nothing.
/// For instance, querying for stats like the number of active particles returns
/// zero, and any particles you create must still be added to the original
/// emitter.
class ParticleEmitterSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   enum 
   {
      EMISSION_ACTIVE_INDEFINITELY = -1
   };

   ParticleEmitterSceneNode();

   void Init();
   /// Initialize the emitter to use the particles owned by the given emitter.
   void Init(ParticleEmitterSceneNode* _source);
   virtual void Deinit();
   
   /// Called by the ParticleEffectSceneNode on every frame.
   virtual void Update(RenderData* renderData);

   /// Reserve space for the given number of particles, excluding particle
   /// SceneNodes.
   virtual void ParticlesReserve(int _reserveCapacity);
   
   /// Add a particle to the pool of inactive particles.
   virtual void ParticleAdd(Particle3D* particle);
   
   /// Without deiniting or deleting it, remove the given particle from the collection.
   /// Return true if the particle was found and removed.
   virtual bool ParticleRemove(Particle3D* particle);
   
   /// Deinit, delete, and remove all particles, both active and inactive, owned by this emitter.
   virtual void ParticlesClear();

   /// Start automatically emitting particles for as long as the emission
   /// active duration permits.  If the countdown is already in progress, this will
   /// reset it.  Newly initialized emitters emit indefinitely.
   virtual void EmissionActiveBegin(RenderData* renderData);
   /// Stop automatically releasing particles.
   virtual void EmissionActiveEnd() { emissionActiveTimeRemaining = 0; }
   /// Return true if the emitter is currently emitting particles automatically over time.
   virtual bool EmissionActiveCheck() { return emissionActiveTimeRemaining != 0; }
   
   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle3D* Emit(RenderData* renderData) = 0;

   /// Set a pointer to the ParticleEffect2D that updates this emitter.
   /// This should only be called by the effect in question.
   void EffectSet(ParticleEffectSceneNode* _effect) { effect = _effect; }
   
   /// Return the number of chances to to emit a particle per second.
   float EmissionFrequencyGet() { return emissionFrequency; }
   /// Set the number of chances to to emit a particle per second.
   void EmissionFrequencySet(float _emissionFrequency) { emissionFrequency = _emissionFrequency; }
   
   /// Return the probability of emitting a particle when the emission frequency
   /// determines it's time to attempt to release one.
   float EmissionChanceGet() { return emissionChance; }
   /// Set the probability of emitting a particle when the emission frequency
   /// determines it's time to attempt to release one.  This should be a
   /// number between 0 and 1.  The default is 1.
   void EmissionChanceSet(float _emissionChance) { emissionChance = _emissionChance; }

   /// Set the number of milliseconds the emitter will emit particles after being
   /// activated.  If this is EMISSION_ACTIVE_INDEFINITELY, emission activity
   /// will not be limited by time.
   void EmissionActiveDurationSet(int _emissionActiveDuration);
   /// Return the number of milliseconds the emitter will emit particles after being
   /// activated.  If this is EMISSION_ACTIVE_INDEFINITELY, emission activity
   /// will not be limited by time.
   int EmissionActiveDurationGet() { return emissionActiveDuration; }

   /// Use the given private seed for random numbers.  If particles of this
   /// emitter are owned by another emitter, they should internally get random
   /// numbers from the other emitter.
   void SeedSet(unsigned int _privateSeed1, unsigned int _privateSeed2)
   {
      privateSeed1 = _privateSeed1;
      privateSeed2 = _privateSeed2;
      usePrivateSeed = true;
   }
   /// Stop using the private seed for random numbers.
   void SeedClear() { usePrivateSeed = false; }
   /// Similar to FrogMath::RandomF, except this optionally uses a private seed.
   float RandomF();
   
   /// Return the number of active particles.
   int ParticlesActiveCountGet();
   /// Return the number of inactive particles.
   int ParticlesInactiveCountGet();

   /// Expire all currently active particles.  Newly expired particles will
   /// not necessarily be available again until the emitter is updated.
   void AllExpire();

   /// When EmissionActiveBegin() is called, automatically perform updates for this
   /// many milliseconds so that it doesn't look like the effect just started.
   /// This should ideally be a multiple of 'preSimulateStepSize'.
   /// Note that the base ParticleEmitterSceneNode does not call EmissionActiveBegin()
   /// when it is initialized, so this won't happen immediately for all implementations.
   unsigned int PreSimulateDurationGet() { return preSimulateDuration; }
   /// See PreSimulateDurationGet()
   void PreSimulateDurationSet(unsigned int _preSimulateDuration) { preSimulateDuration = _preSimulateDuration; }

   /// When doing pre-simulate updates due to preSimulateDuration, use this
   /// as the duration of each step, in milliseconds.
   unsigned int PreSimulateStepSizeGet() { return preSimulateStepSize; }
   /// See PreSimulateStepSizeGet()
   void PreSimulateStepSizeSet(unsigned int _preSimulateStepSize) { preSimulateStepSize = _preSimulateStepSize; }

   /// Perform any pre-simulation now.
   void PreSimulate(RenderData* renderData);
   
   /// Return a pointer to the collection of active particles.
   List<Particle3D*>* ActiveParticlesGet() { return &activeParticles; }
   /// Return a pointer to the collection of inactive particles.
   List<Particle3D*>* InactiveParticlesGet() { return &inactiveParticles; }

   virtual void DebugPrint(int depth);
   virtual void Intersect(RenderData*, const Ray3&, IntersectRecord&) { }

   typedef TransformSceneNode<GroupSceneNode> Inherited;
   
protected:
   /// Move a particle from the pool of inactive particles to the active particle
   /// group, and return a pointer to it.  If there are no inactive particles
   /// available, return NULL.  This does not call the Activate member of
   /// the particle itself.
   Particle3D* EmitHelper();

   /// Handle initialization common to different forms of Init.
   void InitHelper();

   /// True if Init has been called.
   bool initialized;
   
   /// Number of milliseconds before the emitter stops emitting particles or
   /// EMISSION_ACTIVE_INDEFINITELY if emission activity is not currently
   /// limited by time.
   int emissionActiveTimeRemaining;
   /// Number of milliseconds the emitter will emit particles after being
   /// activated.  If this is EMISSION_ACTIVE_INDEFINITELY, emission activity
   /// will not be limited by time.
   int emissionActiveDuration;
   /// Number of chances to emit a particle per second.
   float emissionFrequency;
   /// Used to keep track of when to emit particles.
   float particlesToEmit;
   /// Probability of emitting a particle when the emission frequency
   /// determines it's time to attempt to release one.
   float emissionChance;
   /// When EmissionActiveBegin() is called, automatically perform updates for this
   /// many milliseconds so that it doesn't look like the effect just started.
   /// This should ideally be a multiple of 'preSimulateStepSize'.
   /// Note that the base ParticleEmitterSceneNode does not call EmissionActiveBegin()
   /// when it is initialized, so this won't happen immediately for all implementations.
   unsigned int preSimulateDuration;
   /// When doing pre-simulate updates due to preSimulateDuration, use this
   /// as the duration of each step, in milliseconds.
   unsigned int preSimulateStepSize;

   /// True if the private seed should be used for this emitter's random
   /// numbers.
   bool usePrivateSeed;
   /// Part of this emitter's private seed.
   unsigned int privateSeed1;
   /// Part of this emitter's private seed.
   unsigned int privateSeed2;

   /// Pointer to the particle effect, if any, that owns this emitter.
   ParticleEffectSceneNode* effect;

   /// Particle emitter that this emitter should use for the active and inactive particle lists.
   ParticleEmitterSceneNode* source;
   
   /// Collection of active particles owned by this emitter.
   List<Particle3D*> activeParticles;
   /// Collection of inactive particles owned by this emitter.
   List<Particle3D*> inactiveParticles;
};

//==============================================================================

/// This emits particles from a single point, relative to the emitter.
class ParticleEmitterPointSceneNode : public ParticleEmitterSceneNode
{
public:
   virtual ~ParticleEmitterPointSceneNode() {}
   
   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle3D* Emit(RenderData* renderData);
   
   /// Set the position from which to emit particles.
   virtual void EmissionPointSet(const Point3F& _position)
   {
      position = _position;
   }
   
protected:
   typedef ParticleEmitterSceneNode Inherited;
   
   /// Position from which to emit particles.
   Point3F position;
};

//==============================================================================

/// This emits particles at random points between two given points, relative to
/// the emitter.
class ParticleEmitterLineSceneNode : public ParticleEmitterSceneNode
{
public:
   virtual ~ParticleEmitterLineSceneNode() {}

   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle3D* Emit(RenderData* renderData);
   
   /// Set the two points between which particles will be emitted.
   virtual void EmissionLineSet(const Point3F& _endPoint1,
                                const Point3F& _endPoint2)
   {
      endPoint1 = _endPoint1;
      endPoint2 = _endPoint2;
   }

protected:
   typedef ParticleEmitterSceneNode Inherited;

   /// First endpoint of the line from which to emit particles.
   Point3F endPoint1;
   /// Second endpoint of the line from which to emit particles.
   Point3F endPoint2;
};

//==============================================================================

/// This emits particles at random points in a given box, relative to the
/// emitter.
class ParticleEmitterBoxSceneNode : public ParticleEmitterSceneNode
{
public:
   virtual ~ParticleEmitterBoxSceneNode() {}

   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle3D* Emit(RenderData* renderData);
   
   /// Set the area in which to emit particles.
   virtual void EmissionBoxSet(const Box3F& _emissionArea)
   {
      emissionArea = _emissionArea;
   }

protected:
   typedef ParticleEmitterSceneNode Inherited;

   /// Box in which to emit particles.
   Box3F emissionArea;
};

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__PARTICLEEMITTER3D_H__

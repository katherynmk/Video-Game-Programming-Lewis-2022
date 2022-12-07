#ifndef __FROG__PARTICLEEMITTER2D_H__
#define __FROG__PARTICLEEMITTER2D_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Box2.h"
#include "List.h"

namespace Webfoot {

class Particle2D;
class ParticleEffect2D;

/// By default, don't pre-simulate.
const unsigned int PARTICLE_EMITTER_2D_PRE_SIMULATE_DURATION_DEFAULT = 0;
/// By default, use approximately 30fps to pre-simulate.
const unsigned int PARTICLE_EMITTER_2D_PRE_SIMULATE_STEP_SIZE_DEFAULT = 33;

//==============================================================================

/// Base class for all objects that emit 2D particles.  Rather than generating
/// particles, the emitter uses a finite pool of particles that have been added
/// with Add().  When creating an emitter, call Init() with the number of particles
/// you intend to add to the pool.  You can optionally add it to a ParticleEffect2D.
/// When an emitter is deleted, it deletes all the particles that have been added
/// to it.  Emitters can activate particles intermittently based on their
/// emissionFrequency or explicitly by calling Emit().
///
/// To draw the active particles under the control of an emitter, call the emitter's
/// Draw method at the appropriate time.  The particles will be drawn in the
/// order of the active list.
///
/// You can set up a particle emitter to use the particle collection of another
/// emitter.  Most of the methods of these dependent emitters will do nothing.
/// For instance, calling the Draw method of the dependent emitter will do
/// nothing, querying for stats like the number of active particles returns zero,
/// and any particles you create must still be added to the original emitter.
/// You must still call Update and Deinit on the dependent one, although adding
/// it to a ParticleEffect2D will take care of that for you.
class ParticleEmitter2D
{
public:
   enum 
   {
      EMISSION_ACTIVE_INDEFINITELY = -1
   };

   ParticleEmitter2D();
   virtual ~ParticleEmitter2D() {}
   
   /// Initialize the emitter and reserve space for the given number of particles.
   void Init(int _reserveCapacity);
   /// Initialize the emitter to use the particles owned by the given emitter.
   void Init(ParticleEmitter2D* _source);
   /// Clean up.
   virtual void Deinit();
   
   /// Called by the ParticleEffect2D on every frame.
   virtual void Update(unsigned int dt);

   /// Draw all the active particles in the order of the active list.
   virtual void Draw();
   
   /// Add a particle to the pool of inactive particles.
   virtual void Add(Particle2D* particle);
   
   /// Without deiniting or deleting it, remove the given particle from the collection.
   /// Return true if the particle was found and removed.
   virtual bool Remove(Particle2D* particle);
   
   /// Deinit, delete, and remove all particles, both active and inactive, owned by this emitter.
   virtual void Clear();

   /// Start automatically emitting particles for as long as the emission
   /// active duration permits.  If the countdown is already in progress, this will
   /// reset it.  Newly initialized emitters emit indefinitely.
   virtual void EmissionActiveBegin();
   /// Stop automatically releasing particles.
   virtual void EmissionActiveEnd() { emissionActiveTimeRemaining = 0; }
   /// Return true if the emitter is currently emitting particles automatically over time.
   virtual bool EmissionActiveCheck() { return emissionActiveTimeRemaining != 0; }
   
   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle2D* Emit() = 0;
   
   /// Set a pointer to the ParticleEffect2D that updates this emitter.
   /// This should only be called by the effect in question.
   void EffectSet(ParticleEffect2D* _effect) { effect = _effect; }
   
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

   /// Set the number of milliseconds after a emitter becomes active before
   /// emitting particles.
   void EmissionStartDelaySet(unsigned int _emissionStartDelay);
   /// Return the number of milliseconds after a emitter becomes active before
   /// emitting particles.
   unsigned int EmissionStartDelayGet() { return emissionStartDelay; }

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
   /// Note that the base ParticleEmitter2D does not call EmissionActiveBegin()
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
   void PreSimulate();
   
   /// Return a pointer to the collection of active particles.
   List<Particle2D*>* ActiveParticlesGet() { return &activeParticles; }
   /// Return a pointer to the collection of inactive particles.
   List<Particle2D*>* InactiveParticlesGet() { return &inactiveParticles; }
   
protected:
   /// Move a particle from the pool of inactive particles to the active particle
   /// group, and return a pointer to it.  If there are no inactive particles
   /// available, return NULL.  This does not call the Activate member of
   /// the particle itself.
   Particle2D* EmitHelper();

   /// Handle initialization common to different forms of Init.
   void InitHelper();

   /// True if Init has been called.
   bool initialized;

   /// Number of milliseconds after a emitter becomes active before emitting
   /// particles.
   unsigned int emissionStartDelay;
   /// Number of milliseconds remaining before the emission of particles
   /// should begin.
   unsigned int emissionStartDelayRemaining;
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
   /// Note that the base ParticleEmitter2D does not call EmissionActiveBegin()
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
   ParticleEffect2D* effect;

   /// Particle emitter that this emitter should use for the active and inactive particle lists.
   ParticleEmitter2D* source;
   
   /// Collection of active particles owned by this emitter.
   List<Particle2D*> activeParticles;
   /// Collection of inactive particles owned by this emitter.
   List<Particle2D*> inactiveParticles;
};

//==============================================================================

/// This emits particles from a single point.
class ParticleEmitterPoint2D : public ParticleEmitter2D
{
public:
   virtual ~ParticleEmitterPoint2D() {}
   
   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle2D* Emit();
   
   /// Set the position from which to emit particles.
   virtual void EmissionPointSet(const Point2F& _position)
   {
      position = _position;
   }
   
protected:
   typedef ParticleEmitter2D Inherited;
   
   /// Position from which to emit particles.
   Point2F position;
};

//==============================================================================

/// This emits particles at random points between two given points.
class ParticleEmitterLine2D : public ParticleEmitter2D
{
public:
   virtual ~ParticleEmitterLine2D() {}

   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle2D* Emit();
   
   /// Set the two points between which particles will be emitted.
   virtual void EmissionLineSet(const Point2F& _endPoint1,
                                const Point2F& _endPoint2)
   {
      endPoint1 = _endPoint1;
      endPoint2 = _endPoint2;
   }

protected:
   typedef ParticleEmitter2D Inherited;

   /// First endpoint of the line from which to emit particles.
   Point2F endPoint1;
   /// Second endpoint of the line from which to emit particles.
   Point2F endPoint2;
};

//==============================================================================

/// This emits particles at random points in a given box.
class ParticleEmitterBox2D : public ParticleEmitter2D
{
public:
   virtual ~ParticleEmitterBox2D() {}

   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle2D* Emit();
   
   /// Set the area in which to emit particles.
   virtual void EmissionBoxSet(const Box2F& _emissionArea)
   {
      emissionArea = _emissionArea;
   }

protected:
   typedef ParticleEmitter2D Inherited;

   /// Box in which to emit particles.
   Box2F emissionArea;
};

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__PARTICLEEMITTER2D_H__

#ifndef __FROG__PARTICLEEFFECT2D_H__
#define __FROG__PARTICLEEFFECT2D_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Table.h"
#include "FileManager.h"
#include "Particle2D.h"
#include "ParticleEmitter2D.h"

namespace Webfoot {

class JSONValue;
class ParticleModifier2D;
class ParticleEmitterModifier2D;

//==============================================================================

/// A ParticleEffect2D is a collection of emitters and particles that follow
/// an external specification rather than primarily hard-coded behavior.
class ParticleEffect2D
{
public:
   ParticleEffect2D();

   /// Initialize the effect to be empty, but reserve space for this many
   /// emitters.
   void Init(int emitterCount = 1, HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   /// Initialize the effect based on the settings in the given file.
   /// The path automatically includes the graphics path.  Do not include the
   /// extension.
   void Init(const char* filename, FileManager* fileManager = theFiles,
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   /// Initialize the effect based on the given settings.  'filename' can be
   /// specified for debug output purposes.
   void Init(JSONValue* specifications, HeapID _heapID = HEAP_DEFAULT,
      HeapID _tempHeapID = HEAP_TEMP, const char* filename = NULL);
   void Deinit();

   /// Update the particles and emitters.
   void Update(unsigned int dt);
   /// Draw the emitters in the order they were added, including their particles.
   /// Do not delete or remove any particles or emitters while drawing.
   /// Feel free to call Draw on the individual emitters rather than calling
   /// this method.
   void Draw();
   
   /// Add the given emitter to the manager.
   void Add(ParticleEmitter2D* emitter);
   /// Without deiniting or deleting the given emitter, remove it from the collection.
   /// Return true if successful.
   bool Remove(ParticleEmitter2D* emitter);
   
   /// Deinit, delete, and remove all the emitters in the effect, including their particles.
   void Clear();
   
   /// Return the offset applied to the position of a particle when emitted, in
   /// screen coordinates.
   Point2F PositionGet() { return position; }
   /// Set the offset applied to the position of a particle when emitted, in
   /// screen coordinates.
   void PositionSet(const Point2F& _position) { position = _position; }
   
   /// Return the offset applied to the angle of a particle when emitted, 
   /// in degrees increasing counter-clockwise.
   float RotationGet() { return rotation; }
   /// Set the offset applied to the angle of a particle when emitted, in
   /// degrees increasing counter-clockwise.
   void RotationSet(float _rotation) { rotation = _rotation; }

   /// Start automatically emitting particles for as long as the emission
   /// active durations of the individual emitters permit.  If a countdown is
   /// already in progress, this will reset it.  Newly initialized
   /// emitters emit indefinitely.
   void EmissionActiveBegin();
   /// Stop automatically releasing particles.
   void EmissionActiveEnd();
   /// Return true if any emitter is currently emitting particles automatically over time.
   bool EmissionActiveCheck();

   /// Perform any pre-simulation now.
   void PreSimulate();

   /// Return the total number of active particles across all emitters in the effect.
   int ParticlesActiveCountGet();

   /// Expire all currently active particles.  Newly expired particles will
   /// not necessarily be available again until the emitter is updated.
   void AllExpire();

   /// Return the number of emitters in the effect.
   int EmitterCountGet() { return emitters.SizeGet(); }
   /// Return the given emitter.
   ParticleEmitter2D* EmitterGet(int emitterIndex);
   
protected:
   /// Initialize the effect based on the settings in the given file.
   /// The path automatically includes the graphics path.  Do not include the
   /// extension.
   void InitHelper(const char* filename, FileManager* fileManager,
      HeapID _heapID, HeapID _tempHeapID);
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
   /// Offset applied to newly emitted particles in screen coordinates.
   Point2F position;
   /// Offset applied to the angle of a particle when emitted, in degrees
   /// increasing counter-clockwise.
   float rotation;
   
   /// Collection of emitters that must be updated regularly.
   Table<ParticleEmitter2D*> emitters;
};

//==============================================================================

/// ParticleEffectEmitter2D is a ParticleEmitter2D that applies the settings of
/// a ParticleEffect2D.  It is the base class of all ParticleEmitter2D's that
/// are created automatically by ParticleEffect2D.  Do not directly use the Init
/// methods inherited from ParticleEmitter2D.
class ParticleEffectEmitter2D : public ParticleEmitter2D
{
public:
   /// Create the emitter and its particles according to the given specifications.  
   /// 'filename' may be provided for debug output purposes.
   void Init(JSONValue* specs, ParticleEffect2D* _particleEffect,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP, const char* filename = NULL);
   
   /// Clean up.
   virtual void Deinit();

   /// Called by the ParticleEffect2D on every frame.
   virtual void Update(unsigned int dt);
   /// Draw all the active particles in the order of the active list.
   virtual void Draw();

   /// Emit a particle based on the rules of this emitter.
   /// Return a pointer to the particle if successful or NULL if unsuccessful.
   virtual Particle2D* Emit();
   
   /// Add the given modifier to this emitter.  When the emitter is
   /// deinitialized, the modifier will be deinitialized and deleted.
   void ModifierAdd(ParticleEmitterModifier2D* modifier) { modifiers.Add(modifier); }

   /// Set the position of the emission relative to the emitter position.
   void EmissionPointSet(const Point2F& point) { emissionPoint = point; }
   /// Set the A position of the emission line relative to the emitter position.
   void EmissionLinePointASet(const Point2F& point) { emissionLinePointA = point; }
   /// Set the B position of the emission line relative to the emitter position.
   void EmissionLinePointBSet(const Point2F& point) { emissionLinePointB = point; }
   /// Set the region of the emission box relative to the emitter position.
   void EmissionBoxSet(const Box2F& box) { emissionBox = box; }

   typedef ParticleEmitter2D Inherited;

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
   ParticleEffect2D* particleEffect;
   /// Shape of the area from which particles are emitted. 
   EmissionShape emissionShape;
   /// If using EMISSION_SHAPE_POINT, particles are emitted from this point.
   Point2F emissionPoint;
   /// If using EMISSION_SHAPE_LINE, particles are emitted along a line using
   /// this endpoint.
   Point2F emissionLinePointA;
   /// If using EMISSION_SHAPE_LINE, particles are emitted along a line using
   /// this endpoint.
   Point2F emissionLinePointB;
   /// If using EMISSION_SHAPE_BOX_FILLED, particles are emitted from a random point in
   /// this rectangle.
   Box2F emissionBox;
   
   /// True if 'particleLinearVelocity' should be used instead of the
   /// variables for the range.  
   bool particleInitialLinearVelocityConstant;
   /// Initial velocity for particles when they are emitted, in
   /// pixels/second.
   Point2F particleLinearVelocity;
   /// Minimum angle at which a particle is emitted, in degrees
   float particleLinearVelocityAngleMin;
   /// Maximum angle at which a particle is emitted, in degrees.
   float particleLinearVelocityAngleMax;
   /// Minimum linear speed of a particle that has just been emitted, in
   /// pixels/second.
   float particleLinearSpeedMin;
   /// Maximum linear speed of a particle that has just been emitted, in
   /// pixels/second.
   float particleLinearSpeedMax;
   
   /// True if 'particleLinearAcceleration' should be used instead of the
   /// variables for the range. 
   bool particleInitialLinearAccelerationConstant;
   /// Initial acceleration for particles when they are emitted, in
   /// pixels/second squared. 
   Point2F particleLinearAcceleration;
   /// Minimum angle of the linear acceleration at which particles are emitted
   /// in pixels/second squared.
   float particleLinearAccelerationAngleMin;
   /// Maximum angle of the linear acceleration at which particles are emitted
   /// in degrees.
   float particleLinearAccelerationAngleMax;
   /// Minimum magnitude of the linear acceleration at which particles are 
   /// emitted in pixels/second squared.
   float particleLinearAccelerationMagnitudeMin;
   /// Maximum magnitude of the linear acceleration at which particles are 
   /// emitted in pixels/second squared.
   float particleLinearAccelerationMagnitudeMax;
   
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
   Box2F particleLifeArea;
   /// Minimum duration of the life of a particle in milliseconds.  If this is
   /// -1, it will not expire due to time alone. 
   int particleLifeDurationMin;
   /// Maximum duration of the life of a particle in milliseconds.
   int particleLifeDurationMax;
   
   /// Minimum scale of a particle at the beginning of its lifetime.
   Point2F particleScaleMin;
   /// Maximum scale of a particle at the beginning of its lifetime.
   Point2F particleScaleMax;

   /// Initial color of a particle.
   ColorRGBA8 particleColor;
   
   /// Initial additive blending of a particle.
   float particleAdditiveBlending;
   
   /// Objects that influence the emitter
   Table<ParticleEmitterModifier2D*> modifiers;

   /// True if the emitter should perform pre-simulation when initialized.
   bool preSimulateOnInit;
   
   friend class ParticleEffect2D;
};

//==============================================================================

/// ParticleEffectParticle2D is the base class of all particles created
/// automatically by a ParticleEffectEmitter2D.  Do not directly use the Init
/// methods inherited from Particle2D.
class ParticleEffectParticle2D : public Particle2D
{
public:
   ParticleEffectParticle2D();

   void Init(HeapID heapID);
   /// Initialize the particle with the given sprite animation from the given file.
   /// Do not include the extension when specifying the filename.
   void Init(const char* resourceFilename, const char* animationName, HeapID heapID);
   virtual void Deinit();

   /// Called every frame by the ParticleEmitter2D.
   virtual void Update(unsigned int dt);
   /// Start a new life for the particle.
   virtual void Activate();
   /// End the particle's life.
   /// The particle will not necessarily be available again until the particle system is updated.
   virtual void Expire();

   /// Add the given modifier to this particle.  When the particle is
   /// deinitialized, the modifier will be deinitialized and deleted.
   void ModifierAdd(ParticleModifier2D* modifier) { modifiers.Add(modifier); }

   typedef Particle2D Inherited;

protected:
   /// Set the particle effect for emitting a trail.
   void TrailParticleEffectSet(ParticleEffect2D* _trailParticleEffect) { trailParticleEffect = _trailParticleEffect; }
   /// Set the particle effect to be activated when the particle expires.
   void ExpireParticleEffectSet(ParticleEffect2D* _expireParticleEffect) { expireParticleEffect = _expireParticleEffect; }

   /// Objects that influence the particle
   Table<ParticleModifier2D*> modifiers;
   /// Particle effect for emitting a trail.
   ParticleEffect2D* trailParticleEffect;
   /// Particle effect played when the particle expires.
   ParticleEffect2D* expireParticleEffect;

   friend class ParticleEffectEmitter2D;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PARTICLEEFFECT2D_H__

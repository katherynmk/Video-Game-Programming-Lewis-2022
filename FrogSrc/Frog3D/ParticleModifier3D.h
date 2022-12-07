#ifndef __FROG__FROG3D__PARTICLEMODIFIER3D_H__
#define __FROG__FROG3D__PARTICLEMODIFIER3D_H__

#include "FrogMemory.h"
#include "Color.h"
#include "RenderData.h"

namespace Webfoot {

class ParticleEffectParticle3D;
class ParticleEffectEmitterSceneNode;
class Particle3D;
class Progression;

//==============================================================================

/// ParticleModifier3D is a base class for objects attached to individual
/// particles to affect something.  For example, they could change the color
/// of the particle over time or add accelerations according to different
/// conditions.
class ParticleModifier3D
{
public:
   ParticleModifier3D() { particle = NULL; }
   virtual ~ParticleModifier3D() {}
   
   void Init(ParticleEffectParticle3D* _particle) { particle = _particle; }
   virtual void Deinit() { particle = NULL; }
   
   /// Called on every update of the associated particle.
   virtual void Update(RenderData* renderData) { (void)renderData; }
   /// Called when the associated particle is activated to begin a new life.
   virtual void OnActivate(RenderData* renderData) { (void)renderData; }
   
protected:
   /// Particle affected by this modifier.
   ParticleEffectParticle3D* particle;
};

//==============================================================================

/// ParticleEmitterModifier3D is a base class for objects that affect groups
/// of particles or the emitter itself.
class ParticleEmitterModifier3D
{
public:
   ParticleEmitterModifier3D() { emitter = NULL; }
   virtual ~ParticleEmitterModifier3D() {}
   
   void Init(ParticleEffectEmitterSceneNode* _emitter) { emitter = _emitter; }
   virtual void Deinit() { emitter = NULL; }
   
   /// Called on every update of the associated emitter.
   virtual void Update(RenderData* renderData) { (void)renderData; }
   /// Called when a particle is emitted by the associated emitter, just before
   /// the particle is activated.
   virtual void OnEmit(RenderData* renderData, Particle3D* particle) { (void)renderData; (void)particle; }

protected:
   /// Emitter affected by this modifier.
   ParticleEffectEmitterSceneNode* emitter;
};

//==============================================================================

/// ParticleEmitterModifierColorLinear3D is a ParticleEmitterModifier3D that
/// interpolates a particle's color over time according to a given progression.
/// This is only valid if the normal duration of the particle's life is defined.  
class ParticleEmitterModifierColorLinear3D : public ParticleEmitterModifier3D
{
public:
   virtual ~ParticleEmitterModifierColorLinear3D() {}
   
   void Init(ParticleEffectEmitterSceneNode* _emitter, const ColorRGBA8& _colorBegin,
      const ColorRGBA8& _colorEnd, Progression* _progression);
   
   virtual void Update(RenderData* renderData);
   virtual void OnEmit(RenderData* renderData, Particle3D* particle);
   
   typedef ParticleEmitterModifier3D Inherited;
   
protected:
   /// Color of a particle at the beginning of its life.
   ColorRGBA8 colorBegin;
   /// Color of a particle at the end of it's normal lifespan.
   ColorRGBA8 colorEnd;
   /// Interpolation will follow this progression.
   Progression* progression;
};

//==============================================================================

/// ParticleEmitterModifierAdditiveBlendingLinear3D is a ParticleEmitterModifier3D that
/// interpolates a particle's additive blending setting over time according to a given progression.
/// This is only valid if the normal duration of the particle's life is defined.  
class ParticleEmitterModifierAdditiveBlendingLinear3D : public ParticleEmitterModifier3D
{
public:
   virtual ~ParticleEmitterModifierAdditiveBlendingLinear3D() {}
   
   void Init(ParticleEffectEmitterSceneNode* _emitter, float _additiveBlendingBegin,
      float _additiveBlendingEnd, Progression* _progression);
   
   virtual void Update(RenderData* renderData);
   virtual void OnEmit(RenderData* renderData, Particle3D* particle);
   
   typedef ParticleEmitterModifier3D Inherited;
   
protected:
   /// Additive blending of a particle at the beginning of its life.
   float additiveBlendingBegin;
   /// Additive blending of a particle at the end of it's normal lifespan.
   float additiveBlendingEnd;
   /// Interpolation will follow this progression.
   Progression* progression;
};

//==============================================================================

/// ParticleModifierSizeLinear3D is a ParticleModifier3D that interpolates a
/// particle's size over time according to a given progression.  This is only
/// valid if the normal duration of the particle's life is defined.
class ParticleModifierSizeLinear3D : public ParticleModifier3D
{
public:
   virtual ~ParticleModifierSizeLinear3D() {}

   void Init(ParticleEffectParticle3D* _particle, const Point2F& _sizeBeginMin,
      const Point2F& _sizeBeginMax, const Point2F& _sizeEndMin,
      const Point2F& _sizeEndMax, Progression* _progression);

   /// Called on every update of the associated particle.
   virtual void Update(RenderData* renderData);
   /// Called when the associated particle is activated to begin a new life.
   virtual void OnActivate(RenderData* renderData);

   typedef ParticleModifier3D Inherited;

protected:
   /// Minimum size of a particle at the beginning of its lifetime.
   Point2F sizeBeginMin;
   /// Maximum size of a particle at the beginning of its lifetime.
   Point2F sizeBeginMax;
   /// Minimum size of a particle at the beginning of its lifetime.
   Point2F sizeEndMin;
   /// Maximum size of a particle at the beginning of its lifetime.
   Point2F sizeEndMax;
   /// Interpolation will follow this progression.
   Progression* progression;
   /// Initial size to use for the current life of the particle.
   Point2F sizeBegin;
   /// Final size to use for the current life of the particle.
   Point2F sizeEnd;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FROG3D__PARTICLEMODIFIER3D_H__

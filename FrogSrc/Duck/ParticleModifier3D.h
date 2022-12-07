#ifndef __FROG__DUCK__PARTICLEMODIFIER3D_H__
#define __FROG__DUCK__PARTICLEMODIFIER3D_H__

#include "FrogMemory.h"
#include "Color.h"
#include "SplineLinear.h"

namespace Webfoot {

class Progression;

namespace Duck {

class ParticleEffectParticle3D;
class SceneNodeParticleEffectEmitter;
class Particle3D;

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
   virtual void Update(unsigned int dt) { (void)dt; }
   /// Called when the associated particle is activated to begin a new life.
   virtual void OnActivate() {}
   
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
   
   void Init(SceneNodeParticleEffectEmitter* _emitter) { emitter = _emitter; }
   virtual void Deinit() { emitter = NULL; }
   
   /// Called on every update of the associated emitter.
   virtual void Update(unsigned int dt) { (void)dt; }
   /// Called when a particle is emitted by the associated emitter, just before
   /// the particle is activated.
   virtual void OnEmit(Particle3D* particle) { (void)particle; }

protected:
   /// Emitter affected by this modifier.
   SceneNodeParticleEffectEmitter* emitter;
};

//==============================================================================

/// ParticleEmitterModifierColorLinear3D is a ParticleEmitterModifier3D that
/// interpolates a particle's color over time according to a given progression.
/// This is only valid if the normal duration of the particle's life is defined.  
class ParticleEmitterModifierColorLinear3D : public ParticleEmitterModifier3D
{
public:
   virtual ~ParticleEmitterModifierColorLinear3D() {}
   
   void Init(SceneNodeParticleEffectEmitter* _emitter, const ColorRGBA8& _colorBegin,
      const ColorRGBA8& _colorEnd, Progression* _progression);
   
   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle3D* particle);
   
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

/// ParticleEmitterModifierColorLinear3D is a ParticleEmitterModifier3D that
/// interpolates a particle's color over time according to a SplineLinear.
/// By default, this is only valid if the normal duration of the particle's
/// life is defined.  In this case, the times should be normalized from 0 to
/// 1000 milliseconds, as though the particle's life was 1 second long.  The
/// sampling of the spline is adjusted based on the actual expected life of the
/// particle.  However, it is also possible to request that the times specified
/// in the spline be used as-is.  In this case, the normal duration of the
/// particle's life need not be defined.
class ParticleEmitterModifierColorSplineLinear3D : public ParticleEmitterModifier3D
{
public:
   virtual ~ParticleEmitterModifierColorSplineLinear3D() {}

   void Init(SceneNodeParticleEffectEmitter* _emitter, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle3D* particle);

   typedef ParticleEmitterModifier3D Inherited;

protected:
   /// Spline which controls the progression of color over the particle's life.
   SplineLinear<ColorRGBA8> colorSplineLinear;
   /// True if time should be normalized to the expected life of the particle.
   bool timeNormalized;
};

//==============================================================================

/// ParticleEmitterModifierAdditiveBlendingLinear3D is a ParticleEmitterModifier3D that
/// interpolates a particle's additive blending setting over time according to a given progression.
/// This is only valid if the normal duration of the particle's life is defined.  
class ParticleEmitterModifierAdditiveBlendingLinear3D : public ParticleEmitterModifier3D
{
public:
   virtual ~ParticleEmitterModifierAdditiveBlendingLinear3D() {}
   
   void Init(SceneNodeParticleEffectEmitter* _emitter, float _additiveBlendingBegin,
      float _additiveBlendingEnd, Progression* _progression);
   
   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle3D* particle);
   
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
   virtual void Update(unsigned int dt);
   /// Called when the associated particle is activated to begin a new life.
   virtual void OnActivate();

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

/// ParticleEmitterModifierSizeSplineLinear3D is a ParticleEmitterModifier3D
/// that interpolates a particle's size over time according to a SplineLinear.
/// By default, this is only valid if the normal duration of the particle's
/// life is defined.  In this case, the times should be normalized from 0 to
/// 1000 milliseconds, as though the particle's life was 1 second long.  The
/// sampling of the spline is adjusted based on the actual expected life of the
/// particle.  However, it is also possible to request that the times specified
/// in the spline be used as-is.  In this case, the normal duration of the
/// particle's life need not be defined.
class ParticleEmitterModifierSizeSplineLinear3D : public ParticleEmitterModifier3D
{
public:
   virtual ~ParticleEmitterModifierSizeSplineLinear3D() {}

   void Init(SceneNodeParticleEffectEmitter* _emitter, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle3D* particle);

   typedef ParticleEmitterModifier3D Inherited;

protected:
   /// Spline which controls the progression of size over the particle's life.
   SplineLinear<Point2F> sizeSplineLinear;
   /// True if time should be normalized to the expected life of the particle.
   bool timeNormalized;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__PARTICLEMODIFIER3D_H__

#ifndef __WAGEPARTICLEEFFECT_H__
#define __WAGEPARTICLEEFFECT_H__

#include "Frog.h"
#include "WageDrawable.h"

namespace Webfoot {

//==============================================================================

class WageParticleEffect : public ParticleEffect2D, public WageDrawable
{
public:
   typedef ParticleEffect2D Inherited;

   WageParticleEffect();
   virtual ~WageParticleEffect() {}

   virtual void Init(const char* filename, const Point3F& _position, const int _startDelay = 0);
   virtual void Init(JSONValue* specifications, const Point3F& _position, const int _startDelay = 0);
   virtual void Update(unsigned int dt);
   virtual void Draw();

   /// Set the x/y location of the particle effect
   void PositionSet(Point2F _position) { position3.x = _position.x; position3.y = _position.y; }
   /// Set the x/y/z location of the particle effect
   void Position3Set(Point3F _position3) { position3 = _position3; }
   /// Get the location of the particle effect
   Point3F Position3Get() { return position3; }
   /// Returns the depth value of the particle
   virtual float DepthGet() { return position3.z; }
   /// Check if marked for automatic deletion/garbage collection when finished
   bool AutoDeleteCheck() { return autoDelete; }
   /// Set if marked for automatic deletion/garbage collection when finished
   void AutoDeleteSet(bool _autoDelete) { autoDelete = _autoDelete; }
   /// Finished check
   bool FinishedCheck() { return (startDelay <= 0 && !Inherited::EmissionActiveCheck() && Inherited::ParticlesActiveCountGet() == 0); }
   /// End the particle effect
   void FinishedSet() { startDelay = 0; Inherited::EmissionActiveEnd(); }

protected:
   /// Position in 3D space of the origin for particle emitters
   Point3F position3;
   /// Mark for automatic deletion/garbage collection when finished
   bool autoDelete;
   /// Start delay before updating particle effect
   int startDelay;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGEPARTICLEEFFECT_H__

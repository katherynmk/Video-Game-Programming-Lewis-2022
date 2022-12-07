#ifndef __FROG__GUI__PARTICLEEFFECTWIDGET_H__
#define __FROG__GUI__PARTICLEEFFECTWIDGET_H__

#include "FrogMemory.h"
#include "Color.h"
#include "GUI/Widget.h"

namespace Webfoot {

class LayerWidget;
class ParticleEffect2D;

//=============================================================================

/// ParticleEffectWidget maintains and displays a ParticleEffect2D.
class ParticleEffectWidget : public Widget
{
public:
   ParticleEffectWidget();
   virtual ~ParticleEffectWidget() {}

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   /// Call this or the other Init immediately after construction.
   virtual void Init(const char* _name, Widget* _parent);
   /// Call this or the other Init immediately after construction.
   /// Initialize a new ParticleEffectWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Draw the widget.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();
   
   /// Return the particle effect managed by this widget.
   ParticleEffect2D* ParticleEffectGet() { return particleEffect; }
   /// Use the particle effect specified in the given file.
   void ParticleEffectSet(const char* filename);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ParticleEffectWidget(); }

protected:
   typedef Widget Inherited;
   
   /// Particle effect managed by this widget.
   ParticleEffect2D* particleEffect;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__PARTICLEEFFECTWIDGET_H__

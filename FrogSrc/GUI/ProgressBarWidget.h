#ifndef __FROG__GUI__PROGRESSBARWIDGET_H__
#define __FROG__GUI__PROGRESSBARWIDGET_H__

#include "FrogMemory.h"
#include "FloatRange.h"
#include "GUI/ContainerWidget.h"

namespace Webfoot {

class WidgetChildSprite;
class SpriteWidget;
class ProgressBarSpriteWidget;

//=============================================================================

/// Widget that can be used to show progress as a graphic is gradually revealed.
/// The clipping area is automatically set based on the current value and bounds.
/// If the progress bar is in "indeterminate mode", rather than showing an amount
/// of progress, it'll highlight a moving section of the progress sprite.
/// By default the maximum is 100, the minimum is 0, the minimum step size is
/// 1, and the initial value is 0.
/// Call Init immediately after construction.
class ProgressBarWidget : public ContainerWidget, public FloatRange
{
public:
   ProgressBarWidget();
   virtual ~ProgressBarWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new ProgressBarWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its children.
   virtual void Update(unsigned int dt);
   /// Draw the widget and its children.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Set the size in pixels of the increment at which to reveal the progress sprite.
   virtual void PixelStepSet(float _pixelStep) { pixelStep = _pixelStep; }
   /// Get the size in pixels of the increment at which to reveal the progress sprite.
   virtual float PixelStepGet() { return pixelStep; }

   /// Return true if the widget is in indeterminate mode.
   virtual bool IndeterminateModeCheck() { return indeterminateMode; }
   /// Set whether the widget is in indeterminate mode.
   virtual void IndeterminateModeSet(bool _indeterminateMode);

   /// Return the width of the area to highlight in indeterminate mode.
   virtual int IndeterminateEffectBarSizeGet() { return indeterminateEffectBarWidth; }
   /// Set the width of the area to highlight in indeterminate mode.
   virtual void IndeterminateEffectBarSizeSet(int _indeterminateEffectBarWidth) { indeterminateEffectBarWidth = _indeterminateEffectBarWidth; }
   
   /// Return the number of milliseconds for the indeterminate mode effect to loop.
   virtual float IndeterminateEffectDurationGet() { return indeterminateEffectDuration; }
   /// Set the number of milliseconds for the indeterminate mode effect to loop.
   virtual void IndeterminateEffectDurationSet(float _indeterminateEffectDuration) { indeterminateEffectDuration = _indeterminateEffectDuration; }
   
   /// Return the current position in the indeterminate mode effect loop in milliseconds.
   virtual float IndeterminateEffectPositionGet() { return indeterminateEffectPosition; }
   /// Set the current position in the indeterminate mode effect loop in milliseconds.
   virtual void IndeterminateEffectPositionSet(float _indeterminateEffectPosition) { indeterminateEffectPosition = _indeterminateEffectPosition; }

   virtual bool OpaqueCheck(const Point2F& testPosition);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ProgressBarWidget(); }

private:
   /// True if indeterminate mode is on.
   bool indeterminateMode;
   /// Size of the moving area to highlight in indeterminate mode.
   int indeterminateEffectBarWidth;
   /// Number of milliseconds for the indeterminate mode effect to loop.
   float indeterminateEffectDuration;
   /// Current position in the indeterminate mode effect loop in milliseconds.
   float indeterminateEffectPosition;

   /// Reveal the progressSprite at increments of this many pixels.
   float pixelStep;
   
   /// Sprite displayed behind the other child widgets.
   WidgetChildSprite* backgroundSprite;
   /// Sprite to be gradually revealed to indicate progress.
   ProgressBarSpriteWidget* progressSprite;

protected:
   typedef ContainerWidget Inherited;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__PROGRESSBARWIDGET_H__

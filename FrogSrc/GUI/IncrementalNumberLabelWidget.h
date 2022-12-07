#ifndef __INCREMENTALNUMBERLABELWIDGET_H__
#define __INCREMENTALNUMBERLABELWIDGET_H__

#include "FrogMemory.h"
#include "GUI/NumberLabelWidget.h"

namespace Webfoot {

//==============================================================================

/// An IncrementalNumberLabelWidget will try to catch up to its target value, used for things
/// likes scores or money. Has the option to pulse its scale as it increments
class IncrementalNumberLabelWidget : public NumberLabelWidget
{
public:
   typedef NumberLabelWidget Inherited;

   virtual ~IncrementalNumberLabelWidget() {}
   
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet() { return "IncrementalNumberLabelWidget"; }
   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) IncrementalNumberLabelWidget(); }

   virtual void Update(unsigned int dt);

   /// Sets the target value, will try to catch up to this value.
   void TargetValueSet(double _value);

   /// Sets the value immediately, no incrementing.
   virtual void ValueSet(double _value);

   /// Returns true if the widget is not incrementing.  
   bool FinishedIncrementingCheck() { return (targetValue == displayedValue); }

   // Tell the widget to pulse as it catches up to its target value.
   void PulseSet(bool _pulse);

protected:
   /// Performs the pulsing of the scale
   void PulseUpdate(unsigned int dt);

   /// The sound to play when incrementing
   Sound* sound;
    /// The value this widget tries to display
   double targetValue;
   /// The currentValue displayed
   double displayedValue;
   /// Scale of the value
   float displayedScale;
   /// the starting scale of the widget as defined in JSON
   Point2F startScale;
   /// Amount per MS to scale
   float displayedScaleRate;
   /// How much to pulse up to past the initial scale
   float scaleOffsetMax;
   /// How fast to increment
   float incrementRate;
   /// Controls the exponential side of the speed at which the displayed value
   /// approaches the target.  This must be greater than or equal to 0 and less
   /// than or equal to 1.  The distance to the target value decays
   /// exponentially based on this number, so that the decay slows to a stop.
   float differenceDecaySpeed;
   /// True if the widget should pulse
   bool pulse;
   /// True if the widget should display as a float, false for int
   bool displayAsFloat;
   /// If display as float, use this format
   const char* floatFormat;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __INCREMENTALNUMBERLABELWIDGET_H__

#ifndef __FROG__FLOATRANGE_H__
#define __FROG__FLOATRANGE_H__

#include "FrogMemory.h"

namespace Webfoot {

//=============================================================================

/// A range implemented in floats with a maximum, minimum, and current value.
/// There is also a minimum step, and the value must be a multiple of this
/// minimum step from the minimum of the range.  For instance, if the minimum
/// was 3 and the minimum step was 2, then 5 and 7 would be valid values, but 4
/// and 6 would not.  A minimum step of 0 is a special case in which the minimum
/// step is ignored.  It is also the default.
class FloatRange
{
public:
   FloatRange();
   virtual ~FloatRange() {}

   /// Set the current value within the range.  The final value
   /// will be rounded to a multiple of stepMinimum from the current minimum.
   virtual void ValueSet(float _value);
   /// Return the current value.
   virtual float ValueGet() { return value; }

   /// Return the greatest possible value in the range.
   virtual float MaximumGet() { return maximum; }
   /// Set the greatest possible value in the range.
   virtual void MaximumSet(float _maximum) { maximum = _maximum; }
   /// Return the lowest possible value in the range.
   virtual float MinimumGet() { return minimum; }
   /// Set the lowest possible value in the range.
   /// Adjust the value accordingly.
   virtual void MinimumSet(float _minimum);

   /// Return the number of which the value must be a multiple from the minimum.
   virtual float StepMinimumGet() { return stepMinimum; }
   /// Set the number of which the value must be a multiple from the minimum.
   /// Adjust the value accordingly.
   virtual void StepMinimumSet(float _stepMinimum);
   /// Return the given value rounded to the nearest multiple of the minimum step
   /// from the range's minimum.  This will not restrict the value to the bounds
   /// of the range.
   virtual float StepMinimumApply(float _value);

   /// Return the difference between the maximum and minimum or the range.
   virtual float WidthGet() { return (maximum - minimum); }

private:
   /// The current value within the range.
   float value;
   /// The greatest possible value in the range.
   float maximum;
   /// The lowest possible value in the range.
   float minimum;
   /// The current value must be a multiple of this from the minimum value.
   float stepMinimum;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FLOATRANGE_H__

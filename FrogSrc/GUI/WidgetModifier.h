#ifndef __FROG__GUI__WIDGETMODIFIER_H__
#define __FROG__GUI__WIDGETMODIFIER_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Color.h"
#include "TimedTransition.h"

namespace Webfoot {

class Widget;
class JSONValue;

/// Array of strings which can be used to help categorize modifiers.
#define WIDGET_MODIFIER_TAGS_KEY "Tags"

/// Name to use when specifying a WidgetModifierSplineLinear.
#define WIDGET_MODIFIER_SPLINE_LINEAR_TYPE_NAME "SplineLinear"
/// Key for the initial offset to time, in millseconds.
#define WIDGET_MODIFIER_SPLINE_LINEAR_TIME_OFFSET_KEY "TimeOffset"
#define WIDGET_MODIFIER_SPLINE_LINEAR_TIME_OFFSET_DEFAULT 0
/// Key for the control points of the position spline.
#define WIDGET_MODIFIER_SPLINE_LINEAR_POSITION_SPLINE_KEY "Position"
/// Key for the control points of the scale spline.
#define WIDGET_MODIFIER_SPLINE_LINEAR_SCALE_SPLINE_KEY "Scale"
/// Key for the control points of the rotation spline.
#define WIDGET_MODIFIER_SPLINE_LINEAR_ROTATION_SPLINE_KEY "Rotation"
/// Key for the control points of the color spline.
#define WIDGET_MODIFIER_SPLINE_LINEAR_COLOR_SPLINE_KEY "Color"

/// Name to use when specifying a WidgetModifierRandom.
#define WIDGET_MODIFIER_RANDOM_TYPE_NAME "Random"
/// Minimum amount of random additional position offset applied by a
/// WidgetModifierRandom.
#define WIDGET_MODIFIER_RANDOM_POSITION_OFFSET_LENGTH_MIN_KEY "PositionOffsetLengthMin"
/// Maximum amount of random additional position offset applied by a
/// WidgetModifierRandom.
#define WIDGET_MODIFIER_RANDOM_POSITION_OFFSET_LENGTH_MAX_KEY "PositionOffsetLengthMax"
/// The random additional rotation applied by a WidgetModifierRandom can vary
/// by this many degrees in either direction.
#define WIDGET_MODIFIER_RANDOM_ROTATION_DEVIATION_KEY "RotationDeviation"
/// Minimum angle of random additional rotation applied by a
/// WidgetModifierRandom, in degrees.
#define WIDGET_MODIFIER_RANDOM_ROTATION_MIN_KEY "RotationMin"
/// Maximum angle of random additional rotation applied by a
/// WidgetModifierRandom, in degrees.
#define WIDGET_MODIFIER_RANDOM_ROTATION_MAX_KEY "RotationMax"
/// Minimum random scale applied by a WidgetModifierRandom.
#define WIDGET_MODIFIER_RANDOM_SCALE_MIN_KEY "ScaleMin"
/// Maximum random scale applied by a WidgetModifierRandom.
#define WIDGET_MODIFIER_RANDOM_SCALE_MAX_KEY "ScaleMax"

//==============================================================================

/// WidgetModifier is a module for adding behavior to widgets.
class WidgetModifier
{
public:
   WidgetModifier() { name = NULL; widget = NULL; }
   virtual ~WidgetModifier() {}
   
   /// Initialize a new modifier with the defaults.
   virtual void Init(const char* _name, Widget* _widget);
   /// Initialize a new modifier based on the specifications in the given JSONValue.
   /// Give the modifier the specified name.  Make it remember to affect the given widget.
   virtual void Init(const char* _name, Widget* _widget, JSONValue* specifications);
   /// Clean up the modifier.
   virtual void Deinit();
   /// Called on every frame to update the modifier.
   virtual void Update(unsigned int dt) {}

   /// Return the name of this modifier.
   virtual const char* NameGet() { return name; }

   /// Return any temporary offset to the position caused by the modifier.
   virtual Point2F PositionOffsetGet() { return Point2F::Create(0.0f, 0.0f); }
   /// Return any temporary additional rotation caused by the modifier.
   virtual float RotationGet() { return 0; }
   /// Return any temporary further scaling caused by the modifier.
   virtual Point2F ScaleGet() { return Point2F::Create(1.0f, 1.0f); }
   /// Return any temporary further color modulation caused by the modifier.
   virtual ColorRGBA8 ColorGet() { return COLOR_RGBA8_WHITE; }

   /// Return true if the modifier is active.
   virtual bool EnabledCheck() { return enabled; }
   /// Set whether the modifier should be active.
   virtual void EnabledSet(bool _enabled) { enabled = _enabled; }

   /// Return true if modifier should be paused.
   virtual bool PausedCheck() { return paused; }
   /// Set whether the modifier should be paused.
   virtual void PausedSet(bool _paused) { paused = _paused; }

   /// Add the given string to this modifier's tags if it isn't already present.
   virtual void TagAdd(const char* tagName);
   /// Return true if this modifier has the given tag.
   virtual bool TagCheck(const char* tagName);
   /// Remove the given tag from this widget.
   virtual void TagRemove(const char* tagName);

   /// Return true if this modifier should influence position if the modifier
   /// itself is enabled.
   virtual bool PositionEnabledCheck() { return positionEnabled; }
   /// Set whether this modifier should influence position if the modifier
   /// itself is enabled.
   virtual void PositionEnabledSet(bool _positionEnabled) { positionEnabled = _positionEnabled; }

   /// Return true if this modifier should influence rotation if the modifier
   /// itself is enabled.
   virtual bool RotationEnabledCheck() { return rotationEnabled; }
   /// Set whether this modifier should influence position if the modifier
   /// itself is enabled.
   virtual void RotationEnabledSet(bool _rotationEnabled) { rotationEnabled = _rotationEnabled; }

   /// Return true if this modifier should influence scale if the modifier
   /// itself is enabled.
   virtual bool ScaleEnabledCheck() { return scaleEnabled; }
   /// Set whether this modifier should influence scale if the modifier
   /// itself is enabled.
   virtual void ScaleEnabledSet(bool _scaleEnabled) { scaleEnabled = _scaleEnabled; }

   /// Return true if this modifier should influence color if the modifier
   /// itself is enabled.
   virtual bool ColorEnabledCheck() { return colorEnabled; }
   /// Set whether this modifier should influence color if the modifier
   /// itself is enabled.
   virtual void ColorEnabledSet(bool _colorEnabled) { colorEnabled = _colorEnabled; }

protected:
   /// Name of the modifier instance.
   const char* name;
   /// The widget to be affected.
   Widget* widget;
   /// True if the modifier is currently active.
   bool enabled;
   /// True if position modifications are allowed.
   bool positionEnabled;
   /// True if rotation modifications are allowed.
   bool rotationEnabled;
   /// True if scale modifications are allowed.
   bool scaleEnabled;
   /// True if color modifications are allowed.
   bool colorEnabled;
   /// True if time should continue to pass for this modifier.
   bool paused;
   /// Collection of strings used to identify this modifier as part of a group.
   Table<const char*> tags;
};

//==============================================================================

/// A WidgetModifier that linearly interpolates position, rotation, scale, and
/// color using splines.
class WidgetModifierSplineLinear : public WidgetModifier
{
public:
   typedef WidgetModifier Inherited;
   
   virtual ~WidgetModifierSplineLinear() {}
   virtual void Init(const char* _name, Widget* _widget, JSONValue* specifications);
   virtual void Deinit();
   
   virtual void Update(unsigned int dt);
   
   /// Factory method for making WidgetModifierSplineLinear objects.
   static WidgetModifier* Create(const char* name, Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);

   /// Return any offset to the position caused by the transition.
   virtual Point2F PositionOffsetGet();
   /// Return any additional rotation caused by the transition.
   virtual float RotationGet();
   /// Return any additional scaling caused by the transition.
   virtual Point2F ScaleGet();
   /// Return any additional color modulation caused by the transition.
   virtual ColorRGBA8 ColorGet();

   /// Return the current time of the animation.
   virtual unsigned int TimeGet() { return time; }
   /// Set the time of the animation.
   virtual void TimeSet(unsigned int _time) { time = _time; }

protected:
   /// Number of milliseconds into the animation.
   unsigned int time;
   /// Duration of the animation in milliseconds.
   unsigned int duration;
   /// Underlying spline for position.
   SplineLinear<Point2F> positionSpline;
   /// Underlying spline for scale.
   SplineLinear<Point2F> scaleSpline;
   /// Underlying spline for rotation.
   SplineLinear<float> rotationSpline;
   /// Underlying spline for color.
   SplineLinear<ColorRGBA8> colorSpline;
};

//==============================================================================

/// A WidgetModifier that applies random influences to position, rotation, and
/// scale.
class WidgetModifierRandom : public WidgetModifier
{
public:
   typedef WidgetModifier Inherited;

   virtual ~WidgetModifierRandom() {}
   virtual void Init(const char* _name, Widget* _widget, JSONValue* specifications);

   /// Factory method for making WidgetModifierRandom objects.
   static WidgetModifier* Create(const char* name, Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);

   /// Return any offset to the position caused by the transition.
   virtual Point2F PositionOffsetGet();
   /// Return any additional rotation caused by the transition.
   virtual float RotationGet();
   /// Return any additional scaling caused by the transition.
   virtual Point2F ScaleGet();

protected:
   /// Position offset to be applied.
   Point2F positionOffset;
   /// Scale to be applied.
   Point2F scale;
   /// Rotation to be applied.
   float rotation;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__WIDGETMODIFIER_H__

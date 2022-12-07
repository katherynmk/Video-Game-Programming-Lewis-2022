#ifndef __FROG__DUCK__ANIMATION_H__
#define __FROG__DUCK__ANIMATION_H__

#include "FrogMemory.h"
#include "BSONIterator.h"
#include "SplineLinear.h"
#include "Table.h"

/// Frame rate of the animation when it was exported in frames per second.
#define DUCK_ANIMATION_FRAME_RATE_KEY "FrameRate"
/// First frame of the animation when it was exported.
#define DUCK_ANIMATION_FRAME_BEGIN_KEY "FrameBegin"
/// Final frame of the animation when it was exported.
#define DUCK_ANIMATION_FRAME_END_KEY "FrameEnd"
/// Collection of data on how individual properties vary in this animation
/// over time.
#define DUCK_ANIMATION_CHANNELS_KEY "Channels"
/// Name of the object to be affected by the animation channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_OBJECT_KEY "TargetObject"
/// Type of the object to be affected by the animation channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY "TargetObjectType"
/// Name of an ancestor of the object to be affected by the animation channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_OBJECT_ANCESTOR_KEY "TargetObjectAncestor"
/// "TargetObjectType" for when the target object is a scene node.
#define DUCK_ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME "SceneNode"
/// Name of the subobject to be affected by the animation channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_SUBOBJECT_KEY "TargetSubobject"
/// Type of the subobject to be affected by the animation channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_SUBOBJECT_TYPE_KEY "TargetSubobjectType"
/// "TargetSubobjectType" for when the target subobject is a morph target of a
/// SceneNode.
#define DUCK_ANIMATION_CHANNEL_TARGET_SUBOBJECT_TYPE_SCENE_NODE_MORPH_TARGET_NAME "MorphTarget"
/// Property of the object to be affected by the animation channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_PROPERTY_KEY "TargetProperty"
/// "TargetProperty" for the local position.
#define DUCK_ANIMATION_CHANNEL_TARGET_PROPERTY_POSITION_OFFSET_NAME "PositionOffset"
/// "TargetProperty" for the local rotation.
#define DUCK_ANIMATION_CHANNEL_TARGET_PROPERTY_ROTATION_NAME "Rotation"
/// "TargetProperty" for the local scale.
#define DUCK_ANIMATION_CHANNEL_TARGET_PROPERTY_SCALE_NAME "Scale"
/// "TargetProperty" for the weight of a morph channel.
#define DUCK_ANIMATION_CHANNEL_TARGET_PROPERTY_SCENE_NODE_MORPH_TARGET_WEIGHT_NAME "Weight"
/// Format of the channel data.
#define DUCK_ANIMATION_CHANNEL_TYPE_KEY "Type"
/// Array of points to provide to a SlineLinear for an animation channel.
#define DUCK_ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY "Points"

/// Type name for AnimationChannelFloatSplineLinear
#define DUCK_ANIMATION_CHANNEL_FLOAT_SPLINE_LINEAR_TYPE_NAME "FloatSplineLinear"
/// Type name for AnimationChannelPoint3FSplineLinear
#define DUCK_ANIMATION_CHANNEL_POINT3F_SPLINE_LINEAR_TYPE_NAME "Point3FSplineLinear"
/// Type name for AnimationChannelQuaternionSplineLinear
#define DUCK_ANIMATION_CHANNEL_QUATERNION_SPLINE_LINEAR_TYPE_NAME "QuaternionSplineLinear"

namespace Webfoot {
namespace Duck {

class AnimationChannel;
class Scene;

//==============================================================================

/// Groups of data that vary together over time.
class Animation
{
public:
   Animation();

   void Init(Scene* _scene, BSONIterator specifications);
   void Deinit();

   /// Return the scene from which this mesh was loaded, if any.
   Scene* SceneGet() { return scene; }

   /// Return the number of channels in this animation.
   int ChannelCountGet() { return channels.SizeGet(); }
   /// Return the given channel of this animation.
   AnimationChannel* ChannelGet(int channelIndex);

   /// Return the frame rate of this animation in frames per second.  The unit
   /// of time for the channels depends on this frame rate.  For example, if
   /// the frame rate is 1000, then times in this animation are given in
   /// milliseconds.  If it's 30, then times are in 30ths of a second.
   float FrameRateGet() { return frameRate; }
   /// Return the first frame of the animation when it was exported.
   int FrameBeginGet() { return frameBegin; }
   /// Return teh final frame of the animation when it was exported.
   int FrameEndGet() { return frameEnd; }

protected:
   /// Scene of which this mesh is a part.
   Scene* scene;
   /// See 'FrameRateGet'.
   float frameRate;
   /// First frame of the animation when it was exported.
   int frameBegin;
   /// Final frame of the animation when it was exported.
   int frameEnd;
   /// Data on how individual properties vary in this animation over time.
   Table<AnimationChannel*> channels;
};

//==============================================================================

/// Base class for data on how an individual property varies in an animation
/// over time.  The unit of time depends on the parent Animation's frame rate.
/// See Animation::FrameRateGet for details.
class AnimationChannel
{
public:
   AnimationChannel();

   virtual void Init(Animation* _animation, BSONIterator specifications);
   virtual void Deinit();

   /// Return the type of the object with the property modified by this channel.
   const char* TargetObjectTypeNameGet() { return targetObjectTypeName; }
   /// Name of the object with the property modified by this channel.
   const char* TargetObjectNameGet() { return targetObjectName; }
   /// Name of an ancestor of the object with the property modified by this channel.
   const char* TargetObjectAncestorNameGet() { return targetObjectAncestorName; }
   /// Return the type of the subobject with the property modified by this channel.
   const char* TargetSubobjectTypeNameGet() { return targetSubobjectTypeName; }
   /// Name of the subobject with the property modified by this channel.
   const char* TargetSubobjectNameGet() { return targetSubobjectName; }
   /// Name of the property to be modified by this channel.
   const char* TargetPropertyNameGet() { return targetPropertyName; }

protected:
   /// Animation of which this channel is a part.
   Animation* animation;
   /// Type of the object with the property modified by this channel.
   const char* targetObjectTypeName;
   /// Name of the object with the property modified by this channel.
   const char* targetObjectName;
   /// Name of an ancestor of the object with the property modified by this channel.
   const char* targetObjectAncestorName;
   /// Type of the subobject with the property modified by this channel.
   const char* targetSubobjectTypeName;
   /// Name of the subobject with the property modified by this channel.
   const char* targetSubobjectName;
   /// Name of the property to be modified by this channel.
   const char* targetPropertyName;
};

//==============================================================================

/// An animation channel which provides a float.
class AnimationChannelFloat : public AnimationChannel
{
public:
   /// Return the value for the given time.  The units of time depend on the
   /// animation's frame rate.  See Animation::FrameRateGet for details.
   virtual float ValueGet(float time) = 0;
};

//==============================================================================

/// An animation channel which provides a Point3F.
class AnimationChannelPoint3F : public AnimationChannel
{
public:
   /// Return the value for the given time.  The units of time depend on the
   /// animation's frame rate.  See Animation::FrameRateGet for details.
   virtual Point3F ValueGet(float time) = 0;
};

//==============================================================================

/// An animation channel which provides a Quaternion.
class AnimationChannelQuaternion : public AnimationChannel
{
public:
   /// Return the value for the given time.  The units of time depend on the
   /// animation's frame rate.  See Animation::FrameRateGet for details.
   virtual Quaternion ValueGet(float time) = 0;
};

//==============================================================================

/// An animation channel in which the values are defined with a SplineLinear.
template<typename T, typename BaseClass>
class AnimationChannelSplineLinear : public BaseClass
{
public:
   virtual void Init(Animation* _animation, BSONIterator specifications);
   virtual void Deinit() { splineLinear.Deinit(); Inherited::Deinit(); }

   virtual T ValueGet(float time);

   static AnimationChannel* Create() { return frog_new AnimationChannelSplineLinear<T, BaseClass>(); }

   typedef BaseClass Inherited;

protected:
   /// Object containing how the values should vary over time.
   SplineLinear<T> splineLinear;
};

//------------------------------------------------------------------------------

template<typename T, typename BaseClass>
void AnimationChannelSplineLinear<T, BaseClass>::Init(Animation* _animation, BSONIterator specifications)
{
   splineLinear.Init();
   Inherited::Init(_animation, specifications);

   // Load the points of the spline.
   BSONIterator pointsSpecs = specifications.Get(DUCK_ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY);
   if(pointsSpecs.ArrayCheck())
      splineLinear.PointsAdd(pointsSpecs);
}

//------------------------------------------------------------------------------

template<typename T, typename BaseClass>
T AnimationChannelSplineLinear<T, BaseClass>::ValueGet(float time)
{
   // SplineLinear's notion of time is normally in milliseconds, but here that
   // may nor may not be the case.  It depends on the animation's frame rate.
   return splineLinear.ValueGet(time);
}

//------------------------------------------------------------------------------

/// AnimationChannelSplineLinear for float values.
typedef AnimationChannelSplineLinear<float, AnimationChannelFloat> AnimationChannelFloatSplineLinear;
/// AnimationChannelSplineLinear for Point3F values.
typedef AnimationChannelSplineLinear<Point3F, AnimationChannelPoint3F> AnimationChannelPoint3FSplineLinear;
/// AnimationChannelSplineLinear for Quaternion values.
typedef AnimationChannelSplineLinear<Quaternion, AnimationChannelQuaternion> AnimationChannelQuaternionSplineLinear;

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__ANIMATION_H__

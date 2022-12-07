#ifndef __FROG__DUCK__ANIMATIONPLAYER_H__
#define __FROG__DUCK__ANIMATIONPLAYER_H__

#include "FrogMemory.h"
#include "SoundManager.h"
#include "Table.h"
#include "TimedTransition.h"

namespace Webfoot {

class JSONValue;
class SpriteAnimation;

namespace Duck {

class Animation;
class AnimationChannel;
class AnimationPlayerAnimatedProperty;
class AnimationPlayerAnimation;
class AnimationPlayerAnimationChannel;
class AnimationPlayerAnimationEvent;
class SceneNode;
class SceneNodeMeshMorphTarget;

/// Collection of animations to be loaded by an AnimationPlayer.
#define DUCK_ANIMATION_PLAYER_ANIMATIONS_KEY "Animations"
/// Animation to which additive animations are relative.
#define DUCK_ANIMATION_PLAYER_REST_POSE_ANIMATION_KEY "RestPoseAnimation"
/// Use the given frame of the underlying animation as the starting point.
#define DUCK_ANIMATION_PLAYER_ANIMATION_FRAME_BEGIN_KEY "FrameBegin"
/// Use the given frame of the underlying animation as the ending point.
#define DUCK_ANIMATION_PLAYER_ANIMATION_FRAME_END_KEY "FrameEnd"
/// Priority number used when determining the influence of different animations.
/// Animations with numerically higher priority take precedence.
#define DUCK_ANIMATION_PLAYER_ANIMATION_PRIORITY_KEY "Priority"
/// How to blend a given animation with other animations.
#define DUCK_ANIMATION_PLAYER_ANIMATION_BLEND_TYPE_KEY "BlendType"
/// Array of commands to be executed in-order to set the masking weights of properties.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_KEY "Mask"
/// Type of a command for setting up the masking weights of properties.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_COMMAND_TYPE_KEY "Type"
/// Weight to be set by a command for setting up masking weights of properties.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_COMMAND_WEIGHT_KEY "Weight"
/// Named groups of animations.
#define DUCK_ANIMATION_PLAYER_ANIMATION_GROUPS_KEY "AnimationGroups"

/// Name to assign to default animations.
#define DUCK_ANIMATION_PLAYER_ANIMATION_NAME_DEFAULT "Default"

/// Higher-priority animations override the influence of lower-priority
/// animations.
#define DUCK_ANIMATION_PLAYER_ANIMATION_BLEND_TYPE_OVERRIDE_NAME "Override"
/// The influence of the animation is added to the target property.
#define DUCK_ANIMATION_PLAYER_ANIMATION_BLEND_TYPE_ADDITIVE_NAME "Additive"

/// Mask command for affecting all objects in the animation.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_COMMAND_TYPE_SET_ALL_NAME "SetAll"
/// Mask command for affecting channels for a given target object.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_COMMAND_TYPE_SET_OBJECT_NAME "SetObject"
/// Mask command for affecting channels for all descendants of given target
/// object.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_COMMAND_TYPE_SET_DESCENDANTS_NAME "SetDescendants"
/// Mask command for affecting channels for a given target object and its
/// descendants.
#define DUCK_ANIMATION_PLAYER_ANIMATION_MASK_COMMAND_TYPE_SET_OBJECT_AND_DESCENDANTS_NAME "SetObjectAndDescendants"

/// Collection of events that take place during an animation.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENTS_KEY "Events"
/// Name of an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_NAME_KEY "Name"
/// Type of an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_TYPE_KEY "Type"
/// Use the given frame of the underlying animation as the time at which the
/// event occurs.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_FRAME_KEY "Frame"
/// Name of the target SceneNode of an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_TARGET_SCENE_NODE_KEY "SceneNode"
/// Sound specified by an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_FILENAME_KEY "Filename"
/// True if a sound should be looped.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_LOOP_KEY "Loop"
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_LOOP_DEFAULT false
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_MUSIC_LOOP_DEFAULT true
/// Sound usage that should affect this sound's volume.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_USAGE_KEY "Usage"
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_USAGE_DEFAULT Sound::SFX
/// Volume at which the sound should be played.  [0-100]
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_VOLUME_KEY "Volume"
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SOUND_VOLUME_DEFAULT Sound::VOLUME_DEFAULT
/// Name of the event which is the target of an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_TARGET_EVENT_KEY "Event"
/// Name of a sprite animation to use for an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SPRITE_ANIMATION_KEY "Animation"
/// Filename of a sprite resource file to use for an event.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_SPRITE_RESOURCE_FILENAME_KEY "SpriteResourceFilename"
/// True if the target scene node should be visible and false if it should be
/// hidden.
#define DUCK_ANIMATION_PLAYER_ANIMATION_EVENT_VISIBLE_KEY "Visible"

//==============================================================================

/// Coordinates the updating, mixing, and application of animations to a tree
/// of scene nodes.  This does not typically apply to an entire scene, but
/// rather to a subset of it, like a single character.
class AnimationPlayer
{
public:
   /// How the animation should progress.
   enum PlayType
   {
      /// Play the animation from beginning to end, then stop showing the
      /// animation.
      PLAY_TYPE_ONE_SHOT,
      /// Play the animation from beginning to end, and hold at the end of the
      /// animation.
      PLAY_TYPE_ONE_SHOT_HOLD,
      /// Play the animation in a loop until told to stop.
      PLAY_TYPE_LOOP
   };

   AnimationPlayer();

   /// Initialize the player to animate the properties of the given tree of
   /// nodes.  Matching animated properties to objects is based on names, so
   /// avoid having multiple animated objects in the given tree that have the
   /// same names.  If no explicit specifications are given, the system will
   /// attempt to set up a default animation which plays the full timeline
   /// exported with the given scene node.
   void Init(SceneNode* _rootNode, JSONValue* specifications = NULL);
   void Deinit();

   /// Move time forward by the given number of milliseconds.
   void Update(unsigned int dt) { Update((float)dt); }
   /// Move time forward by the given number of milliseconds.
   void Update(float dt);
   /// Call this once all animations have been configured for the current frame
   /// to set the animated properties of the affected objects.
   void Apply();

   /// Return the number of animations in the player.
   int AnimationCountGet() { return playerAnimations.SizeGet(); }
   /// Return the player's object for the given animation.
   AnimationPlayerAnimation* AnimationGet(int index);
   /// Return the player's object for the given animation.
   AnimationPlayerAnimation* AnimationGet(const char* name);

   /// Begin playing the given animation.
   void Play(const char* animationName, PlayType _playType = PLAY_TYPE_ONE_SHOT,
      unsigned int _easeInDuration = 0, unsigned int _easeOutDuration = 0, float _weight = 1.0f);
   /// Stop playing the given animation over the specified period of time, in
   /// milliseconds.
   void Stop(const char* animationName, unsigned int _easeOutDuration);
   /// Return true if the given animation is playing.
   bool PlayingCheck(const char* animationName);

   /// Randomly choose an animation from the given group and begin playing it.
   /// Return the animation which was chosen, if any.
   AnimationPlayerAnimation* GroupPlayOne(const char* animationGroupName, PlayType _playType = PLAY_TYPE_ONE_SHOT,
      unsigned int _easeInDuration = 0, unsigned int _easeOutDuration = 0, float _weight = 1.0f);
   /// Stop playing all animations in the given group over the specified period
   /// of time, in milliseconds.
   void GroupStop(const char* animationGroupName, unsigned int easeOutDuration = 0);
   /// Return true if an animation from the given group is playing.
   bool GroupPlayingCheck(const char* animationGroupName);

   /// Return the root of the tree of nodes to be affected by animations.
   SceneNode* RootNodeGet() { return rootNode; }

protected:
   /// Get the animated property object which would be appropriate for the
   /// given channel.  Create this animated property object if it does not
   /// already exist.  Return NULL if unsuccessful.
   AnimationPlayerAnimatedProperty* AnimatedPropertyGet(AnimationChannel* channel);

   /// Root of the tree of nodes to be affected by animations.
   SceneNode* rootNode;
   /// Named groups of animations.
   JSONValue* animationGroups;
   /// Animation to which additive animations are relative.
   AnimationPlayerAnimation* restPosePlayerAnimation;
   /// Collection of animations to be played, mixed, and applied.
   Table<AnimationPlayerAnimation*> playerAnimations;
   /// Collection of specific properties of specific objects which are affected
   /// by animations in the player.
   Table<AnimationPlayerAnimatedProperty*> animatedProperties;

   friend class AnimationPlayerAnimatedProperty;
   friend class AnimationPlayerAnimatedPropertySceneNode;
   friend class AnimationPlayerAnimation;
   friend class AnimationPlayerAnimationChannel;
};

//==============================================================================

/// Used to managed the state of a given animation and control how it is mixed
/// with others.
class AnimationPlayerAnimation
{
public:
   /// How the animation should be blended with others influencing the same
   /// properties.
   enum BlendType
   {
      /// Higher-priority animations override the influence of lower-priority
      /// animations.
      BLEND_TYPE_OVERRIDE,
      /// The influence of the animation is added to the target property.
      BLEND_TYPE_ADDITIVE
   };

   AnimationPlayerAnimation();

   void Init(const char* _name, AnimationPlayer* _animationPlayer, JSONValue* specifications);
   void Deinit();

   /// Move time forward by the given number of milliseconds.
   void Update(float dt);

   /// Return the name for this animation.
   const char* NameGet() { return name; }

   /// Begin playing this animation.
   void Play(AnimationPlayer::PlayType _playType = AnimationPlayer::PLAY_TYPE_ONE_SHOT,
      unsigned int _easeInDuration = 0, unsigned int _easeOutDuration = 0, float _weight = 1.0f);

   /// Stop playing this animation over the specified period of time, in
   /// milliseconds.
   void Stop(unsigned int _easeOutDuration = 0);

   /// Return true if this animation is currently playing.
   bool PlayingCheck() { return playing; }

   /// Set the influence of the associated animation before priorities are
   /// applied.
   void WeightSet(float _weight) { weight = _weight; }
   /// Return influence of the associated animation before priorities are
   /// applied.
   float WeightGet() { return weight; }

   /// Return the duration of the animation in milliseconds.
   float DurationGet() { return duration; }

   /// Set the number of milliseconds into the animation.
   void TimeSet(float _time);
   /// Return the number of milliseconds into the animation.
   float TimeGet() { return time; }

   /// Set a coefficient applied to time for animation purposes.  This should
   /// be non-negative.  1.0f plays at the default speed, 2.0f plays at twice
   /// the default speed, and so on.
   void TimeCoefficientSet(float _timeCoefficient) { timeCoefficient = _timeCoefficient; }
   /// See 'AnimationTimeCoefficientSet'.
   float TimeCoefficientGet() { return timeCoefficient; }

   /// Set the priority number used when determining the influence of different
   /// animations.
   void PrioritySet(float _priority) { priority = _priority; }
   /// Return the priority number used when determining the influence of
   /// different animations.
   float PriorityGet() { return priority; }

   /// Return how the animation should be blended with others influencing the
   /// same properties.
   BlendType BlendTypeGet() { return blendType; }

   /// Return the current time for this animation in the time units of the
   /// underlying animation data.
   float TimeFramesGet();

   /// Return the first cutscene event with the given name.
   AnimationPlayerAnimationEvent* EventGet(const char* eventName);

   /// Return the animation associated with this object.
   Animation* AnimationGet() { return animation; }
   /// Return the 'AnimationPlayer' to which this object belongs.
   AnimationPlayer* AnimationPlayerGet() { return animationPlayer; }

protected:
   /// Whether the animation is currently easing, and if so, how.
   enum EasingState
   {
      /// Not easing in and not easing out.
      EASING_STATE_NOT_EASING,
      /// Transitioning from zero influence to full influence.
      EASING_STATE_EASING_IN,
      /// Transitioning from full influence to zero influence.
      EASING_STATE_EASING_OUT
   };

   /// Helper function for following JSON instructions for which properties
   /// should be affected by an animation.
   void MaskCommandExecute(JSONValue* commandSpecs);

   /// Name of the animation for player purposes.
   const char* name;
   /// Animation associated with this object.
   Animation* animation;
   /// 'AnimationPlayer' to which this object belongs.
   AnimationPlayer* animationPlayer;
   /// Number of milliseconds into the animation.
   float time;
   /// Coefficient applied to changes in time for animation updating purposes.
   float timeCoefficient;
   /// Cached duration of the animation in milliseconds.
   float duration;
   /// Index of the next event to begin.
   int eventBeginIndex;
   /// Use the given frame of the underlying animation as the starting point.
   int frameBegin;
   /// Use the given frame of the underlying animation as the ending point.
   int frameEnd;
   /// True if this animation is currently playing.
   bool playing;
   /// Influence of the associated animation before priorities are applied.
   float weight;
   /// Priority number used when determining the influence of different animations.
   float priority;
   /// Most recent duration for easing the animation out, in milliseconds.
   unsigned int easeOutDuration;
   /// How the animation should be played. (one-shot, looping, etc.)
   AnimationPlayer::PlayType playType;
   /// How the animation should be blended with others influencing the same
   /// properties.
   BlendType blendType;
   /// Current EasingState of the animation.
   EasingState easingState;
   /// Accumulator for changes in time for reducing rounding errors.
   float easingCoefficientDtAccumulator;
   /// Helper for easing the weight of the animation.
   TimedTransitionLinear<float> easingCoefficient;
   /// AnimationPlayer representations of the channels of the animation.
   Table<AnimationPlayerAnimationChannel*> playerChannels;
   /// Properties to be influenced by one or more animations in the player.
   Table<AnimationPlayerAnimatedProperty*> animatedProperites;
   /// Events that take place over the course of an animation.
   Table<AnimationPlayerAnimationEvent*> animationEvents;

   friend class AnimationPlayerAnimatedProperty;
};

//==============================================================================

/// Represents a specific channel from a given animation for AnimationPlayer
/// purposes.  This allows you to control which parts of an animation should
/// actually affect the target properties.  For example, if you wanted an
/// animation of a character waving to affect only the upper body, you could set
/// the weights of all the unrelated channels in the waving animation to zero.
class AnimationPlayerAnimationChannel
{
public:
   AnimationPlayerAnimationChannel();

   void Init(AnimationPlayerAnimation* _playerAnimation, AnimationChannel* _channel,
      AnimationPlayerAnimatedProperty* _animatedProperty, float _weight);
   void Deinit();

   /// Return the AnimationPlayer representation of the animation of which this
   /// channel is a part.
   AnimationPlayerAnimation* PlayerAnimationGet() { return playerAnimation; }
   /// Return the underlying animation data for this channel.
   AnimationChannel* ChannelGet() { return channel; }

protected:
   /// AnimationPlayer representation of the animation of which this channel is
   /// a part.
   AnimationPlayerAnimation* playerAnimation;
   /// Underlying animation data for this channel.
   AnimationChannel* channel;
   /// Property to be affected.
   AnimationPlayerAnimatedProperty* animatedProperty;
   /// Influence of this channel.  This is mainly used for masking.
   float weight;

   friend class AnimationPlayer;
   friend class AnimationPlayerAnimation;
   friend class AnimationPlayerAnimatedProperty;
};

//==============================================================================

/// This points to a specific property of a specific object to be affected by
/// one or more animations in an AnimationPlayer.
class AnimationPlayerAnimatedProperty
{
public:
   AnimationPlayerAnimatedProperty();
   virtual ~AnimationPlayerAnimatedProperty() {}

   virtual void Init(AnimationPlayer* _animationPlayer,
      const char* _targetObjectTypeName, const char* _targetObjectName,
      const char* _targetObjectAncestorName,
      const char* _targetSubobjectTypeName, const char* _targetSubobjectName,
      const char* _targetPropertyName);
   virtual void Deinit();

   /// Mix and apply the animation channels affecting this property.
   virtual void Apply() = 0;

protected:
   /// Add the given channel to the collection of those which affect this
   /// property.
   void PlayerChannelAdd(AnimationPlayerAnimationChannel* playerChannel);
   /// Get the channel for this property from the given animation.  Return NULL
   /// if unsuccessful.
   AnimationPlayerAnimationChannel* PlayerChannelGet(AnimationPlayerAnimation* playerAnimation);
   /// Assuming the value for this property is a float, get the value to
   /// use right now based on the relevant animation channels.  Return true if
   /// successful.
   bool PropertyValueFloatGet(float* value);
   /// Assuming the value for this property is a Point3F, get the value to
   /// use right now based on the relevant animation channels.  Return true if
   /// successful.
   bool PropertyValuePoint3FGet(Point3F* value);
   /// Assuming the value for this property is a Quaternion, get the value to
   /// use right now based on the relevant animation channels.  Return true if
   /// successful.
   bool PropertyValueQuaternionGet(Quaternion* value);

   /// True if this property should be affected by animations of the associated
   /// player.  This can be false if all relevant channels have been masked
   /// away.
   bool enabled;
   /// Animation player for this object.
   AnimationPlayer* animationPlayer;
   /// Type of the object with the property to be modified.
   const char* targetObjectTypeName;
   /// Name of the object with the property to be modified
   const char* targetObjectName;
   /// Name of an ancestor of the object with the property to be modified.
   const char* targetObjectAncestorName;
   /// Type of the subobject with the property to be modified.
   const char* targetSubobjectTypeName;
   /// Name of the subobject with the property to be modified
   const char* targetSubobjectName;
   /// Name of the property to be modified.
   const char* targetPropertyName;
   /// Channels which affect this property.
   Table<AnimationPlayerAnimationChannel*> playerChannels;

   friend class AnimationPlayer;
   friend class AnimationPlayerAnimation;
};

//==============================================================================

/// AnimationPlayerAnimatedProperty for animating a property of a SceneNode.
class AnimationPlayerAnimatedPropertySceneNode : public AnimationPlayerAnimatedProperty
{
public:
   AnimationPlayerAnimatedPropertySceneNode();

   virtual void Init(AnimationPlayer* _animationPlayer, const char* _targetObjectTypeName,
      const char* _targetObjectName, const char* _targetSubobjectTypeName,
      const char* _targetObjectAncestorName,
      const char* _targetSubobjectName, const char* _targetPropertyName);
   virtual void Deinit();

   /// Return the scene node with the property to be affected.
   SceneNode* SceneNodeGet() { return sceneNode; }

   typedef AnimationPlayerAnimatedProperty Inherited;

protected:
   /// SceneNode to be affected.
   SceneNode* sceneNode;
};

//==============================================================================

/// AnimationPlayerAnimatedProperty for animating the position of a SceneNode.
class AnimationPlayerAnimatedPropertySceneNodePosition : public AnimationPlayerAnimatedPropertySceneNode
{
public:
   virtual void Apply();

   typedef AnimationPlayerAnimatedPropertySceneNode Inherited;
};

//==============================================================================

/// AnimationPlayerAnimatedProperty for animating the rotation of a SceneNode.
class AnimationPlayerAnimatedPropertySceneNodeRotation : public AnimationPlayerAnimatedPropertySceneNode
{
public:
   virtual void Apply();

   typedef AnimationPlayerAnimatedPropertySceneNode Inherited;
};

//==============================================================================

/// AnimationPlayerAnimatedProperty for animating the scale of a SceneNode.
class AnimationPlayerAnimatedPropertySceneNodeScale : public AnimationPlayerAnimatedPropertySceneNode
{
public:
   virtual void Apply();

   typedef AnimationPlayerAnimatedPropertySceneNode Inherited;
};

//==============================================================================

/// AnimationPlayerAnimatedProperty for animating the weight property of a
/// SceneNode's morph target.
class AnimationPlayerAnimatedPropertySceneNodeMorphTargetWeight : public AnimationPlayerAnimatedPropertySceneNode
{
public:
   AnimationPlayerAnimatedPropertySceneNodeMorphTargetWeight();

   virtual void Init(AnimationPlayer* _animationPlayer, const char* _targetObjectTypeName,
      const char* _targetObjectName, const char* _targetSubobjectTypeName,
      const char* _targetObjectAncestorName,
      const char* _targetSubobjectName, const char* _targetPropertyName);
   virtual void Deinit();
   virtual void Apply();

   typedef AnimationPlayerAnimatedPropertySceneNode Inherited;

protected:
   /// SceneNode to be affected.
   SceneNodeMeshMorphTarget* morphTarget;
};

//==============================================================================

/// An event that exists on the timeline of an animation.
class AnimationPlayerAnimationEvent
{
public:
   AnimationPlayerAnimationEvent();

   virtual void Init(const char* _name, AnimationPlayerAnimation* _playerAnimation, JSONValue* _specifications);
   virtual void Deinit();

   /// Called when the event should begin.
   virtual void OnEventBegin() {}

   /// Return the name of this event.
   const char* NameGet() { return name; }
   /// Return the time at which this event should begin, in milliseconds.
   float TimeGet() const { return time; }

   /// Comparator for sorting events into increasing time.
   static bool TimeComparator(const AnimationPlayerAnimationEvent* first,
      const AnimationPlayerAnimationEvent* second)
   {
      return first->TimeGet() < second->TimeGet();
   }

protected:
   /// Convenience function for returning the root root of the tree of nodes to
   /// be affected by this animations of the associated AnimationPlayer.
   SceneNode* AnimationPlayerRootNodeGet();

   /// Animation of which this event is a part.
   AnimationPlayerAnimation* playerAnimation;
   /// Name of the event.
   const char* name;
   /// Time at which this event should begin in milliseconds.
   float time;
   /// Specifications used to set up the event.
   JSONValue* specifications;
};

//==============================================================================

/// Begin playing a song.
class AnimationPlayerAnimationEvent_MusicPlay : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_MusicPlay(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Stop playing music.
class AnimationPlayerAnimationEvent_MusicStop : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_MusicStop(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Expire all the currently active particles in the given SceneNodeParticleEffect.
class AnimationPlayerAnimationEvent_ParticlesAllExpire : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_ParticlesAllExpire(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Begin emitting particles from a given SceneNodeParticleEffect.
class AnimationPlayerAnimationEvent_ParticlesBegin : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_ParticlesBegin(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Stop emitting particles from a given SceneNodeParticleEffect.
class AnimationPlayerAnimationEvent_ParticlesEnd : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_ParticlesEnd(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Play a sound.
class AnimationPlayerAnimationEvent_SoundPlay : public AnimationPlayerAnimationEvent
{
public:
   AnimationPlayerAnimationEvent_SoundPlay();

   virtual void Init(const char* _name, AnimationPlayerAnimation* _playerAnimation, JSONValue* _specifications);
   virtual void Deinit();

   virtual void OnEventBegin();

   /// Return the SoundSource used by this event to play its sound.
   SoundSource* SoundSourceGet() { return soundSource; }

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_SoundPlay(); }

   typedef AnimationPlayerAnimationEvent Inherited;

protected:
   /// Sound to be played.
   Sound* sound;
   /// Used to play the sound.
   SoundSource* soundSource;
};

//==============================================================================

/// Stop playing a sound started by an AnimationPlayerAnimationEvent_SoundPlay.
class AnimationPlayerAnimationEvent_SoundStop : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_SoundStop(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Begin streaming a sound.
class AnimationPlayerAnimationEvent_SoundStreamPlay : public AnimationPlayerAnimationEvent
{
public:
   AnimationPlayerAnimationEvent_SoundStreamPlay();

   virtual void Init(const char* _name, AnimationPlayerAnimation* _playerAnimation, JSONValue* _specifications);
   virtual void Deinit();

   virtual void OnEventBegin();

   /// Return the SoundStreamSource used by this event to play its sound.
   SoundStreamSource* SoundStreamSourceGet() { return soundStreamSource; }

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_SoundStreamPlay(); }

   typedef AnimationPlayerAnimationEvent Inherited;

protected:
   /// Used to play the sound stream.
   SoundStreamSource* soundStreamSource;
};

//==============================================================================

/// Stop playing a streaming sound started by an
/// AnimationPlayerAnimationEvent_SoundStreamPlay.
class AnimationPlayerAnimationEvent_SoundStreamStop : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_SoundStreamStop(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Set the animation of a given SceneNodeSprite.
class AnimationPlayerAnimationEvent_SpriteAnimationSet : public AnimationPlayerAnimationEvent
{
public:
   AnimationPlayerAnimationEvent_SpriteAnimationSet();

   virtual void Init(const char* _name, AnimationPlayerAnimation* _playerAnimation, JSONValue* _specifications);
   virtual void Deinit();

   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_SpriteAnimationSet(); }

   typedef AnimationPlayerAnimationEvent Inherited;

protected:
   /// Filename of the sprite animation's resource file.
   const char* spriteResourceFilename;
   /// Name of the sprite animation to play.
   const char* spriteAnimationName;
   /// Animation to be played.
   SpriteAnimation* preloadedAnimation;
};

//==============================================================================

/// Set whether the target scene node and its descendants should be hidden as a
/// group.
class AnimationPlayerAnimationEvent_VisibleHierarchicalSet : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_VisibleHierarchicalSet(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

/// Set whether the target scene node should be hidden, independently of its
/// ancestors and descendants.
class AnimationPlayerAnimationEvent_VisibleSpecificSet : public AnimationPlayerAnimationEvent
{
public:
   virtual void OnEventBegin();

   static AnimationPlayerAnimationEvent* Create() { return frog_new AnimationPlayerAnimationEvent_VisibleSpecificSet(); }

   typedef AnimationPlayerAnimationEvent Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__ANIMATIONPLAYER_H__

#ifndef __FROG__GUI__CUTSCENEWIDGET_H__
#define __FROG__GUI__CUTSCENEWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"
#include "FileManager.h"

namespace Webfoot {

class CutsceneWidgetSegment;
class CutsceneWidgetEvent;
class StateContainerWidget;
class TextDocumentWidget;
class LabelWidget;
class ParticleEffect2D;
class SpriteAnimation;
class SpriteWidget;
class SoundStreamSource;

/// Name of the folder which contains the cutscenes.
#define CUTSCENE_WIDGET_CUTSCENE_FOLDER_PATH "Cutscenes"
/// Printf pattern for cutscene folders
#define CUTSCENE_WIDGET_CUTSCENE_PATH_PATTERN CUTSCENE_WIDGET_CUTSCENE_FOLDER_PATH "/%s"
/// Printf pattern for the cutscene segment JSON specifications.
#define CUTSCENE_WIDGET_SEGMENT_SPECS_FILENAME_PATTERN CUTSCENE_WIDGET_CUTSCENE_PATH_PATTERN "/Segment%03d"
/// Duration of the cutscene segment in milliseconds.
#define CUTSCENE_WIDGET_SEGMENT_DURATION_KEY "Duration"
/// Widgets that make up the stage states.
#define CUTSCENE_WIDGET_STAGE_STATES_KEY "StageStates"
/// Widget modifiers with this tag should be synchronized to the cutscene
/// timeline.
#define CUTSCENE_WIDGET_CUTSCENE_SYNC_MODIFIER_TAG "CutsceneSync"
/// Events that occur over the course of the cutscene segment.
#define CUTSCENE_WIDGET_EVENTS_KEY "Events"
/// Name of an event.
#define CUTSCENE_WIDGET_EVENT_NAME_KEY "Name"
/// Type of the given event.
#define CUTSCENE_WIDGET_EVENT_TYPE_KEY "Type"
/// Time at which an event should begin.
#define CUTSCENE_WIDGET_EVENT_TIME_KEY "Time"
#define CUTSCENE_WIDGET_EVENT_TIME_DEFAULT 0
/// Animated background to be displayed.
#define CUTSCENE_WIDGET_EVENT_ANIMATED_BACKGROUND_FILENAME_KEY "Filename"
/// Filename of the sprite animation's resource file.
#define CUTSCENE_WIDGET_EVENT_SPRITE_RESOURCE_FILENAME_KEY "SpriteResourceFilename"
/// Name of the sprite animation to play.
#define CUTSCENE_WIDGET_EVENT_SPRITE_ANIMATION_KEY "Animation"
/// Name of the target widget of an event.
#define CUTSCENE_WIDGET_EVENT_TARGET_WIDGET_KEY "Widget"
/// Type name for a AnimatedBackgroundSet event.
#define CUTSCENE_WIDGET_ANIMATED_BACKGROUND_SET_EVENT_TYPE_NAME "AnimatedBackgroundSet"
/// Type name for a DialogPlay event.
#define CUTSCENE_WIDGET_DIALOG_PLAY_EVENT_TYPE_NAME "DialogPlay"
/// Type name for a Generic event.
#define CUTSCENE_WIDGET_GENERIC_EVENT_TYPE_NAME "Generic"
/// Type name for a Goto event.
#define CUTSCENE_WIDGET_GOTO_EVENT_TYPE_NAME "Goto"
/// Type name for a MusicPlay event.
#define CUTSCENE_WIDGET_MUSIC_PLAY_EVENT_TYPE_NAME "MusicPlay"
/// Sound specified by an event.
#define CUTSCENE_WIDGET_EVENT_SOUND_FILENAME_KEY "Filename"
/// Type name for a SoundPlay event.
#define CUTSCENE_WIDGET_SOUND_PLAY_EVENT_TYPE_NAME "SoundPlay"
/// Type name for a SoundStreamPlay event.
#define CUTSCENE_WIDGET_SOUND_STREAM_PLAY_EVENT_TYPE_NAME "SoundStreamPlay"
/// Type name for a SpriteAnimationSet event.
#define CUTSCENE_WIDGET_SPRITE_ANIMATION_SET_EVENT_TYPE_NAME "SpriteAnimationSet"

//==============================================================================

/// CutsceneWidget displays a cutscene.
class CutsceneWidget : public ContainerWidget
{
public:
   /// Create a CutsceneWidgetEvent of the given type, but do not initialize it.
   typedef CutsceneWidgetEvent* (*CutsceneWidgetEventFactory)(const char* type, HeapID heapID);

   CutsceneWidget();
   virtual ~CutsceneWidget() {}
   
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);
   
   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Begin playing the given cutscene.
   virtual void CutsceneSet(const char* _cutsceneName);
   /// Return true if the cutscene is over.
   virtual bool CutsceneFinishedCheck() { return cutsceneFinished; }
   /// Clean up the current cutscene.
   virtual void CutsceneClear();
   /// Return the name of the current cutscene, if any.
   virtual const char* CutsceneNameGet() { return cutsceneName; }
   /// Return the sprite resource filename used by default for the current
   /// cutscene.
   virtual const char* CutsceneSpriteResourceFilenameGet();
   /// Return true if the given cutscene exists.
   static bool CutsceneExistsCheck(const char* _cutsceneName);

   /// Return the time in the current cutscene segment's timeline.
   virtual unsigned int SegmentTimeGet() { return (unsigned int)segmentTime; }
   /// Set the time to the given point in the current cutscene segment.  This
   /// may be very different from allowing time to pass normally to this point,
   /// so it should only be used in certain situations.
   virtual void SegmentTimeSet(unsigned int _segmentTime);
   /// Same as 'SegmentTimeSet', but it goes to the event with the given index
   /// rather than specifying a time directly.
   virtual void SegmentTimeSetByEventIndex(int eventIndex);
   /// Same as 'SegmentTimeSet', but it goes to the event with the given name
   /// rather than specifying a time directly.
   virtual void SegmentTimeSetByEventName(const char* eventName);
   /// Return the duration of the given segment in the current cutscene.
   virtual unsigned int SegmentDurationGet(int segmentIndex);
   /// Return the current segment index.
   virtual int SegmentIndexGet() { return currentSegmentIndex; }
   /// Set the time to the given part of the given segment.  This may be very
   /// different from allowing time to pass normally to this point, so it
   /// should only be used in certain situations.
   virtual void SegmentIndexSet(int _segmentIndex, unsigned int _segmentTime = 0);
   /// Same as 'SegmentIndexSet', but it goes to the event with the given index
   /// rather than specifying a time directly.
   virtual void SegmentIndexSetByEventIndex(int _segmentIndex, int eventIndex);
   /// Same as 'SegmentIndexSet', but it goes to the event with the given name
   /// rather than specifying a time directly.
   virtual void SegmentIndexSetByEventName(int _segmentIndex, const char* eventName);

   /// Return the number of segments in the current cutscene.
   virtual int SegmentCountGet() { return segments.SizeGet(); }
   /// Return the given segment.
   virtual CutsceneWidgetSegment* SegmentGet(int segmentIndex);
   /// Return the index of the first segment with the given name.  Return -1 if
   /// no match is found.
   virtual int SegmentIndexGet(const char* _segmentName);

   /// Switch to the given stage state of the givent cutscene segment.
   virtual void StageStateSet(const char* _stateName, int segmentIndex);
   /// Return the widget containing the individual segments of the cutscene.
   /// The content of the cutscene is displayed in its descendants.
   virtual StateContainerWidget* StageWidgetGet() { return stageWidget; }
   /// Return the widget containing the stage states of the given segment.
   virtual StateContainerWidget* StageSegmentWidgetGet(int segmentIndex);
   /// Return the descendant of the stage widget for the given segment by the given path.
   Widget* StageSegmentDescendantGetByPath(int segmentIndex, const char* path);

   /// Switch to the given dialog style.
   virtual void DialogStyleSet(const char* dialogStyleName);
   /// Begin displaying the given text key as dialog text in the current style.
   virtual void DialogTextKeySet(const char* _dialogTextKey);
   /// Begin displaying the given text as dialog text in the current style.
   virtual void DialogTextSet(const char* _dialogText);
   /// Begin displaying the given text key as the dialog title text in the
   /// current style.
   virtual void DialogTitleTextKeySet(const char* _dialogTitleTextKey);
   /// Begin displaying the given text as the dialog title text in the current
   /// style.
   virtual void DialogTitleTextSet(const char* _dialogTitleText);
   /// Set the default animation of the dialog portrait in the current style.
   virtual void DialogPortraitAnimationSet(const char* _spriteResourceFilename, const char* _animationName);
   /// Set the talking animation of the dialog portrait in the current style.
   virtual void DialogPortraitTalkAnimationSet(const char* _spriteResourceFilename, const char* _animationName);
   /// Set the relative position of the current dialog style widget.
   virtual void DialogStylePositionRelativeSet(const Point2F& _positionRelative);
   /// True if dialog is still displaying.
   virtual bool DialogPendingCheck();
   /// If the teletype effect is active, finish it.  If waiting to proceed to
   /// the next page, proceed to the next page.  If waiting to dismiss the final
   /// page of a dialog, dismiss it.
   virtual void DialogContinue();
   /// Close any current dialog.
   virtual void DialogFinish();
   /// Finish the teletype effect for dialog text.
   virtual void DialogTeletypeEffectFinish();
   /// Return true if the teletype effect should be used to bring dialog text
   /// onto the screen.
   virtual bool DialogTeletypeEffectEnabledCheck() { return dialogTeletypeEffectEnabled; }
   /// Set whether the teletype effect should be used to bring dialog text onto
   /// the screen.
   virtual void DialogTeletypeEffectEnabledSet(bool _dialogTeletypeEffectEnabled);
   /// Name of the sound file to stream as speech.
   virtual void DialogSpeechPlay(const char* filename);

   /// Begin fading to the specified color at the given cutscene time for the
   /// given number of milliseconds.
   virtual void FadeToColor(unsigned int _fadeTimeBegin, unsigned int _fadeDuration, ColorRGBA8 _fadeColorEnd);

   /// Return the number of cutscene events in the given segment.
   virtual int CutsceneEventCountGet(int segmentIndex);
   /// Return the given cutscene event in the given segment.
   virtual CutsceneWidgetEvent* CutsceneEventGet(int eventIndex, int segmentIndex);
   /// Return the first cutscene event with the given name in the given
   /// segment.
   virtual CutsceneWidgetEvent* CutsceneEventGet(const char* eventName, int segmentIndex);
   /// Return the index of the first cutscene event with the given name in the
   /// given segment.
   virtual int CutsceneEventIndexGet(const char* eventName, int segmentIndex);

   /// Use the given JSON data for event templates.
   virtual void CutsceneEventTemplatesSet(JSONValue* _eventTemplates);
   /// Apply the event templates to the given specs.
   virtual void CutsceneEventTemplatesApply(JSONValue* eventSpecs, HeapID tempHeapID = HEAP_TEMP);

   /// Type of the function that can receive the PreCutsceneSegmentBegin event.
   /// The CutsceneWidget* is the widget in which the cutscene is occuring, the
   /// int is the index of the new segment, and the void* is the userData pointer.
   typedef void (*PreCutsceneSegmentBeginCallback)(CutsceneWidget*, int, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the PreCutsceneSegmentBegin event occurs.
   /// For now, only one function can be registered per instance.
   void PreCutsceneSegmentBeginRegister(PreCutsceneSegmentBeginCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnCutsceneSegmentBegin event occurs.
   void PreCutsceneSegmentBeginUnregister(PreCutsceneSegmentBeginCallback callback);

   /// Type of the function that can receive the OnCutsceneSegmentBegin event.
   /// The CutsceneWidget* is the widget in which the cutscene is occuring, the
   /// int is the index of the new segment, and the void* is the userData pointer.
   typedef void (*OnCutsceneSegmentBeginCallback)(CutsceneWidget*, int, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnCutsceneSegmentBegin event occurs.
   /// For now, only one function can be registered per instance.
   void OnCutsceneSegmentBeginRegister(OnCutsceneSegmentBeginCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnCutsceneSegmentBegin event occurs.
   void OnCutsceneSegmentBeginUnregister(OnCutsceneSegmentBeginCallback callback);

   /// Type of the function that can receive the OnCutsceneEventBegin event.
   /// The CutsceneWidget* is the widget in which the cutscene is occuring, the
   /// CutsceneWidgetEvent represents the event, and the void* is the userData
   /// pointer.  Return true if the event should happen and return false if it
   /// should not.
   typedef bool (*OnCutsceneEventBeginCallback)(CutsceneWidget*, CutsceneWidgetEvent*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnCutsceneEventBegin event occurs.
   /// For now, only one function can be registered per instance.
   void OnCutsceneEventBeginRegister(OnCutsceneEventBeginCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnCutsceneEventBegin event occurs.
   void OnCutsceneEventBeginUnregister(OnCutsceneEventBeginCallback callback);
   /// Convenience function for finding a CutsceneWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnCutsceneEventBeginRegister(const char* path, OnCutsceneEventBeginCallback callback, void* userData = NULL);

   /// Associate the given factory with the given type name.
   void CutsceneWidgetEventFactoryRegister(const char* name, CutsceneWidgetEventFactory factory);
   /// Return the factory with the given type name.
   CutsceneWidgetEventFactory CutsceneWidgetEventFactoryGet(const char* name);

   /// Return the coefficient used by debug builds for fast-fowarding.
   float DebugCutsceneTimeCoefficientGet() { return debugCutsceneTimeCoefficient; }
   /// Set the coefficient used by debug builds for fast-fowarding.
   void DebugCutsceneTimeCoefficientSet(float _debugCutsceneTimeCoefficient) { debugCutsceneTimeCoefficient = _debugCutsceneTimeCoefficient; }
   /// Begin the given segment.  This is likely too buggy for general use, but
   /// it can be used in certain situations internal to CutsceneWidget and for
   /// debugging.
   virtual void DebugSegmentIndexSet(int segmentIndex);
   /// Debug feature for adjusting the time within a cutscene.  This is likely
   /// buggy, so generally only use it for debug purposes.  It's slower than
   /// the normal SegmentTimeSet, but tries to make the result closer to how it
   /// would seem if time had passed normally to this point.
   void DebugSegmentTimeSet(unsigned int _segmentTime);
   /// Enable or disable a debug feature that prevents cutscene segments and
   /// cutscenes from ending when the end is reached.
   void DebugSegmentFinishAllowedSet(bool _debugSegmentFinishAllowed) { debugSegmentFinishAllowed = _debugSegmentFinishAllowed; }
   /// Debug feature for allowing or disallowing automatic reloading of
   /// cutscenes.
   void DebugAutomaticReloadAllowedSet(bool _debugAutomaticReloadAllowed) { debugAutomaticReloadAllowed = _debugAutomaticReloadAllowed; }
   /// Debug event for when a change in the files has been detected.
   virtual void DebugOnFileChange();
   /// Refresh the file modification times to avoid false-positives for
   /// external changes to files.
   void DebugFileModificationTimesRefresh();

   /// Type of the function that can receive the DebugOnFileChange event.
   /// The CutsceneWidget* is the widget with the cutscene with a file that
   /// changed, and the void* is the userData pointer.
   typedef void (*DebugOnFileChangeCallback)(CutsceneWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the DebugOnFileChange event occurs.  The event might not be
   /// called on all platforms or outside of a debug build.
   /// For now, only one function can be registered per instance.
   void DebugOnFileChangeRegister(DebugOnFileChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the DebugOnFileChange event occurs.
   void DebugOnFileChangeUnregister(DebugOnFileChangeCallback callback);
   
   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) CutsceneWidget(); }

   typedef ContainerWidget Inherited;

protected:
   typedef Map<const char*, CutsceneWidgetEventFactory> CutsceneWidgetEventFactoryMap;

   /// Called before a cutscene segment begins.
   virtual void PreCutsceneSegmentBegin(int _segmentIndex);
   /// Called when a cutscene segment begins.
   virtual void OnCutsceneSegmentBegin(int _segmentIndex);
   /// Called when a cutscene event begins.
   virtual void OnCutsceneEventBegin(CutsceneWidgetEvent* event);
   /// Helper function for setting up the given segment.
   virtual void SegmentCreateHelper(int segmentIndex);
   /// Helper function for setting up the StageWidgets for the given segment.
   virtual void StageSegmentWidgetsCreate(int segmentIndex);
   /// Helper function for setting up the events for the given segment.
   virtual void SegmentEventsCreate(int segmentIndex);
   /// Helper function for recursively gathering the WidgetModifiers relevant
   /// to the segment.
   virtual void SegmentModifiersGather(Widget* widget, CutsceneWidgetSegment* segment);
   /// Hide the indicator that the dialog is waiting for player input to
   /// continue.
   virtual void DialogContinueIndicatorHide();
   /// Reveal the indicator that the dialog is waiting for player input to
   /// continue.
   virtual void DialogContinueIndicatorShow();
   /// Refresh which dialog portrait animation should be displaying.
   virtual void DialogPortraitsRefresh();
   /// Clean up the most recent dialog.
   virtual void DialogClear();
   /// Set the position of the current dialog style widget to its default.
   virtual void DialogStylePositionReset();
   /// Return the current color to use for fading.
   virtual ColorRGBA8 FadeColorGet();
   /// Helper function for cleaning up fading.
   virtual void FadeClear();
   /// Update the fading.
   virtual void FadeUpdate(unsigned int dt);
   /// Apply the latest fade color.
   virtual void FadeApply();
   /// Helper function for updating the appropriate segments.
   virtual void SegmentUpdateHelper(unsigned int dt, bool processEvents);
   /// Helper function for setting the segment.
   virtual void SegmentSetHelper(int segmentIndex, bool resetStageState);
   /// Helper function for setting the segment time.
   virtual void SegmentTimeSetHelper(unsigned int _segmentTime, int _targetEventIndex = -1);
   /// Debug helper function for finding widgets of certain types.
   virtual void DebugSegmentTimeSetWidgetsGather(Widget* widget, Table<Sprite*>* sprites, Table<ParticleEffect2D*>* particleEffects);

   /// Name of the current cutscene, if any.
   const char* cutsceneName;
   /// Sprite resource filename for the current cutscene.
   char cutsceneSpriteResourceFilename[FROG_PATH_MAX];
   /// True if a cutscene is in the process of loading.
   bool cutsceneLoading;
   /// True if the end of the cutscene has been reached.
   bool cutsceneFinished;
   /// Index of the current segment of the cutscene.
   int currentSegmentIndex;
   /// Time in the timeline of the current cutscene segment in milliseconds.
   unsigned int segmentTime;
   /// Index of the next event to begin.
   int eventBeginIndex;
   /// Counter for whether a SegmentTimeSetHelper is in progress.
   int segmentTimeSetHelperCounter;
   /// True if a SegmentTimeSet is waiting to be executed.
   bool segmentTimeSetPending;
   /// If 'segmentTimeSetPending' is true, this is the target time.
   unsigned int segmentTimeSetPendingSegmentTime;
   /// If 'segmentTimeSetPending' is true and this is not -1, this is the next
   /// event which should be executed.
   int segmentTimeSetPendingEventIndex;
   /// Counter for whether a SegmentUpdateHelper call is in-progress.
   int segentUpdateHelperCounter;
   /// True if a dialog should be showing and false if it should be clear on
   /// the next update.
   bool dialogPending;
   /// True if the teletype effect should be used to bring dialog text onto the
   /// screen.
   bool dialogTeletypeEffectEnabled;
   /// Name of the dialog style which should be used by default.
   const char* dialogStyleNameDefault;
   /// Widget containing the individual segments of the cutscene.
   StateContainerWidget* stageWidget;
   /// Widget drawn above the stage for fading.
   ContainerWidget* fadeWidget;
   /// Widget containing the individual dialog styles available to cutscenes.
   StateContainerWidget* dialogStylesWidget;
   /// Widget in the current style for displaying dialog text.
   TextDocumentWidget* dialogTextWidget;
   /// Widget in the current style for displaying dialog title text.
   LabelWidget* dialogTitleTextWidget;
   /// Widget which is displayed when the dialog is waiting for player input to
   /// continue.
   Widget* dialogContinueIndicatorWidget;
   /// Widget in the current style for displaying a portrait.
   SpriteWidget* dialogPortraitWidget;
   /// Filename of the portrait default sprite animation's resource file.
   const char* dialogPortraitSpriteResourceFilename;
   /// Name of the portrait default sprite animation.
   const char* dialogPortraitSpriteAnimationName;
   /// Filename of the portrait talking sprite animation's resource file.
   const char* dialogPortraitTalkSpriteResourceFilename;
   /// Name of the portrait talking sprite animation.
   const char* dialogPortraitTalkSpriteAnimationName;
   /// Object used to stream speech for a dialog.
   SoundStreamSource* dialogSpeechSoundStreamSource;
   /// Default positions of the dialog styles.
   Table<Point2F> dialogStylePositionOffsetDefaults;
   /// Color to use for the beginning of a fade.
   ColorRGBA8 fadeColorBegin;
   /// Color to use for the end of a fade.
   ColorRGBA8 fadeColorEnd;
   /// Time remaining in the current fade in milliseconds.
   unsigned int fadeTimer;
   /// Duration of the current fade in milliseconds.
   unsigned int fadeDuration;
   /// Templates to use when setting up events.
   JSONValue* eventTemplates;
   /// Function to be called for the PreCutsceneSegmentBegin event.
   PreCutsceneSegmentBeginCallback preCutsceneSegmentBeginCallback;
   /// Pointer to be passed to the PreCutsceneSegmentBegin callback.
   void* preCutsceneSegmentBeginUserData;
   /// Function to be called for the OnCutsceneSegmentBegin event.
   OnCutsceneSegmentBeginCallback onCutsceneSegmentBeginCallback;
   /// Pointer to be passed to the OnCutsceneSegmentBegin callback.
   void* onCutsceneSegmentBeginUserData;
   /// Function to be called for the OnCutsceneEventBegin event.
   OnCutsceneEventBeginCallback onCutsceneEventBeginCallback;
   /// Pointer to be passed to the OnCutsceneEventBegin callback.
   void* onCutsceneEventBeginUserData;
   /// Collection of segments of the current cutscene.
   Table<CutsceneWidgetSegment*> segments;
   /// Map of names to functions that can create the associated objects.
   CutsceneWidgetEventFactoryMap cutsceneWidgetEventFactories;

   /// Debug builds use this for fast-forwarding.
   float debugCutsceneTimeCoefficient;
   /// Helps to avoid underflow when using a time coefficient less than 1.0.
   float debugCutsceneDTAccumulator;
   /// Debug feature that prevents cutscene segments and cutscenes from ending
   /// when the end is reached.
   bool debugSegmentFinishAllowed;
   /// True if automatic reloading is allowed.
   bool debugAutomaticReloadAllowed;
   /// Function to be called for the DebugOnFileChange event.
   DebugOnFileChangeCallback debugOnFileChangeCallback;
   /// Pointer to be passed to the DebugOnFileChange callback.
   void* debugOnFileChangeUserData;

   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// Called when the program gains the focus of the OS.
      void OnOSFocusGain();
   #endif //#if _DEBUG && PLATFORM_IS_WINDOWS
};

//==============================================================================

/// A single segment of a cutscene.
class CutsceneWidgetSegment
{
public:
   CutsceneWidgetSegment();
   void Init();
   void Deinit();

   //// Return the specifications for this segment.
   JSONValue* SpecificationsGet() { return specs; }

   /// Set the name of this segment.
   void NameSet(const char* _name);
   /// Return the name of this segment, if any.
   const char* NameGet() { return name; }

protected:
   /// Name of the segment.
   const char* name;
   /// Specifications for the segment.
   JSONValue* specs;
   /// Duration of the segment in milliseconds.
   unsigned int duration;
   /// WidgetModifiers which should have their time synchronized with the
   /// cutscene.
   Table<WidgetModifier*> syncedModifiers;
   /// WidgetModifiers which should loop independently of the timeline.
   Table<WidgetModifier*> independentModifiers;
   /// Events which occur over the course of a cutscene.
   Table<CutsceneWidgetEvent*> events;

   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// File modification time from when the segment file was last loaded.
      int64 segmentFileModificationTime;
   #endif //#if defined _DEBUG && PLATFORM_IS_WINDOWS

   friend class CutsceneWidget;
};

//==============================================================================

/// Base class for an event which occurs during a cutscene.
class CutsceneWidgetEvent
{
public:
   CutsceneWidgetEvent();

   virtual void Init(const char* _name, int _segmentIndex, CutsceneWidget* _cutsceneWidget, JSONValue* _specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt) { (void)dt; }

   /// Called when the event should begin.
   virtual void OnEventBegin() { onEventBeginCalled = true; }
   /// Return true if OnEventBegin has already been called.
   virtual bool OnEventBeginCalledCheck() { return onEventBeginCalled; }
   /// Return true if this event needs the timeline to be stopped for now
   /// before beginning the next event.
   virtual bool TimelineBlockCheck() { return false; }

   /// Return the name of this event.
   const char* NameGet() { return name; }
   /// Return the specifications used to create this event.
   JSONValue* SpecificationsGet() { return specifications; }
   /// Return the time at which this event should begin.
   unsigned int TimeGet() const { return time; }

   /// Comparator for sorting events into increasing time.
   static bool TimeComparator(const CutsceneWidgetEvent* first,
      const CutsceneWidgetEvent* second)
   {
      return first->TimeGet() < second->TimeGet();
   }

   /// Reset the event to be as if it hadn't yet begun.
   virtual void Reset() { onEventBeginCalled = false; }

protected:
   /// Name of this event.
   const char* name;
   /// CutsceneWidget of which this event is a part.
   CutsceneWidget* cutsceneWidget;
   /// Index of the segment of which this event is a part.
   int segmentIndex;
   /// Specifications used to set up the event.
   JSONValue* specifications;
   /// Time at which this event should begin.
   unsigned int time;
   /// True if 'OnEventBegin' has been called.
   bool onEventBeginCalled;
};

//==============================================================================

/// Displays a dialog.
class CutsceneWidgetEvent_DialogPlay : public CutsceneWidgetEvent
{
public:
   CutsceneWidgetEvent_DialogPlay();

   virtual void Init(const char* _name, int _segmentIndex, CutsceneWidget* _cutsceneWidget, JSONValue* _specifications);
   virtual void Deinit();
   virtual void OnEventBegin();
   virtual bool TimelineBlockCheck();

   /// Set which text should be displayed for this dialog.
   void TextSet(const char* _text);
   /// Set which text key should be displayed for this dialog.
   void TextKeySet(const char* _textKey);

   static CutsceneWidgetEvent* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) CutsceneWidgetEvent_DialogPlay(); }

   typedef CutsceneWidgetEvent Inherited;

protected:
   /// Text to display for the dialog.
   const char* text;
   /// Text key to display for the dialog.
   const char* textKey;
   /// Filename of the portrait default sprite animation's resource file.
   const char* portraitSpriteResourceFilename;
   /// Name of the portrait default sprite animation.
   const char* portraitSpriteAnimationName;
   /// Filename of the portrait talking sprite animation's resource file.
   const char* portraitTalkSpriteResourceFilename;
   /// Name of the portrait talking sprite animation.
   const char* portraitTalkSpriteAnimationName;
   /// Preloaded default sprite animation.
   SpriteAnimation* portraitAnimation;
   /// Preloaded talking sprite animation.
   SpriteAnimation* portraitTalkAnimation;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__CUTSCENEWIDGET_H__

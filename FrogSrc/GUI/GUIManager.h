#ifndef __FROG__GUI__GUIMANAGER_H__
#define __FROG__GUI__GUIMANAGER_H__

#include "FrogMemory.h"
#include "Utility.h"
#include "List.h"
#include "Map.h"
#include "Point2.h"
#include "MouseManager.h"
#include "Table.h"
#include "Debug.h"
#include "HoldTracker.h"
#include "Color.h"
#include "Matrix43.h"
#include "Texture.h"

namespace Webfoot {

class LayerWidget;
class Widget;
class ContainerWidget;
class WidgetTransition;
class WidgetModifier;
class WidgetLayout;
class JSONValue;
class Sound;
class Font;
class SpriteWidget;

// Some platforms use multi-touch by default.
#if PLATFORM_IS_IOS || PLATFORM_IS_ANDROID || PLATFORM_IS_WII || PLATFORM_IS_EMSCRIPTEN
   #define GUI_MANAGER_USE_MULTIPLE_MICE_DEFAULT true
#else
   #define GUI_MANAGER_USE_MULTIPLE_MICE_DEFAULT false
#endif

/// Default number of objects for keeping track of which widgets have the focus.
#define GUI_FOCUS_COUNT_DEFAULT 5

/// Pattern for the folder of a given layer.
#define GUI_LAYER_FOLDER_NAME_PATTERN "GUI/%s"

/// Type name for a sprite widget
#define SPRITE_WIDGET_TYPE "Sprite"
/// Type name for a press button widget
#define PRESS_BUTTON_WIDGET_TYPE "PressButton"
/// Type name for a toggle button widget
#define TOGGLE_BUTTON_WIDGET_TYPE "ToggleButton"
/// Type name for a slider widget
#define SLIDER_WIDGET_TYPE "Slider"
/// Type name for a label widget
#define LABEL_WIDGET_TYPE "Label"
/// Type name for a scroll box widget
#define SCROLL_BOX_WIDGET_TYPE "ScrollBox"
/// Type name for a drop down list widget
#define DROP_DOWN_LIST_WIDGET_TYPE "DropDownList"
/// Type name for a state container widget
#define STATE_CONTAINER_WIDGET_TYPE "StateContainer"
/// Type name for a TableWidget
#define TABLE_WIDGET_TYPE "Table"
/// Type name for a button used for the title of a TableWidget column.
#define TABLE_COLUMN_TITLE_BUTTON_WIDGET_TYPE "TableColumnTitleButton"
/// Type name for a container widget
#define CONTAINER_WIDGET_TYPE "Container"
/// Type name for a number label widget
#define NUMBER_LABEL_WIDGET_TYPE "NumberLabel"
/// Type name for a ParticleEffect2D widget
#define PARTICLE_EFFECT_WIDGET_TYPE "ParticleEffect"
/// Type name for a progress bar widget
#define PROGRESS_BAR_WIDGET_TYPE "ProgressBar"
/// Type name for a RadioButtonContainerWidget
#define RADIO_BUTTON_CONTAINER_WIDGET_TYPE "RadioButtonContainer"
/// Type name for a RadioButtonWidget
#define RADIO_BUTTON_WIDGET_TYPE "RadioButton"
/// Type name for a ring selector widget
#define RING_SELECTOR_WIDGET_TYPE "RingSelector"
/// Type name for a text document widget
#define TEXT_DOCUMENT_WIDGET_TYPE "TextDocument"
/// Type name for a text entry widget
#define TEXT_ENTRY_WIDGET_TYPE "TextEntry"
/// Type name for a keyboard widget
#define KEYBOARD_WIDGET_TYPE "Keyboard"
/// Type name for a joystick widget
#define JOYSTICK_WIDGET_TYPE "Joystick"
/// Type name for a screen fill sprite widget
#define SCREEN_FILL_SPRITE_WIDGET_TYPE "ScreenFillSprite"
/// Type name for an incremental number label widget
#define INCREMENTAL_NUMBER_LABEL_WIDGET_TYPE "IncrementalNumberLabel"
/// Type name for an item list selector widget
#define ITEM_LIST_SELECTOR_WIDGET_TYPE "ItemListSelector"
/// Type name for a JSON editor widget
#define JSON_EDITOR_WIDGET_TYPE "JSONEditor"
/// Type name for a LinearSelectorWidget
#define LINEAR_SELECTOR_WIDGET_TYPE "LinearSelector"
/// Type name for a TreeWidget
#define TREE_WIDGET_TYPE "Tree"
/// Type name for a CutsceneWidget
#define CUTSCENE_WIDGET_TYPE "Cutscene"
/// Type name for a JSON widget.
#define JSON_WIDGET_TYPE "JSON"
/// Type name for a ColorSelector widget.
#define COLOR_SELECTOR_WIDGET_TYPE "ColorSelector"
/// Type name for a FileSelector widget.
#define FILE_SELECTOR_WIDGET_TYPE "FileSelector"
/// Type name for a MaskTexture widget.
#define MASK_TEXTURE_WIDGET_TYPE "MaskTexture"
/// Used in widget specifications when a template should be used.
#define WIDGET_TEMPLATE_INHERITED_KEY "Inherited"

/// Type of the function that can receive the LayerWidget::OnInit event.
/// The LayerWidget* is the layer that was initialized, and the void*
/// is the userData pointer.
typedef void (*LayerWidgetOnInitCallback)(LayerWidget*, void*);

//=============================================================================

/// GUIFocus helps keep track of which widget is currently selected for input
/// from a set of associated devices, like keyboards or gamepads.
/// This includes a stack of Widget pointers to help restore focus after an
/// interruption.  Note that restoration will not work if the widget is freed
/// during the interruption.  Methods like UpPressed or LeftJustReleased will
/// not be accurate for the current frame until theGUI->Update is called.
class GUIFocus
{
public:
   GUIFocus();
   
   void Init(int _focusIndex);
   void Deinit();
   void Update(unsigned int dt);

   /// Return the widget that currently has the focus.
   Widget* WidgetGet();
   /// Remember that the given widget now has the focus.
   /// 'keypadDirectionMovedFocus' should be true if the focus moved because a
   /// GUIKeypad indicated a direction.  It should be false if the focus was
   /// moved by something else, like GUIManager::FocusWidgetSet.
   void WidgetSet(Widget* widget, bool keypadDirectionMovedFocus);

   /// Return the widget that had this object's focus the last time an
   /// affirmation began.  If there was no widget with this focus or if that
   /// widget is no longer available, this should be NULL.
   Widget* LastPressWidgetGet() { return lastPressWidget; }
   /// See LastPressWidgetGet.
   void LastPressWidgetSet(Widget* widget) { lastPressWidget = widget; }
   
   /// Push a level onto the widget focus stack.  This makes it easy to restore
   /// the focus later.
   void WidgetPush();
   /// Remove a level of the focus stack.
   void WidgetPop();
   /// Clear the focus stack.
   void WidgetStackClear();

   /// Set whether the widget with this focus should respond to input.
   void EnabledSet(bool _enabled);
   /// Return true if the widget with this focus should respond to input.
   bool EnabledCheck() { return enabled; }

   /// Set whether the widget which has this focus should be highlighted,
   /// barring something like all the input devices being disconnected.
   void HighlightedSet(bool _highlighted) { highlighted = _highlighted; }
   /// Return the most recent setting from HighlightedSet.
   bool HighlightedCheck() { return highlighted; }
   /// Return true if the widget which has this focus should be highlighted.
   /// This is affected by things other than HighlightedSet, like whether any
   /// associated input devices are connected.
   bool HighlightedEffectiveCheck();

   /// Return true if there are any connected keypads associated with this
   /// focus.
   bool KeypadConnectedCheck();

   /// Return true if the keypads for this focus are affirming the current
   /// widget.
   bool AffirmativePressed() { return affirmativePressed; }
   /// Return true if the keypads for this focus just began affirming the
   /// current widget, as of this update.
   bool AffirmativeJustPressed() { return affirmativePressed && !lastAffirmativePressed; }
   /// Return true if the keypads for this focus just stopped affirming the
   /// current widget, as of this update.
   bool AffirmativeJustReleased() { return !affirmativePressed && lastAffirmativePressed; }

   /// Return true if the keypads for this focus are indicating negative.
   bool NegativePressed() { return negativePressed; }
   /// Return true if the keypads for this focus just began indicate negative,
   /// as of this update.
   bool NegativeJustPressed() { return negativePressed && !lastNegativePressed; }
   /// Return true if the keypads for this focus just stopped indicating
   /// negative, as of this update.
   bool NegativeJustReleased() { return !negativePressed && lastNegativePressed; }

   /// Return true if the keypads for this focus are currently indicating up.
   bool UpPressed() { return DirectionPressed(DIRECTION_UP); }
   /// Return true if the keypads for this focus just began indicating up, as
   /// of this update.
   bool UpJustPressed() { return DirectionJustPressed(DIRECTION_UP); }
   /// Return true if the keypads for this focus just stopped indicating up,
   /// as of this update.
   bool UpJustReleased() { return DirectionJustReleased(DIRECTION_UP); }
   /// Return true if this frame would be appropriate for a repeated upward
   /// move of the focus.
   bool UpHeld() { return DirectionHeld(DIRECTION_UP); }

   /// Return true if the keypads for this focus are currently indicating
   /// down.
   bool DownPressed() { return DirectionPressed(DIRECTION_DOWN); }
   /// Return true if the keypads for this focus just began indicating down,
   /// as of this update.
   bool DownJustPressed() { return DirectionJustPressed(DIRECTION_DOWN); }
   /// Return true if the keypads for this focus just stopped indicating down,
   /// as of this update.
   bool DownJustReleased() { return DirectionJustReleased(DIRECTION_DOWN); }
   /// Return true if this frame would be appropriate for a repeated downward
   /// move of the focus.
   bool DownHeld() { return DirectionHeld(DIRECTION_DOWN); }

   /// Return true if the keypads for this focus are currently indicating
   /// left.
   bool LeftPressed() { return DirectionPressed(DIRECTION_LEFT); }
   /// Return true if the keypads for this focus just began indicating left,
   /// as of this update.
   bool LeftJustPressed() { return DirectionJustPressed(DIRECTION_LEFT); }
   /// Return true if the keypads for this focus just stopped indicating left,
   /// as of this update.
   bool LeftJustReleased() { return DirectionJustReleased(DIRECTION_LEFT); }
   /// Return true if this frame would be appropriate for a repeated leftward
   /// move of the focus.
   bool LeftHeld() { return DirectionHeld(DIRECTION_LEFT); }

   /// Return true if the keypads for this focus are currently indicating
   /// right.
   bool RightPressed() { return DirectionPressed(DIRECTION_RIGHT); }
   /// Return true if the keypads for this focus just began indicating right,
   /// as of this update.
   bool RightJustPressed() { return DirectionJustPressed(DIRECTION_RIGHT); }
   /// Return true if the keypads for this focus just stopped indicating
   /// right, as of this update.
   bool RightJustReleased() { return DirectionJustReleased(DIRECTION_RIGHT); }
   /// Return true if this frame would be appropriate for a repeated rightward
   /// move of the focus.
   bool RightHeld() { return DirectionHeld(DIRECTION_RIGHT); }

   /// Return true if the keypads for this focus are currently indicating
   /// the given direction.
   bool DirectionPressed(Direction direction);
   /// Return true if the keypads for this focus just began indicating the
   /// given direction, as of this update.
   bool DirectionJustPressed(Direction direction);
   /// Return true if the keypads for this focus just stopped indicating
   /// the given direction, as of this update.
   bool DirectionJustReleased(Direction direction);
   /// Return true if this frame would be appropriate for a repeated move of
   /// the focus in the given direction.
   bool DirectionHeld(Direction direction);

   /// Notify the focus object that a widget has been added to the garbage.
   void WidgetGarbageAddNotify(Widget* widget);

protected:
   /// Index of this GUIFocus object.
   int focusIndex;
   /// True if the widget with this focus should respond to input.
   bool enabled;
   /// True if this focus should be highlighted, barring something like all the
   /// input devices being disconnected.
   bool highlighted;
   /// True if the keypads for this selection are currently affirming.
   bool affirmativePressed;
   /// Value of 'affirmativePressed' on the previous update.
   bool lastAffirmativePressed;
   /// True if the keypads for this selection are currently indicating
   /// negative.
   bool negativePressed;
   /// Value of 'negativePressed' on the previous update.
   bool lastNegativePressed;
   /// See LastPressWidgetGet.
   Widget* lastPressWidget;
   /// Stack of selected widgets.
   List<Widget*> widgetStack;
   /// True if the keypads for this selection are currently indicating the
   /// corresponding direction.
   bool directionPressed[DIRECTION_CARDINAL_COUNT];
   /// Values from 'directionPressed' on the previous update.
   bool lastDirectionPressed[DIRECTION_CARDINAL_COUNT];
   /// True if this frame would be appropriate for a repeated move of the
   /// focus.
   bool directionHeld[DIRECTION_CARDINAL_COUNT];
   /// Keeps track of whether the focus should move in particular directions.
   HoldTracker directionHoldTrackers[DIRECTION_CARDINAL_COUNT];
};

//=============================================================================

/// GUIKeypad is a base class for different types of directional input devices
/// that must interact with the GUI, including keyboards and gamepads.  A given
/// keypad can be associated with multiple input foci, if needed.  By default,
/// keypads are associated with input focus index 0.
class GUIKeypad
{
public:
   virtual ~GUIKeypad() {}

   void Init() { focusFlags = 1; }
   virtual void Deinit() { focusFlags = 1; }

   /// Return true if the keypad is connected.
   virtual bool ConnectedCheck() = 0;

   /// Return true if the keypad's button for affirming a selection is
   /// currently pressed.
   virtual bool AffirmativePressed() = 0;
   /// Return true if the keypad's button for indicating negative is currently
   /// pressed.
   virtual bool NegativePressed() = 0;

   /// Return true if the keypad is indicating a direction of up.
   virtual bool UpPressed() = 0;
   /// Return true if the keypad is indicating a direction of down.
   virtual bool DownPressed() = 0;
   /// Return true if the keypad is indicating a direction of left.
   virtual bool LeftPressed() = 0;
   /// Return true if the keypad is indicating a direction of right.
   virtual bool RightPressed() = 0;

   /// Return true if this keypad affects the given focus.
   bool FocusIndexCheck(int index);
   /// Set whether this keypad affects the given focus.
   /// For now, the permitted range of keypad focus indices is [0, 31].
   void FocusIndexSet(int index, bool affects);

protected:
   /// Flags for keeping track of which foci can be affected by this.
   uint32 focusFlags;
};

//=============================================================================

/// Associates a mouse with any addition data needed for gui purposes.
struct GUIMouse
{
   /// The mouse in question.
   Mouse* mouse;
   /// Widget that was under the cursor of the associated mouse on its last
   /// left mouse button press.  If there was no widget underneath or if that
   /// widget is no longer available, this should be NULL.
   Widget* lastPressWidget;
   /// Widget that was under the cursor of the associated mouse on the last update.
   /// If there was no widget underneath, this should be NULL.
   Widget* lastWidgetUnderCursor;
   /// Most recent widget to receive an OnClick event due to this mouse.
   Widget* lastClickWidget;
   /// Timer for determining whether a widget has been double-clicked.
   unsigned int doubleClickTimer;
   /// Widget that was under the cursor of the associated mouse on its last
   /// right mouse button press.  If there was no widget underneath or if that
   /// widget is no longer available, this should be NULL.
   Widget* lastRightPressWidget;
   /// Widget that is currently being drag-scrolled.
   Widget* lastDragScrollWidget;
   /// Position of the cursor when the mouse's left button was last pressed.
   Point2F lastPressPosition;
   /// Position of the cursor when a drag-scroll of this mouse last began.
   Point2F lastDragScrollBeginPosition;
   /// True if a tooltip should be visible.
   bool tooltipVisible;
   /// Timer which controls when tooltips appear.
   unsigned int tooltipTimer;
   /// Most recently computed bounds for a tooltip.
   Box2F tooltipBounds;
   
   void Init(Mouse* _mouse)
   {
      mouse = _mouse;
      lastPressWidget = NULL;
      lastWidgetUnderCursor = NULL;
      lastClickWidget = NULL;
      doubleClickTimer = 0;
      lastRightPressWidget = NULL;
      lastDragScrollWidget = NULL;
      lastPressPosition.Set(0.0f, 0.0f);
      lastDragScrollBeginPosition.Set(0.0f, 0.0f);
      tooltipVisible = false;
      tooltipTimer = 0;
      tooltipBounds.Set(0.0f, 0.0f, 0.0f, 0.0f);
      focusFlags = 1;
   }

   /// Clear the double-click timer so that the next press won't count as a
   /// double-click.
   void DoubleClickTimerClear() { doubleClickTimer = 0; }

   /// Return true if this mouse affects the given focus.
   bool FocusIndexCheck(int index);
   /// Set whether this mouse affects the given focus.
   /// For now, the permitted range of keypad focus indices is [0, 31].
   void FocusIndexSet(int index, bool affects);

protected:
   /// Flags for keeping track of which foci can be affected by this.
   uint32 focusFlags;
};

//=============================================================================

/// A single entry in the stack of mask texture widgets used while drawing.
struct MaskTextureStackEntry
{
   /// Texture to use as a mask.
   Texture* maskTexture;
   /// Matrix for determining which part of the texture mask to sample.
   Matrix43 maskMatrix;
};

//=============================================================================

/// Data cached by GUIManager::Preload
struct GUIManagerPreloadEntry
{
   /// Number of times this has been preloaded versus the number of times the
   /// preloading has been undone.
   int counter;
   /// Details used to build the LayerWidget
   JSONValue* specifications;
#if defined _DEBUG && PLATFORM_IS_WINDOWS
   /// Time at which the file containing the widget specifications was last modified.
   int64 widgetsFileModificationTime;
#endif //#if _DEBUG && PLATFORM_IS_WINDOWS
};

//=============================================================================

/// Manages the loading, updating, displaying, and cleanup of guis.  The gui
/// system is based on a stack of collections of widgets.  Each collection of
/// widgets is referred to as a layer.  By default, each layer causes all
/// layers below it to be treated as disabled, but this can be overridden.
class GUIManager
{
public:
   /// Create a WidgetTransition object of the given type for the specified widget
   /// using the given JSON specification.  If 'specificationTable' is NULL,
   /// use the defaults for the given type.  The function should not assume that
   /// initialization of the Widget has completed.
   typedef WidgetTransition* (*TransitionFactory)(Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);
   /// Create a WidgetModifier object with the given instance name and type name
   /// for the specified widget using the given JSON specification.  If
   /// 'specificationTable' is NULL, use the defaults for the given type.  The
   /// function should not assume that initialization of the Widget has completed.
   typedef WidgetModifier* (*ModifierFactory)(const char* name, Widget* widget, const char* type, JSONValue* specifications, HeapID heapID);
   /// Create a WidgetLayout of the given type, but do not initialize it.
   typedef WidgetLayout* (*LayoutFactory)(const char* type, HeapID heapID);
   /// Create a widget of the given type, but do not initialize it.
   typedef Widget* (*WidgetFactory)(const char* type, HeapID heapID);
   
   GUIManager();

   /// Initialize the GUIManager.  Specify whether multiple pointing devices
   /// should be allowed.  Specify the number of objects for keeping track of
   /// which widgets have the input focus [0-32].  Set '_scriptingEnabled' to
   /// true if scripting features should be enabled.
   void Init(bool _useMultipleMice = GUI_MANAGER_USE_MULTIPLE_MICE_DEFAULT,
      int _focusCount = GUI_FOCUS_COUNT_DEFAULT,
      bool _scriptingEnabled = false);
   void Deinit();
   /// Iterate through all the gui stack objects updating them.
   /// Call the widgets to respond to different events.
   void Update(unsigned int dt);
   /// Iterate through all the gui stack objects drawing them if they should be visible.
   void Draw();

   /// Add the layer with the given name to the top of the stack.  Return true if successful.
   /// 'onInitCallback' will be called when the layer's OnInit event occurs.  'userData'
   /// will be passed to the callback at that time to provide context.
   bool Push(const char* name, LayerWidgetOnInitCallback onInitCallback = NULL, void* userData = NULL);
   /// Remove the top layer from the stack.
   void Pop();
   /// Remove the layer with the given name.  Return true if successful.
   bool Remove(const char* name);
   /// Insert the layer with the given name above the topmost layer with the referencePointName.
   /// Return true if successful.
   /// 'onInitCallback' will be called when the layer's OnInit event occurs.  'userData'
   /// will be passed to the callback at that time to provide context.
   bool InsertAbove(const char* referencePointName, const char* name,
      LayerWidgetOnInitCallback onInitCallback = NULL, void* userData = NULL);
   /// Insert the layer with the given name below the topmost layer with the referencePointName.
   /// Return true if successful.
   /// 'onInitCallback' will be called when the layer's OnInit event occurs.  'userData'
   /// will be passed to the callback at that time to provide context.
   bool InsertBelow(const char* referencePointName, const char* name,
      LayerWidgetOnInitCallback onInitCallback = NULL, void* userData = NULL);
   /// Return true if a layer with the given name is defined.
   bool ExistsCheck(const char* layerName);

   /// Return a pointer to the given widget based on the given path.
   /// For example, if a widget named "Example" is in a container named
   /// "Foo" in a layer named "Bar", its path is "Bar.Foo.Example".
   /// If the widget is not found, return NULL.
   Widget* WidgetGetByPath(const char* path);

   /// Given a widget and an array of widget paths, return the index of the
   /// widget in the array of paths.  Return -1 if none of the specified paths
   /// correspond to the given widget.  Return -1 if 'widget' is NULL.
   int WidgetPathIndexGet(Widget* widget, const char** paths, int pathCount);

   /// If the given mouse is being used by the GUI, return the top-most
   /// enabled widget, if any, that is considered to be under its cursor as
   /// of the last Update().
   Widget* WidgetUnderMouseGet(Mouse* mouse);

   /// Refresh the text for all the widgets.  This is called automatically
   /// when the language changes.
   void TextRefresh();

   /// Preload the sprite data of the layer with the given name.
   void Preload(const char* name);
   /// Undo the preloading of the sprite data of the layer with the given name.
   /// This will not necessarily free anything, but it will decrement reference counts.
   void UndoPreload(const char* name);

   /// Add this widget to the list of widgets to be deinitialized/deleted at the end of the next update.
   void GarbageAdd(Widget* widget);

   /// Return the number of mice used by the gui system.
   inline int GUIMouseCountGet() { return mice.SizeGet(); }
   /// Return the entry for mouse with the given index.
   GUIMouse* GUIMouseGet(int index)
   {
      assert(index >= 0);
      assert(index < mice.SizeGet());
      return &(mice[index]);
   }

   /// Return true if scripting features are enabled.
   bool ScriptingEnabledCheck() { return scriptingEnabled; }

   /// Set whether the GUI should use theMice instead of theMouse.
   void MultipleMiceSet(bool _useMultipleMice);
   /// Return true if the GUI should use theMice instead of theMouse.
   bool MultipleMiceCheck() { return useMultipleMice; }

   /// Tell all widgets to begin the given transition.
   void TransitionBegin(const char* name);
   /// Return true if any of the widgets are currently in a transition.
   bool TransitioningCheck();
   /// Finish any pending transitions.
   void TransitionsFinish();

   /// Return true if the widgets are currently allowed to accept input.
   bool AcceptingInputCheck();

   /// Set the default sound to be played for widget OnMouseOver events.
   /// Set to NULL to use no sound by default.
   void MouseOverSoundSet(Sound* _mouseOverSound);
   void MouseOverSoundSet(const char* filename);
   /// Set the default sound to be played for widget OnPress events while visible and enabled.
   /// Set to NULL to use no sound by default.
   void PressSoundSet(Sound* _pressSound);
   void PressSoundSet(const char* filename);
   /// Set the default sound to be played for widget OnPress events while visible and disabled.
   /// Set to NULL to use no sound by default.
   void InvalidPressSoundSet(Sound* _invalidPressSound);
   void InvalidPressSoundSet(const char* filename);
   /// Set the default sound to be played for widget OnClick events.
   /// Set to NULL to use no sound by default.
   void ClickSoundSet(Sound* _clickSound);
   void ClickSoundSet(const char* filename);

   /// Associate the given WidgetTransition factory with the given name.
   void TransitionFactoryRegister(const char* name, TransitionFactory transitionFactory);
   /// Return the WidgetTransition factory with the given name.
   TransitionFactory TransitionFactoryGet(const char* name);

   /// Associate the given WidgetModifier factory with the given name.
   void ModifierFactoryRegister(const char* name, ModifierFactory modifierFactory);
   /// Return the WidgetModifier factory with the given name.
   ModifierFactory ModifierFactoryGet(const char* name);

   /// Associate the given WidgetLayout factory with the given type name.
   void LayoutFactoryRegister(const char* name, LayoutFactory layoutFactory);
   /// Return the WidgetLayout factory with the given type name.
   LayoutFactory LayoutFactoryGet(const char* name);

   /// Associate the given Widget factory with the given type name.
   void WidgetFactoryRegister(const char* name, WidgetFactory widgetFactory);
   /// Return the Widget factory with the given type name.
   WidgetFactory WidgetFactoryGet(const char* name);

   /// Set the starting clip area to be applied to widgets.
   void ClipAreaBaseSet(const Box2F& _clipAreaBase);
   /// Restore the base clip area to the default behavior.
   void ClipAreaBaseRestoreDefault();
   /// Return the starting clip area to be applied to the widgets.
   Box2F ClipAreaBaseGet() { return clipAreaBase; }

   /// Deinit all widgets in the garbage.  Delete them if appropriate.
   /// It is important to do this at a point when the call stack does not contain Widget methods.
   /// We don't want to delete an object to which we must return.
   void GarbageDeinit();

   /// Add the given GUIKeypad object to the manager.  Objects added this way
   /// will automatically be deinited and deleted by the manager.
   void GUIKeypadAdd(GUIKeypad* guiKeypad);
   /// Return the number of GUIKeypad objects.
   int GUIKeypadCountGet() { return guiKeypads.SizeGet(); }
   /// Return the GUIKeypad at the given index.
   GUIKeypad* GUIKeypadGet(int index);

   /// Return which widget, if any, has the given input focus.
   Widget* FocusWidgetGet(int focusIndex = 0);
   /// Set which widget has the given input focus.  This implicitly enables and
   /// disables the focus depending on whether 'widget' is NULL.
   void FocusWidgetSet(Widget* widget, int focusIndex = 0);
   /// Set which widget has the given input focus.
   void FocusWidgetSetByPath(const char* path, int focusIndex = 0) { FocusWidgetSet(WidgetGetByPath(path), focusIndex); }
   /// Push the stacks of which widgets have each focus.
   void FociWidgetsPush();
   /// Pop the stacks of which widgets have each focus.
   void FociWidgetsPop();
   /// Clear the stacks of which widgets have each focus.
   void FociWidgetStacksClear();
   /// Set all the input foci to highlighted or not.
   void FociHighlightedSet(bool _highlighted);
   /// Set whether keypad input should automatically enable and highlight any
   /// associated focus.
   void FocusEnableAndHighlightOnKeypadInputSet(bool _focusEnableAndHighlightOnKeypadInput) { focusEnableAndHighlightOnKeypadInput = _focusEnableAndHighlightOnKeypadInput; }
   /// Return true if keypad input should automatically enable and highlight any
   /// associated focus.
   bool FocusEnableAndHighlightOnKeypadInputCheck() { return focusEnableAndHighlightOnKeypadInput; }

   /// Set up the input focus links between the given group of widgets.
   /// 'paths' should list descending rows of paths of widgets.  'columns' and
   /// 'rows' should be the dimensions of that grid.  Use NULL for the path
   /// for empty entries in the grid.  Use 'wrapX' and 'wrapY' to control
   /// whether additional links are set to wrap the focus around horizontally
   /// and vertically.
   void FocusLinkGridSetupByPaths(const char** paths, int columns, int rows,
      bool wrapX = true, bool wrapY = true, HeapID heapTempID = HEAP_TEMP);
   /// Same as 'FocusLinkGridSetupByPaths', except it uses pointers to
   /// widgets rather than paths of widgets.
   void FocusLinkGridSetup(Widget** widgets, int columns, int rows,
      bool wrapX = true, bool wrapY = true);

   /// Return the number of objects for keeping track of which widgets have the
   /// input focus.
   int FocusCountGet() { return foci.SizeGet(); }
   /// Return the object for keeping track of the given focus.
   GUIFocus* FocusGet(int focusIndex);

   /// Increment the global count for the number of active GUI transitions.
   void TransitionActiveCountIncrement() { transitionActiveCount++; }
   /// Decrement the global count for the number of active GUI transitions.
   void TransitionActiveCountDecrement() { transitionActiveCount--; }

   /// Set the maximum number of milliseconds between clicks for it to count as
   /// a double-click.
   void DoubleClickIntervalDurationSet(unsigned int _doubleClickIntervalDuration) { doubleClickIntervalDuration = _doubleClickIntervalDuration; }

   /// Use the given JSON data for widget templates.
   void WidgetTemplatesSet(JSONValue* _widgetTemplates);
   /// Apply the widget templates to the given specifications.
   void WidgetTemplatesApply(JSONValue* specifications, HeapID heapID);

   /// Use the given JSON data for color templates.
   void ColorTemplatesSet(JSONValue* _colorTemplates);
   /// Return the color to use based on the given string representation.
   ColorRGBA8 ColorGet(const char* colorString, const ColorRGBA8& colorDefault);

   /// Return true if all bounds should be drawn.
   bool BoundsDrawAllCheck() { return boundsDrawAll; }
   /// Set whether all bounds should be drawn.
   void BoundsDrawAllSet(bool _boundsDrawAll) { boundsDrawAll = _boundsDrawAll; }

   /// Set the bounds of the cursor, relative to its mouse position, for
   /// purposes of placing tooltips.
   void TooltipCursorBoundsSet(const Box2F& _tooltipCursorBounds) { tooltipCursorBounds = _tooltipCursorBounds; }
   /// Return the bounds of the cursor, relative to its mouse position, for
   /// purposes of placing tooltips.
   Box2F TooltipCursorBoundsSet() { return tooltipCursorBounds; }
   /// Set the font to use when drawing tooltips.
   void TooltipFontSet(const char* _tooltipFontName);
   /// Return the font to use when drawing tooltips.
   Font* TooltipFontGet() { return tooltipFont; }
   /// Set the text scale to apply to tooltips.
   void TooltipTextScaleSet(const Point2F& _tooltipTextScale) { tooltipTextScale = _tooltipTextScale; }
   /// Return the text scale to apply to tooltips.
   Point2F TooltipTextScaleGet() { return tooltipTextScale; }
   /// Set the color to use for drawing the text of a tooltip.
   void TooltipTextColorSet(const ColorRGBA8& _tooltipTextColor) { tooltipTextColor = _tooltipTextColor; }
   /// Return the color to use for drawing the text of a tooltip.
   ColorRGBA8 TooltipTextColorGet() { return tooltipTextColor; }
   /// Set the color to use for the background of a tooltip.
   void TooltipBackgroundColorSet(const ColorRGBA8& _tooltipBackgroundColor) { tooltipBackgroundColor = _tooltipBackgroundColor; }
   /// Return the color to use for the background of a tooltip.
   ColorRGBA8 TooltipBackgroundColorGet() { return tooltipBackgroundColor; }
   /// Set the amount of padding on either side of the text of a tooltip.
   void TooltipTextPaddingSet(const Point2F& _tooltipTextPadding) { tooltipTextPadding = _tooltipTextPadding; }
   /// Return the amount of padding on either side of the text of a tooltip.
   Point2F TooltipTextPaddingGet() { return tooltipTextPadding; }
   /// Set the number of milliseconds for which a cursor must stay over a
   /// single widget before displaying its tooltip.
   void TooltipStartDelaySet(unsigned int _tooltipStartDelay) { tooltipStartDelay = _tooltipStartDelay; }
   /// Return the number of milliseconds for which a cursor must stay over a
   /// single widget before displaying its tooltip.
   unsigned int TooltipStartDelayGet() { return tooltipStartDelay; }

   /// Begin using the given widget for masking.
   void MaskSpriteWidgetPush(SpriteWidget* maskSpriteWidget);
   /// Pop the stack of masks.
   void MaskTexturePop();

   static GUIManager instance;

private:
   typedef Map<const char*, TransitionFactory> TransitionFactoryMap;
   typedef Map<const char*, ModifierFactory> ModifierFactoryMap;
   typedef Map<const char*, LayoutFactory> LayoutFactoryMap;
   typedef Map<const char*, WidgetFactory> WidgetFactoryMap;
   typedef Map<const char*, GUIManagerPreloadEntry> LayerPreloadMap;

   /// Load and return the layer with the given name.  Return false if the
   /// layer is not found.
   LayerWidget* LayerInit(const char* name);

   /// Return the top-most active widget that overlaps the given point.
   /// Return NULL if none overlap it.
   Widget* WidgetAtPointGet(const Point2F& point);

   /// Helper function for adjusting the focus due to a mouse press.
   void MousePressWidgetHelper(GUIMouse* guiMouse, Widget* widget);

   /// Increment the reference count for the LayerWidget with the given name.
   void LayerReferenceCountUp(const char* guiName);
   /// Decrement the reference count for the LayerWidget with the given name.
   /// Return true if it has reached zero.
   bool LayerReferenceCountDown(const char* guiName);

   /// Draw any tooltips.
   void TooltipsDraw();
   /// Refresh the tooltip for the given cursor.
   void TooltipUpdate(GUIMouse* guiMouse);

   /// Called after a state has been deinitialized.
   void GameStatePostDeinit();

   /// Called when the "OnWindowChange" event is triggered.
   void OnWindowChange();

#if defined _DEBUG && PLATFORM_IS_WINDOWS
   /// Called when the program gains the focus of the OS.
   void OnOSFocusGain();

   /// Return the time at which the widget specification file of the given
   /// layer was last modified.
   int64 LayerWidgetsFileModificationTimeGet(const char* name);

   /// True if the GUIManager should check on the next update for file
   /// modifications that could require automatic reloading.
   bool debugCheckFileModifications;
#endif //#if _DEBUG && PLATFORM_IS_WINDOWS

   /// True if the system is between Init and Deinit calls.
   bool initialized;
   /// true if the system is in the middle of deleting garbage.
   bool deletingGarbage;
   /// True if the ProgressionManager should be deinitialized when this is
   /// deinitialized.
   bool deinitProgressionManager;
   /// True if the ParticleManager2D should be deinitialized when this is
   /// deinitialized.
   bool deinitParticleManager2D;
   /// True if the ClipboardManager should be deinitialized when this is
   /// deinitialized.
   bool deinitClipboardManager;
   /// True if the GUI should use theMice instead of theMouse.
   bool useMultipleMice;
   /// True if scripting features should be enabled.
   bool scriptingEnabled;
   /// Clip area a widget should use when clipping is supposedly disabled.
   Box2F clipAreaBase;
   /// True if 'clipAreaBase' has been set explicitly.
   bool clipAreaBaseExplicit;
   /// Number of WidgetTransitions that are active.
   int transitionActiveCount;
   /// Maximum number of milliseconds between clicks for it to count as a
   /// double-click.
   unsigned int doubleClickIntervalDuration;
   /// True if all bounds should be drawn.
   bool boundsDrawAll;
   /// True if keypad input should automatically enable and highlight any
   /// associated focus.
   bool focusEnableAndHighlightOnKeypadInput;

   /// Temporary pointer to a widget.  This would ordinarily be a local
   /// variable, but it's a field to handle situations where a callback
   /// destroys a widget.
   Widget* currentWidgetUnderPoint;

   /// The root of the tree of widgets.
   ContainerWidget* rootWidget;
   
   /// Beginning of a singly-linked list of widgets to be deinitialized/deleted
   /// at the end of the next update.
   Widget* garbageHead;

   /// JSON specifications for color templates.
   JSONValue* colorTemplates;
   /// JSON specifications for widget templates.
   JSONValue* widgetTemplates;

   /// Bounds of the cursor, relative to its mouse position, for purposes of
   /// placing tooltips.
   Box2F tooltipCursorBounds;
   /// Text scale to apply to tooltips.
   Point2F tooltipTextScale;
   /// Font to use when drawing tooltips.
   Font* tooltipFont;
   /// Color to use for drawing the text of a tooltip.
   ColorRGBA8 tooltipTextColor;
   /// Color to use for the background of a tooltip.
   ColorRGBA8 tooltipBackgroundColor;
   /// Amount of padding on either side of the text of a tooltip.
   Point2F tooltipTextPadding;
   /// Number of milliseconds for which a cursor must stay over a single widget
   /// before displaying its tooltip.
   unsigned int tooltipStartDelay;

   /// Keep track of how many of which GUI layers are loaded.
   Map<StringHashPair, int> guiReferenceCounts;
   /// Collection of mice that are used for gui input purposes.
   Table<GUIMouse> mice;
   /// Map of names to functions that can create the associated objects.
   TransitionFactoryMap transitionFactories;
   /// Map of names to functions that can create the associated objects.
   ModifierFactoryMap modifierFactories;
   /// Map of names to functions that can create the associated objects.
   LayoutFactoryMap layoutFactories;
   /// Map of names to functions that can create the associated objects.
   WidgetFactoryMap widgetFactories;
   /// Map of layer names to preloaded data for that layer.
   LayerPreloadMap layerPreloadMap;
   /// Collection of GUIKeypad objects which can interact with the GUI.
   Table<GUIKeypad*> guiKeypads;
   /// Collection of objects to help keep track of which widgets should have
   /// the input focus of the keypad.
   Table<GUIFocus*> foci;
   /// Stack of mask textures used while drawing.
   Table<MaskTextureStackEntry> maskTextureStack;

   friend class LayerWidget;
};

//=============================================================================

GUIManager* const theGUI = &GUIManager::instance;

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__GUIMANAGER_H__

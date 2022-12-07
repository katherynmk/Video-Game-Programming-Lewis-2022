#ifndef __FROG__GUI__WIDGET_H__
#define __FROG__GUI__WIDGET_H__

#include "FrogMemory.h"
#include "Box2.h"
#include "Color.h"
#include "Sound.h"
#include "HoldTracker.h"
#include "Matrix32.h"
#include "GUI/GUIManager.h"

namespace Webfoot {

class LayerWidget;
class Font;
class WidgetTransition;
class WidgetModifier;
struct GUIMouse;
class JSONValue;
class GUIManager;
class SpriteWidget;

/// Name of the global lua table containing a tree of tables with
/// the event handlers for all the widgets.
#define WIDGET_ROOT_EVENT_HANDLER_TABLE "GUI"
#define WIDGET_ROOT_EVENT_HANDLER_TABLE_NAME_LENGTH 3

/// Key for the name of a widget.
#define WIDGET_NAME_KEY "Name"
/// Key for the position of the widget in screen coordinates.
#define WIDGET_POSITION_KEY "Position"
/// Key for the position of the widget relative to its parent.
#define WIDGET_POSITION_OFFSET_KEY "PositionOffset"
/// Local counter-clockwise rotation of a widget about its origin in degrees.
#define WIDGET_ROTATION_KEY "Rotation"
/// Local scale of the widget.
#define WIDGET_SCALE_KEY "Scale"
/// Color of the widget.
#define WIDGET_COLOR_KEY "Color"
/// Key for the name of the font to use for the widget's text.
#define GUI_WIDGET_FONT_KEY "Font"
/// Key for the click region of the widget
#define WIDGET_BOUNDS_KEY "Bounds"
/// Key for the list of modifier definitions.
#define WIDGET_MODIFIERS_KEY "Modifiers"
/// Key for the name of the modifier type.
#define WIDGET_MODIFIER_TYPE_KEY "Type"

//=============================================================================

/// Base class for a single widget.
/// Callbacks are provided for events that depend on other widgets, like OnPress.
/// Events that don't involve other widgets, like the mouse being moved,
/// do not have handlers.  Those cases should be handled in the inheritor's
/// Update function.  Only ContainerWidgets should contain other Widgets.
/// Aside from the root Widget, all Widgets should be kept in ContainerWidgets.
/// A Widget must not be in more than one ContainerWidget.  Upon being removed
/// from its parent, a Widget is added to the GUIManager's garbage list.  It will
/// then be deinitialized at the next end of a GUIManager update and deleted if needed.
/// Call Init immediately after construction.
/// Calling Deinit should free everything used by the widget except the widget itself,
/// and it should only be called by the GUIManager.  Keeping cleanup out of the destructor
/// allows us to have static widgets.
class Widget
{
public:
   Widget();
   virtual ~Widget() { }

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   virtual void Init(const char* _name, Widget* _parent);

   /// Initialize a new widget based on the specifications in the given JSONValue.
   /// Give the widget the specified name.  Make it remember it has the given parent Widget.
   /// If no name is given, it will default to the widget type name.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);
   /// Call this immediately before calling Draw().
   virtual void PreDraw();
   /// Draw the widget.
   virtual void Draw();
   /// Call this immediately after calling Draw().
   virtual void PostDraw();

   /// Mark the object so it knows it is out of the tree of widgets and will soon be deinitialized.
   virtual void GarbageSet() { remove = true; parent = NULL; }
   /// Return true if the object specifically is scheduled for deletion.
   inline bool GarbageCheck() { return remove; }
   /// Return true if this object or one of its ancestors is marked for deletion.
   virtual bool GarbageRecursiveCheck();

   /// Return true if the widget should be displayed.
   virtual bool VisibleCheck() { return (visible && (!parent || (parent && parent->VisibleCheck()))); }
   /// Set whether the widget should be displayed.
   virtual void VisibleSet(bool _visible) { visible = _visible; hideOnTransitionFinish = false; }
   /// Convenience function for setting whether the widget at the given path
   /// is visible.
   static void VisibleSet(const char* path, bool visible);

   /// Return true if the widget can respond to user input.
   virtual bool EnabledCheck();
   /// Set whether the widget can respond to user input.
   virtual void EnabledSet(bool _enabled) { enabled = _enabled; }
   /// Convenience function for setting whether the widget at the given path
   /// is enabled.
   static void EnabledSet(const char* path, bool enabled);
   /// Check non-recursively whether this widget should be enabled.  It may
   /// still be disabled for most purposes due to an ancestor, even if this
   /// returns true.
   bool EnabledSpecificCheck() { return enabled; }

   /// Return the name of this widget.
   virtual const char* NameGet() { return name; }
   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return a pointer to this widget's parent
   virtual Widget* ParentGet() { return parent; }
   /// Set a pointer to this widget's parent
   virtual void ParentSet(Widget* _parent) { parent = _parent; }

   /// Refresh the text for the widget.  Call this after a language change.
   virtual void TextRefresh() {}

   // Return the first widget found with the given name.  This will first check
   // this widget, then its descendants, and then it will back up toward the
   // root, expanding the scope of the search to ancestors and other descendants
   // of ancestors.
   virtual Widget* WidgetGetByName(const char* widgetName, bool checkAncestors = true, Widget* childToSkip = NULL);

   /// Return the top-most widget in this object, including the object itself,
   /// that overlaps the given point.  Return NULL if none overlap it.  The point
   /// is in the local coordinates of the parent.
   virtual Widget* WidgetAtPointGet(const Point2F& point);

   /// Return false if this widget and its descendants should be ignored when
   /// testing whether a given point, like a mouse cursor, is over them.
   virtual bool PointTestEnabledCheck() { return (pointTestEnabled && (!parent || (parent && parent->PointTestEnabledCheck()))); }
   /// Set whether this widget and its descendants should be ignored when
   /// testing whether a given point, like a mouse cursor, is over them.
   virtual void PointTestEnabledSet(bool _pointTestEnabled) { pointTestEnabled = _pointTestEnabled; }

   /// Return true if this widget should perform per-pixel tests in
   /// WidgetAtPointGet().  See 'OpaqueCheck'.
   virtual bool PointTestPerPixelCheck() { return pointTestPerPixel; }
   /// Set whether this widget should perform per-pixel tests in
   /// WidgetAtPointGet().  See 'OpaqueCheck'.  In addition to the per-pixel
   /// point test, it will still need to pass the Bounds point test.
   virtual void PointTestPerPixelSet(bool _pointTestPerPixel) { pointTestPerPixel = _pointTestPerPixel; }

   /// Helper function used by WidgetAtPointGet for per-pixel tests.
   /// 'testPosition' should be given in the local coordinate space of this
   /// widget.  See 'PointTestPerPixelSet'.  For now, this does not include the
   /// effect of text or of vector drawing, like BoundsDraw or
   /// BoundsFilledDraw.  It does not necessarily work with all underlying
   /// bitmap formats.  On some platforms, like OpenGL, the underlying sprite
   /// animations may need to be kept in memory with 'KeepBitmapData'.  This
   /// does not account for clipping, but other functions that call this, like
   /// WidgetAtPointGet, may account for it.
   virtual bool OpaqueCheck(const Point2F& testPosition) { (void)testPosition; return false; }

   /// Return true if the given point, in local coordinates, is fully masked
   /// away.
   virtual bool PointMaskedCheck(const Point2F& point);

   /// Return a pointer to the child with the given index.
   /// Return NULL if the index is out of bounds.
   virtual Widget* ChildGet(int index) { (void)index; return NULL; }
   /// Return the number of immediate children.
   virtual int ChildCountGet() { return 0; }
   /// Remove the child with the given index from the container and add it to the garbage.
   /// Return true if successful.
   virtual bool ChildRemove(int index) { (void)index; return false; }

   /// Called when the left mouse button is pressed with the cursor over the widget.
   virtual void OnPress();
   /// Called when the left mouse button is released with the cursor over the widget.
   virtual void OnRelease() {}
   /// Called when the left mouse button is quickly pressed, released, and
   /// pressed again over the same widget.
   virtual void OnDoubleClick() {}
   /// Called when the left mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnClick();
   /// Called when the cursor is over the widget on the current update, but it was
   /// not on the previous update.
   virtual void OnMouseOver();
   /// Called when the cursor was over the widget on the previous update, but it is
   /// not on the current update.
   virtual void OnMouseOut() {}
   /// Called when the widget is dragged by the cursor.
   virtual void OnDrag();
   /// Called when a widget is no longer being dragged.
   virtual void OnDragEnd() {}
   /// Called when the left mouse button is held over the widget.
   /// While the button remains held over the widget, the first call
   /// will be made HoldStartDelay milliseconds after
   /// the press and every HoldIntervalDuration milliseconds after that.
   /// 'consecutiveCalls' is the number of updates in a row before this update
   /// where OnHold was called.
   virtual void OnHold(int consecutiveCalls) { (void)consecutiveCalls; }
   /// Called when the right mouse button is pressed with the cursor over the widget.
   virtual void OnRightPress() {}
   /// Called when the right mouse button is released with the cursor over the widget.
   virtual void OnRightRelease() {}
   /// Called when the right mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnRightClick() {}
   /// Called when a mouse wheel is scrolled over this widget, assuming the
   /// widget can handle mouse scrolling.
   virtual void OnMouseScroll(int scrollAmount) { (void)scrollAmount; }
   /// Called when mouse drag scrolling begins.  This is different from
   /// dragging the widget.
   virtual void OnMouseDragScrollBegin() {}
   /// Called when a mouse drags over the widget to make it scroll.  This is
   /// different from dragging the widget.
   virtual void OnMouseDragScroll(const Point2F& scrollAmount) { (void)scrollAmount; }
   /// Called when the list of mice for the GUIManager has changed.
   virtual void OnMiceReset();
   /// Called when the widget gains the given focus.
   /// 'keypadDirectionMovedFocus' should be true if the focus moved because a
   /// keypad indicated a direction.  It should be false if the focus was moved
   /// by something like GUIManager::FocusWidgetSet.
   virtual void OnFocusGain(int focusIndex, bool keypadDirectionMovedFocus);
   /// Called when the widget loses the given focus.
   /// 'keypadDirectionMovedFocus' should be true if the focus moved because a
   /// keypad indicated a direction.  It should be false if the focus was moved
   /// by something like GUIManager::FocusWidgetSet.
   virtual void OnFocusLose(int focusIndex, bool keypadDirectionMovedFocus) { (void)focusIndex; (void)keypadDirectionMovedFocus; }
   /// Called when an input focus begins to affirm this widget.
   virtual void OnKeypadPress();
   /// Called when there are no affirmative buttons pressed, just as of this
   /// update, for a given input focus.
   virtual void OnKeypadRelease() {}
   /// Called when an input focus finishes affirming this widget.
   virtual void OnKeypadClick();
   /// Called when the left mouse button is pressed with the cursor over the
   /// given descendant the widget.
   virtual void OnDescendantPress(Widget* widget);

   /// Called when this widget or an ancestor is about to be removed from
   /// the tree of widgets.
   virtual void PreRemove() {}
   /// Called when this widget or an ancestor has just been removed from
   /// the tree of widgets.
   virtual void PostRemove() {}

   /// Return the current position of the widget relative to the parent.
   /// By default, this includes the temporary effects of active transitions
   /// and modifiers.  All position accessor functions call this either
   /// directly or indirectly.
   virtual Point2F PositionRelativeGet(bool includeEffects = true);
   /// Return the current position of the widget relative to the parent in integers.
   Point2I PositionRelativeIntegerGet(bool includeEffects = true);

   /// Return the current position of the widget in screen coordinates.
   Point2F PositionAbsoluteGet();
   /// Return the current position of the widget in integers in screen coordinates.
   Point2I PositionAbsoluteIntegerGet();

   /// Set the current position of the widget relative to its parent.
   /// Beware that this does not compensate for the temporary effects of active transitions and modifiers.
   /// All overloaded versions of PositionRelativeSet and PositionAbsoluteSet call this function
   /// either directly or indirectly.
   virtual void PositionRelativeSet(const Point2F& _position) { position = _position; }
   /// Set the current position of the widget relative to its parent.
   void PositionRelativeSet(float x, float y) { PositionRelativeSet(Point2F::Create(x, y)); }
   /// Set the current position of the widget relative to its parent.
   void PositionRelativeSet(int x, int y) { PositionRelativeSet(Point2F::Create((float)x, (float)y)); }
   /// Set the current position of the widget relative to its parent.
   void PositionRelativeSet(const Point2I& _position)
   {
      PositionRelativeSet(Point2F::Create(_position));
   }

   /// Set the current position of the widget in screen coordinates.
   /// All overloaded versions of PositionAbsoluteSet call this function.
   virtual void PositionAbsoluteSet(Point2F _position);
   /// Set the current position of the widget in screen coordinates.
   void PositionAbsoluteSet(float x, float y) { PositionAbsoluteSet(Point2F::Create(x, y)); }
   /// Set the current position of the widget in screen coordinates.
   void PositionAbsoluteSet(int x, int y) { PositionAbsoluteSet(Point2F::Create((float)x, (float)y)); }
   /// Set the current position of the widget in screen coordinates.
   void PositionAbsoluteSet(Point2I _position)
   {
      PositionAbsoluteSet(Point2F::Create((float)_position.x, (float)_position.y));
   }

   /// Set the area in which this widget can be clicked relative to the widget's origin.
   virtual void BoundsSet(Box2F _bounds) { bounds = _bounds; }
   /// Set the area in which this widget can be clicked relative to the widget's origin.
   virtual void BoundsSet(Box2I _bounds) { BoundsSet(Box2F::Create(_bounds)); }
   /// Return the area in which this widget can be clicked relative to the widget's origin.
   virtual Box2F BoundsGet() { return bounds; }

   /// Return the depth of this widget's position, relative to its parent.
   virtual float DepthRelativeGet() { return depth; }
   /// Return the absolute depth of this widget's position.
   virtual float DepthAbsoluteGet();
   /// Set the depth of this widget's position, relative to its parent.
   virtual void DepthRelativeSet(float _depth) { depth = _depth; }
   /// Set the absolute depth of this widget's position.
   virtual void DepthAbsoluteSet(float _depth);

   /// Set whether this widget can be moved by dragging.  By default, this is not enabled.
   virtual void CanDragSet(bool _canDrag) { canDrag = _canDrag; }
   /// Return true if this widget can be moved by dragging it.
   virtual bool CanDragCheck() { return canDrag; }

   /// Restrict dragging so that the current drag buffer area stays inside the given area
   /// relative to the parent.
   void DragAreaSet(Box2F _dragArea) { restrictDrag = true; dragArea = _dragArea; dragAreaUseParentBounds = false; }
   /// Use the parent's bounds as the drag area.
   void DragAreaSetToParentBounds() { restrictDrag = true; dragAreaUseParentBounds = true; }
   /// Remove restrictions on where the widget can be dragged.
	void DragAreaClear() { restrictDrag = false; }
   /// See 'DragAreaSet'.  Remember, this is relative to the parent's position,
   /// not this widget's position.  If there is no drag area, return
   /// {0, 0, 0, 0}.
   virtual Box2F DragAreaGet();
	
   /// Keep the given area, relative to the widget's origin, within the drag area while dragging is active.
   void DragBufferAreaSet(Box2F _dragBufferArea)
   {
      dragBufferAreaCopy = _dragBufferArea;
      dragBufferArea = &dragBufferAreaCopy;
   }
   /// Keep the bounds of the widget within the drag area while dragging is active.
   /// As the bounds change, the drag buffer area will automatically follow those changes.
   /// This is the default behavior when dragging widgets.
   void DragBufferAreaSetToBounds() { dragBufferArea = &bounds; }
   /// Keep the origin of the widget within the drag area while dragging is active.
   void DragBufferAreaSetToOrigin()
   {
      DragBufferAreaSet(Box2F::Create(0.0f, 0.0f, 0.0f, 0.0f));
   }
   /// See 'DragBufferAreaSet'.
   virtual Box2F DragBufferAreaGet() { return *dragBufferArea; }

   /// Return true if this widget is currently being dragged.
   bool DraggingCheck() { return dragging; }

   /// Set the area to which the widget and its children will be clipped for drawing and clicking.
   /// Give the area relative to the parent's position.
   /// This does not take into account the clip areas of the ancestors of this widget.
   virtual void ClipAreaSet(Box2F _clipArea);
   /// Return the area to which the widget and its children will be clipped for drawing and clicking.
   /// The area will be relative to the parent's position.
   /// This does not take into account the clip areas of the ancestors of this widget.
   /// If not currently using a clip area, return the default.
   virtual Box2F ClipAreaGet();
   /// Return the area to which the widget and its children will be clipped for drawing and clicking.
   /// The area will be in this widget's local coordinates.
   /// This takes into account the clip areas of the ancestors of this widget.
   /// This also compensates for scaling, but not for rotation.
   /// If 'ignoreFurtherClipping' is true, the clip areas of this widget and its
   /// ancestors will not practically further restrict the clipping.
   virtual Box2F ClipAreaEffectiveGet(bool ignoreFurtherClipping = false);
   /// Stop using a clip area for this widget.  Defer to the parent for ClipAreaEffectiveGet(), or
   /// if there is no parent, use the default clip area.
   virtual void ClipAreaClear() { useClipArea = false; }
   /// Return true if this widget currently uses its own clip area.
   virtual bool ClipAreaUseCheck() { return useClipArea; }
   /// True if the clip areas of ancestors should not clip this widget or its descendants.
   virtual bool AncestorClippingIgnoreCheck() { return ancestorClippingIgnore; }
   /// Set to true if the clip areas of ancestors should not clip this widget or its descendants.
   virtual void AncestorClippingIgnoreSet(bool _ancestorClippingIgnore) { ancestorClippingIgnore = _ancestorClippingIgnore; }
   /// Return true if widget clipping should be applied when drawing this
   /// widget and its descendants.
   virtual bool ClippingAffectsDrawingCheck();
   /// Set whether widget clipping should be applied when drawing this widget
   /// and its descendants.
   virtual void ClippingAffectsDrawingSet(bool _clippingAffectsDrawing) { clippingAffectsDrawing = _clippingAffectsDrawing; useClippingAffectsDrawing = true; }
   /// Defer to the parent's setting for whether clipping should affect drawing.
   virtual void ClippingAffectsDrawingClear() { useClippingAffectsDrawing = false; }

   /// Return the number of milliseconds after OnPress before OnHold is called.
   virtual unsigned int HoldStartDelayGet() { return pressHoldTracker.StartDelayGet(); }
   /// Set the number of milliseconds after OnPress before OnHold is called.
   virtual void HoldStartDelaySet(unsigned int _holdStartDelay) { pressHoldTracker.StartDelaySet(_holdStartDelay); }
   /// Return the number of milliseconds between calls to OnHold.
   virtual unsigned int HoldIntervalDurationGet() { return pressHoldTracker.IntervalDurationGet(); }
   /// Set the number of milliseconds between calls to OnHold.
   virtual void HoldIntervalDurationSet(unsigned int _holdIntervalDuration) { pressHoldTracker.IntervalDurationSet(_holdIntervalDuration); }

   /// Return the filename of the sprite resource file to use for this widget.
   /// Return NULL if this widget currently has no sprite resource file.
   virtual const char* SpriteResourceFilenameGet();
   /// Set the filename of the sprite resource file to use for this widget and
   /// its descendants.  Return NULL if this widget currently has no sprite
   /// resource file.
   virtual void SpriteResourceFilenameSet(const char* _spriteResourceFilename);

   /// Set the font to be used when drawing text.  Set to NULL to
   /// make the widget use its parent's font.
   virtual void FontSet(Font* _font);
   /// Return the font to use for this widget.  If a font is not defined for
   /// the widget, this will defer to the widget's ancestors.  If there is no
   /// font defined and no parent, NULL will be returned.
   virtual Font* FontGet();

   /// Set the sound to be played for OnMouseOver events.  Set to NULL to
   /// make the widget use its parent's mouseOverSound.
   virtual void MouseOverSoundSet(Sound* _mouseOverSound);
   virtual void MouseOverSoundSet(const char* filename);
   /// Return the sound to play for this widget's OnMouseOver event.  If a sound is
   /// not defined for the widget, this will defer to the widget's ancestors.
   /// If there is no sound defined and no parent, NULL will be returned.
   virtual Sound* MouseOverSoundGet();
   /// Return true if this Widget instance should automatically play mouseOver
   /// sounds.
   virtual bool MouseOverSoundEnabledCheck() { return mouseOverSoundEnabled; }
   /// Set whether this Widget instance should automatically play mouseOver
   /// sounds.
   virtual void MouseOverSoundEnabledSet(bool enabled) { mouseOverSoundEnabled = enabled; }

   /// Set the sound to be played for OnPress events while visible and enabled.
   /// Set to NULL to make the widget use its parent's pressSound.
   virtual void PressSoundSet(Sound* _pressSound);
   virtual void PressSoundSet(const char* filename);
   /// Return the sound to play for this widget's OnPress event while visible and enabled.
   /// If a sound is not defined for the widget, this will defer to the widget's ancestors.
   /// If there is no sound defined and no parent, NULL will be returned.
   virtual Sound* PressSoundGet();
   /// Return true if this Widget instance should automatically play press
   /// sounds.
   virtual bool PressSoundEnabledCheck() { return pressSoundEnabled; }
   /// Set whether this Widget instance should automatically play press
   /// sounds.
   virtual void PressSoundEnabledSet(bool enabled) { pressSoundEnabled = enabled; }

   /// Set the sound to be played for OnPress events while visible and disabled.
   /// Set to NULL to make the widget use its parent's invalidPressSound.
   virtual void InvalidPressSoundSet(Sound* _invalidPressSound);
   virtual void InvalidPressSoundSet(const char* filename);
   /// Return the sound to play for this widget's OnPress event while visble and disabled.
   /// If a sound is not defined for the widget, this will defer to the widget's ancestors.
   /// If there is no sound defined and no parent, NULL will be returned.
   virtual Sound* InvalidPressSoundGet();
   /// Return true if this Widget instance should automatically play invalidPress
   /// sounds.
   virtual bool InvalidPressSoundEnabledCheck() { return invalidPressSoundEnabled; }
   /// Set whether this Widget instance should automatically play invalidPress
   /// sounds.
   virtual void InvalidPressSoundEnabledSet(bool enabled) { invalidPressSoundEnabled = enabled; }

   /// Set the sound to be played for OnClick events.  Set to NULL to
   /// make the widget use its parent's pressSound.
   virtual void ClickSoundSet(Sound* _clickSound);
   virtual void ClickSoundSet(const char* filename);
   /// Return the sound to play for this widget's OnClick event.  If a sound is
   /// not defined for the widget, this will defer to the widget's ancestors.
   /// If there is no sound defined and no parent, NULL will be returned.
   virtual Sound* ClickSoundGet();
   /// Return true if this Widget instance should automatically play click
   /// sounds.
   virtual bool ClickSoundEnabledCheck() { return clickSoundEnabled; }
   /// Set whether this Widget instance should automatically play click
   /// sounds.
   virtual void ClickSoundEnabledSet(bool enabled) { clickSoundEnabled = enabled; }

   /// Recursively create and return a string that gives the name
   /// of the Lua table for this widget.  Leave runningLength
   /// at 1 for the initial call.
   virtual char* EventHandlerTableBuild(size_t runningLength = 1);
   /// Return the string that gives the name of the Lua table for this widget.
   /// This uses a cached copy of the string returned by BuildEventHandlerPath.
   virtual const char* EventHandlerTableGet() { return eventHandlerTable; }

   /// Remember that this child or a descendant of this child is the most recent
   /// descendant to have called an event script.  This will recursively call
   /// the same function on this widget's parent.
   virtual void EventChildSet(Widget* _eventChild);
   /// Return a pointer to the child that is either the most recent descendant
   /// to have called an event script or an ancestor of the most recent descendant to have
   /// called an event script.  Return NULL if no descendant has called one yet or if no
   /// descendant has called one more recently than this widget.
   /// Basically, this allows for a traversal from the root node to the node that last
   /// called an event script.
   virtual Widget* EventChildGet() { return eventChild; }

   /// Call when this widget is the leading candidate to get the keyboard focus.
   /// This is typically the widget that was clicked or an ancestor thereof.
   /// Return the widget that should actually receive the focus.
   /// Return NULL if no widget should have the focus.
   virtual Widget* WidgetForFocusGet();

   /// Return true if this widget should accept input focus when pressed by a
   /// mouse.
   virtual bool FocusOnMousePressCheck() { return focusOnMousePress; }
   /// Set whether this widget should accept input focus when pressed by a
   /// mouse.
   virtual void FocusOnMousePressSet(bool _focusOnMousePress) { focusOnMousePress = _focusOnMousePress; }
   /// Return true if this widget should default to accepting input focus when
   /// pressed by a mouse.
   virtual bool FocusOnMousePressDefaultCheck() { return false; }

   /// Return true if this widget has any input focus.
   virtual bool HasAnyFocusCheck();

   /// Return true if this widget should respond to mouse wheel scrolling.
   virtual bool CanHandleMouseScroll() { return false; }
   /// Return true if this widget should respond to mouse drag-scrolling.
   virtual bool CanHandleMouseDragScroll() { return false; }

   /// If the given mouse is being used by the gui, return true if this is
   /// the top-most enabled widget, if any, that is considered to be under
   /// its cursor as of the last Update().
   virtual bool MouseOverCheck(Mouse* mouse) { return this == theGUI->WidgetUnderMouseGet(mouse); }

   /// If the given mouse is being used by the gui, return true if
   /// the top-most enabled widget, if any, that is considered to be under
   /// its cursor as of the last Update() is one of the descendants of
   /// this widget.
   virtual bool MouseOverDescendantCheck(Mouse* mouse) { (void)mouse; return false; }

   /// Return true if the given widget is a descendant of this one.
   virtual bool WidgetDescendantCheck(Widget* widget) { (void)widget; return false; }

   /// Return true if the widget should be deleted after its Deinit is called
   /// by the GUIManager.
   virtual bool DeleteAfterDeinitCheck() { return true; }

   /// Return a pointer to the most recent mouse to press the widget.
   /// This is set just before the OnPress method is called.
   GUIMouse* MousePressingMostRecentlyGet() { return mostRecentPressingMouse; }
   /// Set the most recent mouse to press the widget.
   /// This is called just before the OnPress method is called.
   void MousePressingMostRecentlySet(GUIMouse* _mostRecentPressingMouse) { mostRecentPressingMouse = _mostRecentPressingMouse; }

   /// Return a pointer to the most recent mouse to right-press the widget.
   /// This is set just before the OnRightPress method is called.
   GUIMouse* MouseRightPressingMostRecentlyGet() { return mostRecentRightPressingMouse; }
   /// Set the most recent mouse to right-press the widget.
   /// This is called just before the OnRightPress method is called.
   void MouseRightPressingMostRecentlySet(GUIMouse* _mostRecentRightPressingMouse) { mostRecentRightPressingMouse = _mostRecentRightPressingMouse; }

   /// Return a pointer to the most recent mouse to drag-scroll the widget.
   /// This is set before the OnMouseDragScrollBegin method is called.
   GUIMouse* MouseDragScrollingMostRecentlyGet() { return mostRecentDragScrollingMouse; }
   /// Set the most recent mouse to drag-scroll the widget.
   /// This is called before the OnMouseDragScrollBegin method is called.
   void MouseDragScrollingMostRecentlySet(GUIMouse* _mostRecentDragScrollingMouse) { mostRecentDragScrollingMouse = _mostRecentDragScrollingMouse; }

   /// Return true if the most recent OnPress was a double-click.
   /// This is set before the OnPress method is called.
   bool MostRecentPressDoubleClickCheck() { return mostRecentPressDoubleClick; }
   /// Set whether the most recent OnPress was a double-click.
   /// This is set before the OnPress method is called.
   void MostRecentPressDoubleClickSet(bool _mostRecentPressDoubleClick) { mostRecentPressDoubleClick = _mostRecentPressDoubleClick; }

   /// Add the given transition to this widget's collection.
   void TransitionAdd(const char* name, WidgetTransition* _transition);
   /// Return the transition, if any, with the given name.
   WidgetTransition* TransitionGet(const char* name);
   /// Begin the transition with the given name.
   virtual void TransitionBegin(const char* name);
   /// Return true if this widget or any of its descendants are in a transition
   /// that has not yet finished.
   virtual bool TransitioningCheck();
   /// Return true if this widget or any of its ancestors are in a transition
   /// that has not yet finished.
   bool AffectedByTransitionCheck();
   /// Finish any pending transitions of this and any descendants.
   virtual void TransitionsFinish();
   /// Hide the widget the next time a transition ends.  Setting the visibility
   /// of the widget cancels this.
   virtual void HideOnTransitionFinish() { hideOnTransitionFinish = true; }
   /// Return true if this widget is currently able to accept input.
   bool AcceptingInputCheck();

   /// Add the given modifier to this widget's collection.
   void ModifierAdd(WidgetModifier* modifier);
   /// Return the given modifier.
   WidgetModifier* ModifierGet(int modifierIndex);
   /// Return the given modifier.
   WidgetModifier* ModifierGet(const char* modifierName);
   /// Return the number of modifiers.
   int ModifierCountGet() { return modifiers.SizeGet(); }

   /// In debug builds, return true if this widget should draw its bounds.
   virtual bool BoundsDrawCheck() { return boundsDraw; }
   /// In debug builds, set whether this widget should draw its bounds.
   virtual void BoundsDrawSet(bool _boundsDraw) { boundsDraw = _boundsDraw; }

   /// Return the color to use when drawing the bounds of the Widget in debug builds.
   virtual ColorRGBA8 BoundsColorGet() { return boundsColor; }
   /// Set the color to use when drawing the bounds of the Widget in debug builds.
   virtual void BoundsColorSet(const ColorRGBA8& _boundsColor) { boundsColor = _boundsColor; }

   /// In debug builds, return true if this widget should draw its bounds filled-in.
   virtual bool BoundsFilledDrawCheck() { return boundsFilledDraw; }
   /// In debug builds, set whether this widget should draw its bounds filled-in.
   virtual void BoundsFilledDrawSet(bool _boundsFilledDraw) { boundsFilledDraw = _boundsFilledDraw; }

   /// Return the color to use when drawing the filled bounds of the Widget.
   virtual ColorRGBA8 BoundsFilledColorGet() { return boundsFilledColor; }
   /// Set the color to use when drawing the filled bounds of the Widget.
   virtual void BoundsFilledColorSet(const ColorRGBA8& _boundsFilledColor) { boundsFilledColor = _boundsFilledColor; }

   /// Return the local counter-clockwise rotation about the Widget's origin in degrees.
   /// By default, this includes the temporary effects of active transitions and modifiers.
   /// Beware that rotation is not compatible with all other behavior, and
   /// should be used only for certain effects.
   virtual float RotationGet(bool includeEffects = true);
   /// Set the local counter-clockwise rotation about the Widget's origin in degrees.
   /// Beware that this does not compensate for the temporary effects of active transitions and modifiers.
   /// Beware that rotation is not compatible with all other behavior, and
   /// should be used only for certain effects.
   virtual void RotationSet(float _rotation) { rotation = _rotation; }

   /// Return the local scaling performed relative the Widget's origin.
   /// By default, this includes the temporary effects of active transitions and modifiers.
   /// Beware that scale is not compatible with all other behavior, and
   /// should be used only for certain effects.
   virtual Point2F ScaleGet(bool includeEffects = true);
   /// Set the local rotation about the Widget's origin.
   /// Beware that this does not compensate for the temporary effects of active transitions and modifiers.
   /// Beware that scale is not compatible with all other behavior, and
   /// should be used only for certain effects.
   virtual void ScaleSet(Point2F _scale) { scale = _scale; }

   /// Return the transform of this widget relative to the origin.
   virtual Matrix32 TransformAbsoluteGet();

   /// Return the inverse of the transform of this widget relative to the origin.
   virtual Matrix32 TransformInverseAbsoluteGet();

   /// Return the color to apply to this widget.  The effect of this is not
   /// generally hierarchical, and this should be used only for certain effects.
   /// By default, this includes the temporary effects of active transitions
   /// and modifiers.
   virtual ColorRGBA8 ColorGet(bool includeEffects = true);
   /// Set the color to apply to this widget.  The effect of this is not
   /// generally hierarchical, and this should be used only for certain effects.
   virtual void ColorSet(const ColorRGBA8& _color) { color = _color; }

   /// Set the coefficient used to modify the influence of mouse scrolling.
   virtual void MouseScrollCoefficientSet(float _mouseScrollCoefficient) { mouseScrollCoefficient = _mouseScrollCoefficient; }
   /// Return the coefficient used to modify the influence of mouse scrolling.
   virtual float MouseScrollCoefficientGet() { return mouseScrollCoefficient; }

   /// Set whether mouse scrolling over this widget should be processed.
   virtual void MouseScrollEnabledSet(bool _mouseScrollEnabled) { mouseScrollEnabled = _mouseScrollEnabled; }
   /// Return true if mouse scrolling over this widget should be processed.
   virtual bool MouseScrollEnabledCheck() { return mouseScrollEnabled; }

   /// Set whether mouse scrolling over this widget should be deferred to its
   /// parent even if this widget could ordinarily process it.
   virtual void MouseScrollDeferredSet(bool _mouseScrollDeferred) { mouseScrollDeferred = _mouseScrollDeferred; }
   /// Return true if mouse scrolling over this widget should be deferred to
   /// its parent even if this widget could ordinarily process it.
   virtual bool MouseScrollDeferredCheck() { return mouseScrollDeferred; }

   /// Set whether mouse drag scrolling over this widget should be processed.
   virtual void MouseDragScrollEnabledSet(bool _mouseDragScrollEnabled) { mouseDragScrollEnabled = _mouseDragScrollEnabled; }
   /// Return true if mouse drag scrolling over this widget should be processed.
   virtual bool MouseDragScrollEnabledCheck() { return mouseDragScrollEnabled; }

   /// Set whether mouse drag scrolling over this widget should be deferred to
   /// its parent even if this widget could ordinarily process it.
   virtual void MouseDragScrollDeferredSet(bool _mouseDragScrollDeferred) { mouseDragScrollDeferred = _mouseDragScrollDeferred; }
   /// Return true if mouse drag scrolling over this widget should be deferred
   /// to its parent even if this widget could ordinarily process it.
   virtual bool MouseDragScrollDeferredCheck() { return mouseDragScrollDeferred; }
   
   /// Return true if the widget is currently pressed.
   virtual bool PressedCheck() { return isPressed; }

   /// Called with the base clip area for the widgets is changed.
   virtual void OnClipAreaBaseChange();

   /// Return the widget associated with the given direction for input focus
   /// purposes.
   virtual Widget* FocusLinkGet(Direction direction);
   /// Set which widget is associated with the given direction for input
   /// focus purposes.  If a path was set with 'FocusLinkPathSet',
   /// and it hasn't been cleared, that path will take effect again the next
   /// time the links are refreshed.  If you want to override a setting from
   /// 'FocusLinkPathSet', you may want to call
   /// 'FocusLinkPathSet' with NULL before calling this.
   virtual void FocusLinkSet(Direction direction, Widget* widget);
   /// Set to path to use when refreshing the given input focus link.  If
   /// 'path' is set to NULL, this does not mean that the link will necessarily
   /// be set to NULL.  It just means it won't explicitly be set to something
   /// specific.
   virtual void FocusLinkPathSet(Direction direction, const char* path);
   /// See 'FocusLinkPathSet'.
   virtual const char* FocusLinkPathGet(Direction direction);
   /// Recompute the focus links for this widget and its descendants.
   virtual void FocusLinksRefresh();

   /// Set the coefficient used to modify the influence of keypad scrolling.
   virtual void KeypadDirectionCoefficientSet(float _keypadDirectionCoefficient) { keypadDirectionCoefficient = _keypadDirectionCoefficient; }
   /// Return the coefficient used to modify the influence of keypad scrolling.
   virtual float KeypadDirectionCoefficientGet() { return keypadDirectionCoefficient; }

   /// Return true if this widget can receive input focus.
   virtual bool CanHaveFocusCheck() { return canHaveFocus; }
   /// Set whether this widget can receive input focus.
   virtual void CanHaveFocusSet(bool _canHaveFocus) { canHaveFocus = _canHaveFocus; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// Return the offset from the cursor to the origin of the widget when dragging.
   virtual Point2F DragOffsetGet() { return dragOffset; }

   /// Return an explicit string to be displayed for a tooltip.
   virtual const char* TooltipTextGet() { return tooltipText; }
   /// Set an explicit string to be displayed for a tooltip.
   virtual void TooltipTextSet(const char* _tooltipText);

   /// Return the key for theText for the string to be displayed for a tooltip.
   /// Return NULL if a text key is not in use.
   virtual const char* TooltipTextKeyGet() { return tooltipTextKey; }
   /// Set the key for theText for the string to be displayed for a tooltip.
   virtual void TooltipTextKeySet(const char* _tooltipTextKey);

   /// Return the name of the widget to use as a mask when drawing this widget
   /// and its descendants.
   virtual const char* MaskSpriteWidgetNameGet() { return maskSpriteWidgetName; }
   /// Set the name of the widget to use as a mask when drawing this widget and
   /// its descendants.
   virtual void MaskSpriteWidgetNameSet(const char* _maskSpriteWidgetName);

   /// Return true if this widget should be updated by parent container widgets.
   bool ShouldUpdateCheck() { return shouldUpdate; }
   /// Set whether this widget should be updated by parent container widgets.
   void ShouldUpdateSet(bool _shouldUpdate) { shouldUpdate = _shouldUpdate; }

protected:
   /// Help determine the transform of this widget relative to the origin.
   virtual void TransformAbsoluteGetHelper(Matrix32* transform);
   /// Help determine the inverse transform of this widget relative to the origin.
   virtual void TransformInverseAbsoluteGetHelper(Matrix32* transform);

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return true; }

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget type should play a mouseOver sound by default.
   virtual bool MouseOverSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play a press sound by default.
   virtual bool PressSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play an invalidPress sound by default.
   virtual bool InvalidPressSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play a click sound by default.
   virtual bool ClickSoundEnabledDefaultCheck() { return false; }

   /// Call this widget's event handling function with the given 'functionName'.
   /// Specify the printf-style pattern for the parameters.  Give the actual
   /// parameters to the event function as the variable set of parameters
   /// to this function.  All boolean parameters must be provided as strings.
   /// ScriptManager::BoolToString can help with that.  Return true if the 
   /// event function is defined.  This won't actually attempt the call if
   /// the event function is not defined.  The buffer is large, but finite.
   /// Keep the final function call string sizes under 2K.
   virtual bool EventScriptCallF(const char* functionName, const char* parameterPattern, ...);
   /// VEventScriptCallF is the 'va_list' equivalent of EventScriptCallF.
   virtual bool VEventScriptCallF(const char* functionName, const char* parameterPattern, va_list arguments);

   /// Print the path of this widget to debug output.  This is not necessarily
   /// the same as the path used for Lua callbacks and is intended for
   /// debugging purposes only.
   void DebugPrintPath();

private:
   typedef Map<const char*, WidgetTransition*> TransitionMap;

   /// Helper class for polygon-based point tests.
   struct PolygonEdge
   {
      PolygonEdge(const Point2F& _start, const Point2F& _end)
      {
         start = _start;
         end = _end;
      }

      /// Beginning of an edge.
      Point2F start;
      /// End of an edge.
      Point2F end;
   };

   /// Helper function for printing the path of this widget to debug output.
   virtual void DebugPrintPathHelper();

   /// True if this object can respond to user input.
   bool enabled;
   /// True if this object should be displayed.
   bool visible;
   /// True if this widget is currently being dragged.
   bool dragging;
   /// True if this widget can be dragged.
   bool canDrag;
   /// True if the widget's 'dragBufferArea' should not leave the area specified by 'dragArea'
   bool restrictDrag;
   /// True if this widget should be updated by parent container widgets.
   bool shouldUpdate;
   /// True if the widget should be removed on the next end of an update of the GUIManager.
   bool remove;
   /// Used by GUIManager's linked list of widgets to clean up.
   Widget* garbageNext;
   /// True if the parent's bounding area should be used as the dragArea.
   bool dragAreaUseParentBounds;
   /// True if the widget was pressed and the mouse that did it has not yet been released
   /// or disconnected.
   bool isPressed;
   /// True if this widget should receive input focus when pressed by a mouse.
   bool focusOnMousePress;
   /// True if the widget's clip area should be applied.  If false, any relevant queries
   /// will defer to the parent if present.  If false, and there is no parent, the
   /// default clip area will be returned.
   bool useClipArea;
   /// True if the clip areas of ancestors should not clip this widget or its
   /// descendants.
   bool ancestorClippingIgnore;
   /// True if widget clipping should be applied when drawing, assuming
   /// useClippingAffectsDrawing is true.
   bool clippingAffectsDrawing;
   /// True if clippingAffectsDrawing should be used rather than falling back
   /// on the parent.
   bool useClippingAffectsDrawing;
   /// Filename of the sprite resource file to use for this widget and its
   /// descendants.
   const char* spriteResourceFilename;
   /// True if 'font' should be unloaded when this object no longer needs it.
   bool fontUnload;
   /// Font with which to draw text.  If this is NULL, use the parent's font.
   Font* font;
   /// Sound to play when an OnMouseOver event occurs.  If this is NULL, use the parent's mouseOverSound.
   Sound* mouseOverSound;
   /// True if 'mouseOverSound' should be unloaded.
   bool mouseOverSoundUnload;
   /// Sound to play when an OnPress event occurs while visible and enabled.  If this is NULL, use the parent's pressSound.
   Sound* pressSound;
   /// True if 'pressSound' should be unloaded.
   bool pressSoundUnload;
   /// Sound to play when an OnPress event occurs while visible and disabled.  If this is NULL, use the parent's invalidPressSound.
   Sound* invalidPressSound;
   /// True if 'invalidPressSound' should be unloaded.
   bool invalidPressSoundUnload;
   /// Sound to play when an OnClick event occurs while visible and enabled.  If this is NULL, use the parent's clickSound.
   Sound* clickSound;
   /// True if 'clickSound' should be unloaded.
   bool clickSoundUnload;
   /// True if this Widget instance should automatically play mouseOver
   /// sounds.
   bool mouseOverSoundEnabled;
   /// True if this Widget instance should automatically play press
   /// sounds.
   bool pressSoundEnabled;
   /// True if this Widget instance should automatically play invalidPress
   /// sounds.
   bool invalidPressSoundEnabled;
   /// True if this Widget instance should automatically play click
   /// sounds.
   bool clickSoundEnabled;
   /// Controls when to call OnHold.
   HoldTracker pressHoldTracker;
   /// Name of the widget.
   char* name;
   /// String containing the path to the table for this widget's event handling functions
   char* eventHandlerTable;
   /// Pointer to this widget's parent
   Widget* parent;
   /// Pointer to the child of this widget where either this child or a descendant of this
   /// child is the most recent descendant to have called an event script.
   Widget* eventChild;
   /// Position of the widget relative to the parent.
   Point2F position;
   /// Depth of this widget's position, relative to its parent.
   float depth;
   /// Local rotation of the widget about its origin.
   float rotation;
   /// Local scale of the widget performed from its origin.
   Point2F scale;
   /// Color to apply to this widget.  The effect of this is not
   /// generally hierarchical.
   ColorRGBA8 color;
   /// Offset from the cursor to the origin of the widget when dragging.
   Point2F dragOffset;
   /// Area relative to the widget's position where the user can click
   Box2F bounds;
   /// Area of the screen in which the widget's dragBufferArea can be dragged.
   Box2F dragArea;
   /// Area relative to the widget's position that must stay within the dragArea.
   /// This defaults to the bounds.
   Box2F* dragBufferArea;
   /// Internal copy of a dragBufferArea when working with a specific value
   /// for the Box2 rather than a reference.
   Box2F dragBufferAreaCopy;
   /// Area to which the widget and its children will be clipped for drawing and clicking.
   Box2F clipArea;
   /// Pointer to the most recent mouse to press the widget.  This is set
   /// just before the OnPress method is called.
   GUIMouse* mostRecentPressingMouse;
   /// Pointer to the most recent mouse to right-press the widget.  This is set
   /// just before the OnRightPress method is called.
   GUIMouse* mostRecentRightPressingMouse;
   /// Pointer to the most recent mouse to drag-scroll the widget.
   /// This is set just before the OnMouseDragScroll method is called.
   GUIMouse* mostRecentDragScrollingMouse;
   /// True if the most recent OnClick was a double-click.
   bool mostRecentPressDoubleClick;
   /// The currently executing transition, if any.
   WidgetTransition* transition;
   /// True if there is a transition and it isn't finished yet.
   bool transitionNotFinished;
   /// True if the widget should be hidden the time a transition ends.
   bool hideOnTransitionFinish;
   /// False if this widget and its descendants should be ignored when
   /// testing whether a given point, like a mouse cursor, is over them.
   bool pointTestEnabled;
   /// True if this widget should perform per-pixel tests in
   /// WidgetAtPointGet().
   bool pointTestPerPixel;
   /// Map of names to WidgetTransitions.
   TransitionMap transitions;
   /// True if 'transitions' is initialized.
   bool transitionsInitialized;
   /// Collection of WidgetModifiers.
   Table<WidgetModifier*> modifiers;
   /// True if 'modifiers' is initialized.
   bool modifiersInitialized;
   /// Coefficient used to modify the influence of mouse scrolling.
   float mouseScrollCoefficient;
   /// True if mouse scrolling over this widget should be processed.
   bool mouseScrollEnabled;
   /// True if mouse scrolling over this widget should be deferred to its
   /// parent even if this widget could ordinarily process it.
   bool mouseScrollDeferred;
   /// True if mouse drag scrolling over this widget should be processed.
   bool mouseDragScrollEnabled;
   /// True if mouse drag scrolling over this widget should be deferred to its
   /// parent even if this widget could ordinarily process it.
   bool mouseDragScrollDeferred;
   /// Which widgets are associated with directions for input focus purposes.
   Widget* focusLinks[DIRECTION_CARDINAL_COUNT];
   /// Paths for explicit input focus links to other widgets.
   const char* focusLinkPaths[DIRECTION_CARDINAL_COUNT];
   /// Coefficient used to modify the influence of keypad scrolling.
   float keypadDirectionCoefficient;
   /// True if this widget can receive input focus.
   bool canHaveFocus;
   /// Collection of edges used for polygon point testing.
   Table<PolygonEdge> pointTestPolygonEdges;
   /// Text to actually display for a tooltip.
   const char* tooltipText;
   /// If defined, this will be passed to theText to get the string to display
   /// for a tooltip.
   const char* tooltipTextKey;
   /// True if the widget should draw its bounds.
   bool boundsDraw;
   /// Color used to draw the bounds.
   ColorRGBA8 boundsColor;
   /// True if the widget should draw its bounds filled.
   bool boundsFilledDraw;
   /// Color used to draw the filled bounds.
   ColorRGBA8 boundsFilledColor;
   /// True if the widget should draw its point test polygon.
   bool pointTestPolygonDraw;
   /// Color used to draw the point test polygon.
   ColorRGBA8 pointTestPolygonColor;
   /// Name of the widget to use as a mask when drawing this widget and its
   /// descendants.
   const char* maskSpriteWidgetName;
   /// Cached pointer to the widget to use as the texture mask.
   SpriteWidget* maskSpriteWidgetCached;
   /// True if 'maskSpriteWidgetCached' should be refreshed the next time it's
   ///needed.
   bool maskSpriteWidgetCachedDirty;

   friend class GUIManager;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __MODULES__GUI__WIDGET_H__

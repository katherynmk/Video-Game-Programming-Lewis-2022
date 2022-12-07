#ifndef __FROG__MOUSECOMMON_H__
#define __FROG__MOUSECOMMON_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Box2.h"
#include "Debug.h"

namespace Webfoot {

//==============================================================================

/// A simple interface for reacting to input from a mouse.
class MouseCommon
{
public:
   MouseCommon();

   /// Initialize the cursor to be at the given position.  Keep the cursor
   /// within the given region.
	void Init(const Point2I& _position, const Box2I& _region);

   /// Called at the beginning of a MouseManager update.
   void PreUpdate();
   /// Called at the end of a MouseManager update.
   /// Set 'hasFocus' to true if the application currently has keyboard focus.
   void PostUpdate(bool hasFocus);
	
   /// Return true if the left mouse button is currently pressed.
   bool LeftPressed() const;
   /// Return true if the left mouse button was just pressed within the last frame.
   bool LeftJustPressed() const;
   /// Return true if the left mouse button was just released within the last frame.
   bool LeftJustReleased() const;

   /// Return true if the right mouse button is currently pressed.
   bool RightPressed() const;
   /// Return true if the right mouse button was just pressed within the last frame.
   bool RightJustPressed() const;
   /// Return true if the right mouse button was just released within the last frame.
   bool RightJustReleased() const;

   /// Return true if the right mouse button is currently pressed.
   bool MiddlePressed() const;
   /// Return true if the right mouse button was just pressed within the last frame.
   bool MiddleJustPressed() const;
   /// Return true if the right mouse button was just released within the last frame.
   bool MiddleJustReleased() const;

   /// Return true if the mouse is connected.
   bool ConnectedCheck() const { return connected; }

   /// Return how much the scroll wheel moved during the last frame.
   int ScrollGet() const { return scroll; }
   
	/// Return true if the cursor moved during the last frame.  If the player moved their
   /// mouse, but due to clipping, the cursor didn't move, this will return false.
	bool CursorMoveCheck() const { return position != lastPosition; }

   /// Return true if the mouse moved during the last frame.  If the player moved their
   /// mouse, but due to clipping, the cursor didn't move, this will return true.
	bool MouseMoveCheck() const;

   /// Return a vector of how the cursor moved on the last frame after the
   /// clipping region was applied.
   Point2I CursorMoveGet() const { return position - lastPosition; }

   /// Return a vector of how the mouse moved on the last frame before the
   /// clipping region was applied.
   Point2I MouseMoveGet() const { return movementUnclipped; }

	/// Return the position of the cursor within the mouse region.
	const Point2I& PositionGet() const { return position; }
   /// Return the position of the cursor within the mouse region as a Point2F.
   const Point2F PositionFloatGet() const { return Point2F::Create((float)position.x, (float)position.y); }
	/// Return the x position of the cursor within the mouse region.
	int XGet() const { return position.x; }
	/// Return the y position of the cursor within the mouse region.
	int YGet() const { return position.y; }
   /// Set the position of the cursor.
   void PositionSet(Point2I _position) { position = _position; }
   /// Set the position of the cursor.
   void PositionSet(Point2F _position) { position = Point2I::Create((int)_position.x, (int)_position.y); }
   /// Set the position of the cursor.
   void PositionSet(int x, int y) { position = Point2I::Create(x, y); }
   /// Get the button flags of the cursor.
   int ButtonsGet(void) { return (buttonsCurrent); }
   /// Set the button flags of the cursor.
   void ButtonsSet(int buttonFlags) { buttonsNext = buttonFlags; }
   
   /// Bit flags for the mouse buttons
   enum Button
   {
      LEFT = 1,
      RIGHT = 2,
      MIDDLE = 4
   };

   /// Return a platform-independent string representation of the given
   /// button code.  Return NULL if unsuccessful.
   static const char* ButtonCodeToString(int buttonCode);
   /// Return the key code that correspends to the given platform-independent
   /// string.  Return -1 if unsuccessful.
   static int ButtonStringToCode(const char* buttonString);

   /// Set whether input should be treated as touch input or non-touch input
   /// regardless of what it really is.
   void SimulatedTouchscreenSet(bool _simulatedTouchscreen) { useSimulatedTouchscreen = true; simulatedTouchscreen = _simulatedTouchscreen; }
   /// Clear the effect of calls to 'SimulatedTouchscreenSet'
   void SimulatedTouchscreenClear() { useSimulatedTouchscreen = false; }

   /// Return true if the most recent input for this mouse was touch input.
   bool LastInputTouchCheck() { return useSimulatedTouchscreen ? simulatedTouchscreen : lastInputTouch; }

   /// Return the region of the screen in which the cursor must stay
   Box2I RegionGet() { return region; }

   /// Set whether the cursor position should wrap around rather than being
   /// clamped.
   void PositionWrapSet(bool _positionWrap) { positionWrap = _positionWrap; }
   /// Return true if the cursor position should wrap around rather than being
   /// clamped.
   bool PositionWrapCheck() { return positionWrap; }

protected:   
   /// States of the buttons for the next update with focus
   int buttonsNext;
   /// States of the buttons on the must recent update with focus
   int buttonsCurrent;
   /// Status of the buttons from the update with focus before buttonsCurrent.
   int buttonsLast;
   /// The amount the scroll wheel moved on the last frame
   int scroll;
   /// True if this mouse should be treated as connected.
   bool connected;
   /// When 'useSimulatedTouchscreen' is true, this determines whether this
   /// mouse's input should be treated as touch input, regardless of whether it
   /// really is.
   bool simulatedTouchscreen;
   /// See 'simulatedTouchscreen'.
   bool useSimulatedTouchscreen;
   /// True if the most recent input for this mouse was touch input.
   bool lastInputTouch;
   /// The position of the cursor as of the last update
   Point2I position;
   /// The position of the cursor on the update before last
   Point2I lastPosition;
   /// Change in position before the mouse region was applied.
   Point2I movementUnclipped;
   /// The region of the screen in which the cursor must stay
   Box2I region;
   /// True if the cursor position should wrap around rather than being
   /// clamped.
   bool positionWrap;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__MOUSECOMMON_H__

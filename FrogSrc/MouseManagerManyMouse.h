#ifndef __FROG__MOUSEMANAGERMANYMOUSE_H__
#define __FROG__MOUSEMANAGERMANYMOUSE_H__

#include "UtilityC.h"

EXTERN_C_BEGIN
   void FrogMouseDown (unsigned short buttonFlag);
   void FrogMouseUp   (unsigned short buttonFlag);
   void FrogMouseMove (int x, int y);
EXTERN_C_END

#ifdef __cplusplus

#include "FrogMemory.h"
#include "MouseCommon.h"

namespace Webfoot {

class MouseManagerManyMouse;

//==============================================================================

/// A mouse class that gives MouseManagerManyMouse full access.
class Mouse : public MouseCommon
{
public:
   void Init(const Point2I& _position, const Box2I& _region);
   
   void PreUpdate();
   void PostUpdate(bool hasFocus);

   typedef MouseCommon Inherited;
   friend class MouseManagerManyMouse;

protected:
   /// Form of movementUnclipped that allows mouse sensitivity to be applied
   /// before casting to integers.
   Point2F movementUnclippedFloat;
};

//==============================================================================

/// Handles a set of mice using the ManyMouse library
class MouseManagerManyMouse
{
public:
   /// Initialize the manager, the mice, and restrict the mice to the given region.
   /// If no region is specified, the bounds of the screen will be used instead.
   void Init(const Box2I* _region = NULL);
   void Deinit();
   
   /// Update the associated mouse objects.
   /// Set 'hasFocus' to false if the game's window does not currently have the focus.
   /// Certain messages will be ignored accordingly.  This should be called even
   /// when the window does not have focus so that certain messages are still handled.
   void Update(bool hasFocus);

   /// Let the manager know that a device has been connected or disconnected.
   void DeviceChangeNotify() { deviceChanged = true; }

   enum
   {
      /// Maximium number of separate mice to track.
      MOUSE_COUNT_MAX = 8
   };
   
   /// Return the maximium number of separate mice to track.
   int MouseCountMaxGet() { return MOUSE_COUNT_MAX; }

   /// True if 'hasFocus' was true in the last call to Update.
   bool lastFocus;
   /// True if the manager has been notified of a device change, but has not yet handled it.
   bool deviceChanged;
   /// Singleton instance
   static MouseManagerManyMouse instance;
   /// A mouse that treats all the mice as one.
   static Mouse mouse;
   /// Separate mouse objects for the individual physical mice.
   static Mouse mice[MOUSE_COUNT_MAX];
   /// Pointers to the individual elements of 'mice' so that 'theMice' can be an array of pointers.
   static Mouse* micePointers[MOUSE_COUNT_MAX];
};

/// Pointer the the singleton instance.
MouseManagerManyMouse* const theMouseManager = &MouseManagerManyMouse::instance;
/// A mouse object that takes input from all connected physical mice.
Mouse* const theMouse = &MouseManagerManyMouse::mouse;
/// Separate mouse objects for the individual physical mice.
Mouse** const theMice = (Mouse**)(MouseManagerManyMouse::micePointers);

//==============================================================================

} //namespace Webfoot {

#else

   /// Bit flags for the mouse buttons in the C/Objective-C side
   enum FrogMouseButton
   {
      LEFT = 1,
      RIGHT = 2,
      MIDDLE = 4
   };

#endif //#ifdef __cplusplus

#endif //#ifndef __FROG__MOUSEMANAGERMANYMOUSE_H__

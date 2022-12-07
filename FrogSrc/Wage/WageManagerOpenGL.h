#ifndef __WAGEMANAGEROPENGL_H__
#define __WAGEMANAGEROPENGL_H__

#include "Frog.h"
#include "WageManagerCommon.h"

namespace Webfoot {

//=============================================================================

class WageManagerOpenGL : public WageManagerCommon
{
public:
   typedef WageManagerCommon Inherited;

   enum Direction
   {
      DIRECTION_UP,
      DIRECTION_DOWN,
      DIRECTION_LEFT,
      DIRECTION_RIGHT,
      DIRECTION_COUNT
   };

   WageManagerOpenGL();

   void Init();
   void Deinit();
   void Update(unsigned int dt);
   void UpdateTimeStep(unsigned int dt);
   void Draw();

   /// Mouse sensitivity set
   void MouseSensitivitySet(float _mouseSensitivity) { mouseSensitivity = _mouseSensitivity; }
   /// Mouse vector length minimum set
   void MouseVectorLengthMinSet(float _mouseVectorLengthMin) { mouseVectorLengthMin = _mouseVectorLengthMin; }
   /// Mouse vector length maximum set
   void MouseVectorLengthMaxSet(float _mouseVectorLengthMax) { mouseVectorLengthMax = _mouseVectorLengthMax; }
   
   /// Key press handling (up/down/left/right, action, and menu buttons)
   virtual bool KeyUpJustPressed();
   virtual bool KeyUpPressed();
   virtual bool KeyDownJustPressed();
   virtual bool KeyDownPressed();
   virtual bool KeyLeftJustPressed();
   virtual bool KeyLeftPressed();
   virtual bool KeyRightJustPressed();
   virtual bool KeyRightPressed();
   virtual bool KeyActionJustPressed();
   virtual bool KeyActionPressed();
   virtual bool KeySecondaryActionJustPressed();
   virtual bool KeySecondaryActionPressed();
   virtual bool KeyMenuJustPressed();
   virtual bool KeyMenuPressed();

   /// Assign a JoystickWidget to control the direction keys.
   virtual void JoystickWidgetSet(JoystickWidget* _joystickWidget) { joystickWidget = _joystickWidget; }
   /// Assign a PressButtonWidget to control the action key.
   virtual void KeyActionWidgetSet(PressButtonWidget* _keyActionWidget) { keyActionWidget = _keyActionWidget; }
   /// Assign a PressButtonWidget to control the secondary action key.
   virtual void KeySecondaryActionWidgetSet(PressButtonWidget* _keySecondaryActionWidget) { keySecondaryActionWidget = _keySecondaryActionWidget; }
   /// Use the given gamepad for input.
   virtual void GamepadTypicalSet(GamepadTypical* _gamepadTypical) { gamepadTypical = _gamepadTypical; }

   /// Level editor functions
   /// Draw x-z grid 
   void DrawWorldGrid();
   /// Draw currently targeted cube bounds
   void DrawCubeBounds();
   /// Draw the target
   void DrawTarget();

   /// Debug functions
   void DrawCubeOutline(const Box3F& box, ColorRGBA8 color = COLOR_RGBA8_WHITE);
   /// Set whether outlines should be drawn around the cubes.
   void DrawCubeOutlinesSet(bool _drawCubeOutlines) { drawCubeOutlines = _drawCubeOutlines; }
   /// Return ture if outlines should be drawn around the cubes.
   bool DrawCubeOutlinesCheck() { return drawCubeOutlines; }

   /// Sets up the current texture with default parameters
   void TextureParametersApply(Texture* texture);

   /// Set the region of the screen that Wage should use for drawing.
   /// This is intended as a debug feature for 3DSPC builds, and it is not
   /// available on all platforms.
   void DrawRegionSet(const Box2F& _drawRegion) { drawRegion = _drawRegion; }
   /// Set the maximum number of pixels by which a fragment may be
   /// translated horizontally for stereoscopy.
   /// This is intended as a debug feature for 3DSPC builds, and it is not
   /// available on all platforms.
   void StereoParallaxMaxSet(float _stereoParallaxMax) { stereoParallaxMax = _stereoParallaxMax; }

   static WageManagerOpenGL instance;

protected:
   /// Debug
   bool drawCollisionRectangles;
   bool drawCubeOutlines;
   /// Timer for mouse movement in the y direction
   unsigned int mouseMoveTimerY;
   /// Mouse movement in the x direction
   unsigned int mouseMoveTimerX;
   /// The current state of mouse movement
   bool mouseIsMoving[DIRECTION_COUNT];
   /// Mouse movement vector accumulator
   Point2F mouseVector;
   /// Mouse sensitivity setting
   float mouseSensitivity;
   /// Mouse accumulator vector length min
   float mouseVectorLengthMin;
   /// Mouse accumulator vector length max
   float mouseVectorLengthMax;
   /// Mouse movement accumulator
   Point2F mouseMovement;
   /// Whether to treat the up key as pressed on this frame.
   bool keyUpPressed;
   /// Value of 'keyUpPressed' on the previous frame.
   bool keyUpPressedPrev;
   /// Whether to treat the down key as pressed on this frame.
   bool keyDownPressed;
   /// Value of 'keyDownPressed' on the previous frame.
   bool keyDownPressedPrev;
   /// Whether to treat the left key as pressed on this frame.
   bool keyLeftPressed;
   /// Value of 'keyLeftPressed' on the previous frame.
   bool keyLeftPressedPrev;
   /// Whether to treat the right key as pressed on this frame.
   bool keyRightPressed;
   /// Value of 'keyRightPressed' on the previous frame.
   bool keyRightPressedPrev;
   /// Whether to treat the action key as pressed on this frame.
   bool keyActionPressed;
   /// Value of 'keyActionPressed' on the previous frame.
   bool keyActionPressedPrev;
   /// Whether to treat the secondary action key as pressed on this frame.
   bool keySecondaryActionPressed;
   /// Value of 'keySecondaryActionPressed' on the previous frame.
   bool keySecondaryActionPressedPrev;
   /// JoystickWidget to control the direction keys.
   JoystickWidget* joystickWidget;
   /// PressButtonWidget to control the action key.
   PressButtonWidget* keyActionWidget;
   /// PressButtonWidget to control the secondary action key.
   PressButtonWidget* keySecondaryActionWidget;
   /// Gamepad to use for input.
   GamepadTypical* gamepadTypical;
};

WageManagerOpenGL* const theWageEngine = &WageManagerOpenGL::instance;

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGEMANAGEROPENGL_H__

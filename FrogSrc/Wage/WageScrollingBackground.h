#ifndef __WAGESCROLLINGBACKGROUND_H__
#define __WAGESCROLLINGBACKGROUND_H__

#include "Frog.h"

namespace Webfoot {

//==============================================================================

class WageScrollingBackground
{
public:
   WageScrollingBackground();
   ~WageScrollingBackground() {}

   virtual void Init(JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void Draw();

   /// Check the scrolling background visibility
   bool VisibleCheck() { return visible; }
   /// Set the scrolling background visibility
   void VisibleSet(bool _visible) { visible = _visible; }

protected:
   /// Name of the scrolling background
   const char* name;
   /// Image being scrolled
   Image* image;
   /// Width of the scrolling background (fraction of screen width)
   float width;
   /// Height of the scrolling background (fraction of screen height)
   float height;
   /// Scale of the scrolling background image
   Point2F scale;
   /// Horizontal lines per second
   unsigned int yDuration;
   /// Vertical lines per second
   unsigned int xDuration;
   /// Scroll line
   float line;
   /// Position of the top scrolling background image
   Point2F topPosition;
   /// Position of the bottom scrolling background image
   Point2F bottomPosition;
   /// Top portion of the scrolling background
   Box2F topSubset;
   /// Bottom portion of the scrolling background
   Box2F bottomSubset;
   /// Time accumulator
   unsigned int time;
   /// Whether or not the scrolling background should be drawn
   bool visible;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGESCROLLINGBACKGROUND_H__

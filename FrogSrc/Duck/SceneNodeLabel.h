#ifndef __FROG__DUCK__SCENENODELABEL_H__
#define __FROG__DUCK__SCENENODELABEL_H__

#include "FrogMemory.h"
#include "Sprite.h"
#include "Duck/Drawable.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

class SceneNodeLabelDrawable;

//==============================================================================

/// Scene node for drawing text.
class SceneNodeLabel : public SceneNode
{
public:
   SceneNodeLabel();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet() { return drawablesBoundingSphere; }

   /// Return the text to be displayed by this widget.
   virtual const char* TextGet() { return text; }
   /// Set the text to be displayed by this widget.
   virtual void TextSet(const char* _text);

   /// Return the key for theText for the string to be displayed by this
   /// widget.  Return NULL if a text key is not in use.
   virtual const char* TextKeyGet() { return textKey; }
   /// Set the key for theText for the string to be displayed by this
   /// widget.  This will replace any existing text, including when called with
   /// NULL.  This method will call TextRefresh() internally if appropriate.
   virtual void TextKeySet(const char* _textKey);

   /// Set the box, relative to the node's position, within which the text
   /// should be aligned and clipped.
   virtual void TextBoundsSet(Box2F _textBounds) { textBounds = _textBounds; BoundingVolumesRefresh(); }
   /// Return the box, relative to the node's position, within which the text
   /// should be aligned and clipped.
   virtual Box2F TextBoundsGet() { return textBounds; }

   /// Return the current text alignment.
   virtual int TextAlignmentGet() { return textAlignment; }
   /// Set the text alignment.
   virtual void TextAlignmentSet(int _textAlignment) { textAlignment = _textAlignment; }

   /// Return the scale at which the text will be drawn.
   virtual Point2F TextScaleGet() { return textScale; }
   /// Set the scale at which the text will be drawn.
   virtual void TextScaleSet(const Point2F& _textScale) { textScale = _textScale; }
   /// Return the text scale to actually use when drawing text.
   virtual Point2F TextScaleDisplayGet();
   /// Return true if the text should be scaled down to fit within the text 
   /// bounds.
   virtual bool TextScaleDownToFitCheck() { return textScaleDownToFit; }
   /// Set whether the text should be scaled down to fit within the text
   /// bounds.
   virtual void TextScaleDownToFitSet(bool _textScaleDownToFit) { textScaleDownToFit = _textScaleDownToFit; }

   /// Return the color to apply to the text when drawing.
   virtual ColorRGBA8 TextColorGet() { return textColor; }
   /// Set the color to apply to the text.  The actual color used for
   /// drawing is this value modulated with the result of ColorGet().
   virtual void TextColorSet(const ColorRGBA8& _textColor) { textColor = _textColor; }

   /// Return true if the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   virtual bool TextAscentDescentMaxUseCheck() { return textAscentDescentMaxUse; }
   /// Set whether the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   virtual void TextAscentDescentMaxUseSet(bool _textAscentDescentMaxUse) { textAscentDescentMaxUse = _textAscentDescentMaxUse; }

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeLabel(); }

   typedef SceneNode Inherited;

protected:
   /// Used to display the given text.
   void TextSetHelper(const char* _text);

   /// Return the text as it should be displayed, not necessarily
   /// how it is stored.
   virtual const char* TextDisplayGet() { return TextGet(); }
   /// Return the alignment with which to draw the text right now,
   /// which is not necessarily what's been requested.
   virtual int TextAlignmentDisplayGet() { return TextAlignmentGet(); }
   /// Return the opaque bounds of the string for purposes of automatic
   /// scaling.
   virtual Box2F TextOpaqueBoundsForScalingGet();

   /// Refresh the bounding volume of the label in local coordinates.
   void BoundingVolumesRefresh();

   /// The text to actually display.
   char* text;
   /// If defined, this will be passed to theText to get the string to display.
   char* textKey;
   /// Area relative to the node's position, within which the text should be
   /// aligned and clipped.
   Box2F textBounds;
   /// Combination of flags to use for alignment of the text.
   int textAlignment;
   /// How the text should be scaled when drawing.
   Point2F textScale;
   /// True if text should be scaled down automatically so that it fits within
   /// the TextBounds.
   bool textScaleDownToFit;
   /// Color to be applied to the text when drawing.
   ColorRGBA8 textColor;
   /// True if the maximum ascent and descent of the font should be used when
   /// aligning text vertically, rather than using the height of the string
   /// itself.
   bool textAscentDescentMaxUse;
   /// True if a warning has already been given about this node's font being
   /// NULL.
   bool nullFontWarningGiven;
   /// Object for drawing the label.
   SceneNodeLabelDrawable* drawable;
   /// Bounding sphere of the label in local coordinates.
   Sphere drawablesBoundingSphere;

   friend class SceneNodeLabelDrawableForwardOpenGL;
};

//==============================================================================

/// Helper class for drawing sprites in Duck.
class SceneNodeLabelDrawable : public Drawable
{
public:
   SceneNodeLabelDrawable();

   void Init(SceneNodeLabel* _sceneNodeLabel);
   void Deinit();

   virtual SceneNode* DrawableSceneNodeGet() { return sceneNodeLabel; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

protected:
   /// SceneNodeLabel to which this drawable belongs.
   SceneNodeLabel* sceneNodeLabel;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODELABEL_H__

#ifndef __FROG__GUI__COLORSELECTORWIDGET_H__
#define __FROG__GUI__COLORSELECTORWIDGET_H__

#include "FrogMemory.h"
#include "GUI/SliderWidget.h"
#include "GUI/SpriteWidget.h"
#include "GUI/TextEntryWidget.h"

namespace Webfoot {

class Bitmap;
class ColorSelectorHueSaturationCircleWidget;

//==============================================================================

/// Widget for helping choose a color.
class ColorSelectorWidget : public ContainerWidget
{
public:
   ColorSelectorWidget();
   virtual ~ColorSelectorWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new ColorSelectorWidget based on the specifications in
   /// the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();

   virtual const char* WidgetTypeNameGet();
   
   /// Set the color selector to the given color.
   virtual void SelectionSet(const ColorRGBA8& _color, bool userMadeChange);
   /// Set the color selector to the given color.
   virtual void SelectionSet(const ColorHSVA8& _color, bool userMadeChange);
   /// Return the current color.
   virtual ColorRGBA8 SelectionRGBA8Get() { return selectionRGBA8; }
   /// Get the current color in HSVA8.
   virtual ColorHSVA8 SelectionHSVA8Get() { return selectionHSVA8; }
   
   /// Called when the color selection changes.
   virtual void OnSelectionChange(const ColorRGBA8& newSelection, const ColorRGBA8& oldSelection, bool userMadeChange);

   /// Type of the function that can receive the OnSelectionChange event.  The
   /// first color is the new selection, the second color is the old selection, the
   /// bool is true if the change was caused directly by the user interacting
   /// with the widget or its children, the ColorSelectorWidget* is the one that
   /// changed, and the void* is the userData pointer.
   ///
   /// static void OnSelectionChangeCallback(const ColorRGBA8& newSelection,
   ///    const ColorRGBA8& oldSelection, bool userMadeChange,
   ///    ColorSelectorWidget* colorSelectorWidget, void* userData);
   typedef void (*OnSelectionChangeCallback)(const ColorRGBA8&, const ColorRGBA8&, bool, ColorSelectorWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnColorChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionChangeRegister(OnSelectionChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnColorChange event occurs.
   void OnSelectionChangeUnregister(OnSelectionChangeCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionChangeRegister(const char* path, OnSelectionChangeCallback callback, void* userData = NULL);

   /// Create a bitmap of the given size containing a hue/saturation circle of
   /// the given radius.
   static Bitmap* HueSaturationCircleBitmapCreate(const Point2I& size, float radius, HeapID heapID = HEAP_DEFAULT);

   typedef ContainerWidget Inherited;

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ColorSelectorWidget(); }

protected:
   /// Helper function for setting up the widgets for a given channel.
   virtual void ChannelInitHelper(JSONValue* specs,
      SliderWidget** sliderWidget, TextEntryWidget** textEntryWidget,
      const char* labelKey, const char* sliderKey, const char* textEntryKey,
      SliderWidget::OnValueChangeCallback onSliderValueChange,
      TextEntryWidget::OnEnterPressCallback onEnterPress,
      TextEntryWidget::OnFocusLoseCallback onFocusLose);
   /// Refresh the widgets based on the current selection.
   virtual void WidgetsRefresh();
   /// Helper function for refreshing the TextEntry and Slider widgets for a
   /// single channel.
   virtual void ChannelWidgetsRefresh(uchar channelValue, SliderWidget* sliderWidget,
      TextEntryWidget* textEntryWidget);

   /// Called when a slider widget for one of the red, green, blue, or alpha
   /// channels changes.
   virtual void OnRGBASliderValueChange(float newValue, bool userMadeChange, SliderWidget* sliderWidget);
   /// Called when the enter key is pressed for one of the red, green, blue, or
   /// alpha text entry widgets.
   virtual void OnRGBATextEntryEnterPress(TextEntryWidget* textEntryWidget);
   /// Called when a slider widget for one of the hue, saturation, or value
   /// channels changes.
   virtual void OnHSVSliderValueChange(float newValue, bool userMadeChange, SliderWidget* sliderWidget);
   /// Called when the enter key is pressed for one of the hue, saturation, or
   /// value text entry widgets.
   virtual void OnHSVTextEntryEnterPress(TextEntryWidget* textEntryWidget);

   /// Called when a slider widget for one of the red, green, blue, or alpha
   /// channels changes.
   static void _OnRGBASliderValueChange(float newValue, float, bool userMadeChange,
      SliderWidget* sliderWidget, void* userData);
   /// Called when the enter key is pressed for one of the red, green, blue, or
   /// alpha text entry widgets.
   static void _OnRGBATextEntryEnterPress(TextEntryWidget* textEntryWidget, void* userData);
   /// Called when one of the red, green, blue, or alpha text entry widgets
   /// loses focus.
   static void _OnRGBATextEntryFocusLose(TextEntryWidget* textEntryWidget, void* userData);
   /// Called when a slider widget for one of the hue, saturation, or value
   /// channels changes.
   static void _OnHSVSliderValueChange(float newValue, float, bool userMadeChange,
      SliderWidget* sliderWidget, void* userData);
   /// Called when the enter key is pressed for one of the hue, saturation, or
   /// value text entry widgets.
   static void _OnHSVTextEntryEnterPress(TextEntryWidget* textEntryWidget, void* userData);
   /// Called when one of the hue, saturation, or value text entry widgets loses
   /// focus.
   static void _OnHSVTextEntryFocusLose(TextEntryWidget* textEntryWidget, void* userData);

   /// Current selected color.
   ColorRGBA8 selectionRGBA8;
   /// Current selected color in HSVA8.
   ColorHSVA8 selectionHSVA8;
   /// Widget which displays a preview of the selected color using its filled
   /// bounds.
   ContainerWidget* colorPreviewWidget;
   /// Widget for showing and selecting the hue and saturation.
   ColorSelectorHueSaturationCircleWidget* hueSaturationCircleWidget;
   /// Slider widget for the red channel.
   SliderWidget* redSlider;
   /// TextEntry widget for the red channel.
   TextEntryWidget* redTextEntry;
   /// Slider widget for the green channel.
   SliderWidget* greenSlider;
   /// TextEntry widget for the green channel.
   TextEntryWidget* greenTextEntry;
   /// Slider widget for the blue channel.
   SliderWidget* blueSlider;
   /// TextEntry widget for the blue channel.
   TextEntryWidget* blueTextEntry;
   /// Slider widget for the alpha channel.
   SliderWidget* alphaSlider;
   /// TextEntry widget for the alpha channel.
   TextEntryWidget* alphaTextEntry;
   /// Slider widget for the hue channel.
   SliderWidget* hueSlider;
   /// TextEntry widget for the hue channel.
   TextEntryWidget* hueTextEntry;
   /// Slider widget for the saturation channel.
   SliderWidget* saturationSlider;
   /// TextEntry widget for the saturation channel.
   TextEntryWidget* saturationTextEntry;
   /// Slider widget for the value channel.
   SliderWidget* valueSlider;
   /// TextEntry widget for the value channel.
   TextEntryWidget* valueTextEntry;
   /// Function to be called for the OnSelectionChange event.
   OnSelectionChangeCallback onSelectionChangeCallback;
   /// Pointer to be passed to the OnSelectionChange callback.
   void* onSelectionChangeUserData;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__COLORSELECTORWIDGET_H__

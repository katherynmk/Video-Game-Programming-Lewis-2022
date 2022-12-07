#ifndef __FROG__GUI_RADIOBUTTONWIDGET_H__
#define __FROG__GUI_RADIOBUTTONWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"
#include "GUI/ToggleButtonWidget.h"

namespace Webfoot {

//==============================================================================

/// A RadioButtonWidget is a type of ToggleButtonWidget where only one in a group
/// can be toggled on.
class RadioButtonWidget : public ToggleButtonWidget
{
public:
   RadioButtonWidget();
   virtual ~RadioButtonWidget() {}

   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();

   /// Called when the left mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnClick();
   /// Called when an input focus finishes affirming this widget.
   virtual void OnKeypadClick();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Set whether the button is toggled to on.
   virtual void ToggledOnSet(bool _toggledOn);
   /// Return true if this button should toggle between on and off when clicked.
   virtual bool ToggleOnClickCheck() { return false; }
   /// Return true if this button should default to toggling when clicked.
   virtual bool ToggleOnClickDefaultGet() { return false; }
   
   /// Return the name of the radio button group of which this is a member.
   /// If this returns NULL, the radio button group is assumed to be established
   /// by an ancestral ContainerWidget. 
   virtual const char* RadioButtonGroupNameGet() { return radioButtonGroupName; }
   /// See RadioButtonGroupNameGet().  Just calling this function will not force the other
   /// buttons in the new group to turn off, nor will it turn on a button from
   /// the old group. 
   virtual void RadioButtonGroupNameSet(const char* _radioButtonGroupName);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) RadioButtonWidget(); }

protected:  
   typedef ToggleButtonWidget Inherited;

   /// Recursive helper function for toggling off any radio buttons except this
   /// RadioButtonWidget instance from the given widget down through its
   /// descendants.
   virtual void RadioButtonsToggleOff(Widget* widget);
   
   /// Name of the radio button group of which this is a member.  If this is
   /// NULL, assume the radio button group is established by an ancestral
   /// ContainerWidget. 
   const char* radioButtonGroupName;
};

//==============================================================================

/// A RadioButtonContainerWidget is one way of establishing a mutually exclusive
/// set of radio buttons.
class RadioButtonContainerWidget : public ContainerWidget
{
public:
   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) RadioButtonContainerWidget(); }
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI_RADIOBUTTONWIDGET_H__

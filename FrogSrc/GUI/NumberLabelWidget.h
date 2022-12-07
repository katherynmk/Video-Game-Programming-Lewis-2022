#ifndef __FROG__GUI__NUMBERLABELWIDGET_H__
#define __FROG__GUI__NUMBERLABELWIDGET_H__

#include "FrogMemory.h"
#include "GUI/LabelWidget.h"

namespace Webfoot {

//=============================================================================

/// Widget that displays a number in a single font aligned within the text bounds.
/// This text representation of the number can be determined by a Lua function.
/// The TextKey and TextExpression are ignored for this label.
/// Call Init immediately after construction.
class NumberLabelWidget : public LabelWidget
{
public:
   NumberLabelWidget();
   virtual ~NumberLabelWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new NumberLabelWidget based on the specifications in the given JSONValue
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Reload the text for the widget.
   /// Call this after a language change.
   virtual void TextRefresh();

   /// Return the current value of this widget's number
   double ValueGet() { return value; }
   /// Set the value for this widget's number and update its appearance.
   virtual void ValueSet(double _value);

   /// Return the Lua expression that evaluates to the function that will be called
   /// with the widget's value to get the string representation.
   /// Return NULL if a Lua format is not in use.
   virtual const char* TextFormatExpressionGet() { return textFormatExpression; }
   /// Set the Lua expression that evaluates to the function that will be called
   /// with the widget's value to get the string representation.
   /// For example, TextFormatExpressionSet("StringFormatInteger")
   /// Call with NULL to use the default format, which is the printf-style %f.
   /// This function calls its TextRefresh method internally.
   /// This is mutually exclusive with TextFormatFunctionSet, and calling
   /// this will clear that setting.
   virtual void TextFormatExpressionSet(const char* _textFormatExpression);

   /// Given the value, buffer and the buffer size, this will build a string 
   /// with the value provided and store it in the buffer.
   typedef void(*TextFormatFunction)(double value, char* buffer, size_t bufferSize, void* userData);   

   /// Return the current TextFormatFunction, if any, being used to build the strings
   /// to be displayed.
   virtual TextFormatFunction TextFormatFunctionGet() { return textFormatFunction; }
   /// Set a C function for building a string to display based on the current
   /// value.  This is mutually exclusive with TextFormatExpressionSet, and
   /// calling this function will clear that setting.  Call with NULL to
   /// use the default format, which is the printf-style %f.
   virtual void TextFormatFunctionSet(TextFormatFunction _textFormatFunction, void* userData = NULL);

   /// Return the maximum number of characters that can be typed.
   virtual int TextLengthMaxGet() { return maxTextLength; }
   /// Set the maximum number of characters that can be typed.
   virtual void TextLengthMaxSet(int _maxTextLength);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) NumberLabelWidget(); }

   /// TextFormatFunction for displaying the number as a signed floating point value.
   static void TextFormatFunctionFloat(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for displaying the number as a signed integer.
   static void TextFormatFunctionInteger(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for displaying the number with separators to make the
   /// number more readable.  For now, this only supports the system of comma
   /// separators used in English.
   static void TextFormatFunctionIntegerWithDigitGrouping(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for displaying the number as a signed integer number
   /// of dollars.
   static void TextFormatFunctionIntegerDollars(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for taking the number as a signed integer number
   /// of cents and displaying them as dollars and cents.
   static void TextFormatFunctionDollarsAndCentsFromCents(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for taking the number as a signed integer number
   /// of cents and displaying them as dollars.
   static void TextFormatFunctionIntegerDollarsFromCents(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for displaying the number as a signed integer
   /// percentage.
   static void TextFormatFunctionIntegerPercentage(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for displaying a time given in seconds as a number of
   /// minutes and seconds with leading zeros.
   static void TextFormatFunctionMinutesAndSecondsFromSeconds(double value, char* buffer, size_t bufferSize, void* userData);
   /// TextFormatFunction for displaying a time given in seconds as a number of
   /// hours, minutes, and seconds with leading zeros.
   static void TextFormatFunctionHoursMinutesAndSecondsFromSeconds(double value, char* buffer, size_t bufferSize, void* userData);

protected:
   typedef LabelWidget Inherited;

   /// Return true if TextRefresh() is allowed to use the TextKey.
   virtual bool TextKeyUseCheck() { return false; }
   /// Return true if TextRefresh() is allowed to use the TextExpression.
   virtual bool TextExpressionUseCheck() { return false; }

   /// The number stored by this widget.
   double value;
   /// The Lua expression that evaluates to the function that will be called
   /// with the widget's value to get the string representation
   char* textFormatExpression;
   /// Function used to build the strings to be displayed.
   TextFormatFunction textFormatFunction;
   /// Pointer to be passed to the text format function.
   void* textFormatUserData;
   /// Maximum number of characters.
   int maxTextLength;
   /// Size of the text buffer.
   int textBufferSize;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__NUMBERLABELWIDGET_H__

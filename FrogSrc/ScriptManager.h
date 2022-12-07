#ifndef __FROG__SCRIPTMANAGER_H__
#define __FROG__SCRIPTMANAGER_H__

#include "FrogMemory.h"
#include "Utility.h"
#include "FileManager.h"
#include "Point2.h"
#include "Box2.h"

struct lua_State;

namespace Webfoot {

struct ColorRGBA8;

/// Size of the temporary string buffer used for building commands.
#define SCRIPT_MANAGER_SCRIPT_STRING_SIZE 512
#define SCRIPT_MANAGER_GLOBAL_TABLE "_G"

#define LUAVAR_TABLE "LuaVar"

//==============================================================================

class ScriptManager
{
private:
   /// Helper class that encapsulates everything needed to make calls to a Lua
   /// function.
   class ScriptFunctor
   {
   public:
      ScriptFunctor(ScriptManager* aInterpreter, const char* aMethodName) { interpreter = aInterpreter; methodName = aMethodName; }
      
      int operator()() { return interpreter->Call(methodName); }
      int operator()(int a) { return interpreter->Call(methodName, a); }
      
   private:
      ScriptManager* interpreter;
      const char* methodName;
   };

public:
   ScriptManager();
   ~ScriptManager();

   void Init(HeapID _heapId = HEAP_DEFAULT, FileManager* _fileManager = theFiles);
   void Deinit();

   /// Exectute the given script file.  If no FileManager is provided here, it will use
   /// the current file system.  If a FileManager is specified, this
   /// ScriptManager will temporarily switch to that file system until this call returns.
   /// Return true if successful.
   bool ExecuteFile(const char* fileName, FileManager* _fileManager = NULL);
   /// Execute the given string of script.  Return true if successful.
   bool Execute(const char* script);

   /// Build a string using the given pattern and parameters, then execute it in Lua.
   /// The buffer is large, but finite, so do not make commands larger than 
   /// SCRIPT_MANAGER_SCRIPT_STRING_SIZE bytes.  If the command is truncated
   /// it will not be executed.  Return true if no there are no errors from Lua
   /// and no truncation.
   bool Executef(const char *_pattern, ...);
   /// Build a string using the given pattern and parameters, then execute it in Lua.
   /// The buffer is large, but finite, so do not make commands larger than
   /// SCRIPT_MANAGER_SCRIPT_STRING_SIZE bytes.  If the command is truncated
   /// it will not be executed.  Return true if no there are no errors from Lua
   /// and no truncation.
   bool Vexecutef(const char *_pattern, va_list arguments);

   /// Return true if the given expression evaluates to nil.
   /// The buffer is large, but finite, so do not make expressions larger than
   /// SCRIPT_MANAGER_SCRIPT_STRING_SIZE minus the size of the rest of the
   /// command needed to test whether it's nil (485 characters including 
   /// the null-terminator, as of this comment).  If truncation occurs or if the
   /// expression fails to evaluate, return true.
   bool ExpressionNilCheckf(const char* _pattern, ...);
   /// Return true if the given expression evaluates to nil.
   /// The buffer is large, but finite, so do not make expressions larger than
   /// SCRIPT_MANAGER_SCRIPT_STRING_SIZE minus the size of the rest of the
   /// command needed to test whether it's nil (485 characters including 
   /// the null-terminator, as of this comment).  If truncation occurs or if the
   /// expression fails to evaluate, return true.
   bool VexpressionNilCheckf(const char* _pattern, va_list arguements);

   /// Return the current number of KB in use by this object's Lua instance.
   int ScriptMemoryGet() const;

   /// Return true if the given global variable is nil
   bool NilCheck(const char* field);
   /// Return true if the given entry in the given table is nil
   bool NilCheck(const char* table, const char* field);
   /// Return true if the given entry in the given table is nil
   bool NilCheck(const char* table, int index);

   /// Return true if the given entry in the given table is a string
   bool StringCheck(const char* table, const char* field);
   /// Return true if the given entry in the given table is a boolean
   bool BooleanCheck(const char* table, const char* field);
   /// Return true if the given entry in the given table is a number.  Booleans don't count here.
   bool NumberCheck(const char* table, const char* field);
   /// Return true if the given entry in the given table is non-light user data.
   bool FullUserDataCheck(const char* table, const char* field);
   /// Return true if the given entry in the given table is light user data.
   bool LightUserDataCheck(const char* table, const char* field);
   /// Return the string in the given entry of the given table.
   const char *StringGet(const char* table, const char* field);
   /// Return the string in the given entry of the table2 where table2 is inside table1.
   const char *StringGet(const char* table1, const char* table2, const char* field);
   /// Assign the given string value to the specified field of the given table.
   void StringSet(const char* table, const char* field, const char* value);
   /// Get a string from a numerically indexed table
   const char *StringGet(const char* table, int index);
   /// Return the number with the given field name in the given table.
   double NumberGet(const char* table, const char* field);
   /// Get a number from a numerically indexed table.
   double NumberGet(const char* table, int index);
   /// Assign the given numerical value to the specified field of the given table.
   void NumberSet(const char* table, const char* field, double value);
   /// Assign the given boolean value to the specified field of the given table.
   void BooleanSet(const char* table, const char* field, bool value);
   /// Return the value of the boolean in the given field of the given table.
   inline bool BooleanGet(const char* table, const char* field)
   {
      return (NumberGet(table, field) != 0.0f);
   }
   /// Create a buffer of the given size, copy the data from the given source,
   /// and store it at the given field of the given table.
   void FullUserDataSet(const char* table, const char* field, void* data, size_t dataSize);
   /// If the entry at the given field of the given table is non-light user data, return
   /// a pointer to it and set 'dataSize' to its size.  If it is not non-light user data,
   /// return NULL.
   void* FullUserDataGet(const char* table, const char* field, size_t* dataSize);
   /// Store the given pointer in the given field of the given table.
   void LightUserDataSet(const char* table, const char* field, void* data);
   /// If the given field of the given table is light user data, return it.
   /// If not, return NULL.
   void* LightUserDataGet(const char* table, const char* field);
   
   /// Return true if the table contains a function with the given name
   int FunctionCheck(const char* table, const char* function);
   
   /// If 'object' is the name of a string, return the number of characters.
   /// If it is a table, return the number of numerically indexed items.
   /// If it is full userdata, return the size of the block of memory allocated for it.
   /// For all other types, return 0.
   size_t SizeGet(const char* object);
   /// If the given field of the given table is a string, return the number of characters.
   /// If it is a table, return the number of numerically indexed items.
   /// If it is full userdata, return the size of the block of memory allocated for it.
   /// For all other types, return 0.
   size_t SizeGet(const char* table, const char* field);

   /// Use this to iterate through Lua tables that are indexed with strings.
   /// Return the next key that is a string in the given table after the specified 'key'.
   /// If this is the first call, use NULL for the 'key'.  Returns NULL if there
   /// are no more left in the table.  Remember that they will not necessarily be in
   /// the same order as in the Lua script.
   const char* StringKeyNextGet(const char* table, const char* key);

   void FunctionAdd(const char* name, void (*function)());
   void FunctionAdd(const char* name, void (*function)(float));
   void FunctionAdd(const char* name, void (*function)(float, float));
   void FunctionAdd(const char* name, void (*function)(float, float, float));
   void FunctionAdd(const char* name, void (*function)(float, float, float, float));
   void FunctionAdd(const char* name, void (*function)(float, bool));
   void FunctionAdd(const char* name, void (*function)(const char*));
   void FunctionAdd(const char* name, void (*function)(const char*, const char*));
   void FunctionAdd(const char* name, void (*function)(const char*, const char*, const char*));
   void FunctionAdd(const char* name, void (*function)(const char*, float));
   void FunctionAdd(const char* name, void (*function)(const char*, float, float));
   void FunctionAdd(const char* name, void (*function)(const char*, float, float, float));
   void FunctionAdd(const char* name, void (*function)(const char*, float, float, float, float));
   void FunctionAdd(const char* name, void (*function)(const char*, const char*, float));
   void FunctionAdd(const char* name, void (*function)(const char*, bool));
   void FunctionAdd(const char* name, void (*function)(bool));
   void FunctionAdd(const char* name, void (*function)(float, float, const char *));
   void FunctionAdd(const char* name, float  (*function)(float, float, const char *));

   void FunctionAdd(const char* name, bool (*function)());
   void FunctionAdd(const char* name, bool (*function)(const char*));

   void FunctionAdd(const char* name, const char* (*function)());
   void FunctionAdd(const char* name, const char* (*function)(float));
   void FunctionAdd(const char* name, const char* (*function)(const char*));

   void FunctionAdd(const char* name, float (*function)());
   void FunctionAdd(const char* name, float (*function)(float));
   void FunctionAdd(const char* name, float (*function)(const char*));
   void FunctionAdd(const char* name, float (*function)(const char*, const char*));
   void FunctionAdd(const char* name, float (*function)(const char*, const char*, const char*));
   void FunctionAdd(const char* name, float (*function)(const char*, const char*, float));
   void FunctionAdd(const char* name, float (*function)(const char*, float, float));

   /// By default we do a full gargabe collect - this is most situations.
   void Update(bool full = true);
   
   /// Call a the given function in the global table.
   int Call(const char* methodName);
   /// Call a the given function in the global table and pass the given value as its parameter.
   int Call(const char* methodName, int value);
   /// Call a the given function in the global table and pass the given value as its parameter.
   int Call(const char* methodName, const char* value);
   
   /// Call a function that is a member of a table.
   int TableCall(const char* table, const char* methodName);
   /// Call a function that is a member of table2 where table2 is inside table1.
   int TableCall(const char* table1, const char* table2, const char* methodName);

   static ScriptManager instance;

   ScriptFunctor operator[](const char* methodName) { return ScriptFunctor(this, methodName); }

   /// Call this with 'true' to display DebugMessages when errors are encountered.
   void UseDebugMessageSet(bool _useDebugMessage) { useDebugMessage = _useDebugMessage; }

   /// Used by Lua to allocate and free memory.
   static void* ReallocStatic(void* userData, void* ptr, size_t oldSize, size_t newSize);
   /// Called by ReallocStatic
   void* Realloc(void* ptr, size_t newSize);

   /// Return the FileManager currently set for this ScriptManager.
   FileManager* FileManagerGet() { return fileManager; }
   /// Return the HeapID currently set for this ScriptManager.
   HeapID HeapIDGet() { return heapId; }

   /// Create a new global table stack.  Return the name by which it should be
   /// referenced.  _G[theReturnedName] points to the current position.
   /// Use this name when using all the GlobalTableStack functions.
   /// Return NULL if 'startTable' is nil.
   ///
   /// In C++, theScripts only has good accessors and mutators for
   /// variables in global tables.  These functions refer to such tables by name.
   /// Also, we often need to traverse these trees recursively.  To address these
   /// needs, the GlobalTableStack system allows one to safely navigate through a
   /// tree of tables using a single name to keep track of the current position.
   /// That name corresponds to a global variable set to the current
   /// position.  This name can be passed to theScripts's GetString,
   /// SetString, GetNumber, etc. functions as the 'table' parameter. The
   /// system accomplishes all this while only making changes to a couple random
   /// unused global variables per stack.  This means the tree being traversed is
   /// not changed.  The one exception would be to traverse _G.
   const char* GlobalTableStackInit(const char* startTable);
   /// Clean up the stack.
   void GlobalTableStackDeinit(const char* stackName);
   /// Push the given element of the current table on top of the stack.
   void GlobalTableStackPush(const char* stackName, const char* index);
   /// Push the given element of the current table on top of the stack.
   void GlobalTableStackPush(const char* stackName, int index);
   /// Pop the top table off the stack.
   void GlobalTableStackPop(const char* stackName);

   /// Load a Point2F from a string in the specified 'field' of the given Lua 'table'.
   /// If the point is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If 'defaultValue' is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "x|y" where both values are floats.
   bool Point2FGet(Point2F* destination, const char* table, const char* field,
      const Point2F* defaultValue = NULL, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);
   /// Same as the other Point2FGet, except this gets the point from a numerically indexed entry
   /// in the given table.
   bool Point2FGet(Point2F* destination, const char* table, int field,
      const Point2F* defaultValue = NULL, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);

   /// Load a Box2F from a string in the specified 'field' of the given Lua 'table'.
   /// If the string is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If default is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "x|y|width|height" where all values are floats.
   bool Box2FGet(Box2F* destination, const char* table, const char* field,
      const Box2F* defaultValue = NULL, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);
   /// Same as the other Box2FGet, except this gets the box from a numerically indexed entry
   /// in the given table.
   bool Box2FGet(Box2F* destination, const char* table, int field,
      const Box2F* defaultValue = NULL, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);

   /// Load a ColorRGBA8 from a string in the specified 'field' of the given Lua 'table'.
   /// If the string is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If default is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format is "red|green|blue|alpha" where each is an integer
   /// between 0 and 255 (inclusive).
   bool ColorRGBA8Get(ColorRGBA8* destination, const char* table, const char* field,
      const ColorRGBA8* defaultValue = NULL, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);
   /// Same as the other ColorRGBA8Get, except this gets the box from a numerically indexed entry
   /// in the given table.
   bool ColorRGBA8Get(ColorRGBA8* destination, const char* table, int field,
      const ColorRGBA8* defaultValue = NULL, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);

   /// Load a double from the specified 'field' of the given Lua 'table'.
   /// If the item is not a number, set 'destination' to the 'defaultValue' and return false.
   /// Return true if successful.  'requesterType' indicates what type of object was asking for the value.
   /// 'requesterName' indicates the name of the instance of the object that was asking for the value.
   bool NumberGet(double* destination, const char* table, const char* field,
      double defaultValue = 0.0, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);

   /// Load a boolean from the specified 'field' of the given Lua 'table'.
   /// If the item is not a boolean, set 'destination' to the 'defaultValue' and return false.
   /// Return true if successful.  'requesterType' indicates what type of object was asking for the value.
   /// 'requesterName' indicates the name of the instance of the object that was asking for the value.
   bool BooleanGet(bool* destination, const char* table, const char* field,
      bool defaultValue = false, const char* requesterType = NULL, const char* requesterName = NULL,
      bool required = false);

   /// Set the given field of the given table to nil.
   void NilSet(const char* table, const char* field);
   /// Set the given index of the given table to nil.
   void NilSet(const char* table, int index);

   /// Return the string Lua expects for the given boolean value.
   const char* BoolToString(bool value);
   /// If the given string is NULL, return "nil".  Otherwise return "\"value\"".
   /// This allocates a new string, so be sure to delete[] it when you're done with it.
   /// This is useful when passing a parameter to Lua printf-style where a string could
   /// be NULL.
   const char* StringOrNil(const char* value, HeapID _heapID = HEAP_TEMP);

private:
   /// Struct that holds everything for a given Lua instance.
   lua_State* state;
   /// If 'true', DebugMessages will be displayed for script errors.
   bool useDebugMessage;

   /// Heap to use for all internal allocations.
   HeapID heapId;
   /// File system to use by default.
   FileManager* fileManager;
   
   static int MethodProxyB(lua_State* state);
   static int MethodProxyBS(lua_State* state);
   static int MethodProxyV(lua_State* state);
   static int MethodProxyVN(lua_State* state);
   static int MethodProxyVNN(lua_State* state);
   static int MethodProxyVNNN(lua_State* state);
   static int MethodProxyVNNNN(lua_State* state);
   static int MethodProxyVNB(lua_State* state);
   static int MethodProxyVS(lua_State* state);
   static int MethodProxyVSS(lua_State* state);
   static int MethodProxyVSSS(lua_State* state);
   static int MethodProxyVSN(lua_State* state);
   static int MethodProxyVSNN(lua_State* state);
   static int MethodProxyVSNNN(lua_State* state);
   static int MethodProxyVSSN(lua_State* state);
   static int MethodProxyVSNNNN(lua_State* state);
   static int MethodProxyVSB(lua_State* state);
   static int MethodProxyVB(lua_State* state);
   static int MethodProxyVNNS(lua_State* state);
   static int MethodProxyNNNS(lua_State* state);

   static int MethodProxyN(lua_State* state);
   static int MethodProxyNN(lua_State* state);
   static int MethodProxyNS(lua_State* state);
   static int MethodProxyNSS(lua_State* state);
   static int MethodProxyNSSS(lua_State* state);
   static int MethodProxyNSSN(lua_State* state);
   static int MethodProxyNSNN(lua_State* state);

   static int MethodProxyS(lua_State* state);
   static int MethodProxySN(lua_State* state);
   static int MethodProxySS(lua_State* state);
   
   void MethodProxyRegister(const char* name, int (*proxy)(lua_State*));
   
   void StackDump();
};

//=============================================================================

ScriptManager *const theScripts = &ScriptManager::instance;

} //namespace Webfoot

#endif //#ifndef __FROG__SCRIPTMANAGER_H__

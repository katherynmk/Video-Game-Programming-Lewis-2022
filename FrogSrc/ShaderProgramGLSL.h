#ifndef __FROG__SHADERPROGRAMGLSL_H__
#define __FROG__SHADERPROGRAMGLSL_H__

#include "FrogMemory.h"
#include "Table.h"
#include "FrogOpenGL.h"
#include "FileManager.h"

namespace Webfoot {

struct ColorRGB8;
struct ColorRGBA32F;
class Matrix44;

//==============================================================================

/// ShaderProgramGLSL handles the common tasks of dealing with GLSL shaders.
class ShaderProgramGLSL
{
public:
   /// Types of shaders that make up a shader program.
   enum ShaderType
   {
      VERTEX,
      FRAGMENT,
      GEOMETRY_ARB,
      SHADER_TYPE_COUNT
   };

   ShaderProgramGLSL();

   /// Initialize the shader program to use the given name for debugging purposes.
   void Init(const char* _debugName = NULL, HeapID _heapID = HEAP_DEFAULT);
   void Deinit();

   /// Add the given string as a section of sources for the given shader.
   void SourceStringAdd(ShaderType shaderType, const char* source);
   /// Add the contents of the given file as a section of sources for the given shader.
   void SourceFileAdd(ShaderType shaderType, const char* filename, FileManager* fileManager = theFiles);

   /// Call this after compiling, but before linking.  If you need to change attributes after linking,
   /// call 'Link' again after the changes have been made.
   void AttributeSet(const char* name, int attributeIndex);

   /// Set the given fragment out variable to be used as the given color output.  Call this after
   /// compiling, but before linking.
   void FragmentOutputSet(const char* name, int colorIndex = 0);

   /// Once all the sources have been provided, compile them.  Return true if
   /// successful.
   bool Compile();
   /// After compiling and setting up the vertex attributes with AttributeSet,
   /// you can link.  Return true if successful.
   bool Link();

   /// Return the location of the uniform with the given name.  Return
   /// -1 if the uniform is not found.
   GLint UniformLocationGet(const char* name);

   /// Set the uniform variable with the given name to the given value.
   void UniformSet(const char* name, bool value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, bool value);
   void UniformSet(const char* name, int value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, int value);
   void UniformSet(const char* name, float value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, float value);
   void UniformSet(const char* name, const Point2F& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const Point2F& value);
   void UniformSet(const char* name, const Point3F& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const Point3F& value);
   void UniformSet(const char* name, const Point4F& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const Point4F& value);
   void UniformSet(const char* name, const ColorRGB8& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const ColorRGB8& value);
   void UniformSet(const char* name, const ColorRGBA8& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const ColorRGBA8& value);
   void UniformSet(const char* name, const ColorRGBA32F& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const ColorRGBA32F& value);
   void UniformSet(const char* name, const Matrix43& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const Matrix43& value);
   void UniformSet(const char* name, const Matrix44& value) { UniformSet(UniformLocationGet(name), value); }
   void UniformSet(GLint uniformLocation, const Matrix44& value);
   /// Set the upper-left 3x3 of the given matrix to the given 3x3 matrix uniform.
   void UniformMatrix33Set(const char* name, const Matrix43& value) { UniformMatrix33Set(UniformLocationGet(name), value); }
   void UniformMatrix33Set(GLint uniformLocation, const Matrix43& value);

   /// Return the native handle for the shader program.
   inline GLuint ShaderProgramIDGet() const { return shaderProgramID; }

   /// Debug function to print information about how the shader program would
   /// work under the current conditions.  Return true if validation is
   /// successful.
   bool Validate();

   /// Print the combined sources for the shaders to the debug output.
   void SourcesPrint();

protected:
   /// One section of sources.
   struct ShaderSourceEntry
   {
      /// Text of the sources for this entry.
      const char* sourceString;
      /// Filename used to load this entry's source text, if applicable.
      const char* filename;
      /// File manager used to load this entry's source text, if applicable.
      FileManager* fileManager;
   };

   /// Helper function for creating, compiling, and attaching the given shader.
   /// Return true if successful.
   bool CompileHelper(ShaderType shaderType);

   /// Native handle for the shader program.
   GLuint shaderProgramID;
   /// Native handles for the individual shaders (vertex, fragment, etc.)
   GLuint shaderIDs[SHADER_TYPE_COUNT];
   /// Heap used for long-term allocations.
   HeapID heapID;
   /// Name used to identify this shader program for debug purposes.
   const char* debugName;
   /// Collection of the sources used to build this shader program.
   Table<ShaderSourceEntry> shaderSources[SHADER_TYPE_COUNT];
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SHADERPROGRAMGLSL_H__

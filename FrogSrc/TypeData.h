#ifndef __FROG__TYPEDATA_H__
#define __FROG__TYPEDATA_H__

#include "FrogMemory.h"
#include <float.h>

//-----------------------------------------------------------------------------

namespace Webfoot
{
//-----------------------------------------------------------------------------

   // Generic interface
   template<typename T> class TypeData
   {
   public:
      static T Minimum()         { return 0; }
      static T Maximum();

      // Allows other TypeDatas to override what "Zero" means (eg. for intervals)
      static T Zero()            { return 0; }

      static T Identity()        { return 1; }

      enum { isIntegral       = false };
      enum { isFloat          = false };
      enum { isPOD            = false };
      enum { isBitwiseCopySafe   = false };
      enum { isPointer        = false };

      // ParameterUpcast is the type that the C compiler will use when passing parameters through "..."
      typedef T ParameterUpcast;

      // MathematicalUpcast is the type that the C compiler will use when performing basic operations
      typedef T MathematicalUpcast;

      // MathematicalFloatUpcast is the type that the C compiler will need/use when performing <cmath> functions
      typedef float MathematicalFloatUpcast;

      // ParameterType is the type most appropriate for passing this data around
      typedef const T& ParameterType;
   };

//-----------------------------------------------------------------------------

   template<typename T> class TypeData<T*>
   {
   public:
      static T* Zero()           { return 0; }

      enum { isFloat          = false };
      enum { isPOD            = true  };
      enum { isBitwiseCopySafe   = true  };
      enum { isPointer        = true  };

      typedef T* ParameterUpcast;
      typedef T* ParameterType;
   };

//-----------------------------------------------------------------------------

   template<> class TypeData<bool>
   {
   public:
      static bool Minimum()         { return false;   }
      static bool Maximum()         { return true; }
      static bool Zero()            { return false;   }
      static bool Identity()        { return true; }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef int ParameterUpcast;
      typedef int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef bool ParameterType;
   };

//---------------------------------------------------------------------------

   template<> class TypeData<int8>
   {
   public:
      static signed char Minimum()  { return -128; }
      static signed char Maximum()  { return  127; }
      static signed char Zero()     { return 0; }
      static signed char Identity() { return 1; }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef int ParameterUpcast;
      typedef int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef signed char ParameterType;
   };

   template<> class TypeData<uint8>
   {
   public:
      static unsigned char Minimum()   { return 0; }
      static unsigned char Maximum()   { return  255; }
      static unsigned char Zero()      { return 0; }
      static unsigned char Identity()  { return 1; }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef unsigned int ParameterUpcast;
      typedef unsigned int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef unsigned char ParameterType;
   };

//---------------------------------------------------------------------------

   //template<> class TypeData<wchar_t>
   //{
   //public:
   //   enum { isIntegral       = true };
   //   enum { isFloat          = false };
   //   enum { isPOD            = true };
   //   enum { isBitwiseCopySafe   = true };
   //   enum { isPointer        = false };
   //
   //   typedef wchar_t ParameterUpcast;
   //   typedef wchar_t ParameterType;
   //};

   template<> class TypeData<int16>
   {
   public:
      static signed short Minimum() { return -32768; }
      static signed short Maximum() { return  32767; }
      static signed short Zero()    { return   0; }
      static signed short Identity()   { return   1; }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef int ParameterUpcast;
      typedef int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef signed short ParameterType;
   };

   template<> class TypeData<uint16>
   {
   public:
      static unsigned short Minimum()     { return 0;    }
      static unsigned short Maximum()     { return 65535;   }
      static unsigned short Zero()     { return 0;    }
      static unsigned short Identity() { return 1;    }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef unsigned int ParameterUpcast;
      typedef unsigned int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef unsigned short ParameterType;
   };

//---------------------------------------------------------------------------

   template<> class TypeData<int32>
   {
   public:
      static signed int Minimum()      { return 0x80000000; }
      static signed int Maximum()      { return 0x7FFFFFFF; }
      static signed int Zero()      { return 0;          }
      static signed int Identity()  { return 1;          }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef int ParameterUpcast;
      typedef int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef signed int ParameterType;
   };

   template<> class TypeData<uint32>
   {
   public:
      static unsigned int Minimum() { return 0;          }
      static unsigned int Maximum() { return 0xFFFFFFFF; }
      static unsigned int Zero()    { return 0;          }
      static unsigned int Identity()   { return 1;          }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef unsigned int ParameterUpcast;
      typedef unsigned int MathematicalUpcast;
      typedef float MathematicalFloatUpcast;
      typedef unsigned int ParameterType;
   };

//---------------------------------------------------------------------------

   template<> class TypeData<int64>
   {
   public:
      static signed long long Minimum()   { return 0x8000000000000000LL;   }
      static signed long long Maximum()   { return 0x7FFFFFFFFFFFFFFFLL;   }
      static signed long long Zero()      { return 0;                }
      static signed long long Identity()  { return 1;                }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef long long ParameterUpcast;
      typedef long long MathematicalUpcast;
      typedef double MathematicalFloatUpcast;
      typedef signed long long ParameterType;
   };

   template<> class TypeData<uint64>
   {
   public:
      static unsigned long long Minimum()    { return 0;                }
      static unsigned long long Maximum()    { return 0xFFFFFFFFFFFFFFFFULL;   }
      static unsigned long long Zero()    { return 0;                }
      static unsigned long long Identity()   { return 1;                }

      enum { isIntegral       = true };
      enum { isFloat          = false };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef unsigned long long ParameterUpcast;
      typedef unsigned long long MathematicalUpcast;
      typedef double MathematicalFloatUpcast;
      typedef unsigned long long ParameterType;
   };

//---------------------------------------------------------------------------

   template<> class TypeData<float>
   {
   public:
      static float Minimum()     { return -FLT_MAX;   }
      static float Maximum()     { return FLT_MAX; }
      static float Zero()        { return 0.0f;    }
      static float Identity()    { return 1.0f;    }

      enum { isIntegral       = false };
      enum { isFloat          = true };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef double ParameterUpcast;
      typedef float  MathematicalUpcast;
      typedef float  MathematicalFloatUpcast;
      typedef float  ParameterType;
   };

//---------------------------------------------------------------------------

   template<> class TypeData<double>
   {
   public:
      static double Minimum()       { return -DBL_MAX;   }
      static double Maximum()       { return DBL_MAX; }
      static double Zero()       { return 0.0;     }
      static double Identity()      { return 1.0;     }

      enum { isIntegral       = false };
      enum { isFloat          = true };
      enum { isPOD            = true };
      enum { isBitwiseCopySafe   = true };
      enum { isPointer        = false };

      typedef double ParameterUpcast;
      typedef double MathematicalUpcast;
      typedef double MathematicalFloatUpcast;
      typedef double ParameterType;
   };

//-----------------------------------------------------------------------------
} // namespace Webfoot
//-----------------------------------------------------------------------------

#endif //#ifndef __FROG__TYPEDATA_H__

#ifndef __FROG__FIXEDPOINT_H__
#define __FROG__FIXEDPOINT_H__

/***************************************************************************/

#include "FrogMemory.h"
#include "TypeData.h"
#include <stdlib.h>

/***************************************************************************/

namespace Webfoot
{
/***************************************************************************/

/// \addtogroup DataTypes
/// @{

/** \brief Template class for fixed point arithmetic.

	\param Fractional represents the number of bits used for fractional component
	\param T represents the container type.
	\code FixedPoint<10, unsigned short> p; // a 6.10 unsigned number. \endcode
	Care has to be taken when using multiply and divide that intermediate results do not exceed integer limits
*/
template<int Fractional, typename T = int, typename S = typename TypeData<T>::MathematicalUpcast> struct FixedPoint
{
public:
	T rawValue;

	enum { NumberOfFractionalBits = Fractional };

	inline FixedPoint()					throw()	{ }
	inline FixedPoint(int i)			throw()	{ rawValue = (T) (i << Fractional);			}
	inline FixedPoint(float f)			throw()	{ rawValue = (T) (f * (1 << Fractional));	}
	inline FixedPoint(T aValue, bool)	throw()	{ rawValue = aValue;						}

	template<bool EqualFractional, bool GreaterFractional, int f, typename T2, typename S2> 
		class _AssignHelper
	{
	public:
		static inline void Assign(FixedPoint& a, const FixedPoint<f, T2, S2>& b) throw();
	};
		
	template<bool b, int f, typename T2, typename S2> 
		class _AssignHelper<true, b, f, T2, S2>
	{
	public:
		static inline void Assign(FixedPoint& a, const FixedPoint<f, T2, S2>& b) throw()	{ a.rawValue = (T) b.rawValue;	}
	};

	template<int f, typename T2, typename S2> 
		class _AssignHelper<false, true, f, T2, S2>
	{
	public:
		static inline void Assign(FixedPoint& a, const FixedPoint<f, T2, S2>& b) throw()	{ a.rawValue = (T) (b.rawValue >> (f-Fractional));	}
	};

	template<int f, typename T2, typename S2> 
		class _AssignHelper<false, false, f, T2, S2>
	{
	public:
		static inline void Assign(FixedPoint& a, const FixedPoint<f, T2, S2>& b) throw()	{ a.rawValue = (T) (b.rawValue << (Fractional-f));	}
	};

	template<int f, typename T2, typename S2> 
		inline FixedPoint(const FixedPoint<f, T2, S2>& a) throw() { _AssignHelper<f == Fractional, (f > Fractional), f, T2, S2>::Assign(*this, a);	}

	template<int f, typename T2, typename S2> 
		void Set(const FixedPoint<f, T2, S2>& a) throw()	{ _AssignHelper<f == Fractional, (f > Fractional), f, T2, S2>::Assign(*this, a);	}

	friend const FixedPoint operator+(const FixedPoint &a, const FixedPoint &b) throw()	{ return FixedPoint(a.rawValue + b.rawValue, true);			}
	friend const FixedPoint operator-(const FixedPoint &a, const FixedPoint &b) throw()	{ return FixedPoint(a.rawValue - b.rawValue, true);			}

	template<int f, typename T2, typename S2> 
	friend inline FixedPoint<f+Fractional, S> operator*(const FixedPoint &a, const FixedPoint<f, T2, S2> &b)	{ return FixedPoint<f+Fractional, S>( (S) a.rawValue * (S2) b.rawValue, true); 	}

	friend FixedPoint operator/(const FixedPoint &a, const FixedPoint &b)	{ return FixedPoint((T) (((S) a.rawValue << Fractional)/b.rawValue), true);	}
	// Special case optimizations
	friend const FixedPoint operator*(int a, const FixedPoint &b)	throw()	{ return FixedPoint(a*b.rawValue, true);	}
	const FixedPoint operator*(int a) 			const				throw()	{ return FixedPoint(a*rawValue, true);		}
	const FixedPoint operator/(int a) 			const				throw()	{ return FixedPoint(rawValue / a, true);	}

	template<typename T2, typename S2>
	const FixedPoint<Fractional/2, T2, S2> operator/( const FixedPoint<Fractional/2, T2, S2>& a) const throw() { return FixedPoint<Fractional/2, T2, S2>((T2) (rawValue / a.rawValue), true); }

	// And functions to make sure that the special case optimizations don't get used when they shouldn't be.
	friend const FixedPoint operator*(float a, const FixedPoint &b) throw()	{ return FixedPoint(a)*b;			}
	const FixedPoint operator*(float a) 		const				 throw()	{ return *this * FixedPoint(a);	}
	const FixedPoint operator/(float a) 		const				 throw()	{ return *this / FixedPoint(a);	}

	inline FixedPoint	operator-()			const throw()				{ return FixedPoint((T) -rawValue, true);		}
	inline FixedPoint	operator+()			const throw()				{ return *this;											}
	inline FixedPoint	operator>>(int a)	const throw()				{ return FixedPoint(rawValue >> a, true);	} 
	inline FixedPoint	operator<<(int a)	const throw()				{ return FixedPoint(rawValue << a, true);	} 

	inline FixedPoint& operator+=(const FixedPoint &a)		throw()		{ rawValue += a.rawValue;										return *this;	}
	inline FixedPoint& operator-=(const FixedPoint &a)		throw()		{ rawValue -= a.rawValue;										return *this;	}
	inline FixedPoint& operator*=(int a)					throw()		{ rawValue *= a;												return *this;	}
	inline FixedPoint& operator*=(float f)					throw()		{ rawValue *= f;												return *this;	}
	inline FixedPoint& operator*=(const FixedPoint &a)		throw()		{ rawValue = (T) (((S) rawValue * a.rawValue) >> Fractional);	return *this;	}
	inline FixedPoint& operator/=(int a)					throw()		{ rawValue /= a;												return *this;	}
	inline FixedPoint& operator/=(float f)					throw()		{ rawValue /= f;												return *this;	}
	inline FixedPoint& operator/=(const FixedPoint &a)		throw()		{ rawValue = (T) (((S) rawValue << Fractional)/a.rawValue);		return *this;	}
	inline FixedPoint& operator>>=(int a)					throw()		{ rawValue >>= a;												return *this;	}
	inline FixedPoint& operator<<=(int a)					throw()		{ rawValue <<= a;												return *this;	}

	template<int f, typename T2, typename S2> 
	inline FixedPoint& operator=(const FixedPoint<f, T2, S2>& a) throw() { _AssignHelper<f == Fractional, (f > Fractional), f, T2, S2>::Assign(*this, a); return *this;	}

	inline FixedPoint& operator=(const FixedPoint &a)	throw()		{ rawValue = a.rawValue;					return *this;		}
	inline FixedPoint& operator=(float f)				throw()		{ rawValue = (T) (f * (1 << Fractional));	return *this;		}
	inline FixedPoint& operator=(int a)					throw()		{ rawValue = (T) (a << Fractional);			return *this;		}

	int			IntegralGet()	const				throw()		{ return rawValue >> Fractional;								}
	FixedPoint	FractionalGet()	const				throw()		{ return FixedPoint(rawValue & ~(-1 << Fractional), true);		}

	int			Floor()			const				throw()		{ return rawValue >> Fractional;								}
	int			Ceil()			const				throw()		{ return (rawValue + ((1 << Fractional) - 1)) >> Fractional;	}
	int			Round()			const				throw()		{ return (rawValue + (1 << (Fractional - 1))) >> Fractional;	}
	int			AsInteger()		const				throw()		{ return (int) (rawValue >> Fractional);						}
	float		AsFloat()		const				throw()		{ return rawValue * ( 1.0f / (1 << Fractional));				}
	double		AsDouble()		const				throw()		{ return rawValue * ( 1.0 / (1 << Fractional));					}

	// Comparison operators
	friend bool operator==(const FixedPoint &a, const FixedPoint &b) throw()	{ return a.rawValue == b.rawValue;		}
	friend bool operator!=(const FixedPoint &a, const FixedPoint &b) throw()	{ return a.rawValue != b.rawValue;		}
	friend bool operator<=(const FixedPoint &a, const FixedPoint &b) throw()	{ return a.rawValue <= b.rawValue;		}
	friend bool operator>=(const FixedPoint &a, const FixedPoint &b) throw()	{ return a.rawValue >= b.rawValue;		}
	friend bool operator<(const FixedPoint &a, const FixedPoint &b)	 throw()	{ return a.rawValue <  b.rawValue;		}
	friend bool operator>(const FixedPoint &a, const FixedPoint &b)	 throw()	{ return a.rawValue >  b.rawValue;		}
	
	friend FixedPoint abs(const FixedPoint &a) throw() 							{ return FixedPoint(::abs(a.rawValue), true); }
};

/***************************************************************************/

typedef FixedPoint<4> TextureCoordinateUnit;
typedef FixedPoint<6> PackedWorldUnit;
typedef FixedPoint<12, int, long long> WorldUnit;
typedef FixedPoint<12> NormalUnit;

/***************************************************************************/

template<int Fractional, typename T, typename S> class TypeData< FixedPoint<Fractional, T, S> >
{
public:
	static FixedPoint<Fractional, T, S> Minimum()			{ return FixedPoint<Fractional, T, S>(TypeData<T>::Minimum(), true); }
	static FixedPoint<Fractional, T, S> Maximum()			{ return FixedPoint<Fractional, T, S>(TypeData<T>::Maximum(), true); }

	// Allows other TypeDatas to override what "Zero" means (eg. for intervals)
	static FixedPoint<Fractional, T, S> Zero()				{ return FixedPoint<Fractional, T, S>(0); }

	static FixedPoint<Fractional, T, S> Identity()			{ return FixedPoint<Fractional, T, S>(1); }

	enum { isIntegral			= true  };
	enum { isFloat				= false };
	enum { isPOD				= true  };
	enum { isBitwiseCopySafe	= true  };
	enum { isPointer			= false };

	typedef FixedPoint<Fractional, T, S> MathematicalUpcast;
	typedef const FixedPoint<Fractional, T, S>& ParameterType;
};

/// @}
/***************************************************************************/
} // namespace Webfoot
/***************************************************************************/

#endif //#ifndef __FROG__FIXEDPOINT_H__
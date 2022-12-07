#ifndef __FROG__UTILITY_H__
#define __FROG__UTILITY_H__

#include "FrogMemory.h"
#include "Port.h"
#include "FrogString.h"
#include "UtilityC.h"
#include "Platform.h"

namespace Webfoot {

template<typename T> class Box2;
template<typename T> class Box3;
template<typename T> struct Point2;
template<typename T> struct Point3;
struct ColorRGBA8;
struct ColorRGBA32F;
class Quaternion;

class FileManager;

/// This macro evaluates to true in iOS and iOSPC builds.  It must not be used
/// in library code, since the IOSPC macro should only be set in iOSPC game
/// projects.  Only use this macro in game-specific code.
#define IOS_OR_IOSPC (PLATFORM_IS_IOS || (PLATFORM_IS_WINDOWS && IOSPC))

/// This macro evaluates to true in Android and AndroidPC builds.  It must not be used
/// in library code, since the ANDROIDPC macro should only be set in AndroidPC game
/// projects.  Only use this macro in game-specific code.
#define ANDROID_OR_ANDROIDPC (PLATFORM_IS_ANDROID || (PLATFORM_IS_WINDOWS && ANDROIDPC))

/// This macro evaluates to true in 3DS and 3DSPC builds.  It must not be used
/// in library code, since the 3DSPC macro should only be set in 3DSPC game
/// projects.  Only use this macro in game-specific code.
#define CTR_OR_CTRPC (PLATFORM_IS_3DS || (PLATFORM_IS_WINDOWS && CTRPC))

/// This macro evaluates to true in Xbox360 and Xbox360PC builds.  It must not
/// be used in library code, since the XBOX360PC macro should only be set in
/// Xbox360PC game projects.  Only use this macro in game-specific code.
#define XBOX360_OR_XBOX360PC (PLATFORM_IS_XBOX360 || (PLATFORM_IS_WINDOWS && XBOX360PC))

/// This macro evaluates to true in PS3 and PS3PC builds.  It must not be used
/// in library code, since the PS3PC macro should only be set in PS3PC game
/// projects.  Only use this macro in game-specific code.
#define PS3_OR_PS3PC (PLATFORM_IS_PS3 || (PLATFORM_IS_WINDOWS && PS3PC))

/// The number of bits in a byte.  (Just for the sake of avoiding a magic number)
#define BITS_PER_BYTE 8

/// Maximum number of bytes in a UTF-8 character.
#define UTF8_CHARACTER_SIZE_MAX 4
/// Maximum number of bytes in a UTF-16 character.  Note this is in bytes, not shorts.
#define UTF16_CHARACTER_SIZE_MAX 4
/// Maximum number of bytes in a Modified UTF-8 character.
#define MODIFIED_UTF8_CHARACTER_SIZE_MAX 6

/// Event triggered on some platforms when the game loses the OS focus.
/// Whether the game initially has the focus depends on the platform and sitiation.
#define ON_OS_FOCUS_LOSE_EVENT_NAME "OnOSFocusLose"
/// Event triggered on some platforms when the game gains the OS focus.
/// Whether the game initially has the focus depends on the platform and sitiation.
#define ON_OS_FOCUS_GAIN_EVENT_NAME "OnOSFocusGain"
/// Event triggered on some platforms when the resolution changes or when the
/// game switches between fullscreen and windowed mode.
#define ON_WINDOW_CHANGE_EVENT_NAME "OnWindowChange"
/// Event triggered on some platforms when a change is made to the setting for
/// how and when the system cursor position should be locked.
#define ON_SYSTEM_CURSOR_POSITION_LOCK_CHANGE "OnSystemCursorPositionLockChange"

//===============================================================================

// Alignment values that are used for various purposes
enum Alignment
{
   ALIGNMENT_INVALID = -1,
   /// The left side of the sprite will be on the given x.
   ALIGN_LEFT = 0,
   /// The sprite will be centered horizontally on the given x.
   ALIGN_CENTER_X = 1,
   /// The right side of the sprite will be on the given x.
   ALIGN_RIGHT = 2,
   /// The top of the sprite will be on the given y.
   ALIGN_TOP = 0,
   /// The sprite will be centered vertically on the given y.
   ALIGN_CENTER_Y = 4,
   /// The bottom of the sprite will be on the given y.
   ALIGN_BOTTOM = 8
};

/// Shared enumeration of cardinal and diagonal directions.
enum Direction
{
   /// No direction specified
   DIRECTION_NONE = -1,
   /// Up
   DIRECTION_UP,
   /// Down
   DIRECTION_DOWN,
   /// Left
   DIRECTION_LEFT,
   /// Right
   DIRECTION_RIGHT,
   /// Number of cardinal directions
   DIRECTION_CARDINAL_COUNT,
   /// Up-Right
   DIRECTION_UP_RIGHT = DIRECTION_CARDINAL_COUNT,
   /// Down-Right
   DIRECTION_DOWN_RIGHT,
   /// Down-Left
   DIRECTION_DOWN_LEFT,
   /// Up-Left
   DIRECTION_UP_LEFT,
   /// Number of directions, including cardinals and diagonals
   DIRECTION_COUNT
};

/// Common ways to interpret a string.
enum TextType
{
   /// Display text as-is
   TEXT_PLAIN,
   /// Use string as a key with theText
   TEXT_KEY,
   /// Use string as a Lua expression.
   TEXT_LUA_EXPRESSION,
   /// Use string as the filename of a text file.
   TEXT_FILENAME
};

/// Return the alignment for the given name.  Return ALIGNMENT_INVALID if
/// unsuccessful.
int AlignmentStringToCode(const char* alignmentName);
/// Return a string representation of the given alignment.  Return NULL if
/// unsuccessful.
const char* AlignmentCodeToString(int alignment);

/// Return the direction for the given name.  Return DIRECTION_NONE if
/// unsuccessful.
int DirectionStringToCode(const char* directionName);
/// Return a string representation of the given direction.  Return NULL if
/// unsuccessful.
const char* DirectionCodeToString(int direction);
/// Return the direction opposite the given direction.
Direction DirectionOppositeGet(Direction direction);

//===============================================================================

template<typename T> inline const T& Minimum(const T &a, const T &b) { return (a < b) ? a : b; }
template<typename T> inline const T& Maximum(const T &a, const T &b) { return (a > b) ? a : b; }

//===============================================================================

template<typename T> inline void Swap(T &a, T &b)				
{ 
	T c(a); 
	a = b; 
	b = c;	  
}

template<typename T> inline void SwapByCopy(T &a, T &b)
{
	char Temp[sizeof(T)];
	memcpy(Temp, &a, sizeof(T));
	memcpy(&a, &b, sizeof(T));
	memcpy(&b, Temp, sizeof(T));
}

//===============================================================================

// The FROG_SWAP functions assume that the underlying number is little-endian.
// If the current platform is big-endian, they will perform byte swapping.
// The FROG_FORCE_SWAP functions always perform byte swapping.  This helps
// little-endian tools produce big-endian output.

// The FROG_SWAP_LE functions convert a native number to little endian and
// convert little endian numbers to native numbers.
// The FROG_SWAP_BE functions convert a native number to big endian and convert
// big endian numbers to native numbers.

#if FROG_BIG_ENDIAN
   #define FROG_SWAP_BE_UINT16(x) (x)
   #define FROG_SWAP_BE_INT16(x) (x)
   #define FROG_SWAP_BE_UINT32(x) (x)
   #define FROG_SWAP_BE_INT32(x) (x)
   #define FROG_SWAP_BE_UINT64(x) (x)
   #define FROG_SWAP_BE_INT64(x) (x)
   #define FROG_SWAP_BE_FLOAT32(x) (x)
   #define FROG_SWAP_BE_FLOAT64(x) (x)
   
   #define FROG_SWAP_LE_UINT16(x) FROG_FORCE_SWAP_UINT16(x)
   #define FROG_SWAP_LE_INT16(x) FROG_FORCE_SWAP_INT16(x)
   #define FROG_SWAP_LE_UINT32(x) FROG_FORCE_SWAP_UINT32(x)
   #define FROG_SWAP_LE_INT32(x) FROG_FORCE_SWAP_INT32(x)
   #define FROG_SWAP_LE_UINT64(x) FROG_FORCE_SWAP_UINT64(x)
   #define FROG_SWAP_LE_INT64(x) FROG_FORCE_SWAP_INT64(x)
   #define FROG_SWAP_LE_FLOAT32(x) FROG_FORCE_SWAP_FLOAT32(x)
   #define FROG_SWAP_LE_FLOAT64(x) FROG_FORCE_SWAP_FLOAT64(x)
#else
   #define FROG_SWAP_BE_UINT16(x) FROG_FORCE_SWAP_UINT16(x)
   #define FROG_SWAP_BE_INT16(x) FROG_FORCE_SWAP_INT16(x)
   #define FROG_SWAP_BE_UINT32(x) FROG_FORCE_SWAP_UINT32(x)
   #define FROG_SWAP_BE_INT32(x) FROG_FORCE_SWAP_INT32(x)
   #define FROG_SWAP_BE_UINT64(x) FROG_FORCE_SWAP_UINT64(x)
   #define FROG_SWAP_BE_INT64(x) FROG_FORCE_SWAP_INT64(x)
   #define FROG_SWAP_BE_FLOAT32(x) FROG_FORCE_SWAP_FLOAT32(x)
   #define FROG_SWAP_BE_FLOAT64(x) FROG_FORCE_SWAP_FLOAT64(x)
   
   #define FROG_SWAP_LE_UINT16(x) (x)
   #define FROG_SWAP_LE_INT16(x) (x)
   #define FROG_SWAP_LE_UINT32(x) (x)
   #define FROG_SWAP_LE_INT32(x) (x)
   #define FROG_SWAP_LE_UINT64(x) (x)
   #define FROG_SWAP_LE_INT64(x) (x)
   #define FROG_SWAP_LE_FLOAT32(x) (x)
   #define FROG_SWAP_LE_FLOAT64(x) (x)
#endif

#define FROG_SWAP_UINT16(x) FROG_SWAP_LE_UINT16(x)
#define FROG_SWAP_INT16(x) FROG_SWAP_LE_INT16(x)
#define FROG_SWAP_UINT32(x) FROG_SWAP_LE_UINT32(x)
#define FROG_SWAP_INT32(x) FROG_SWAP_LE_INT32(x)
#define FROG_SWAP_UINT64(x) FROG_SWAP_LE_UINT64(x)
#define FROG_SWAP_INT64(x) FROG_SWAP_LE_INT64(x)
#define FROG_SWAP_FLOAT32(x) FROG_SWAP_LE_FLOAT32(x)
#define FROG_SWAP_FLOAT64(x) FROG_SWAP_LE_FLOAT64(x)

#define FROG_FORCE_SWAP_UINT16(x) (uint16) (         \
          (((uint16)x) << 8)    |    \
          (((uint16)x) >> 8)         \
       )

#define FROG_FORCE_SWAP_INT16(x)  (int16) (         \
          (((uint16)x) << 8)    |    \
          (((uint16)x) >> 8)         \
       )

#define FROG_FORCE_SWAP_UINT32(x)  (uint32)            \
      (                               \
        ( x << 24)                 |  \
        ( ((uint32) x) >> 24)      |  \
        ( (x & 0x0000FF00) << 8 )  |  \
        ( (x & 0x00FF0000) >> 8 )     \
      )

#define FROG_FORCE_SWAP_INT32(x)  (int32)            \
      (                               \
        ( x << 24)                 |  \
        ( ((uint32) x) >> 24)      |  \
        ( (x & 0x0000FF00) << 8 )  |  \
        ( (x & 0x00FF0000) >> 8 )     \
      )

#define FROG_FORCE_SWAP_UINT64(x) (uint64) \
   ( \
      ((x & 0x00000000000000FF) << (7*8)) | \
      ((x & 0x000000000000FF00) << (5*8)) | \
      ((x & 0x0000000000FF0000) << (3*8)) | \
      ((x & 0x00000000FF000000) << (1*8)) | \
      ((x & 0x000000FF00000000) >> (1*8)) | \
      ((x & 0x0000FF0000000000) >> (3*8)) | \
      ((x & 0x00FF000000000000) >> (5*8)) | \
      (((uint64)x & 0xFF00000000000000) >> (7*8)) \
   )
   
#define FROG_FORCE_SWAP_INT64(x) ((int64)(FROG_FORCE_SWAP_UINT64(x)))

inline float32 FROG_FORCE_SWAP_FLOAT32(float32 x)
{
   float32 result;
   *(reinterpret_cast<uint32*>(&result)) = FROG_FORCE_SWAP_UINT32(*(reinterpret_cast<uint32*>(&x)));
   return result;
}

inline float64 FROG_FORCE_SWAP_FLOAT64(float64 x)
{
   float64 result;
   *(reinterpret_cast<uint64*>(&result)) = FROG_FORCE_SWAP_UINT64(*(reinterpret_cast<uint64*>(&x)));
   return result;
}

/// Build a uint16 from two bytes.
#define BUILD_UINT16(lowByte, highByte) (((uint16)lowByte & 0xFF) | (((uint16)highByte & 0xFF) << 8))

//===============================================================================

class CPUMemoryMethods
{
public:
	static void Set16(void *destination, int16 value, size_t numberOfBytes) 
	{
		int16* d = reinterpret_cast<int16*>(destination);
		for(size_t i = 0; i < numberOfBytes; i += 2) *d++ = value;
	}
	static void Set32(void *destination, int32 value, size_t numberOfBytes)
	{
		int32* d = reinterpret_cast<int32*>(destination);
		for(size_t i = 0; i < numberOfBytes; i += 4) *d++ = value;
	}
	static void Clear16(void *destination, size_t numberOfBytes) { Set16(destination, 0, numberOfBytes); }
	static void Clear32(void *destination, size_t numberOfBytes) { Set32(destination, 0, numberOfBytes); }
	static void Copy16(void *destination, const void *source, size_t numberOfBytes) { memcpy(destination, source, numberOfBytes); }
	static void Copy32(void *destination, const void *source, size_t numberOfBytes) { memcpy(destination, source, numberOfBytes); }
	//static void CopyBackwards16(void *destination, const void *source, size_t numberOfBytes);
	//static void CopyBackwards32(void *destination, const void *source, size_t numberOfBytes);
};

static CPUMemoryMethods *const cpu = NULL;

//===============================================================================

/// Return a new copy of the given string.  Delete it with StringDelete.
char* StringClone(const char* string, HeapID heapId = HEAP_DEFAULT);

/// Return a new, null-terminated copy of the given non-null-terminated string.
/// 'stringLength' is the number of characters in 'string' (There is no null-
/// terminator).  Delete it with StringDelete.
char* StringClone(const char* string, size_t stringLength, HeapID heapId = HEAP_DEFAULT);

/// If the given string is defined, delete it and NULL the pointer.
void StringDelete(const char*& string);
/// If the given string is defined, delete it and NULL the pointer.
inline void StringDelete(char*& string) { StringDelete((const char*&)string); }
/// If the given string is defined, zero the text up to the null-terminator,
/// delete it, and NULL the pointer.
void StringZeroDelete(char*& string);

/// If the given string is defined, delete it and NULL the pointer.
void StringDelete(const short*& string);
/// If the given string is defined, delete it and NULL the pointer.
inline void StringDelete(short*& string) { StringDelete((const short*&)string); }

/// A comparison function for use with Map that returns true if the first string
/// should be sorted lower than the second.
bool StringComparator(const char* const& keyA, const char* const& keyB);

/// A hash function for use with HashTable that returns a hash of the given
/// string.
size_t StringHash(const char* const& str);
/// A function for use with HashTable that returns true if two strings are
/// equivalent.
bool StringsEqualCheck(const char* const& a, const char* const& b);

/// Given a rectangle encoded in a string, extract that information and
/// put it in the given Box2F.  Return true if successful.
/// The format of the string must be "x|y|width|height" where all values are floats.
bool Box2FExtract(const char* string, Box2<float>* box);
/// Print a string representation of the given Box2F to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool Box2FPrint(char* buffer, size_t bufferSize, const Box2<float>& box);

/// Given a rectangle encoded in a string, extract that information and
/// put it in the given Box3F.  Return true if successful.
/// The format of the string must be "x|y|width|height" where all values are floats.
bool Box3FExtract(const char* string, Box3<float>* box);
/// Print a string representation of the given Box3F to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool Box3FPrint(char* buffer, size_t bufferSize, const Box3<float>& box);

/// Given a point encoded in a string, extract that information and
/// put it in the given Point2I.  Return true if successful.
/// The format of the string must be "x|y" where both values are ints.
bool Point2IExtract(const char* string, Point2<int>* point);
/// Print a string representation of the given Point2I to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool Point2IPrint(char* buffer, size_t bufferSize, const Point2<int>& point);

/// Given a point encoded in a string, extract that information and
/// put it in the given Point2F.  Return true if successful.
/// The format of the string must be "x|y" where both values are floats.
bool Point2FExtract(const char* string, Point2<float>* point);
/// Print a string representation of the given Point2F to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool Point2FPrint(char* buffer, size_t bufferSize, const Point2<float>& point);

/// Given a point encoded in a string, extract that information and
/// put it in the given Point3F.  Return true if successful.
/// The format of the string must be "x|y|z" where all values are floats.
bool Point3FExtract(const char* string, Point3<float>* point);
/// Print a string representation of the given Point3F to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool Point3FPrint(char* buffer, size_t bufferSize, const Point3<float>& point);

/// Given a color encoded in a string, extract that information and
/// put it in the given ColorRGBA8.  Return true if successful.
/// The format is "red|green|blue|alpha" where each is an integer
/// between 0 and 255 (inclusive).
bool ColorRGBA8Extract(const char* string, ColorRGBA8* color);
/// Print a string representation of the given ColorRGBA8 to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool ColorRGBA8Print(char* buffer, size_t bufferSize, const ColorRGBA8& color);

/// Given a color encoded in a string, extract that information and
/// put it in the given ColorRGBA32F.  Return true if successful.
/// The format is "red|green|blue|alpha" where each is a floating point number.
bool ColorRGBA32FExtract(const char* string, ColorRGBA32F* color);
/// Print a string representation of the given ColorRGBA32F to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool ColorRGBA32FPrint(char* buffer, size_t bufferSize, const ColorRGBA32F& color);

/// Given a quaternion encoded in a string, extract that information and
/// put it in the given Quaternion.  Return true if successful.
/// The format is "w|x|y|z" where all values are floats.
bool QuaternionExtract(const char* string, Quaternion* quaternion);
/// Print a string representation of the given Quaternion to the given buffer.
/// 'bufferSize' is the size of 'buffer', including the space for the null
/// terminator.  Return true if successful.
bool QuaternionPrint(char* buffer, size_t bufferSize, const Quaternion& quaternion);

/// Return true if the given string is a valid email address.  This is not an
/// exhuastive list of tests, and unusual or international email addresses may be
/// rejected.
bool EmailAddressValidCheck(const char* address);

//===============================================================================

// Some simple CRC tools - ideal for text handles
struct CRC
{
	static uint32 MakeCrc(const char *text, size_t size);

	// Build a CRC either from text or from a value
	template<class _T> uint32 Val(_T val) { return MakeCrc( (char *)&val, sizeof(_T) ); }
	static uint32 Str(const char * text);
};

//===============================================================================

/// Convert the given NULL-terminated string to upper-case.  Note that this does
/// not check for buffer overruns, and it only works for the first 128 character
/// codes from ASCII.
char* FrogStrupr(char* str);

/// To avoid unecessary complications in LUA we replace characters which could
/// cause problem by the universal unsuported sign '?'.  Be sure to provide
/// a UTF-8 string.
void ReplaceDangerousCharactersUTF8(unsigned char *stringcheck);

// Unless otherwise noted, UTF-16 here means either UTF-16LE or UTF-16BE,
// depending on the local endian-ness.

/// Decode the UTF-8 character at 'src' and store its unicode value in 'dest'.
/// Return the size of the character in bytes if successful.
/// Return -1 if unsuccessful.
int UTF8CharacterToUnicode(uint32* dest, const char* src);
/// Decode the UTF-16 (native endian) character at 'src' and store its unicode
/// value in 'dest'.  Return the size of the character in bytes if successful.
/// Note this is in bytes, not in shorts.  Return -1 if unsuccessful.
int UTF16CharacterToUnicode(uint32* dest, const short* src);
/// Convert the given unicode character 'src' to UTF-8, and store it in 'dest'.
/// 'destSizeBytes' is the size of the 'dest' buffer in bytes.  Return the
/// number of bytes written if successful.  Return -1 if unsuccessful.
int UnicodeCharacterToUTF8(char* dest, uint32 src, size_t destSizeBytes);
/// Convert the given unicode character 'src' to UTF-16 (native endian), and store it
/// in 'dest'. 'destSizeBytes' is the size of the 'dest' buffer in bytes, not
/// shorts.  Return the number of bytes written if successful.  Note this is in
/// bytes, not in shorts.  Return -1 if unsuccessful.
int UnicodeCharacterToUTF16(short* dest, uint32 src, size_t destSizeBytes);
/// Convert the given UTF-8 string 'src' to UTF-16 (native endian) and save it
/// in 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Note this is in bytes, not in shorts.
/// Return true if successful.  The UTF-16 byte order mark is not included.
bool UTF8ToUTF16(short* dest, const char* src, size_t destSizeBytes);
/// Convert the given UTF-16 (native endian) string 'src' to UTF-8 and save it
/// in 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Return true if successful.
/// The UTF-16 byte order mark is assumed to be absent.
bool UTF16ToUTF8(char* dest, const short* src, size_t destSizeBytes);
/// Return the number of bytes needed to encode the given UTF-8 string 'str' in UTF-16
/// (native endian), including the space for the null-terminator.  Note this
/// is in bytes, not in shorts.  Return -1 if unsuccessful.
int UTF8ToUTF16Size(const char* str);
/// Return the number of bytes needed to encode the given UTF-16 (native endian)
/// string 'str' in UTF-8, including the space for the null-terminator.
/// Return -1 if unsuccessful.
int UTF16ToUTF8Size(const short* str);
/// Return the number of UTF8 characters in the given null-terminated UTF-8
/// string.  Return -1 if unsuccessful.
int UTF8CharacterCount(const char* str);
/// Given the index of a character in a given UTF-8 string, return the number
/// of bytes between the beginning of the string and the beginning of that
/// character.  Return -1 if unsucessful.
int UTF8CharacterByteIndexGet(const char* str, int characterIndex);
/// Return a new copy of the given UTF-8 string encoded as a native UTF-16
/// string.  Delete it with StringDelete.
short* UTF8ToUTF16StringClone(const char* string, HeapID heapId = HEAP_DEFAULT);
/// Return a new copy of the given native UTF-16 string encoded as a UTF-8
/// string.  Delete it with StringDelete.
char* UTF16ToUTF8StringClone(const short* string, HeapID heapId = HEAP_DEFAULT);

//===============================================================================

/// Decode the Modified UTF-8 character at 'src' and store its unicode value in
/// 'dest'.  Return the size of the character in bytes if successful.  Modified
/// UTF-8 encoding of unicode values over 0xFFFF is ambiguous, and this
/// implementation assumes only 1, 2, or 3 byte encodings are used. 
/// Return -1 if unsuccessful.
int ModifiedUTF8CharacterToUnicode(uint32* dest, const char* src);
/// Convert the given unicode character 'src' to Modified UTF-8, and store it in
/// 'dest'.  'destSizeBytes' is the size of the 'dest' buffer in bytes.  Return
/// the number of bytes written if successful.  Return -1 if unsuccessful.
int UnicodeCharacterToModifiedUTF8(char* dest, uint32 src, size_t destSizeBytes);

/// Convert the given UTF-8 string 'src' to Modified UTF-8, and save it in
/// 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Rather than looking for a null-terminator in
/// 'src', or even assuming one is present, 'srcLengthBytes' will be used as the
/// expected length.  Any '\0' characters encountered in that span will be
/// encoded using Modified UTF-8's two-byte encoding.  'dest' should still be
/// null-terminated in the usual C way.  Return true if successful.
bool UTF8ToModifiedUTF8(char* dest, const char* src, size_t destSizeBytes, size_t srcLengthBytes);
/// Same as the other form of UTF8ToModifiedUTF8, except that this assumes 'src'
/// is null-terminated.  This means the resulting string should not contain any
/// of Modified UTF-8's two-byte zeros.
inline bool UTF8ToModifiedUTF8(char* dest, const char* src, size_t destSizeBytes) { return UTF8ToModifiedUTF8(dest, src, destSizeBytes, strlen(src)); }

/// Convert the given Modified UTF-8 string 'src' to UTF-8, and save it in
/// 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Rather than looking for a null-terminator in
/// 'src', or even assuming one is present, 'srcLengthBytes' will be used as the
/// expected length.  Any '\0' characters encountered in that span will be
/// treated as corrupted.  Modified UTF-8 encoding of unicode values over
/// 0xFFFF is ambiguous, and this implementation assumes only 1, 2, or 3 byte
/// encodings are used.  Return true if successful.
bool ModifiedUTF8ToUTF8(char* dest, const char* src, size_t destSizeBytes, size_t srcLengthBytes);
/// Same as the other form of ModifiedUTF8ToUTF8, except that this assumes 'src'
/// is null-terminated.
inline bool ModifiedUTF8ToUTF8(char* dest, const char* src, size_t destSizeBytes) { return ModifiedUTF8ToUTF8(dest, src, destSizeBytes, strlen(src)); }

/// Return the number of bytes needed to encode the given UTF-8 string 'str' in
/// Modified UTF-8, including the space for the null-terminator.  Rather than
/// assuming 'str' is null-terminated, use the given length for the string in
/// bytes.  Any '\0' characters encountered in that span will be assumed to be
/// translated to Modified UTF-8's two-byte encoding.  Return -1 if unsuccessful.
int UTF8ToModifiedUTF8Size(const char* str, size_t strLength);
/// Same as the other form of UTF8ToModifiedUTF8Size, except this assumes 'str'
/// is null-terminated.
inline int UTF8ToModifiedUTF8Size(const char* str) { return UTF8ToModifiedUTF8Size(str, strlen(str)); }

/// Return the number of bytes needed to encode the given Modified UTF-8 string
/// 'str' in UTF-8, including the space for the null-terminator.  Rather than
/// assuming 'str' is null-terminated, use the given length for the string in
/// bytes.  Any '\0' characters encountered in that span will be treated as
/// corrupted.  Modified UTF-8 encoding of unicode values over 0xFFFF is
/// ambiguous, and this implementation assumes only 1, 2, or 3 byte encodings
/// are used.  Return -1 if unsuccessful.
int ModifiedUTF8ToUTF8Size(const char* str, size_t strLength);
/// Same as the other form of ModifiedUTF8ToUTF8Size, except this assumes 'str'
/// is null-terminated.
inline int ModifiedUTF8ToUTF8Size(const char* str) { return ModifiedUTF8ToUTF8Size(str, strlen(str)); }

/// Return a new copy of the given UTF-8 string encoded as a modified UTF-8
/// string.  Delete it with StringDelete.
char* UTF8ToModifiedUTF8StringClone(const char* string, HeapID heapId = HEAP_DEFAULT);
/// Return a new copy of the given modified UTF-8 string encoded as a UTF-8
/// string.  Delete it with StringDelete.
char* ModifiedUTF8ToUTF8StringClone(const char* string, HeapID heapId = HEAP_DEFAULT);

//===============================================================================

/// Return true if all characters before the null terminator are valid UTF-8.
bool UTF8Check(const char* str);
/// Truncate the given UTF-8 string as-needed so that it contains no invalid
/// UTF-8 characters and is no longer than the given size, including the null terminator.
/// Return true if the string was already fine and no truncation was needed.
/// False will be returned if there was a problem with the parameters of if
/// the text was truncated.
bool UTF8Truncate(char* text, size_t bufferSize);

/// Warning: This does not behave exactly like the normal snprintf.
/// This behaves exactly like FrogSnprintf, except it will truncate
/// the string further if needed to avoid any invalid UTF-8 characters.
bool UTF8Snprintf(char* buffer, size_t bufferSize, const char* format, ...);
/// Warning: This does not behave exactly like the normal vsnprintf.
/// This behaves exactly like FrogVsnprintf, except it will truncate
/// the string further if needed to avoid any invalid UTF-8 characters.
bool UTF8Vsnprintf(char* buffer, size_t bufferSize, const char* format, va_list args);
/// Warning: This does not behave exactly like the normal strncpy.
/// This behaves exactly like FrogStrncpy, except it will truncate
/// the string further if needed to avoid any invalid UTF-8 characters.
bool UTF8Strncpy(char* destination, const char* source, size_t destinationSize);
/// Warning: This does not behave exactly like the normal strncat.
/// This behaves exactly like FrogStrncat, except it will truncate
/// the string further if needed to avoid any invalid UTF-8 characters.
bool UTF8Strncat(char* destination, const char* source, size_t destinationSize);

/// Warning: This behaves differently from the normal strtok.
/// Find the next item in the given list delimitted by one of the 'delimiters'
/// characters.  On each iteration, skip any leading characters from the 'delimiters' list.
/// For the first call in the traversal, 'nextListPosition' should
/// point to the beginning of the string to be tokenized.  For subsequent
/// calls, use the pointer returned by the previous call.  If a token is found,
/// it will be copied to 'token', taking care not to overflow it.
/// 'tokenBufferSize' is the size of the 'token' buffer including the space for
/// the null terminator.  Return NULL if the end of the list has been reached.
/// This does not actually modify the list to be tokenized.  This is not
/// internally stateful, so you can have traversals on multiple
/// threads at once.  For now, the characters in the 'delimiters' string must
/// be single-byte characters.  If 'success' is specified, it will be set to
/// false if truncation or some other problem occurs, and true otherwise.
const char* UTF8Strtok(const char* nextListPosition, const char* delimiters,
   char* token, size_t tokenBufferSize, bool* success);

/// Create and return a string using printf-style formatting.  Allocate the
/// memory for the string from the specified heap.  Delete it with
/// StringDelete.  Return NULL if unsuccessful.  Return NULL if invalid UTF8
/// characters are found in the output.  Beware that this can be significantly
/// slower than UTF8Snprintf.  Since it allocates memory, it can also lead to
/// fragmentation.  Due to a portability issue, there is currently no
/// UTF8Vasprintf.
char* UTF8Asprintf(HeapID heapID, const char* format, ...);

/// Create and return a string that is the result of concatenating the
/// specified strings.  Allocate the memory for the string from the specified
/// heap.  Delete it with StringDelete.  Return NULL if unsuccessful.  Return
/// NULL if invalid UTF8 characters are found in the output.  Beware that this
/// is slower than UTF8Strncpy, but it is potentially much faster than
/// UTF8Asnprintf.  Since it allocates memory, it can also lead to
/// fragmentation.  Due to a portability issue, there is currently no
/// UTF8Vastrcat.
char* UTF8Astrcat(HeapID heapID, int stringCount, ...);

/// Remove leading and trailing whitespace from the given UTF-8 string.  Return
/// true if successful.
bool UTF8WhitespaceTrim(char* str);

//===============================================================================

/// Return the number of bytes needed to encode the given amount of binary data
/// as a UTF-8 hex string, including the space for the null-terminator.  Return
/// -1 if unsuccessful.
int BinaryToUTF8HexStringSize(size_t srcSizeBytes);
/// Convert the given binary data to a UTF-8 hex string, and save it
/// in 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Return true if successful.
bool BinaryToUTF8HexString(char* dest, const void* src, size_t srcSizeBytes, size_t destSizeBytes);
/// Return the number of bytes needed to decode the given UTF-8 hex string to
/// binary.  Return -1 if unsuccessful.
int UTF8HexStringToBinarySize(const char* src);
/// Convert the given UTF-8 hex string to binary, and save it in dest.
/// 'destSizeBytes' is the size of 'dest' in bytes.  Return true if successful.
bool UTF8HexStringToBinary(void* dest, const char* src, size_t destSizeBytes);

//===============================================================================

/// Return the number of bytes needed to encode the given amount of binary data
/// as a UTF-8 RFC 4648 string, including the space for the null-terminator.
/// Return -1 if unsuccessful.
int BinaryToUTF8Base32StringSize(size_t srcSizeBytes);
/// Convert the given binary data to a UTF-8 RFC 4648 Base32 string, and save
/// it in 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Return true if successful.
bool BinaryToUTF8Base32String(char* dest, const void* src, size_t srcSizeBytes, size_t destSizeBytes);
/// Return the number of bytes needed to decode the given UTF-8 RFC 4648 Base32
/// string to binary.  Return -1 if unsuccessful.
int UTF8Base32StringToBinarySize(const char* src);
/// Convert the given UTF-8 RFC 4648 Base32 string to binary, and save it in dest.
/// 'destSizeBytes' is the size of 'dest' in bytes.  Return true if successful.
bool UTF8Base32StringToBinary(void* dest, const char* src, size_t destSizeBytes);

//===============================================================================

/// Return the number of bytes needed to encode the given amount of binary data
/// as a UTF-8 RFC 4648 Base64 string, including the space for the
/// null-terminator.  Return -1 if unsuccessful.
int BinaryToUTF8Base64StringSize(size_t srcSizeBytes);
/// Convert the given binary data to a UTF-8 RFC 4648 Base64 string, and save
/// it in 'dest'.  'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Return true if successful.
bool BinaryToUTF8Base64String(char* dest, const void* src, size_t srcSizeBytes, size_t destSizeBytes);
/// Return the number of bytes needed to decode the given UTF-8 RFC 4648 Base64
/// string to binary.  Return -1 if unsuccessful.
int UTF8Base64StringToBinarySize(const char* src);
/// Convert the given UTF-8 RFC 4648 Base64 string to binary, and save it in
/// dest.  'destSizeBytes' is the size of 'dest' in bytes.  Return true if
/// successful.
bool UTF8Base64StringToBinary(void* dest, const char* src, size_t destSizeBytes);

//===============================================================================

/// Return the number of bytes needed to URL-encode the given UTF-8 string,
/// including the space for the null terminator.  Return -1 if unsuccessful.
int UTF8URLEncodeSize(const char* src);
/// URL-encode the given UTF-8 string, and save it in dest.  'destSizeBytes' is
/// the size of 'dest' in bytes, including the space for the null-terminator.
/// Return true if successful.
bool UTF8URLEncode(char* dest, const char* src, size_t destSizeBytes);
/// Return the number of bytes needed to URL-decode the given URL-encoded UTF-8
/// string, including the space for the null-terminator.  Return -1 if
/// unsuccessful.
int UTF8URLDecodeSize(const char* src);
/// Decode the given URL-encoded UTF-8 string, and save it in 'dest'.
/// 'destSizeBytes' is the size of 'dest' in bytes, including the
/// space for the null-terminator.  Return true if successful.
bool UTF8URLDecode(char* dest, const char* src, size_t destSizeBytes);

//===============================================================================

/// Structure for pairing a string and its CRC.
/// This does not make an internal copy of the string, so be careful.
struct StringHashPair
{
   StringHashPair() { hash = 0; string = NULL; }
   StringHashPair(const char* _string) { Set(_string); }
   
   /// Set the object to use the given string.
   /// This will not make a copy of the string, but it will store the pointer
   /// and calculate the CRC.
   void Set(const char* _string);
   /// Comparator for sorting.
   bool operator< (const StringHashPair& otherKey) const;
   /// Comparator for sorting.
   static bool Comparator(const StringHashPair& a, const StringHashPair& b);

   /// CRC of the string in question.
   unsigned int hash;
   /// The string in question
   const char* string;
};

//===============================================================================

/// Structure that holds a filename, a hash of the filename, and a pointer
/// to the associated FileManager.
/// This does not make an internal copy of the filename, so be careful.
struct FileKey
{
   FileKey() { hash = 0; filename = NULL; fileManager = 0; }
   FileKey(const char* _filename, FileManager* _fileManager) { Set(_filename, _fileManager); }
   
   /// Set the object to use the given filename.
   /// This will not make a copy of the filename, but it will store the pointer
   /// and calculate the CRC.
   void Set(const char* _filename, FileManager* _fileManager);
   /// Comparator for sorting.
   bool operator< (const FileKey& otherKey) const;

   /// Return the filename associated with this key.
   const char* FilenameGet() const { return filename; }
   /// Return the FileManager associated with this key.
   FileManager* FileManagerGet() const { return (FileManager*)fileManager; }

   /// CRC of the filename
   unsigned int hash;
   /// String for the filename
   const char* filename;
   /// FileManager associated with the file.
   size_t fileManager;
};

//===============================================================================

/// Convert the given number of bits to bytes and round up.
template<typename T>
T BitsToBytes(const T bitCount)
{
   T byteCount = bitCount >> 3;
   if(bitCount % BITS_PER_BYTE)
      byteCount++;
   return byteCount;
}

//------------------------------------------------------------------------------

/// Convert the given number of bytes to bits
template<typename T>
T BytesToBits(const T value) { return value << 3; }

//------------------------------------------------------------------------------

/// Return true if the number is a power of 2.
template<typename T>
bool PowerOf2Check(const T value) { return (!((value)&((value)-1))); }

//------------------------------------------------------------------------------

/// Return the given value with only its lowest bit set.
template<typename T>
T OnlyLowestBitSet(const T value) { return ((value) & (-value)); }

//------------------------------------------------------------------------------

/// Round the given integer up to the nearest power of 2.
template<typename T>
T PowerOf2Ceil(const T value)
{
   T pow=OnlyLowestBitSet (value);
   while (pow < value)
   {
      pow <<= 1;
   }
   return (pow);
}

//------------------------------------------------------------------------------

/// Round the given integer up to a multiple of 32.
#define RoundUp32(value) ((value + 31) & ~31)

//------------------------------------------------------------------------------

/// Round the given integer down to a multiple of 32.
#define RoundDown32(value) (value & ~31)

//===============================================================================

/// If 'a' is not NULL, call FrogFree on it, then set it to NULL.
#define SmartFree(a) { if(a) { FrogFree (a); (a) = NULL; }}
/// If 'a' is not NULL, call frog_delete on it, then set it to NULL.
#define SmartDelete(a) { if(a) { frog_delete (a); (a) = NULL; }}
/// If 'a' is not NULL, call its Deinit member, call frog_delete on it, then set it to NULL.
#define SmartDeinitDelete(a)  { if(a) { (a)->Deinit(); frog_delete (a); (a) = NULL; }}

//===============================================================================

/// Copy the value from the given position in a buffer and increment the
/// buffer position.
template<typename T>
inline void BufferRead(T& value, void*& bufferPosition)
{
   value = *((T*)bufferPosition);
   bufferPosition = (void*)((size_t)bufferPosition + sizeof(T));
}

//===============================================================================

/// Show the given URL in the browser.
/// In Android, if you call this from a thread other than the main Frog thread,
/// be sure to call theAndroidPlatform->JNIDeinit() when your calling thread is
/// about to finish.
void URLShow(const char* url);

//------------------------------------------------------------------------------

} //namespace Webfoot

#endif //#ifndef __FROG__UTILITY_H__

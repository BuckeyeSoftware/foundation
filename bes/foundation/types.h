#ifndef BES_FOUNDATION_TYPES_H
#define BES_FOUNDATION_TYPES_H

#include <bes/foundation/config.h>

/** @brief Represents an IEEE-754 single precision floating point type */
typedef float bes_f32;

/** @brief Represents an IEEE-754 double precision floating point type */
typedef double bes_f64;

/** @brief Represents a unsigned 8-bit integer */
typedef unsigned char bes_u8;

/** @brief Represents a signed 8-bit integer */
typedef signed char bes_s8;

/** @brief Represents a unsigned 16-bit integer */
typedef unsigned short bes_u16;

/** @brief Represents a signed 16-bit integer */
typedef signed short bes_s16;

/** @brief Represents a unsigned 32-bit integer */
typedef unsigned int bes_u32;

/** @brief Represents a signed 32-bit integer */
typedef signed int bes_s32;

/** @brief Represents a unsigned 64-bit integer */
typedef unsigned long long bes_u64;

/** @brief Represents a signed 64-bit integer */
typedef signed long long bes_s64;

/** @brief Represents a boolean value */
enum bes_bool
{
	BES_FALSE, /**< Falsy value */
	BES_TRUE /**< Truthy value */
};

/**
 * @brief Unsigned integer type of the result of the sizeof operator.
 *
 * @ref bes_size can store the maximum size of a theoretically
 * possible object of any type (including array). A type whose size cannot
 * be represented by @ref bes_size isn't possible.
 *
 * This type is always used for array indexing and loop counting since
 * using other types, such as int or unsigned int, for array indexing
 * may fail on, e.g. 64-bit systems where the index exceeds {U}INT_MAX
 * or it relies on 32-bit modular arithmetic.
 */
typedef unsigned BES_ADDR bes_size;

/** @brief Unsigned integer type capable of holding a pointer */
typedef unsigned BES_ADDR bes_uintptr;

/** @brief Signed integer type capable of holding a pointer difference */
typedef BES_ADDR bes_ptrdiff;

/** @brief Represents a single byte value */
typedef unsigned char bes_byte;

/** @brief Represents a boolean value */
typedef enum bes_bool bes_bool;

/** @brief Type representing a UTF-16 character */
typedef bes_u16 bes_wchar;

#endif

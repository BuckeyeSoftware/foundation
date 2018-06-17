#ifndef BES_FOUNDATION_MATH_H
#define BES_FOUNDATION_MATH_H

/**
 * @defgroup Math Mathematics
 * @brief Optimized math functions
 * @{
 */

#include <bes/foundation/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Some compilers specify the eval method on the preprocessor by default,
 * search for that first. Falling back to an eval method of zero is fine
 * provided long double is not used anywhere, our math code only uses
 * it when the eval method is 2. */
#ifndef BES_DOXYGEN_IGNORE
#if defined(__FLT_EVAL_METHOD__)
#define BES_FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#else
#define BES_FLT_EVAL_METHOD 0
#endif
#else
#define BES_FLT_EVAL_METHOD unspecified /**< The floating point evaluation mode */
#endif

/* These constants are true of any conforming IEEE 754 implementation of
 * 32 bit and 64 bit floating point arithmetic. Pluck does not work on
 * any other type of platform. */
#define BES_F32_EPSILON 1.1920928955078125e-07F    /**< Smallest representable number for a @ref bes_f32 */
#define BES_F64_EPSILON 2.22044604925031308085e-16 /**< Smallest representable number for a @ref bes_f64 */

/* Long double is an alias for double on MSVC in x86_64 */
#ifndef BES_DOXYGEN_IGNORE
#if defined(BES_COMPILER_MSVC)
#define BES_F80_EPSILON BES_F64_EPSILON
#else
#define BES_F80_EPSILON 1.0842021724855044340e-19L
#endif
#else
#define BES_F80_EPSILON unspecified /**< Smallest representable number for 80-bit float (if supported) */
#endif

#define BES_PI          3.14159265358979323846 /**< Pi constant */
#define BES_PI_HALF     1.57079632679489661923 /**< Pi/2 constant */

#define BES_NAN         bes_nan() /**< Signaling NaN */
#define BES_INFINITY    bes_inf() /**< Positive infinity */

/** @brief Calculate sine of angle given in radians */
BES_EXPORT bes_f32 BES_API
bes_sinf(bes_f32 ang);

/** @brief Calculate cosine of angle given in radians */
BES_EXPORT bes_f32 BES_API
bes_cosf(bes_f32 ang);

/** @brief Calculate tangent of angle given in radians */
BES_EXPORT bes_f32 BES_API
bes_tanf(bes_f32 ang);

/** @brief Calculate absoluate value */
BES_EXPORT bes_f32 BES_API
bes_absf(bes_f32 ang);

/** @brief Calculate inverse square root */
BES_EXPORT bes_f32 BES_API
bes_rsqrtf(bes_f32 v);

/**
 * @brief Calculate square root
 * @param v The value to calculate square root of
 * @warning The following square root only has 11 bits of precision
 */
BES_EXPORT bes_f32 BES_API
bes_sqrtf(bes_f32 v);

/** @brief Calculate power */
BES_EXPORT bes_f64 BES_API
bes_pow(bes_f64 a, bes_f64 b);

/** @brief Round down towards negative infinity */
BES_EXPORT bes_f32 BES_API
bes_floorf(bes_f32 x);

/** @brief Generate signaling NaN */
BES_EXPORT bes_f32 BES_API
bes_nan(void);

/** @brief Generate positive infinity */
BES_EXPORT bes_f32 BES_API
bes_inf(void);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif

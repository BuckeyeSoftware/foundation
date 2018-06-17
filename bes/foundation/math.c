#include <bes/foundation/math.h>
#include <bes/foundation/macros.h>

typedef union bes_flt_shape bes_flt_shape;

union bes_flt_shape
{
	bes_u32 u;
	bes_f32 f;
};

/* On i386 the x87 FPU does everything in 80 bits of precision. Using
 * a double for the reduced range approximations would cause the compiler
 * to generate additional load and stores to reduce the 80-bit results to
 * 64-bit ones. By using the effective type floating-point arithmetic is
 * evaluated on in hardware we can eliminate those conversions for
 * faster execution and smaller icache pollution. */
#if BES_FLT_EVAL_METHOD == 2
typedef long double f64;
typedef f64 f32;
#else
typedef double f64;
typedef float f32;
#endif

/* Several multiples of half pi */
#define k_C1PIO2 (1.0 * BES_PI_HALF)
#define k_C2PIO2 (2.0 * BES_PI_HALF)
#define k_C3PIO2 (3.0 * BES_PI_HALF)
#define k_C4PIO2 (4.0 * BES_PI_HALF)

/* |cos(x) - c(x)| < 2**-34.1 (~[-5.37e-11, 5.295e-11]) */
#define k_C0 -4.99999997251031003e-01
#define k_C1  4.16666233237390632e-02
#define k_C2 -1.38867637746099295e-03
#define k_C3  2.43904487962774091e-05

/* |sin(x)/x - s(x)| < 2**-37.5 (~[-4.89e-12, 4.824e-12]) */
#define k_S0 -1.66666666416265236e-01
#define k_S1  8.33332938588946318e-03
#define k_S2 -1.98393348360966317e-04
#define k_S3  2.71831149398982191e-06

/* |tan(x)/x - t(x)| < 2**-25.5 (~[-2e-08, 2e-08]) */
#define k_T0 3.33331395030791400e-01
#define k_T1 1.33392002712976743e-01
#define k_T2 5.33812378445670394e-02
#define k_T3 2.45283181166547279e-02
#define k_T4 2.97435743359967305e-03
#define k_T5 9.46564784943673167e-03

/* For systems that lack 80-bit long double, F80 becomes an alias for F64 */
#define k_TOINT (1.5 / BES_F80_EPSILON)

/* 25+53 bit fixed point pi constants */
#define k_PIO2H 1.57079631090164184570e+00
#define k_PIO2T 1.58932547735281966916e-08

/* 1/pi/2 */
#define k_INVPIO2 6.36619772367581382433e-01

BES_PURE static inline bes_f32
bes_cosdf(bes_f64 x)
{
	const f64 z = x * x;
	const f64 w = z * z;
	const f64 r = k_C2 + z * k_C3;
	return (bes_f32)(((1.0 + z * k_C0) + w * k_C1) + (w * z) * r);
}

BES_PURE static inline bes_f32
bes_sindf(bes_f64 x)
{
	const f64 z = x * x;
	const f64 w = z * z;
	const f64 r = k_S2 + z * k_S3;
	const f64 s = z * x;
	return (bes_f32)((x + s * (k_S0 + z * k_S1)) + s * w * r);
}

BES_PURE static inline bes_f32
bes_tandf(double x, bes_bool odd)
{
	const f64 z = x*x;

	/* polynomial reduction into independent terms for parallel evaluation */
	const f64 r = k_T4 + z * k_T5;
	const f64 t = k_T2 + z * k_T3;
	const f64 w = z * z;
	const f64 s = z * x;
	const f64 u = k_T0 + z * k_T1;

	/* add up small terms from lowest degree up for efficiency on non-sequential
	 * systems (lower terms tend to be ready earlier.) */
	const f64 v = (x + s * u) + (s * w) * (t + w * r);
	return (bes_f32)(odd ? -1.0/v : v);
}

BES_PURE static inline int
bes_rempio2(bes_f32 x, bes_u32 ix, bes_f64 *y)
{
	/* 25+53 bit pi is good enough for median size */
	if (ix < 0x4DC90FDB)
	{
		/* |x| ~< 2^28*(pi/2) */
		const f64 fn = (f64)x * (f64)k_INVPIO2 + (f64)k_TOINT - (f64)k_TOINT;
		const int n = (bes_s32)fn;
		*y = x - fn * k_PIO2H - fn * k_PIO2T;
		return n;
	}

	/* value is a NaN */
	BES_ASSERT(ix < 0x7F800000 && "NaN");

	/* value is far too large, something is pathologically wrong */
	BES_ASSERT(0 && "Huge");

	return 0;
}

BES_PURE bes_f32
bes_cosf(bes_f32 x)
{
	bes_flt_shape u;
	u.f = x;

	const bes_u32 ix = u.u & 0x7FFFFFFF;
	const bes_u32 sign = u.u >> 31;

	if (ix <= 0x3F490FDA)
	{
		/* |x| ~<= pi/4 */
		return ix < 0x39800000 ? 1.0f : bes_cosdf(x); /* |x| < 2**-12 */
	}

	if (ix <= 0x407B53D1)
	{
		/* |x| ~<= 5*pi/4 */
		if (ix > 0x4016CBE3)
		{
			/* |x| ~> 3*pi/4 */
			return -bes_cosdf(sign ? x + k_C2PIO2 : x - k_C2PIO2);
		}
		return bes_sindf(sign ? x + k_C1PIO2 : k_C1PIO2 - x);
	}

	if (ix <= 0x40E231D5)
	{
		/* |x| ~<= 9*pi/4 */
		if (ix > 0x40AFEDDF)
		{
			/* |x| ~> 7*pi/4 */
			return bes_cosdf(sign ? x + k_C4PIO2 : x - k_C4PIO2);
		}
		return bes_sindf(sign ? -x - k_C3PIO2 : x - k_C3PIO2);
	}

	BES_ASSERT(ix < 0x7F800000 && "NaN");

	bes_f64 y = 0.0;
	const int n = bes_rempio2(x, ix, &y);
	switch (n & 3)
	{
	case 0:
		return bes_cosdf(y);
	case 1:
		return bes_sindf(-y);
	case 2:
		return -bes_cosdf(y);
	}
	return bes_sindf(y);
}

BES_PURE bes_f32
bes_sinf(bes_f32 x)
{
	bes_flt_shape u;
	u.f = x;

	const bes_u32 ix = u.u & 0x7FFFFFFF;
	const bes_u32 sign = u.u >> 31;

	if (ix <= 0x3F490FDA)
	{
		/* |x| ~<= pi/4 */
		return ix < 0x39800000 ? x : bes_sindf(x); /* |x| < 2**-12 */
	}

	if (ix <= 0x407B53D1)
	{
		/* |x| ~<= 5*pi/4 */
		if (ix <= 0x4016CBE3)
		{
			/* |x| ~<= 3pi/4 */
			return sign ? -bes_cosdf(x + k_C1PIO2) : bes_cosdf(x - k_C1PIO2);
		}
		return bes_sindf(sign ? -(x + k_C2PIO2) : -(x - k_C2PIO2));
	}

	if (ix <= 0x40E231D5)
	{
		/* |x| ~<= 9*pi/4 */
		if (ix <= 0x40AFEDDF)
		{
			/* |x| ~<= 7*pi/4 */
			return sign ? bes_cosdf(x + k_C3PIO2) : -bes_cosdf(x - k_C3PIO2);
		}
		return bes_sindf(sign ? x + k_C4PIO2 : x - k_C4PIO2);
	}

	BES_ASSERT(ix < 0x7F800000 && "NaN");

	bes_f64 y = 0.0;
	const int n = bes_rempio2(x, ix, &y);
	switch (n & 3)
	{
	case 0:
		return bes_sindf(y);
	case 1:
		return bes_cosdf(y);
	case 2:
		return bes_sindf(-y);
	}

	return -bes_cosdf(y);
}

BES_PURE bes_f32
bes_tanf(bes_f32 x)
{
	bes_flt_shape u;
	u.f = x;

	const bes_u32 ix = u.u & 0x7FFFFFFF;
	const bes_u32 sign = u.u >> 31;

	if (ix < 0x3F490FDa)
	{
		/* |x| ~<= pi/4 */
		return ix < 0x39800000 ? x : bes_tandf(x, BES_FALSE); /* |x| < 2**-12 */
	}
	if (ix <= 0x407B53D1)
	{
		/* |x| ~<= 5*pi/4 */
		if (ix <= 0x4016CBE3)
		{
			/* |x| ~<= 3pi/4 */
			return bes_tandf((sign ? x + k_C1PIO2 : x - k_C1PIO2), BES_TRUE);
		}
		return bes_tandf((sign ? x + k_C2PIO2 : x - k_C2PIO2), BES_FALSE);
	}
	if (ix <= 0x40E231D5)
	{
		/* |x| ~<= 9*pi/4 */
		if (ix <= 0x40AFEDDF)
		{
			/* |x| ~<= 7*pi/4 */
			return bes_tandf((sign ? x + k_C3PIO2 : x - k_C3PIO2), BES_TRUE);
		}
		return bes_tandf((sign ? x + k_C4PIO2 : x - k_C4PIO2), BES_FALSE);
	}

	bes_f64 y = 0.0;
	const int n = bes_rempio2(x, ix, &y);
	return bes_tandf(y, n&1);
}

BES_PURE bes_f32
bes_absf(bes_f32 v)
{
	bes_flt_shape u;
	u.f = v;
	u.u &= 0x7FFFFFFF;
	return u.f;
}

BES_PURE bes_f32
bes_rsqrtf(bes_f32 x)
{
	/* The approximate inverse sqrt trick from Q3A is sufficent for our
	 * needs. This is only called on the scalar path and by bes_sqrtf
	 * for scalar path too. */
	const bes_f32 x_half = x * 0.5f;
	bes_flt_shape u;
	u.f = x;
	u.u = 0x5F375A86 - (u.u >> 1);
	x = u.f;
	x = x * (1.5f - x_half * x * x);
	return x;
}

BES_PURE bes_f32
bes_sqrtf(bes_f32 v)
{
	return v;
#if 0
	/* The use of inverse square root is faster than a full square root.
	 * On x86 this has ~11 bits of precision and 1/4th the latency. We
	 * can survive with those few bits of precision because Pluck does
	 * not concern itself with accuracy too much. */
	const bes_v128f a = bes_m_set1_ps(v);
	const bes_v128f b = bes_m_rsqrt_ps(a);
	const bes_v128f c = bes_m_mul_ps(a, b);
	return bes_m_cvtss_f32(c);
#endif
}

BES_PURE bes_f64
bes_pow(bes_f64 a, bes_f64 b)
{
	/* Calculate approximation with fraction of the exponent */
	bes_s32 e = (bes_s32)b;

	union
	{
		bes_f64 d;
		bes_s32 x[2];
	} u;

	u.d = a;
	u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;

	/* Exponentiation by squaring with the exponent's integer part
	 * avoiding a sequential dependency on u.d here for out of order
	 * processors. */
	bes_f64 r = 1.0;
	while (e)
	{
		if (e & 1)
		{
			r *= a;
		}
		a *= a;
		e >>= 1;
	}

	return r * u.d;
}

BES_PURE bes_f32
bes_floorf(bes_f32 x)
{
	bes_flt_shape u;
	u.f = x;

	bes_s32 e = (bes_s32)(u.u >> 23 & 0xFF) - 0x7F;
	if (e >= 23)
	{
		return x;
	}

	if (e >= 0)
	{
		const bes_u32 m = 0x007FFFFF >> e;
		if ((u.u & m) == 0)
		{
			return x;
		}

		if (u.u >> 31)
		{
			u.u += m;
		}
		u.u &= ~m;
	}
	else
	{
		if (u.u >> 31 == 0)
		{
			u.u = 0;
		}
		else if (u.u << 1)
		{
			u.f = -1.0f;
		}
	}

	return u.f;
}

BES_PURE bes_f32
bes_inf(void)
{
#if 100 * __GNUC__ + __GNUC_MINOR__ >= 303
	return __builtin_inff();
#else
	static bes_flt_shape s = { 0x7F800000 };
	return s.f;
#endif
}

BES_PURE bes_f32
bes_nan(void)
{
#if 100 * __GNUC__ + __GNUC_MINOR__ >= 303
	return __builtin_nanf("");
#else
	static bes_flt_shape s = { 0x7F800001 };
	return s.f;
#endif
}

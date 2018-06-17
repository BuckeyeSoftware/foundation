#ifndef BES_FOUNDATION_BSWAP_H
#define BES_FOUNDATION_BSWAP_H

#include <bes/foundation/types.h>

static inline bes_u16
bes_bswap_u16(bes_u16 n)
{
	return (bes_u16)((n << 8) | (n >> 8));
}

static inline bes_u32
bes_bswap_u32(bes_u32 n)
{
	n = ((n << 8) & 0xFF00FF00) | ((n >> 8) & 0xFF00FF);
	return (n << 16) | (n >> 16);
}

static inline bes_u64
bes_bswap_u64(bes_u64 n)
{
	n = ((n << 8) & 0xFF00FF00FF00FF00ULL) | ((n >> 8) & 0x00FF00FF00FF00FFULL);
	n = ((n << 16) & 0xFFFF0000FFFF0000ULL) | ((n >> 16) & 0x0000FFFF0000FFFFULL);
	return (n << 32) | (n >> 32);
}

static inline bes_bool
bes_bswap_is_big_endian(void)
{
	union { bes_u16 i; bes_byte c[4]; } u = { 0x0100 };
	u.i = 0x0100;
	return u.c[0] == 1 ? PLUCK_TRUE : PLUCK_FALSE;
}

static inline bes_bool
bes_bswap_is_lil_endian(void)
{
	union { bes_u16 i; bes_byte c[4]; } u = { 0x0100 };
	return u.c[0] == 0 ? PLUCK_TRUE : PLUCK_FALSE;
}

#endif

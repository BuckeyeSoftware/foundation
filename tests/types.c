#include <bes/foundation/test.h>

volatile bes_u8 u8_one = 1;
volatile bes_u16 u16_one = 1;
volatile bes_u32 u32_one = 1;
volatile bes_u64 u64_one = 1;
volatile bes_uintptr uintptr_one = 1;

BES_DEFINE_TEST(bes_f32_is_32_bits)
{
	return sizeof(bes_f32) == 4;
}

BES_DEFINE_TEST(bes_f64_is_64_bits)
{
	return sizeof(bes_f64) == 8;
}

BES_DEFINE_TEST(bes_u8_is_8_bits)
{
	return sizeof(bes_u8) == 1;
}

BES_DEFINE_TEST(bes_u8_is_unsigned)
{
	return (bes_u8)-1 >= u8_one - 1;
}

BES_DEFINE_TEST(bes_s8_is_8_bits)
{
	return sizeof(bes_s8) == 1;
}

BES_DEFINE_TEST(bes_s8_is_signed)
{
	return (bes_s8)0 - 1 < 0;
}

BES_DEFINE_TEST(bes_u16_is_16_bits)
{
	return sizeof(bes_u16) == 2;
}

BES_DEFINE_TEST(bes_u16_is_unsigned)
{
	return (bes_u16)-1 >= 0;
}

BES_DEFINE_TEST(bes_s16_is_16_bits)
{
	return sizeof(bes_s16) == 2;
}

BES_DEFINE_TEST(bes_s16_is_signed)
{
	return (bes_s16)0 - 1 < 0;
}

BES_DEFINE_TEST(bes_u32_is_32_bits)
{
	return sizeof(bes_u32) == 4;
}

BES_DEFINE_TEST(bes_u32_is_unsigned)
{
	return (bes_u32)-1 >= u32_one - 1;
}

BES_DEFINE_TEST(bes_s32_is_32_bits)
{
	return sizeof(bes_s32) == 4;
}

BES_DEFINE_TEST(bes_s32_is_signed)
{
	return (bes_s32)0 - 1 < 0;
}

BES_DEFINE_TEST(bes_u64_is_64_bits)
{
	return sizeof(bes_u64) == 8;
}

BES_DEFINE_TEST(bes_u64_is_unsigned)
{
	return (bes_u64)-1 >= u64_one - 1;
}

BES_DEFINE_TEST(bes_s64_is_64_bits)
{
	return sizeof(bes_s64) == 8;
}

BES_DEFINE_TEST(bes_s64_is_signed)
{
	return (bes_s64)0 - 1 < 0;
}

BES_DEFINE_TEST(bes_bool_false_is_0)
{
	return BES_FALSE == 0;
}

BES_DEFINE_TEST(bes_bool_true_is_1)
{
	return BES_TRUE == 1;
}

BES_DEFINE_TEST(bes_size_is_same_size_as_sizeof_result_type)
{
	return sizeof(sizeof 0) == sizeof(bes_size);
}

BES_DEFINE_TEST(bes_uintptr_is_same_size_as_pointer)
{
	return sizeof(void *) == sizeof(bes_uintptr);
}

BES_DEFINE_TEST(bes_ptrdiff_is_same_size_as_pointer)
{
	return sizeof(void *) == sizeof(bes_ptrdiff);
}

BES_DEFINE_TEST(bes_uintptr_is_unsigned)
{
	return (bes_uintptr)-1 >= uintptr_one - 1;
}

BES_DEFINE_TEST(bes_ptrdiff_is_signed)
{
	return (bes_ptrdiff)0 - 1 < 0;
}

BES_DEFINE_TEST(bes_byte_is_8_bits)
{
	return sizeof(bes_byte) == 1;
}

BES_DEFINE_TEST(bes_byte_is_unsigned)
{
	return (bes_byte)-1 >= 0;
}

BES_DEFINE_TEST(bes_wchar_is_16_bits)
{
	return sizeof(bes_wchar) == 2;
}

BES_DEFINE_TEST(bes_wchar_is_unsigned)
{
	return (bes_wchar)-1 >= 0;
}

BES_DEFINE_TEST_LIST(types_tests)
{
	BES_ADD_TEST(bes_f32_is_32_bits),
	BES_ADD_TEST(bes_f64_is_64_bits),
	BES_ADD_TEST(bes_u8_is_8_bits),
	BES_ADD_TEST(bes_u8_is_unsigned),
	BES_ADD_TEST(bes_s8_is_8_bits),
	BES_ADD_TEST(bes_s8_is_signed),
	BES_ADD_TEST(bes_u16_is_16_bits),
	BES_ADD_TEST(bes_u16_is_unsigned),
	BES_ADD_TEST(bes_s16_is_16_bits),
	BES_ADD_TEST(bes_s16_is_signed),
	BES_ADD_TEST(bes_u32_is_32_bits),
	BES_ADD_TEST(bes_u32_is_unsigned),
	BES_ADD_TEST(bes_s32_is_32_bits),
	BES_ADD_TEST(bes_s32_is_signed),
	BES_ADD_TEST(bes_u64_is_64_bits),
	BES_ADD_TEST(bes_u64_is_unsigned),
	BES_ADD_TEST(bes_s64_is_64_bits),
	BES_ADD_TEST(bes_s64_is_signed),
	BES_ADD_TEST(bes_bool_false_is_0),
	BES_ADD_TEST(bes_bool_true_is_1),
	BES_ADD_TEST(bes_size_is_same_size_as_sizeof_result_type),
	BES_ADD_TEST(bes_uintptr_is_same_size_as_pointer),
	BES_ADD_TEST(bes_ptrdiff_is_same_size_as_pointer),
	BES_ADD_TEST(bes_uintptr_is_unsigned),
	BES_ADD_TEST(bes_ptrdiff_is_signed),
	BES_ADD_TEST(bes_byte_is_8_bits),
	BES_ADD_TEST(bes_byte_is_unsigned),
	BES_ADD_TEST(bes_wchar_is_16_bits),
	BES_ADD_TEST(bes_wchar_is_unsigned)
};

#include <stdio.h>
BES_DEFINE_TEST_COMMAND(test_types_command, "types", types_tests, printf)

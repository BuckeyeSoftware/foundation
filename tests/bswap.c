#include <bes/foundation/test.h>
#include <bes/foundation/bswap.h>

BES_DEFINE_TEST(bswap_u16_swaps_byte_order)
{
	return bes_bswap_u16(0xAABB) == 0xBBAA;
}

BES_DEFINE_TEST(bswap_u32_swaps_byte_order)
{
	return bes_bswap_u32(0xAABBCCDD) == 0xDDCCBBAA;
}

BES_DEFINE_TEST(bswap_u64_swaps_byte_order)
{
	return bes_bswap_u64(0xAABBCCDDEEFF1234) == 0x3412FFEEDDCCBBAA;
}

BES_DEFINE_TEST_LIST(bswap_tests)
{
	BES_ADD_TEST(bswap_u16_swaps_byte_order),
	BES_ADD_TEST(bswap_u32_swaps_byte_order),
	BES_ADD_TEST(bswap_u64_swaps_byte_order)
};

#include <stdio.h>
BES_DEFINE_TEST_COMMAND(test_bswap_command, "bswap", bswap_tests, printf)

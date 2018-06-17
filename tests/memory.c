#include <bes/foundation/test.h>
#include <bes/foundation/memory.h>

BES_DEFINE_TEST(malloc_returns_non_null)
{
	void *x = bes_malloc(1);
	const bes_bool result = x != 0;
	bes_free(x);
	return result;
}

BES_DEFINE_TEST(realloc_from_smaller_to_larger_makes_larger)
{
	void *x = bes_malloc(1);
	x = bes_realloc(x, 2);
	const bes_bool result = x != 0;
	bes_free(x);
	return result;
}

BES_DEFINE_TEST(realloc_from_larger_to_smaller_makes_smaller)
{
	void *x = bes_malloc(1024);
	x = bes_realloc(x, 512);
	const bes_bool result = x != 0;
	bes_free(x);
	return result;
}

BES_DEFINE_TEST_LIST(memory_tests)
{
	BES_ADD_TEST(malloc_returns_non_null),
	BES_ADD_TEST(realloc_from_smaller_to_larger_makes_larger),
	BES_ADD_TEST(realloc_from_larger_to_smaller_makes_smaller)
};

#include <stdio.h>
BES_DEFINE_TEST_COMMAND(test_memory_command, "memory", memory_tests, printf)

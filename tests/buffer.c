#include <bes/foundation/test.h>
#include <bes/foundation/buffer.h>

BES_DEFINE_TEST(empty_buffer_has_size_zero)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	return bes_buffer_size(a) == 0;
}

BES_DEFINE_TEST(buffer_push_on_empty_resizes)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	const bes_size old_size = bes_buffer_size(a);
	bes_buffer_push(a, 1);
	const bes_bool result = bes_buffer_size(a) != old_size;
	bes_buffer_free(a);
	return result;
}

BES_DEFINE_TEST(buffer_push_on_non_empty_resizes)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_push(a, 1);
	bes_buffer_push(a, 2);
	const bes_size new_size = bes_buffer_size(a);
	bes_buffer_free(a);
	return new_size == 2;
}

BES_DEFINE_TEST(buffer_access_after_push_contains_same_value)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_push(a, 100);
	const bes_bool result = a[0] == 100;
	bes_buffer_free(a);
	return result;
}

BES_DEFINE_TEST(buffer_free_resets_buffer)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_push(a, 1);
	bes_buffer_free(a);
	return a == BES_BUFFER_INITIALIZER;
}

BES_DEFINE_TEST(buffer_resize_from_smaller_makes_larger)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_resize(a, 1);
	const bes_bool result = bes_buffer_size(a) == 1;
	bes_buffer_free(a);
	return result;
}

BES_DEFINE_TEST(buffer_resize_from_larger_makes_smaller)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_resize(a, 2);
	bes_buffer_resize(a, 1);
	const bes_bool result = bes_buffer_size(a) == 1;
	bes_buffer_free(a);
	return result;
}

BES_DEFINE_TEST(buffer_expand_makes_larger)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_expand(a, 1);
	const bes_bool result = bes_buffer_size(a) == 1;
	bes_buffer_free(a);
	return result;
}

BES_DEFINE_TEST(buffer_clear_makes_size_zero)
{
	BES_BUFFER(int) a = BES_BUFFER_INITIALIZER;
	bes_buffer_push(a, 1);
	bes_buffer_clear(a);
	const bes_size size = bes_buffer_size(a);
	bes_buffer_free(a);
	return size == 0;
}

BES_DEFINE_TEST(buffer_write_on_empty_resizes)
{
	BES_BUFFER(bes_byte) a = BES_BUFFER_INITIALIZER;
	bes_buffer_write(&a, (const void*)"hi", 2);
	const bes_size size = bes_buffer_size(a);
	bes_buffer_free(a);
	return size == 2;
}

BES_DEFINE_TEST(buffer_read_on_empty_fails)
{
	BES_BUFFER(bes_byte) a = BES_BUFFER_INITIALIZER;
	bes_byte b;
	bes_size offset = 0;
	return !bes_buffer_read(&b, sizeof b, &offset, a);
}

BES_DEFINE_TEST(buffer_read_after_write_with_same_size_succeeds)
{
	BES_BUFFER(bes_byte) a = BES_BUFFER_INITIALIZER;
	bes_byte write = '!';
	bes_byte read;
	bes_size offset = 0;
	bes_buffer_write(&a, &write, sizeof write);
	const bes_bool result = bes_buffer_read(&read, sizeof read, &offset, a);
	bes_buffer_free(a);
	return result;
}

BES_DEFINE_TEST(buffer_read_offset_after_write_advances_by_write_size)
{
	BES_BUFFER(bes_byte) a = BES_BUFFER_INITIALIZER;
	bes_byte write = '!';
	bes_byte read;
	bes_size offset = 0;
	bes_buffer_write(&a, &write, sizeof write);
	bes_buffer_read(&read, sizeof read, &offset, a);
	bes_buffer_free(a);
	return offset = sizeof read;
}

BES_DEFINE_TEST(buffer_read_after_write_contains_same_data)
{
	BES_BUFFER(bes_byte) a = BES_BUFFER_INITIALIZER;
	bes_byte write = '!';
	bes_byte read;
	bes_size offset = 0;
	bes_buffer_write(&a, &write, sizeof write);
	bes_buffer_read(&read, sizeof read, &offset, a);
	bes_buffer_free(a);
	return write == read;
}

BES_DEFINE_TEST_LIST(buffer_tests)
{
	BES_ADD_TEST(empty_buffer_has_size_zero),
	BES_ADD_TEST(buffer_push_on_empty_resizes),
	BES_ADD_TEST(buffer_push_on_non_empty_resizes),
	BES_ADD_TEST(buffer_access_after_push_contains_same_value),
	BES_ADD_TEST(buffer_free_resets_buffer),
	BES_ADD_TEST(buffer_resize_from_smaller_makes_larger),
	BES_ADD_TEST(buffer_resize_from_larger_makes_smaller),
	BES_ADD_TEST(buffer_expand_makes_larger),
	BES_ADD_TEST(buffer_clear_makes_size_zero),
	BES_ADD_TEST(buffer_write_on_empty_resizes),
	BES_ADD_TEST(buffer_read_on_empty_fails),
	BES_ADD_TEST(buffer_read_after_write_with_same_size_succeeds),
	BES_ADD_TEST(buffer_read_offset_after_write_advances_by_write_size),
	BES_ADD_TEST(buffer_read_after_write_contains_same_data)
};

#include <stdio.h>
BES_DEFINE_TEST_COMMAND(test_buffer_command, "buffer", buffer_tests, printf)

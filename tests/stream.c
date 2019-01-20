#include <bes/foundation/test.h>
#include <bes/foundation/stream.h>
#include <bes/foundation/string.h>
#include <bes/foundation/buffer.h>
#include <bes/foundation/memory.h>

// simple memory stream
typedef struct test_stream test_stream;

struct test_stream
{
	BES_BUFFER(char) data;
	bes_u64 position;
};

static bes_bool
test_stream_read(bes_stream *stream, void *data_, bes_size size, bes_size *read_)
{
	test_stream *test = stream->aux;

	bes_size can_read = bes_buffer_size(test->data) - test->position;
	bes_bool result = BES_TRUE;

	if (size > can_read)
	{
		size = can_read;
		result = BES_FALSE;
	}

	bes_memcpy(data_, test->data + test->position, size);

	test->position += size;

	*read_ = size;

	return result;
}

static bes_bool
test_stream_write(bes_stream *stream, const void *data, bes_size size, bes_size *write_)
{
	test_stream *test = stream->aux;

	bes_size can_write = bes_buffer_size(test->data) - test->position;
	bes_bool result = BES_TRUE;

	if (size > can_write)
	{
		size = can_write;
		result = BES_FALSE;
	}

	bes_memcpy(test->data + test->position, data, size);

	test->position += size;

	*write_ = size;

	return result;
}

static bes_bool
test_stream_seek(bes_stream *stream, bes_u64 offset)
{
	test_stream *test = stream->aux;

	if (offset > bes_buffer_size(test->data))
	{
		return BES_FALSE;
	}

	test->position = offset;

	return BES_TRUE;
}

static bes_bool
test_stream_tell(bes_stream *stream, bes_u64 *offset_)
{
	*offset_ = ((test_stream*)stream->aux)->position;
	return BES_TRUE;
}


static bes_bool
test_stream_create(bes_stream *stream_, bes_size size)
{
	if (!stream_ || size == 0)
	{
		return BES_FALSE;
	}

	bes_memset(stream_, 0, sizeof *stream_);

	test_stream *test = bes_malloc(sizeof *test);
	if (!test)
	{
		return BES_FALSE;
	}

	test->data = BES_BUFFER_INITIALIZER;
	test->position = 0;
	if (!bes_buffer_resize(test->data, size))
	{
		bes_free(test);
		return BES_FALSE;
	}

	stream_->read = test_stream_read;
	stream_->write = test_stream_write;
	stream_->seek = test_stream_seek;
	stream_->tell = test_stream_tell;
	stream_->aux = test;

	return BES_TRUE;
}

static void
test_stream_free(bes_stream *stream)
{
	if (stream && stream->aux)
	{
		test_stream *test = stream->aux;
		bes_buffer_free(test->data);
		bes_free(test);
	}
}

BES_DEFINE_TEST(null_stream_read_is_false)
{
	char data[1];
	bes_size read = 0;
	return bes_stream_read(0, data, 1, &read) == BES_FALSE;
}

BES_DEFINE_TEST(null_stream_write_is_false)
{
	char data[1] = { 0 };
	bes_size write = 0;
	return bes_stream_write(0, data, sizeof data, &write) == BES_FALSE;
}

BES_DEFINE_TEST(stream_read_null_data_is_false)
{
	bes_stream stream;
	bes_size read = 0;
	return bes_stream_read(&stream, 0, 1, &read) == BES_FALSE;
}

BES_DEFINE_TEST(stream_write_null_data_is_false)
{
	bes_stream stream;
	bes_size write = 0;
	return bes_stream_write(&stream, 0, 1, &write) == BES_FALSE;
}

BES_DEFINE_TEST(stream_read_zero_size_is_false)
{
	bes_stream stream;
	bes_size read = 0;
	char buffer[1];
	return bes_stream_read(&stream, buffer, 0, &read) == BES_FALSE;
}

BES_DEFINE_TEST(stream_write_zero_size_is_false)
{
	bes_stream stream;
	bes_size write = 0;
	char data[1] = { 0 };
	return bes_stream_write(&stream, data, 0, &write) == BES_FALSE;
}

BES_DEFINE_TEST(stream_read_null_read_is_false)
{
	bes_stream stream;
	char data[1];
	return bes_stream_read(&stream, data, sizeof data, 0) == BES_FALSE;
}

BES_DEFINE_TEST(stream_write_null_write_is_false)
{
	bes_stream stream;
	char data[1] = { 0 };
	return bes_stream_write(&stream, data, sizeof data, 0) == BES_FALSE;
}

BES_DEFINE_TEST(null_stream_seek_is_false)
{
	return bes_stream_seek(0, 1) == BES_FALSE;
}

BES_DEFINE_TEST(null_stream_tell_is_false)
{
	bes_u64 offset = 0;
	return bes_stream_tell(0, &offset) == BES_FALSE;
}

BES_DEFINE_TEST(stream_tell_null_offset_is_false)
{
	bes_stream stream;
	return bes_stream_tell(&stream, 0) == BES_FALSE;
}

BES_DEFINE_TEST(stream_write_works)
{
	bes_stream stream;
	if (!test_stream_create(&stream, 1))
	{
		return BES_FALSE;
	}

	bes_bool result = BES_TRUE;
	char a = 'A';
	bes_size write = 0;
	result &= bes_stream_write(&stream, &a, sizeof a, &write);
	result &= write == 1;

	test_stream_free(&stream);
	return result;
}

BES_DEFINE_TEST(stream_tell_works)
{
	bes_stream stream;
	if (!test_stream_create(&stream, 1))
	{
		return BES_FALSE;
	}

	bes_bool result = BES_TRUE;
	char a = 'A';
	bes_size write = 0;
	bes_u64 tell = 0;
	result &= bes_stream_write(&stream, &a, sizeof a, &write);
	result &= bes_stream_tell(&stream, &tell);
	result &= tell == 1;

	test_stream_free(&stream);
	return result;
}

BES_DEFINE_TEST(stream_seek_works)
{
	bes_stream stream;
	if (!test_stream_create(&stream, 1))
	{
		return BES_FALSE;
	}

	bes_bool result = BES_TRUE;
	char a = 'A';
	bes_size write = 0;
	bes_u64 tell = 0;
	result &= bes_stream_write(&stream, &a, sizeof a, &write);
	result &= bes_stream_seek(&stream, 0);
	result &= bes_stream_tell(&stream, &tell);
	result &= tell == 0;

	test_stream_free(&stream);
	return result;
}

BES_DEFINE_TEST(stream_read_works)
{
	bes_stream stream;
	if (!test_stream_create(&stream, 1))
	{
		return BES_FALSE;
	}

	bes_bool result = BES_TRUE;
	char a = 'A';
	bes_size write = 0;
	bes_size read = 0;
	result &= bes_stream_write(&stream, &a, sizeof a, &write);
	result &= bes_stream_seek(&stream, 0);
	result &= bes_stream_read(&stream, &a, sizeof a, &read);
	result &= read == 1;
	result &= a == 'A';

	test_stream_free(&stream);
	return result;
}

BES_DEFINE_TEST_LIST(stream_tests)
{
	BES_ADD_TEST(null_stream_read_is_false),
	BES_ADD_TEST(null_stream_write_is_false),
	BES_ADD_TEST(stream_read_null_data_is_false),
	BES_ADD_TEST(stream_write_null_data_is_false),
	BES_ADD_TEST(stream_read_zero_size_is_false),
	BES_ADD_TEST(stream_write_zero_size_is_false),
	BES_ADD_TEST(stream_read_null_read_is_false),
	BES_ADD_TEST(stream_write_null_write_is_false),
	BES_ADD_TEST(null_stream_seek_is_false),
	BES_ADD_TEST(null_stream_tell_is_false),
	BES_ADD_TEST(stream_tell_null_offset_is_false),
	BES_ADD_TEST(stream_write_works),
	BES_ADD_TEST(stream_tell_works),
	BES_ADD_TEST(stream_seek_works),
	BES_ADD_TEST(stream_read_works)
};

#include <stdio.h>
BES_DEFINE_TEST_COMMAND(test_stream_command, "stream", stream_tests, printf)

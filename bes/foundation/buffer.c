#include <bes/foundation/buffer.h>
#include <bes/foundation/memory.h>
#include <bes/foundation/string.h>

bes_bool
bes_buffer_grow(void **buffer,
                bes_size elements,
                bes_size type_size)
{
	bes_size count = 0;
	void *data = 0;
	if (*buffer)
	{
		bes_buffer_data *const meta = bes_buffer_meta(*buffer);
		count = 2 * meta->data.capacity + elements;
		data = bes_realloc(meta, type_size * count + sizeof *meta);
		if (!data)
		{
			bes_free(meta);
			return BES_FALSE;
		}
	}
	else
	{
		count = elements + 1;
		data = bes_malloc(type_size * count + sizeof(bes_buffer_data));
		if (!data)
		{
			return BES_FALSE;
		}
		((bes_buffer_data *)data)->data.size = 0;
	}

	bes_buffer_data *meta = (bes_buffer_data *)data;
	meta->data.capacity = count;
	*buffer = meta + 1;
	return BES_TRUE;
}

void
bes_buffer_delete(void *const buffer)
{
	/* This function isn't ever called directly, but rather by bes_buffer_free
	 * which does a null pointer check. The meta data calculation here is
	 * otherwise safe provided that variant is held. */
	BES_ASSERT(buffer);
	bes_free(bes_buffer_meta(buffer));
}

bes_bool
bes_buffer_write(BES_BUFFER(bes_byte) *const buffer,
                 const void *const data,
                 bes_size size)
{
	const bes_size current_size = bes_buffer_size(*buffer);
	if (!bes_buffer_expand(*buffer, size))
	{
		return BES_FALSE;
	}
	bes_memcpy(&(*buffer)[current_size], data, size);
	return BES_TRUE;
}

bes_bool
bes_buffer_read(void *const data_,
                bes_size size,
                bes_size *const offset_,
                const BES_BUFFER(bes_byte) const buffer)
{
	BES_ASSERT(data_);
	BES_ASSERT(offset_);
	BES_ASSERT(buffer);

	/* Check to see if the read is within the bounds of the buffer */
	const bes_size buffer_size = bes_buffer_size(buffer);
	if (*offset_ + size > buffer_size)
	{
		return BES_FALSE;
	}

	/* Zero byte reads should always succeed */
	if (size == 0)
	{
		return BES_TRUE;
	}

	bes_memcpy(data_, &buffer[*offset_], size);
	(*offset_) += size;

	return BES_TRUE;
}

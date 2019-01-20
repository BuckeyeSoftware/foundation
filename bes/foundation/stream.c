#include <bes/foundation/stream.h>

bes_bool
bes_stream_read(bes_stream *stream, void *data_, bes_size size, bes_size *read_)
{
	return stream && data_ && size && read_ ? stream->read(stream, data_, size, read_) : BES_FALSE;
}

bes_bool
bes_stream_write(bes_stream *stream, const void *data, bes_size size, bes_size *write_)
{
	return stream && data && size && write_ ? stream->write(stream, data, size, write_) : BES_FALSE;
}

bes_bool
bes_stream_seek(bes_stream *stream, bes_u64 offset)
{
	return stream ? stream->seek(stream, offset) : BES_FALSE;
}

bes_bool
bes_stream_tell(bes_stream *stream, bes_u64 *offset_)
{
	return stream && offset_ ? stream->tell(stream, offset_) : BES_FALSE;
}

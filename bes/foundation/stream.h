#ifndef BES_STREAM_H
#define BES_STREAM_H

#include <bes/foundation/types.h>

/**
 * @defgroup Stream abstraction
 *
 * @brief Abstraction for streams
 *
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct bes_stream bes_stream;

struct bes_stream
{
	/** @brief Read from stream */
	bes_bool (BES_API *read)(bes_stream *stream, void *data_, bes_size size, bes_size *read_);
	/** @brief Write to stream */
	bes_bool (BES_API *write)(bes_stream *stream, const void *data, bes_size size, bes_size *write_);
	/** @brief Seek in stream */
	bes_bool (BES_API *seek)(bes_stream *stream, bes_u64 offset);
	/** @brief Determine where in stream */
	bes_bool (BES_API *tell)(bes_stream *stream, bes_u64 *offset_);
	/** @brief Optional user specified data */
	void *aux;
};

/**
 * @brief Read from stream
 *
 * @param stream The stream to read from
 * @param data_ The data to read into
 * @param size The amount of bytes to read into @p data_
 * @param read_ The amount of bytes actually read into @p data_ stored here
 *
 * @return BES_TRUE if all @p size bytes could be read into @p _data,
 * BES_FALSE if @p stream is NULL, @p data_ is NULL, @p size is zero,
 * @p read_ is NULL or not all @p size bytes could be read.
 */
BES_EXPORT bes_bool BES_API
bes_stream_read(bes_stream *stream, void *data_, bes_size size, bes_size *read_);

/**
 * @brief Write into stream
 *
 * @param stream The stream to write to
 * @param data The data to write
 * @param size The amount of bytes to read from @p data and write into the stream
 * @param write_ The amount of bytes actually written into the stream stored here
 *
 * @return BES_TRUE if all @p size bytes could be written into the stream
 * BES_FALSE if @p stream is NULL, @p data is NULL, @p size is zero, @p
 * write_ is NULL or not all @p size bytes could be written.
 */
BES_EXPORT bes_bool BES_API
bes_stream_write(bes_stream *stream, const void *data, bes_size size, bes_size *write_);

/**
 * @brief Seek to a position in the stream
 *
 * @param stream The stream to seek
 * @param offset The offset to seek to relative to the beginning of the stream
 *
 * @return BES_TRUE if seeking to @p offset succeeded, BES_FALSE if @p
 * stream is NULL or @p offset could not be seeked to.
 */
BES_EXPORT bes_bool BES_API
bes_stream_seek(bes_stream *steam, bes_u64 offset);

/**
 * @brief Determine position in stream
 *
 * @param stream The stream
 * @param offset_ Offset in the stream written here.
 *
 * @return BES_TRUE if the offset could be determined, BES_FALSE if @p
 * stream is NULL, @p offset_ is NULL or offset could not be determined.
 */
BES_EXPORT bes_bool BES_API
bes_stream_tell(bes_stream *stream, bes_u64 *offset_);

#if defined(__cplusplus)
}
#endif

#endif

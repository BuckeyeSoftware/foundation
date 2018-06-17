#ifndef BES_FOUNDATION_BUFFER_H
#define BES_FOUNDATION_BUFFER_H

/**
 * @defgroup Buffer Buffer
 *
 * @brief Flexible dynamic buffer
 *
 * The following is a flexible dynamic buffer type that is used for
 * managing dynamic collections. It's designed to ensure that buffer
 * contents are always aligned by @ref BES_ALIGNMENT for optimal
 * performance.
 *
 * @{
 */

#include <bes/foundation/types.h>
#include <bes/foundation/macros.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef union bes_buffer_data bes_buffer_data;
typedef struct bes_buffer_meta_data bes_buffer_meta_data;

/** @brief The meta data stored as a header on all buffer objects */
struct bes_buffer_meta_data
{
	bes_size capacity; /**< The current capacity of the buffer */
	bes_size size; /**< The amount of elements in the buffer */
};

/**
 * @brief The meta data type itself used to ensure proper alignment of
 * all @ref BES_BUFFER objects.
 */
union bes_buffer_data
{
	bes_buffer_meta_data data; /**< The meta data */

#ifndef BES_DOXYGEN_IGNORE
	/* Ensure that instances of bes_buffer_data stay a multiple of
	 * BES_ALIGNMENT. */
	bes_byte aligned[(sizeof(bes_buffer_meta_data) + BES_ALIGNMENT - 1) & -BES_ALIGNMENT];
#endif
};

#ifndef BES_DOXYGEN_IGNORE
BES_STATIC_ASSERT(sizeof(bes_buffer_data) == BES_ALIGNMENT);
#endif

/**
 * @brief Source code annotation for buffer objects.
 *
 * Buffers are represented as a heap allocated array of their base type
 * permitting them to be used by interfaces that expect plain pointers
 * as well as permitting the use of array indexing. This macro is used
 * to designate those buffers in source code purely as annotation.
 */
#define BES_BUFFER(TYPE) \
	TYPE*

/**
 * @brief Source code annotation for buffer object initialization
 *
 * Buffers are just represented by a pointer, this just sets it to
 * NULL which is required during the testing phase by various macros
 * which implement buffer functionality.
 */
#define BES_BUFFER_INITIALIZER 0

/**
 * @brief Get access to the meta data of a buffer object
 * @param BUFFER The buffer object
 * @note The buffer object must be a valid buffer object
 */
#define bes_buffer_meta(BUFFER) \
	(&((bes_buffer_data *)(BUFFER))[-1])

/**
 * @brief Tries to grow a buffer to accomodate more elements
 *
 * @param BUFFER The buffer object
 * @param SIZE The amount of more elements to resize the buffer to accomodate
 *
 * @note The buffer object must be a valid buffer object
 * @warning This operation only resizes the capacity of the buffer. It
 * does not update the amount of elements that are in use. For that
 * behavior see @ref bes_buffer_expand instead.
 *
 * @return This macro expands to an expression yielding a boolean result
 * that is BES_TRUE on success.
 */
#define bes_buffer_try_grow(BUFFER, SIZE) \
	(((BUFFER) && bes_buffer_meta(BUFFER)->data.size + (SIZE) < bes_buffer_meta(BUFFER)->data.capacity) \
		? BES_TRUE \
		: bes_buffer_grow((void **)&(BUFFER), (SIZE), sizeof *(BUFFER)))

/**
 * @brief Get the number of elements in a buffer
 * @param BUFFER The buffer object
 * @note The buffer object must be a valid buffer object
 */
#define bes_buffer_size(BUFFER) \
	((BUFFER) ? bes_buffer_meta(BUFFER)->data.size : 0)

/**
 * @brief Expand a buffer with more elements
 *
 * @param BUFFER The buffer object
 * @param SIZE The amount of elements to expand by
 * @note The buffer object must be a valid buffer object
 *
 * @return This macro expands to an expression yielding a boolean result
 * that is BES_TRUE on success.
 */
#define bes_buffer_expand(BUFFER, SIZE) \
	(bes_buffer_try_grow((BUFFER), (SIZE)) \
		? (bes_buffer_meta(BUFFER)->data.size += (SIZE), BES_TRUE) \
		: BES_FALSE)

/**
 * @brief Push back a value onto the buffer while resizing if necessary
 *
 * @param BUFFER The buffer object
 * @param VALUE The value to push
 *
 * @note The buffer object must be a valid buffer object
 *
 * @return This macro expands to an expression yielding a boolean result
 * that is BES_TRUE on success.
 */
#define bes_buffer_push(BUFFER, VALUE) \
	(bes_buffer_try_grow((BUFFER), 1) \
		? ((BUFFER)[bes_buffer_meta(BUFFER)->data.size++] = (VALUE), BES_TRUE) \
		: BES_FALSE)

/**
 * @brief Free a buffer object
 *
 * @param BUFFER The buffer object
 * @note The buffer object must be a valid buffer object
 * @note It's safe to pass NULL.
 *
 * @warning Empty buffer objects are represented by NULL. This macro,
 * after freeing the buffer object sets the pointer that represents that
 * buffer to NULL to make that buffer usable again. However, since buffers
 * are represented by pointers, the act of setting to NULL may not
 * propagate when buffers are passed into functions, since only the local
 * pointer will be set to NULL, not the callee passed buffer.
 * Consider passing buffer objects by pointer to ensure operations such
 * as this one actually NULL out the pointer so the buffer object can be
 * reused safely.
 */
#define bes_buffer_free(BUFFER) \
	(void)((BUFFER) ? (bes_buffer_delete(BUFFER), (BUFFER) = 0) : 0)

/**
 * @brief Resize a buffer to specified size
 *
 * @param BUFFER The buffer object
 * @param SIZE The size to resize the buffer object to
 *
 * The following macro expands to a significant amount of code to resize
 * a buffer object to the requested size. The resulting expansion yields
 * a @ref bes_bool result indicating if the operation succeeded or not.
 * The act of resizing a buffer from a larger original size to a smaller
 * one cannot fail. However the opposite operation, resizing the buffer
 * from a smaller one to a larger one can.
 *
 * @return This macro expands to an expression yielding a boolean result
 * that is BES_TRUE on success.
 */
#define bes_buffer_resize(BUFFER, SIZE) \
	((BUFFER) \
		? (bes_buffer_meta(BUFFER)->data.size >= (SIZE) \
			? (bes_buffer_meta(BUFFER)->data.size = (SIZE), BES_TRUE) \
			: bes_buffer_expand((BUFFER), (SIZE) - bes_buffer_meta(BUFFER)->data.size)) \
		: (bes_buffer_grow((void **)&(BUFFER), (SIZE), sizeof *(BUFFER)) \
			? (bes_buffer_meta(BUFFER)->data.size = (SIZE), BES_TRUE) \
			: BES_FALSE))

/**
 * @brief Clear the contents of the buffer
 * @param BUFFER The buffer object
 * @note The buffer object must be a valid buffer object
 */
#define bes_buffer_clear(BUFFER) \
	(void)((BUFFER) ? bes_buffer_meta(BUFFER)->data.size = 0 : 0)

/**
 * @brief Generic resizing function used by the macros above
 *
 * @param buffer The buffer object
 * @param elements The amount of elements to append
 * @param type_size The size of the type the buffer object encapsulates
 *
 * @return On success the function returns BES_TRUE.
 */
BES_EXPORT bes_bool BES_API
bes_buffer_grow(void **const buffer,
                bes_size elements,
                bes_size type_size);

/**
 * @brief Delete a buffer object
 * @param buffer The buffer object
 * @warning Do not pass an empty buffer object to this function.
 * @warning Do not use this function directly, use @ref bes_buffer_free
 * instead. The former ensures the buffer object isn't empty and resets
 * the pointer to make it safe to reuse.
 */
BES_EXPORT void BES_API
bes_buffer_delete(void *const buffer);

/**
 * @brief Write arbitrary data into a byte buffer
 * @param buffer_ Pointer to the byte buffer object
 * @param data The arbitrary data to write to the byte buffer
 * @param size The amount of bytes to read from @p data and write into @p buffer
 * @return On success this function returns BES_TRUE.
 */
BES_EXPORT bes_bool BES_API
bes_buffer_write(BES_BUFFER(bes_byte) *const buffer_,
                 const void *const data,
                 bes_size size);

/**
 * @brief Read arbitrary data from a byte buffer
 * @param data_ The place to store the data
 * @param size The amount of bytes to copy out of the buffer
 * @param offset_ The offset in the buffer, automatically incremented
 * on successful read.
 * @param buffer The byte buffer
 * @return On success this function returns BES_TRUE.
 */
BES_EXPORT bes_bool BES_API
bes_buffer_read(void *const data_,
                bes_size size,
                bes_size *const offset_,
                const BES_BUFFER(bes_byte) const buffer);


#if defined(__cplusplus)
}
#endif

#endif

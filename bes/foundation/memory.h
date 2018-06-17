#ifndef BES_FOUNDATION_MEMORY_H
#define BES_FOUNDATION_MEMORY_H

/**
 * @defgroup Memory Memory allocation
 *
 * @brief Heap allocation abstraction interface
 *
 * @{
 */
#include <bes/foundation/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Interface used to describe an allocator
 *
 * The functions supplied by this structure must be implemented following
 * the same requirements as malloc, realloc and free.
 */
typedef struct bes_allocator bes_allocator;
struct bes_allocator
{
	/** @brief The allocation function */
	void* (BES_API *allocate)(bes_allocator *allocator, bes_size size);
	/** @brief The reallocation function */
	void* (BES_API *reallocate)(bes_allocator *allocator, void *data, bes_size size);
	/** @brief The deallocation function */
	void (BES_API *deallocate)(bes_allocator *allocator, void *data);
	/** @brief Optional user specified data */
	void *aux;
};

/**
 * @brief Set the allocator to carry out allocations
 * @param allocator The allocator to use for the calling thread
 * @note An allocator must be supplied.
 * @warning The allocator is thread local and must be set by every calling thread
 * @warning If no allocator is specified foundation will terminate.
 * @return If the allocator specified doesn't implement the full interface
 * this will return BES_FALSE to indicate failure.
 */
BES_EXPORT bes_bool BES_API
bes_allocator_set(bes_allocator *const allocator);

/**
 * @brief Allocate memory
 * @param size The size of the allocation request
 * @return On failure this function returns NULL
 */
BES_EXPORT void* BES_API
bes_malloc(bes_size size);

/**
 * @brief Reallocate memory
 * @param ptr The original pointer to resize the allocation of
 * @param size The size to resize the allocation to
 * @return On failure this function returns NULL
 */
BES_EXPORT void* BES_API
bes_realloc(void *const ptr, bes_size size);

/**
 * @brief Free memory
 * @param ptr The pointer to free
 * @note It's safe to pass NULL.
 */
BES_EXPORT void BES_API
bes_free(void *const ptr);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif

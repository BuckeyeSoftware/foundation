#include <bes/foundation/memory.h>

static bes_allocator *g_bes_allocator;

bes_bool
bes_allocator_set(bes_allocator *const allocator)
{
	BES_ASSERT(allocator);

	if (allocator->allocate && allocator->reallocate && allocator->deallocate)
	{
		g_bes_allocator = allocator;
		return BES_TRUE;
	}

	return BES_FALSE;
}

static inline void*
bes_allocator_allocate(bes_size size)
{
	BES_ASSERT(g_bes_allocator);
	return g_bes_allocator->allocate(g_bes_allocator, size);
}

static inline void*
bes_allocator_reallocate(void *const data, bes_size size)
{
	BES_ASSERT(g_bes_allocator);
	return g_bes_allocator->reallocate(g_bes_allocator, data, size);
}

static inline void
bes_allocator_deallocate(void *const data)
{
	/* There's no need to do the assertion check here since it's not
	 * legal to call free with pointers that were not obtained with
	 * the other functions. The other ones would fail sooner. */
	g_bes_allocator->deallocate(g_bes_allocator, data);
}

/* Ensure all allocations are BES_ALIGNED aligned. */
typedef union bes_alloc_header bes_alloc_header;
typedef struct bes_alloc_header_data bes_alloc_header_data;

struct bes_alloc_header_data
{
	bes_size size;
	bes_byte *base;
};

union bes_alloc_header
{
	bes_alloc_header_data data;

	/* Ensure the allocation header is a multiple of the alignment too */
	bes_byte aligned[(sizeof(bes_alloc_header_data) + BES_ALIGNMENT - 1) & -BES_ALIGNMENT];
};

void*
bes_malloc(bes_size size)
{
	BES_ASSERT(g_bes_allocator);

	/* The size doesn't need to be a multiple of the alignment, however it's
	 * good to reduce fragmentation. */
	size = (size + BES_ALIGNMENT - 1) & -BES_ALIGNMENT;

	/* Additional memory may be needed to align base of the allocation */
	bes_byte *base = bes_allocator_allocate(size + sizeof(bes_alloc_header) + BES_ALIGNMENT);
	if (base)
	{
		/* Round up to the alignment, making a small gap if necessary. */
		bes_byte *aligned = (bes_byte *)((bes_uintptr)(base + sizeof(bes_alloc_header) + BES_ALIGNMENT - 1) & -BES_ALIGNMENT);

		bes_alloc_header *node = (bes_alloc_header*)aligned - 1;
		node->data.base = base;
		node->data.size = size;

		return aligned;
	}

	return 0;
}

void*
bes_realloc(void *const ptr, bes_size size)
{
	BES_ASSERT(g_bes_allocator);

	if (ptr)
	{
		/* The size doesn't need to be a multiple of the alignment, however
		 * it's good to reduce fragmentation. */
		size = (size + BES_ALIGNMENT - 1) & -BES_ALIGNMENT;

		bes_alloc_header *node = (bes_alloc_header *)ptr - 1;
		bes_byte *original = node->data.base;
		bes_byte *resize = bes_allocator_reallocate(original, size + sizeof(bes_alloc_header) + BES_ALIGNMENT);
		if (resize)
		{
			/* Round to the alignment, making a small gap if necessary. */
			bes_byte *aligned = (bes_byte *)((bes_uintptr)(resize + sizeof(bes_alloc_header) + BES_ALIGNMENT - 1) & -BES_ALIGNMENT);

			bes_alloc_header *meta = (bes_alloc_header*)aligned - 1;
			meta->data.size = size;
			meta->data.base = resize;

			return aligned;
		}

		return 0;
	}

	return bes_malloc(size);
}

void
bes_free(void *const ptr)
{
	BES_ASSERT(g_bes_allocator);

	if (ptr)
	{
		bes_alloc_header *node = (bes_alloc_header *)ptr - 1;
		bes_allocator_deallocate(node);
	}
}

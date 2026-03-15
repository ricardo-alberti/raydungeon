void*
arena_push(Arena* arena, size_t size, size_t alignment)
{
    if (!size) 
    {
        fprintf(stderr, "Error: invalid alloc size\n");
        exit(1);
    }

    size_t current = (size_t)arena->base + arena->used;
    size_t offset = (alignment - (current % alignment)) % alignment;

    if (arena->used + offset + size > arena->size)
    {
        fprintf(stderr, "Error: could not allocate %zu bytes for arena\n", size);
        exit(1);
    }

    void* result = (char*)arena->base + arena->used + offset;
    arena->used += offset + size;

    return result;
}

Arena
arena_alloc(size_t size)
{
    Arena arena = { 0 };
    arena.base = malloc(size);
    arena.size = size;
    arena.used = 0;

    if (!arena.base)
    {
        fprintf(stderr, "Error: could not allocate %zu bytes for arena\n", size);
        exit(1);
    }

    return arena;
}

void
arena_reset(Arena* arena)
{
    arena->used = 0;
}

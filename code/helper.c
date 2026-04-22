void
zero_memory(void* ptr, size_t size)
{
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < size; ++i)
    {
        p[i] = 0;
    }
}

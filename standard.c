#include "standard.h"

/*
 * copies at most <size-1> chars from <src> to <dst>. Last char is always
 * set to 0, unless <size> is 0. The number of chars copied is returned
 * (excluding the terminating zero).
 * This code has been optimized for size and speed : on x86, it's 45 bytes
 * long, uses only registers, and consumes only 4 cycles per char.
 */
int32_t strlcpy(int8_t *dst, const int8_t *src, int32_t size)
{
    int8_t *orig = dst;
    if (size)
    {
        while (--size && (*dst = *src))
        {
            src++; dst++;
        }
        *dst = 0;
    }
    return dst - orig;
}

/*
 * ScanParse.c - implementation of the tiny sscanf() replacements declared
 * in ScanParse.h. See the header for usage and semantics.
 */
#include "ScanParse.h"
#include <limits.h>

int scan_dec(const char **pp, int *out)
{
    const char *p = *pp;
    const char *start;
    int          sign = 1;
    unsigned int n    = 0;
    unsigned int limit;
    int          overflow = 0;

    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' ||
           *p == '\v' || *p == '\f')
        p++;

    if (*p == '-')      { sign = -1; p++; }
    else if (*p == '+') { p++; }

    /* Largest absolute value representable in the result: INT_MAX for a
     * positive result, |INT_MIN| (== (unsigned)INT_MAX + 1u under two's
     * complement, which is all targets we care about) for a negative
     * result. Accumulate in unsigned so the arithmetic itself never
     * invokes UB; detect overflow against this limit before each add. */
    limit = (sign < 0) ? (unsigned int)INT_MAX + 1u
                       : (unsigned int)INT_MAX;

    start = p;
    while (*p >= '0' && *p <= '9')
    {
        unsigned int d = (unsigned int)(*p - '0');
        if (!overflow)
        {
            if (n > limit / 10u || (n == limit / 10u && d > limit % 10u))
                overflow = 1;
            else
                n = n * 10u + d;
        }
        p++;
    }
    if (p == start)
        return 0; /* no digits consumed */

    if (overflow)
        *out = (sign < 0) ? INT_MIN : INT_MAX;
    else if (sign < 0)
        /* n is in [0, |INT_MIN|]; -(int)n would be UB for n == |INT_MIN|
         * since INT_MAX + 1 is not representable as int, so handle the
         * boundary explicitly. */
        *out = (n == (unsigned int)INT_MAX + 1u) ? INT_MIN : -(int)n;
    else
        *out = (int)n;

    *pp = p;
    return 1;
}

int scan_hex(const char **pp, unsigned int *out)
{
    const char *p = *pp;
    const char *start;
    unsigned int n = 0;
    int          overflow = 0;

    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' ||
           *p == '\v' || *p == '\f')
        p++;

    /* Optional 0x / 0X prefix. We commit to it here, but only ever
     * publish the new cursor on a successful overall parse, so a bare
     * prefix with no hex digit following still looks like a no-op to
     * the caller (same as sscanf %x). */
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
        p += 2;

    start = p;
    for (;;)
    {
        unsigned int d;
        char         c = *p;
        if      (c >= '0' && c <= '9') d = (unsigned int)(c - '0');
        else if (c >= 'a' && c <= 'f') d = (unsigned int)(c - 'a') + 10u;
        else if (c >= 'A' && c <= 'F') d = (unsigned int)(c - 'A') + 10u;
        else break;
        if (!overflow)
        {
            /* If any bit of the top nibble is set, a left shift by 4
             * would drop it; clamp instead. */
            if (n > (UINT_MAX >> 4))
                overflow = 1;
            else
                n = (n << 4) | d;
        }
        p++;
    }
    if (p == start)
        return 0; /* no digits consumed */

    *out = overflow ? UINT_MAX : n;
    *pp  = p;
    return 1;
}

int scan_char(const char **pp, char c)
{
    if (**pp != c)
        return 0;
    (*pp)++;
    return 1;
}

int scan_lit(const char **pp, const char *s)
{
    const char *p = *pp;
    const char *q = s;
    while (*q)
    {
        if (*p != *q)
            return 0;
        p++;
        q++;
    }
    *pp = p;
    return 1;
}

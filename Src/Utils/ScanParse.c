/*
 * ScanParse.c - implementation of the tiny sscanf() replacements declared
 * in ScanParse.h. See the header for usage and semantics.
 */
#include "ScanParse.h"

int scan_skip_ws(const char **pp)
{
    const char *p = *pp;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' ||
           *p == '\v' || *p == '\f')
        p++;
    *pp = p;
    return 1;
}

int scan_dec(const char **pp, int *out)
{
    const char *p = *pp;
    const char *start;
    int sign = 1;
    int n    = 0;

    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' ||
           *p == '\v' || *p == '\f')
        p++;

    if (*p == '-')      { sign = -1; p++; }
    else if (*p == '+') { p++; }

    start = p;
    while (*p >= '0' && *p <= '9')
    {
        n = n * 10 + (int)(*p - '0');
        p++;
    }
    if (p == start)
        return 0; /* no digits consumed */

    *out = sign * n;
    *pp  = p;
    return 1;
}

int scan_hex(const char **pp, unsigned int *out)
{
    const char *p = *pp;
    const char *start;
    unsigned int n = 0;

    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' ||
           *p == '\v' || *p == '\f')
        p++;

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
        n = (n << 4) | d;
        p++;
    }
    if (p == start)
        return 0; /* no digits consumed */

    *out = n;
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

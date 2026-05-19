/*
 * ScanParse.h - tiny zero-allocation sscanf() replacements for the narrow
 * uses we need in blueMSX-libretro.
 *
 * sscanf() is slow and (on glibc) calls malloc() internally for any non-
 * trivial format string. The helpers here are hand-rolled finite-state
 * scanners that touch no heap and only the characters they consume.
 *
 * Each scanner takes a (const char **) cursor. On success it advances the
 * cursor past the consumed input and returns 1. On failure it returns 0
 * and leaves the cursor untouched, so callers can chain checks with &&.
 *
 * Number scanners mimic sscanf's "%d" / "%x" behaviour of skipping leading
 * whitespace before the number itself. They are intentionally minimal:
 * no locale awareness, no overflow check beyond modular wrap. Use them
 * only for trusted / format-controlled input (config files, debugger
 * commands, libretro cheat strings, etc.).
 */
#ifndef SCAN_PARSE_H
#define SCAN_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Skip ASCII whitespace at the cursor. Always succeeds. */
int scan_skip_ws(const char **pp);

/* Parse a signed decimal integer (sscanf %d semantics): skip leading
 * whitespace, optional +/- sign, one or more decimal digits. */
int scan_dec(const char **pp, int *out);

/* Parse an unsigned hexadecimal integer (sscanf %x semantics): skip
 * leading whitespace, optional 0x/0X prefix, one or more hex digits. */
int scan_hex(const char **pp, unsigned int *out);

/* If the next character matches c exactly, consume it and return 1. */
int scan_char(const char **pp, char c);

/* If the next characters match the literal s exactly, consume them and
 * return 1. Does NOT skip whitespace. */
int scan_lit(const char **pp, const char *s);

#ifdef __cplusplus
}
#endif

#endif

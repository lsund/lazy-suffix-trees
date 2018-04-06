
#include "utf.h"

/**
 * Reads the next UTF-8-encoded character from the byte array ranging
 * from {@code *pstart} up to, but not including, {@code end}. If the
 * conversion succeeds, the {@code *pstart} iterator is advanced,
 * the codepoint is stored into {@code *pcp}, and the function returns
 * 0. Otherwise the conversion fails, {@code errno} is set to
 * {@code EILSEQ} and the function returns -1.
 */
int from_utf8(Uchar **pstart, Uchar *end, Uint *pcp)
{
        size_t len, i;
        Uint cp, min;
        Uchar *buf;

        buf = *pstart;
        if (buf == end)
                goto error;

        if (buf[0] < 0x80) {
                len = 1;
                min = 0;
                cp = buf[0];
        } else if (buf[0] < 0xC0) {
                goto error;
        } else if (buf[0] < 0xE0) {
                len = 2;
                min = 1 << 7;
                cp = buf[0] & 0x1F;
        } else if (buf[0] < 0xF0) {
                len = 3;
                min = 1 << (5 + 6);
                cp = buf[0] & 0x0F;
        } else if (buf[0] < 0xF8) {
                len = 4;
                min = 1 << (4 + 6 + 6);
                cp = buf[0] & 0x07;
        } else {
                goto error;
        }

        if (buf + len > end)
                goto error;

        for (i = 1; i < len; i++) {
                if ((buf[i] & 0xC0) != 0x80)
                        goto error;
                cp = (cp << 6) | (buf[i] & 0x3F);
        }

        if (cp < min)
                goto error;

        if (0xD800 <= cp && cp <= 0xDFFF)
                goto error;

        if (0x110000 <= cp)
                goto error;

        *pstart += len;
        *pcp = cp;
        return 0;

error:
        errno = EILSEQ;
        return -1;
}

/* static void assert_valid(const byte **buf, const byte *end, codepoint expected) */
/* { */
/*         codepoint cp; */

/*         if (from_utf8(buf, end, &cp) == -1) { */
/*                 fprintf(stderr, "invalid unicode sequence for codepoint %u\n", expected); */
/*                 exit(EXIT_FAILURE); */
/*         } */

/*         if (cp != expected) { */
/*                 fprintf(stderr, "expected %u, got %u\n", expected, cp); */
/*                 exit(EXIT_FAILURE); */
/*         } */
/* } */

/* static void assert_invalid(const char *name, const byte **buf, const byte *end) */
/* { */
/*         const byte *p; */
/*         codepoint cp; */

/*         p = *buf + 1; */
/*         if (from_utf8(&p, end, &cp) == 0) { */
/*                 fprintf(stderr, "unicode sequence \"%s\" unexpectedly converts to %#x.\n", name, cp); */
/*                 exit(EXIT_FAILURE); */
/*         } */
/*         *buf += (*buf)[0] + 1; */
/* } */

/* static const byte valid[] = { */
/*         0x00, /1* first ASCII *1/ */
/*         0x7F, /1* last ASCII *1/ */
/*         0xC2, 0x80, /1* first two-byte *1/ */
/*         0xDF, 0xBF, /1* last two-byte *1/ */
/*         0xE0, 0xA0, 0x80, /1* first three-byte *1/ */
/*         0xED, 0x9F, 0xBF, /1* last before surrogates *1/ */
/*         0xEE, 0x80, 0x80, /1* first after surrogates *1/ */
/*         0xEF, 0xBF, 0xBF, /1* last three-byte *1/ */
/*         0xF0, 0x90, 0x80, 0x80, /1* first four-byte *1/ */
/*         0xF4, 0x8F, 0xBF, 0xBF /1* last codepoint *1/ */
/* }; */

/* static const byte invalid[] = { */
/*         1, 0x80, */
/*         1, 0xC0, */
/*         1, 0xC1, */
/*         2, 0xC0, 0x80, */
/*         2, 0xC2, 0x00, */
/*         2, 0xC2, 0x7F, */
/*         2, 0xC2, 0xC0, */
/*         3, 0xE0, 0x80, 0x80, */
/*         3, 0xE0, 0x9F, 0xBF, */
/*         3, 0xED, 0xA0, 0x80, */
/*         3, 0xED, 0xBF, 0xBF, */
/*         4, 0xF0, 0x80, 0x80, 0x80, */
/*         4, 0xF0, 0x8F, 0xBF, 0xBF, */
/*         4, 0xF4, 0x90, 0x80, 0x80 */
/* }; */

/* int main() { */
/*         const byte *p, *end; */

/*         p = valid; */
/*         end = valid + sizeof valid; */
/*         assert_valid(&p, end, 0x000000); */
/*         assert_valid(&p, end, 0x00007F); */
/*         assert_valid(&p, end, 0x000080); */
/*         assert_valid(&p, end, 0x0007FF); */
/*         assert_valid(&p, end, 0x000800); */
/*         assert_valid(&p, end, 0x00D7FF); */
/*         assert_valid(&p, end, 0x00E000); */
/*         assert_valid(&p, end, 0x00FFFF); */
/*         assert_valid(&p, end, 0x010000); */
/*         assert_valid(&p, end, 0x10FFFF); */

/*         p = invalid; */
/*         end = invalid + sizeof invalid; */
/*         assert_invalid("80", &p, end); */
/*         assert_invalid("C0", &p, end); */
/*         assert_invalid("C1", &p, end); */
/*         assert_invalid("C0 80", &p, end); */
/*         assert_invalid("C2 00", &p, end); */
/*         assert_invalid("C2 7F", &p, end); */
/*         assert_invalid("C2 C0", &p, end); */
/*         assert_invalid("E0 80 80", &p, end); */
/*         assert_invalid("E0 9F BF", &p, end); */
/*         assert_invalid("ED A0 80", &p, end); */
/*         assert_invalid("ED BF BF", &p, end); */
/*         assert_invalid("F0 80 80 80", &p, end); */
/*         assert_invalid("F0 8F BF BF", &p, end); */
/*         assert_invalid("F4 90 80 80", &p, end); */

/*         return 0; */
/* } */

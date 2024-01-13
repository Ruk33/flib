#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include "text.h" 

size_t letters(char *src)
{
    if (!src)
        return 0;
    
    size_t r = 0;
    
    while (*src) {
        if ((*src & 0xC0) != 0x80)
            r++;
        src++;
    }
    
    return r;
}

size_t bytes(char *src)
{
    if (!src)
        return 0;
    
    // +1 null terminator.
    size_t r = strlen(src) + 1;
    return r;
}

unsigned int letter(char *src, char **next)
{
    if (!src)
        return 0;
    
    unsigned int r = 0;
    if ((*src & 0xC0) != 0x80) {
        int size = 1;
        
        if ((*src & 0xE0) == 0xC0)
            size = 2;
        else if ((*src & 0xF0) == 0xE0)
            size = 3;
        else if ((*src & 0xF8) == 0xF0)
            size = 4;
        
        memcpy(&r, src, size);
        if (next)
            *next = src + size;
    }
    return r;
}

int begins_withn(char *src, char *what, size_t n)
{
    if (!src || !what)
        return 0;
    
    int r = strncmp(src, what, n) == 0;
    return r;
}

int begins_with(char *src, char *what)
{
    if (!src || !what)
        return 0;
    
    int r = begins_withn(src, what, strlen(what));
    return r;
}

int ends_withn(char *src, char *what, size_t n)
{
    if (!src || !what)
        return 0;
    
    size_t src_len = strlen(src);
    if (n > src_len)
        return 0;
    
    src += src_len - n;
    int r = strncmp(src, what, n) == 0;
    return r;
}

int ends_with(char *src, char *what)
{
    if (!src || !what)
        return 0;
    
    int r = ends_withn(src, what, strlen(what));
    return r;
}

int contains(char *src, char *what)
{
    if (!src || !what)
        return 0;
    
    int r = strstr(src, what) != 0;
    return r;
}

char *find(char *src, char *what)
{
    if (!src || !what)
        return 0;
    
    char *r = strstr(src, what);
    return r;
}

int find_index(char *src, char *what)
{
    char *first = find(src, what);
    int r = -1;
    if (first)
        r = (int) (size_t) (first - src);
    return r;
}

char *find_last(char *src, char *what)
{
    if (!src || !what)
        return 0;
    
    char *tail = src;
    char *last = 0;
    while (1) {
        tail = strstr(tail, what);
        if (!tail)
            break;
        last = tail;
        tail++;
    }
    return last;
}

int find_last_index(char *src, char *what)
{
    char *last = find_last(src, what);
    int r = -1;
    if (last)
        r = (int) (size_t) (last - src);
    return r;
}

char *from(char *src, int n)
{
    if (!src)
        return 0;
    
    size_t src_len = strlen(src);
    char *r = src;
    if (n >= 0) {
        r += src_len > n ? n : src_len;
    } else {
        r += src_len;
        n = -n;
        r -= src_len > n ? n : src_len;
    }
    return r;
}

void insertn(char *src, char *what, size_t max, int n)
{
    if (!src || !what)
        return;
    
    char *insert_position = from(src, n);
    size_t len_from_insert = strlen(insert_position);
    memmove(insert_position + max,
            insert_position,
            len_from_insert);
    memcpy(insert_position, what, max);
}

void insert(char *src, char *what, int n)
{
    insertn(src, what, strlen(what), n);
}

void erase_bytes(char *src, int n, size_t bytes)
{
    if (!src || !bytes)
        return;
    
    char *erase_position = from(src, n);
    size_t len_from_erase = strlen(erase_position);
    bytes = bytes < len_from_erase ? bytes : len_from_erase;
    memmove(erase_position,
            erase_position + bytes,
            len_from_erase);
}

void erase(char *src, char *what)
{
    if (!src || !what)
        return;
    
    char *tail = src;
    size_t what_len = strlen(what);
    while (1) {
        tail = find(tail, what);
        if (!tail)
            break;
        erase_bytes(tail, 0, what_len);
    }
}

void erase_first(char *src, char *what)
{
    char *first = find(src, what);
    if (first)
        erase_bytes(first, 0, strlen(what));
}

void erase_last(char *src, char *what)
{
    if (!src || !what)
        return;
    
    char *tail = src;
    char *last = 0;
    while (1) {
        tail = find(tail, what);
        if (!tail)
            break;
        last = tail;
        tail++;
    }
    if (last)
        erase_bytes(last, 0, strlen(what));
}

void replace(char *src, char *original, char *replacement)
{
    if (!src || !original || !replacement)
        return;
    
    size_t original_len = strlen(original);
    char *tail = src;
    while (1) {
        tail = find(tail, original);
        if (!tail)
            break;
        erase_bytes(tail, 0, original_len);
        insert(tail, replacement, 0);
    }
}

void replace_first(char *src, char *original, char *replacement)
{
    char *first = find(src, original);
    if (!first)
        return;
    erase_bytes(first, 0, strlen(original));
    insert(first, replacement, 0);
}

void replace_last(char *src, char *original, char *replacement)
{
    if (!src || !original || !replacement)
        return;
    
    char *tail = src;
    char *last = 0;
    while (1) {
        tail = find(tail, original);
        if (!tail)
            break;
        last = tail;
        tail++;
    }
    if (!last)
        return;
    erase_bytes(last, 0, strlen(original));
    insert(last, replacement, 0);
}

int empty(char *src)
{
    int r = !src || !*src;
    return r;
}

void appendn(char *src, char *what, size_t n)
{
    if (!src || !what || !n)
        return;
    strncat(src, what, n);
}

void append(char *src, char *what)
{
    appendn(src, what, strlen(what));
}

void prependn(char *src, char *what, size_t n)
{
    if (!src || !what || !n)
        return;
    memmove(src + n, src, strlen(src));
    strncpy(src, what, n);
}

void prepend(char *src, char *what)
{
    prependn(src, what, strlen(what));
}

void trim(char *src)
{
    if (!src)
        return;
    
    char *left_space = src;
    while (isspace(*left_space))
        left_space++;
    if (src != left_space)
        memmove(src, left_space, strlen(left_space));
    
    size_t src_len = strlen(src);
    if (!src_len)
        return;
    char *right_space = src + src_len - 1;
    while (isspace(*right_space)) {
        *right_space = 0;
        right_space--;
    }
}

void substr(char *src, int n, size_t max)
{
    if (!src || !max)
        return;
    
    char *at_n = from(src, n);
    size_t at_len = strlen(at_n);
    max = at_len > max ? max : at_len;
    memmove(src, at_n, max);
    memset(at_n + max, 0, at_len - max);
}

void repeat(char *src, char *what, int n, size_t times)
{
    if (!src || !what || !times)
        return;
    
    char *at_n = from(src, n);
    for (size_t i = 0; i < times; i++)
        insert(at_n, what, 0);
}

int uriscan_va(char *src, char *fmt, va_list va)
{
    while (*src && *fmt) {
        if (*fmt == '%' && *(fmt + 1) == 's') {
            fmt++;
            fmt++;
            char *dest = va_arg(va, char *);
            while (*src && *src != *fmt)
                *dest++ = *src++;
            continue;
        }
        if (*fmt == '%' && *(fmt + 1) == 'd') {
            int *dest = va_arg(va, int *);
            if (sscanf(src, "%d", dest) != 1)
                break;
            while (*src && *src >= '0' && *src <= '9') {
                src++;
            }
            continue;
        }
        if (*src == *fmt) {
            src++;
            fmt++;
            continue;
        }
        break;
    }
    return *src == 0 && *fmt == 0;
}

int uriscan(char *src, char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int r = uriscan_va(src, fmt, va);
    va_end(va);
    return r;
}

int vfprintf2(FILE *dest, va_list va)
{
    int r = 0;
    while (1) {
        int fmt = va_arg(va, int);
        if (fmt == 0) 
            break;
        int tmp = 0;
        switch (fmt) {
            case 1: {
                int src = va_arg(va, int);
                tmp = fprintf(dest, "%d", src);
            } break;
            case 2: {
                char *src = va_arg(va, char *);
                tmp = fprintf(dest, "%s", src);
            } break;
            case 3: {
                int n = va_arg(va, int);
                char *src = va_arg(va, char *);
                tmp = fprintf(dest, "%.*s", n, src);
            } break;
            case 4: {
                double src = va_arg(va, double);
                tmp = fprintf(dest, "%f", src);
            } break;
            case 5: {
                unsigned int src = va_arg(va, unsigned int);
                tmp = fprintf(dest, "%u", src);
            } break;
            case 6: {
                char src = va_arg(va, char);
                tmp = fprintf(dest, "%c", src);
            } break;
            case 7: {
                void *src = va_arg(va, void *);
                tmp = fprintf(dest, "%p", src);
            } break;
            case 8: {
                long long src = va_arg(va, long long);
                tmp = fprintf(dest, "%ll", src);
            } break;
            case 9: {
                size_t src = va_arg(va, size_t);
                tmp = fprintf(dest, "%z", src);
            } break;
            case 10: {
                unsigned long long src = va_arg(va, unsigned long long);
                tmp = fprintf(dest, "%llu", src);
            } break;
            default:
            tmp = -1;
            break;
        }
        if (tmp == -1) {
            r = -r;
            break;
        }
        r += tmp;
    }
    return r;
}

int fprintf2_(FILE *dest, ...)
{
    va_list va;
    va_start(va, dest);
    int r = vfprintf2(dest, va);
    va_end(va);
    return r;
}

int vconcat(char *dest, size_t n, va_list va)
{
    int r = 0;
    for (size_t i = 0; i < n;) {
        int fmt = va_arg(va, int);
        if (fmt == 0) 
            break;
        size_t left = n - i;
        int tmp = 0;
        switch (fmt) {
            case 1: {
                int src = va_arg(va, int);
                tmp = snprintf(dest, left, "%d", src);
            } break;
            case 2: {
                char *src = va_arg(va, char *);
                tmp = snprintf(dest, left, "%s", src);
            } break;
            case 3: {
                int n = va_arg(va, int);
                char *src = va_arg(va, char *);
                tmp = snprintf(dest, left, "%.*s", n, src);
            } break;
            case 4: {
                double src = va_arg(va, double);
                tmp = snprintf(dest, left, "%f", src);
            } break;
            case 5: {
                unsigned int src = va_arg(va, unsigned int);
                tmp = snprintf(dest, left, "%u", src);
            } break;
            case 6: {
                char src = va_arg(va, char);
                tmp = snprintf(dest, left, "%c", src);
            } break;
            case 7: {
                void *src = va_arg(va, void *);
                tmp = snprintf(dest, left, "%p", src);
            } break;
            case 8: {
                long long src = va_arg(va, long long);
                tmp = snprintf(dest, left, "%ll", src);
            } break;
            case 9: {
                size_t src = va_arg(va, size_t);
                tmp = snprintf(dest, left, "%z", src);
            } break;
            case 10: {
                unsigned long long src = va_arg(va, unsigned long long);
                tmp = snprintf(dest, left, "%llu", src);
            } break;
            default:
            tmp = -1;
            break;
        }
        if (tmp == -1) {
            r = -r;
            break;
        }
        i += tmp;
        dest += tmp;
        r += tmp;
    }
    *dest = 0;
    return r;
}

int concat_(char *dest, size_t n, ...)
{
    va_list va;
    va_start(va, n);
    int r = vconcat(dest, n, va);
    va_end(va);
    return r;
}

#ifdef run_text
// example on how to use these functions
int main()
{
    int tests = 0;
    
#define test(x) \
(assert((x) && "failed " #x), fprintf(stderr, "passed - %s\n", #x), tests++)
    
    fprintf(stderr, "\n\nrunning tests from text.c\n\n");
    
    char buf[256] = "lorem ipsum dolor sit amet";
    
    test(begins_with(buf, "lorem"));
    test(ends_with(buf, "amet"));
    test(!ends_with(buf, "ame"));
    test(contains(buf, "dolor"));
    test(begins_with(from(buf, -4), "amet"));
    
#if 1
    test(letters("世界") == 2);
#endif
    
#if 1
    test(bytes("世界") == sizeof("世界"));
#endif
    
#if 1
    test(!contains(buf, "franco"));
    insert(buf, "franco ", find_index(buf, "ipsum"));
#endif
    
#if 1
    test(contains(buf, "franco "));
    erase(buf, "franco ");
    test(!contains(buf, "franco "));
#endif
    
#if 1
    test(!contains(buf, "foo"));
    replace(buf, "ipsum", "foo");
    test(!contains(buf, "ipsum"));
    test(contains(buf, "foo"));
#endif
    
#if 1
    append(buf, " this is a new ending");
    test(ends_with(buf, " this is a new ending"));
#endif
    
#if 1
    prepend(buf, "this is a new beginning ");
    test(begins_with(buf, "this is a new beginning "));
#endif
    
#if 1
    prepend(buf, "        ");
    append(buf, "  ");
    trim(buf);
    test(!begins_with(buf, " "));
    test(!ends_with(buf, " "));
#endif
    
#if 1
    substr(buf, 0, 4);
    test(letters(buf) == 4);
    test(begins_with(buf, "this"));
#endif
    
#if 1
    // utf8
    prepend(buf, "世界");
    
    char *next_letter = 0;
    unsigned int utf8_letter = letter(buf, &next_letter);
    // 4 from next letter + 1 for null terminator.
    char printable_utf8_letter[5] = {0};
    memcpy(printable_utf8_letter, &utf8_letter, 4);
#endif
    
#if 1
    repeat(buf, "repeatme", 0, 3);
    test(begins_with(buf, "repeatmerepeatmerepeatme"));
#endif
    
    char name[] = "franco";
    int answer = 42;
    debug(s "this is an experimental way of printing a message. "
          s "there were " d tests s " completed tests! "
          s "you " s name s " may be wondering how this is all possible? "
          s "well, the answer is " d answer s "\n");
    
#if 1
    char dest[32] = {0};
    int printed = concat(dest, sizeof(dest), s "this " s name s " is also great. but then again " d answer);
    debug(s "dest is -> '" s dest s "' and " d printed s " bytes were printed\n");
#endif
    
    debug(s "all good!\n");
    
    return 0;
}
#endif

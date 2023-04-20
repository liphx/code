#if 0

// copy n bytes from s2 to s1, return s1, UB if overlap
void *memcpy(void *restrict s1, const void *restrict s2, size_t n);

// copy n bytes from s2 to s1, ok if overlap
void *memmove(void *s1, const void *s2, size_t n);

// copy string from s2 to s1, including '\0', UB if overlap
char *strcpy(char *restrict s1, const char *restrict s2);

// like strcpy, copy no more than n bytes
char *strncpy(char *restrict s1, const char *restrict s2, size_t n);

// append string from s2 to s1, including '\0', UB if overlap
char *strcat(char *restrict s1, const char *restrict s2);

// like strcat, append no more than n bytes
char *strncat(char *restrict s1, const char *restrict s2, size_t n);

#endif

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void *memcpy_(void *restrict s1, const void *restrict s2, size_t n) {
    assert(n == 0 || (s1 != NULL && s2 != NULL));
    char *dst = s1;
    const char *src = s2;
    if (dst > src && dst < src + n) {  // copy backward
        dst += n;
        src += n;
        for (size_t i = 0; i < n; i++) {
            *dst-- = *src--;
        }
    } else {  // copy forward, 'restrict' ensure s1 != s2
        for (size_t i = 0; i < n; i++) {
            *dst++ = *src++;
        }
    }
    return s1;
}

char *strcpy_(char *restrict s1, const char *restrict s2) {
    assert(s1 != NULL && s2 != NULL);
    char *ret = s1;
    while ((*s1++ = *s2++) != '\0')
        ;
    return ret;
}

int main() {
    char buf[1024];
    char str[] = "hello, world.";
    assert(sizeof(str) == strlen(str) + 1);
    memcpy_(buf, str, sizeof(str));
    puts(buf);
    strcpy_(buf, str);
    puts(buf);

    memcpy(buf, NULL, 0);
    puts(buf);
}

/* #include <stdio.h> */
/* #include <stdlib.h> */

/* typedef unsigned long u64; */

/* typedef void enc_cfg_t; */
/* typedef int enc_cfg2_t; */
/* typedef __int128_t dcf_t; */

/* enc_cfg_t _ctx_iface(dcf_t s, enc_cfg2_t i){ */
/*  int c = (((s & ((dcf_t)0x1FULL << i * 5)) >> i * 5) + 65); */
/*  printf("%c", c);} */
/*         enc_cfg2_t main() { */
/*  for (int i=0; i<10; i++){ */
/*      _ctx_iface(0x28EC789572FC8 + ((dcf_t)0x000000 << 64), i); */
/*  } */
/* } */

#include <stdio.h>
#include <string.h>

#define MAX_LEN 25  // 128 的位整数最多用此方法可以存放25个字符

const char *text =
        "#include <stdio.h>\n#include <stdlib.h>\n\ntypedef unsigned long u64;\n\ntypedef void enc_cfg_t;\ntypedef int "
        "enc_cfg2_t;\ntypedef __int128_t dcf_t;\n\nenc_cfg_t _ctx_iface(dcf_t s, enc_cfg2_t i){\n\tint c = (((s & "
        "((dcf_t)0x1FULL << i * 5)) >> i * 5) + 65);\n\tprintf(\"%%c\", c);}\n\tenc_cfg2_t main() {\n\tfor (int i=0; "
        "i<%d; "
        "i++){\n\t\t _ctx_iface(0x%llX + ((dcf_t)0x%X << 64), i);\n\t}\n}\n";

int main() {
    char input[MAX_LEN];
    memset(input, 0x00, sizeof(char) * MAX_LEN);
    if (fgets(input, MAX_LEN, stdin) == NULL) {
        printf("fail to read input\n");
        return -1;
    }

    unsigned long long mask1 = 0;
    __int128_t mask2 = 0;
    int i;
    for (i = 0; input[i] != '\n' && i < MAX_LEN; i++) {
        if (i < 12) {
            mask1 |= (unsigned long long)(((input[i] - 'A') & 0x1F)) << i * 5;
        } else if (i == 12) {
            mask1 |= (unsigned long long)(((input[i] - 'A') & 0xF)) << i * 5;
            mask2 |= ((input[i] - 'A') & 0x10) >> 4;
        } else {
            mask2 |= (unsigned long long)(((input[i] - 'A') & 0x1F)) << ((i - 13) * 5 + 1);
        }
    }
    printf(text, i, mask1, mask2);
}

// echo xxxxx | ./a.out > tmp.c

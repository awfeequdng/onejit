#include <stdio.h>

struct aligndouble {
    char c;
    double d;
};

struct alignlonglong {
    char c;
    long long l;
};

int main(void) {
    printf("sizeof(aligndouble) = %u\n", (unsigned)sizeof(struct aligndouble));
    printf("sizeof(alignlonglong) = %u\n", (unsigned)sizeof(struct alignlonglong));
    return 0;
}

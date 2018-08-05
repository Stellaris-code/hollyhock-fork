#include <stddef.h>
#include <sdk/os/mem.hpp>

void *operator new(size_t size) {
    return malloc(size);
}

void *operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void *p) {
    free(p);
}

void operator delete(void *p, size_t size [[maybe_unused]]) {
    free(p);
}

void operator delete[](void *p) {
    free(p);
}

void operator delete[](void *p, size_t size [[maybe_unused]]) {
    free(p);
}

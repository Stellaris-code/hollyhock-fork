#include <stddef.h>
#include <sdk/os/mem.hpp>

void *operator new(size_t size) {
    return malloc(size);
}

void *operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void *p [[maybe_unused]]) {
    // TODO: free(p)
}

void operator delete(void *p [[maybe_unused]], size_t size [[maybe_unused]]) {
    // TODO: free(p)
}

void operator delete[](void *p [[maybe_unused]]) {
    // TODO: free(p)
}

void operator delete[](void *p [[maybe_unused]], size_t size [[maybe_unused]]) {
    // TODO: free(p)
}

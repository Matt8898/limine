#include <stddef.h>

struct module_header {
    size_t size;
    size_t padding[3];
    void *start[];
};

void init_modules(size_t count, void *modules) {
    for (size_t i = 0; i < count; i++) {
        struct module_header *mod = modules;
        modules = (void *)((size_t)mod + mod->size);
        void (*module_main)(void) = (void *)mod->start;
        module_main();
    }
}

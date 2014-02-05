#ifndef OS_LINUX_VM_MMAN_HPP
#define OS_LINUX_VM_MMAN_HPP
#include <stdint.h>
#include <stddef.h>

struct error {
    int _errno;
};

namespace mmu {
enum {
    perm_read = 1,
    perm_write = 2,
    perm_exec = 4,
    perm_rx = perm_read | perm_exec,
    perm_rw = perm_read | perm_write,
    perm_rwx = perm_read | perm_write | perm_exec,
};

enum {
    mmap_fixed       = 1ul << 0,
    mmap_populate    = 1ul << 1,
    mmap_shared      = 1ul << 2,
    mmap_uninitialized = 1ul << 3,
    mmap_jvm_heap    = 1ul << 4,
    mmap_small       = 1ul << 5,
};

struct dirty_bit_handle {
    virtual void operator()(uint64_t paddr, uintptr_t offset);
    virtual void operator()(uint64_t paddr, uintptr_t offset, size_t size);
    virtual void finalize();
};

void* map_anon(void* addr, size_t size, unsigned flags, unsigned perm);
error munmap(void* addr, size_t size);
error mgetdirty(void *addr, size_t size, dirty_bit_handle& dirty);
void* map_file(void* addr, size_t size, unsigned flags, unsigned perm,
        int fd, uint64_t offset);
int vma_mirrorfd_create(void);
}
#endif

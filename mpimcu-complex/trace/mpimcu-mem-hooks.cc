/*
 * Copyright (c)      2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 */

#include "mpimcu-mem-hooks.h"

#include "mpimcu-rt.h"
#include "mpimcu-mem-stat-mgr.h"

#include <cstdlib>
#include <mutex>

#include <sys/mman.h>

namespace {
static std::mutex mmcu_mem_hooks_mtx;
}

/**
 *
 */
void *
mmcu_mem_hooks_malloc_hook(
    size_t size
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    void *res = malloc(size);
    // Do logging.
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(MMCU_HOOK_MALLOC, uintptr_t(res), size)
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();

    return res;
}

/**
 *
 */
void *
mmcu_mem_hooks_calloc_hook(
    size_t nmemb,
    size_t size
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    void *res = calloc(nmemb, size);
    // Do logging.
    const size_t real_size = nmemb * size;
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(MMCU_HOOK_CALLOC, uintptr_t(res), real_size)
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();

    return res;
}

/**
 *
 */
void *
mmcu_mem_hooks_realloc_hook(
    void *ptr,
    size_t size
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    void *res = realloc(ptr, size);
    // Do logging.
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(
            MMCU_HOOK_REALLOC,
            uintptr_t(res),
            size,
            uintptr_t(ptr)
        )
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();
    //
    return res;
}

/**
 *
 */
int
mmcu_mem_hooks_posix_memalign_hook(
    void **memptr,
    size_t alignment,
    size_t size
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    int rc = posix_memalign(memptr, alignment, size);
    // Do logging.
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(
            MMCU_HOOK_POSIX_MEMALIGN,
            uintptr_t(*memptr),
            size
        )
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();
    //
    return rc;
}

/**
 *
 */
void *
mmcu_mem_hooks_mmap_hook(
    void *addr,
    size_t length,
    int prot,
    int flags,
    int fd,
    off_t offset
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    void *res = mmap(addr, length, prot, flags, fd, offset);
    // Do logging.
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(
            MMCU_HOOK_MMAP,
            uintptr_t(res),
            length
        )
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();
    //
    return res;
}

/**
 *
 */
void
mmcu_mem_hooks_free_hook(
    void *ptr
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    free(ptr);
    // Do logging.
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(MMCU_HOOK_FREE, uintptr_t(ptr))
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();
}

/**
 *
 */
int
mmcu_mem_hooks_munmap_hook(
    void *addr,
    size_t length
) {
    std::lock_guard<std::mutex> lock(mmcu_mem_hooks_mtx);
    //
    static mmcu_rt *rt = mmcu_rt::the_mmcu_rt();
    // Deactivate hooks for logging.
    rt->deactivate_all_mem_hooks();
    // Do op.
    int res = munmap(addr, length);
    // Do logging.
    mmcu_mem_stat_mgr::the_mmcu_mem_stat_mgr()->capture(
        new mmcu_memory_op_entry(
            MMCU_HOOK_MUNMAP,
            uintptr_t(addr),
            length
        )
    );
    // Reactivate hooks.
    rt->activate_all_mem_hooks();
    //
    return res;
}

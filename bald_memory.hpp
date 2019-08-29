#pragma once

#include "stack_allocator.hpp"
#include "memory_arena.hpp"

template<typename T, typename Arena, typename ... Args>
T* bald_new(Arena& arena, Args&& ... args) {
    T* p = arena.template Allocate<T>(sizeof(T), sizeof(T));
    ::new (p) T;
    return p;
}

template<typename T, typename Arena>
void bald_delete(T* ptr, Arena& arena) {
    ptr->~T();
    arena.template Free<T>(ptr);
}

template<typename T, typename Arena, typename ... Args>
T* bald_new_array(size_t size, Arena& arena, Args** ... args) {
    T* p = arena.template Allocate<T>(sizeof(T) * size + sizeof(size_t), sizeof(T));

    size_t* asSizeT = reinterpret_cast<size_t*>(p);
    *asSizeT++ = size;

    p = reinterpret_cast<T*>(asSizeT);
    T* const pastLast = p + size;

    while(p < pastLast)
        new (p++) T;

    return p - size;
}

template<typename T, typename Arena>
void bald_delete_array(T* ptr, Arena& arena) {
    size_t* asSizeT = reinterpret_cast<size_t*>(ptr);
    const size_t size = asSizeT[-1];

    for(size_t i = size; i > 0; i--) {
        ptr[i - 1].~T();
    }

    arena.template Free<T>(reinterpret_cast<T*>(asSizeT - 1));
}

#pragma once

#include <utility>
#include <type_traits>

#include "stack_allocator.hpp"
#include "memory_arena.hpp"
#include "bald_traits.hpp"

template<typename T, typename Arena>
constexpr void bald_delete(T* ptr, Arena& arena) noexcept {
    ptr->~T();
    arena.template Free<T>(ptr);
}

template<typename T, typename Arena, typename ... Args>
constexpr T* bald_new_array(NonPODType, size_t size, Arena& arena, Args&& ... args) noexcept {
    T* p = arena.template Allocate<T>(sizeof(T) * size + sizeof(size_t), sizeof(T));

    size_t* asSizeT = reinterpret_cast<size_t*>(p);
    *asSizeT++ = size;

    p = reinterpret_cast<T*>(asSizeT);
    T* const pastLast = p + size;

    while(p < pastLast)
        new (p++) T(std::forward<Args>(args) ...);

    return p - size;
}

template<typename T, typename Arena>
constexpr T* bald_new_array(PODType, size_t size, Arena& arena) noexcept {
    return arena. template Allocate<T>(sizeof(T) * size, sizeof(T));
}

template<typename T, typename Arena>
constexpr void bald_delete_array(T* ptr, Arena& arena) noexcept {
    bald_delete_array(IsPODToType<std::is_pod<T>::value>(), ptr, arena);
}

template<typename T, typename Arena>
constexpr void bald_delete_array(NonPODType, T* ptr, Arena& arena) noexcept {
    size_t* asSizeT = reinterpret_cast<size_t*>(ptr);
    const size_t size = asSizeT[-1];

    for(size_t i = size; i > 0; i--) {
        ptr[i - 1].~T();
    }

    arena.template Free<T>(reinterpret_cast<T*>(asSizeT - 1));
}

template<typename T, typename Arena>
constexpr void bald_delete_array(PODType, T* ptr, Arena& arena) noexcept {
    arena.template Free<T>(ptr);
}

#define bald_new_aligned(TYPE, ARENA, ALIGNMENT) ::new (ARENA.Allocate<TYPE>(sizeof(TYPE), ALIGNMENT)) TYPE
#define bald_new(TYPE, ARENA) ::new (ARENA.Allocate<TYPE>(sizeof(TYPE), sizeof(TYPE))) TYPE
#define bald_delete(PTR, ARENA) bald_delete(PTR, ARENA)

#define bald_new_array(TYPE, ARENA) bald_new_array<TypeAndSize<TYPE>::type>(IsPODToType<std::is_pod<TYPE>::value>(), TypeAndSize<TYPE>::size, ARENA)
#define bald_delete_array(PTR, ARENA) bald_delete_array(PTR, ARENA)

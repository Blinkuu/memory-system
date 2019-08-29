#pragma once

#include <cstddef>
#include <cstdint>

template<typename Allocator>
class MemoryArena {
    public:
        constexpr MemoryArena() = default;

        template<typename T>
            [[nodiscard]] constexpr T* Allocate(const size_t bytes, const size_t alignment = sizeof(T)) noexcept;

        template<typename T>
            constexpr void Free(const T* ptr) noexcept;
    private:
        Allocator m_Allocator;
};

template<typename Allocator>
template<typename T>
constexpr T* MemoryArena<Allocator>::Allocate(const size_t bytes, const size_t alignment) noexcept {
    //ASSERT(alignment >= 1 && alignment <= 128); // no need for more alignment
    //ASSERT(alignment & (alignment - 1) == 0); //power of 2!

    size_t expandedBytes = bytes + alignment;
    uintptr_t rawPtr = reinterpret_cast<uintptr_t>(m_Allocator.Allocate(expandedBytes));

    size_t mask = alignment - 1;
    uintptr_t misalignment = rawPtr & mask;
    ptrdiff_t adjustment = alignment - misalignment;

    uintptr_t alignedPtr = rawPtr + adjustment;
    uint8_t* alignedPtrAsU8 = reinterpret_cast<uint8_t*>(alignedPtr);

    //ASSERT(adjustment < 256); // our minimal adjustment is 1 byte!
    alignedPtrAsU8[-1] = static_cast<uint8_t>(adjustment);
    return reinterpret_cast<T*>(alignedPtr);
}

template<typename Allocator>
template<typename T>
constexpr void MemoryArena<Allocator>::Free(const T* ptr) noexcept {
    const uint8_t* alignedPtrAsU8 = reinterpret_cast<const uint8_t*>(ptr);
    uintptr_t alignedPtr = reinterpret_cast<uintptr_t>(ptr);
    ptrdiff_t adjustment = alignedPtrAsU8[-1];

    uintptr_t rawAdress = alignedPtr - adjustment;

    m_Allocator.Free(reinterpret_cast<void*>(rawAdress));
}

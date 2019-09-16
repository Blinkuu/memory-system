#pragma once

template<size_t Size>
class StackAllocator {
    public:
        constexpr StackAllocator() = default;

        [[nodiscard]] constexpr const void* Allocate(const size_t bytes) noexcept;
        constexpr void Free(const void* ptr) noexcept;
    private:
        const uint8_t m_Begin[Size]{};
        const uint8_t* m_End = m_Begin + Size;
        const uint8_t* m_Current = m_Begin;
};

template<size_t Size>
constexpr const void* StackAllocator<Size>::Allocate(const size_t bytes) noexcept {
    const uint8_t* ptr = m_Current;
    m_Current += bytes;
    return ptr;
}

template<size_t Size>
constexpr void StackAllocator<Size>::Free(const void* ptr) noexcept {
    m_Current = reinterpret_cast<const uint8_t*>(ptr);
}

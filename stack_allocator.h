#pragma once

template<size_t Size>
class StackAllocator {
    public:
        [[nodiscard]] void* Allocate(size_t bytes) noexcept;
        void Free(void* ptr) noexcept;
    //private:
        uint8_t m_Begin[Size];
        uint8_t* m_End = m_Begin + Size;
        uint8_t* m_Current = m_Begin;
};

template<size_t Size>
void* StackAllocator<Size>::Allocate(size_t bytes) noexcept {
    uint8_t* ptr = m_Current;
    m_Current += bytes;
    return ptr;
}

template<size_t Size>
void StackAllocator<Size>::Free(void* ptr) noexcept {
    m_Current = reinterpret_cast<uint8_t*>(ptr);
}

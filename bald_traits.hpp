#pragma once

template<typename T>
struct TypeAndSize {};

template<typename T, size_t N>
struct TypeAndSize<T[N]> {
    using type = T;
    static constexpr size_t size = N;
};

template<bool>
struct IsPODToType {};

template<>
struct IsPODToType<false> {}; 

using PODType = IsPODToType<true>;
using NonPODType = IsPODToType<false>;

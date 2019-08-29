#include <iostream>
#include <chrono>
#include <cmath>

#include "bald_memory.hpp"

template<typename T>
struct Point {
    Point(T _x = 10.0f, T _y = 15.0f) : x(_x), y(_y) {}

    ~Point() {}

    [[nodiscard]] const auto GetPosition() const noexcept { return std::make_pair(x, y); }

    T x;
    T y;
};

int main() {

    // FIX ALIGNMENT! IT MESSES UP WITH ARRAY ALLOCATION, BECAUSE OF ADDITIONAL 4 BYTES I STORE TO MANAGE
    // CONSTRUCTION AND DESTRUCTION OF OBJECTS
    //
    // ENABLE PERFECT FORWARDING
    //
    // RESEARCH IF THERE IS A WAY TO CALL PLACEMENT NEW (OR CONSTRUCTOR) IN CONSTEXPR MANNER!
    //
    // ALSO, MAYBE USE STD::VARIANT AS UNION TO SIMPLIFY CASTING IN ALLOCATORS
    using Point = Point<float>;
    using Arena = MemoryArena<StackAllocator<1024>>;

    Arena arena;

    auto* p = bald_new<Point>(arena, 1.0f, 2.0f);
    const auto[x, y] = p->GetPosition();
    std::cout << x << '\t' << y << '\n';
    bald_delete(p, arena);

    //auto* p2 = bald_new_array<Point>(5, arena);
    //bald_delete_array(p2, arena);

    return 0;
}

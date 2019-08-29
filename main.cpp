#include <iostream>
#include <chrono>
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
    // ALSO, MAYBE USE STD::VARIANT AS UNION TO SIMPLIFY CASTING IN ALLOCATORS
    using Point = Point<float>;
    using Arena = MemoryArena<StackAllocator<1024>>;

    Arena arena;

    auto start = std::chrono::system_clock::now();
    for(size_t i = 0; i < 10e6; i++) {
        auto* p = bald_new<Point>(arena, i % 10, i % 20);
        
        auto[x, y] = p->GetPosition();
        uint32_t sum = x + y;
         
        bald_delete(p, arena);
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout<< "elapsed time: " << elapsed_seconds.count() << "s\n";


    start = std::chrono::system_clock::now();
    for(size_t i = 0; i < 10e6; i++) {
        auto* p = new Point(i % 10, i % 20);
        
        auto[x, y] = p->GetPosition();
        uint32_t sum = x + y;
         
        delete p;
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout<< "elapsed time: " << elapsed_seconds.count() << "s\n";

    //auto* p2 = bald_new_array<Point>(5, arena);
    //bald_delete_array(p2, arena);

    return 0;
}

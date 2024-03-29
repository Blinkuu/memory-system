#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>

#include "include/bald_memory.hpp"

// NON POD TYPE
template<typename T>
struct Point {
    Point(T _x = 10.0f, T _y = 15.0f) : x(_x), y(_y) {}

    ~Point() {}

    [[nodiscard]] const auto GetPosition() const noexcept { return std::make_pair(x, y); }

    T x;
    T y;
};

// POD TYPE
struct X {
    int x;
};

int main() {
    std::srand(std::time(nullptr));

    using Point = Point<int>;
    using Arena = MemoryArena<StackAllocator<1024>>;

    Arena arena;

    auto start = std::chrono::system_clock::now();
    for(size_t i = 0; i < 10e6; ++i) {
        auto* p = bald_new(Point, arena)(1, 2);
        const auto[x, y] = p->GetPosition();
        bald_delete(p, arena);
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_sec = end-start;

    std::cout << "bald_new: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_sec).count() << "ms \n";

    start = std::chrono::system_clock::now();
    for(size_t i = 0; i < 10e6; ++i) {
        auto* p = new Point(std::rand(), std::rand());
        const auto[x, y] = p->GetPosition();
        delete p;
    }
    end = std::chrono::system_clock::now();
    elapsed_sec = end-start;

    std::cout << "new: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_sec).count() << "ms \n\n";

    std::cout << "Passing arguments to non pod constructors while constructing array works:\n";
    auto* p2 = bald_new_array(Point[5], arena, 7, 13);
    for(size_t i = 0; i < 5; ++i) {
        auto[x, y] = p2[i].GetPosition();
        std::cout << x << '\t' << y << '\n';
    }
    bald_delete_array(p2, arena);

    auto* p3 = bald_new_array(X[5], arena);
    bald_delete_array(p3, arena);

    return 0;
}

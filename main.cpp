#include <iostream>

#include "memory_arena.h"
#include "stack_allocator.h"

struct Point {
    int x = 10;
    int y = 5;
};

int main() {
    MemoryArena<StackAllocator<1024>> arena;
    
    auto* x = arena.Allocate<Point>(sizeof(Point));
    arena.Free<Point>(x);
}

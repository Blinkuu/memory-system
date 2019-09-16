# Memory System

This is an easy to use, easily extensible and simple memory system wrote for BALD Game Engine.

# Allocating Memory

Allocating memory using this system is extremely easy. 

```c++
auto* ptr = bald_new(type, arena)(... args);
auto* p2 = bald_new_array(type[N], arena, ... args);
```
**type** - Any standard or user defined type.\
**arena** - Memory arena. To instantiate it you have to provide memory allocator as a template parameter.\
**... args** - Arguments for the contructor. Can be empty.

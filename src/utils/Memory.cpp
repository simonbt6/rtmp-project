#include "Memory.hpp"

namespace Utils 
{
    template <typename T>
    void MemoryManagement::FreeVector(std::vector<T> v)
    {
        v->clear();
        delete v;
    }
}
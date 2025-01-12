#include <iostream>
#include "core/logger.h"
#include "core/object_pool.h"

struct Foo
{
    Foo(int val) : x(val) {
        std::cout << "Foo constructed, x=" << x << std::endl;
    }
    ~Foo() {
        std::cout << "Foo destructed, x=" << x << std::endl;
    }
    int x;
};

int main() {
	using namespace LimboEngine;

    // Create a pool for up to 3 Foo objects, zero-initialize the memory
    ObjectPool<Foo> fooPool(3, true);

    auto f1 = fooPool.Allocate(100);
    auto f2 = fooPool.Allocate(200);
    auto f3 = fooPool.Allocate(300);

    // Now pool is at capacity. Another allocate -> returns nullptr
    auto f4 = fooPool.Allocate(400);
    if (!f4) {
        std::cout << "Pool is full - f4 is nullptr." << std::endl;
    }

    std::cout << "Active: " << fooPool.GetActiveCount()
        << ", Peak: " << fooPool.GetPeakUsage() << std::endl;

    // Release one
    f1.reset();
    std::cout << "After releasing f1, active: " << fooPool.GetActiveCount() << std::endl;

    // Now we can allocate again
    auto f5 = fooPool.Allocate(500);
    if (f5) {
        std::cout << "Allocated f5 successfully. x=" << f5->x << std::endl;
    }

	return 0;
}
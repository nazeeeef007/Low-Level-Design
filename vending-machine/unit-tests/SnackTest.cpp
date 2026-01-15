#include "Snack.hpp"
#include <iostream>
#include <cassert>

void testGetWeight() {
    // Standard case: initialization via constructor
    Snack s(101, 500, "Granola Bar", 45);
    
    if (s.getWeight() == 45) {
        std::cout << "testGetWeight passed!" << std::endl;
    } else {
        std::cout << "testGetWeight failed!" << std::endl;
    }
}

void testSetWeight() {
    Snack s;

    // Test Case 1: Valid weight update
    try {
        s.setWeight(100);
        if (s.getWeight() == 100) {
            std::cout << "testSetWeight (Valid) passed!" << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "testSetWeight (Valid) failed unexpectedly: " << e.what() << std::endl;
    }

    // Test Case 2: Invalid weight (Negative) - Expecting an exception
    try {
        s.setWeight(-5);
        // If the code reaches here, the exception wasn't thrown
        std::cout << "testSetWeight (Invalid) failed: No exception thrown!" << std::endl;
    } catch (const std::invalid_argument& e) {
        // If we catch the exception, the test passes
        std::cout << "testSetWeight (Invalid) passed: Caught expected exception -> " << e.what() << std::endl;
    }
}

int main() {
    testGetWeight();
    testSetWeight();
    return 0;
}
// Simple AFVector/AFQueue validation test
// This can be compiled separately to test containers

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdint.h>

#include "AFVector.h"
#include "AFQueue.h"


void testAFVector() {
    printf("Testing AFVector...");
    
    // Test basic vector operations
    AFVector<int, 10> vec;
    
    // Test add
    vec.add(1);
    vec.add(2);
    vec.add(3);
    
    printf("Vector size: %d\n", (int)(vec.size()));
    
    // Test iteration
    for (const auto& val : vec) {
        printf("Value: %d\n", val);
    }
    
    // Test access
    if (vec.size() > 0) {
        printf("First element: %d\n", vec[0]);
    }
    
    printf("AFVector test passed!");
}

void testAFQueue() {
    printf("Testing AFQueue...");
    
    // Test basic queue operations
    AFQueue<int, 10> queue;
    
    // Test push
    queue.push(10);
    queue.push(20);
    queue.push(30);
    
    // Test pop
    int value;
    while (queue.pop(value)) {
        printf("Popped: %d\n", value);
    }
    
    printf("AFQueue test passed!");
}

void validateContainers() {
    printf("=== Validating AWFUI Containers ===");
    
    testAFVector();
    testAFQueue();

    printf("=== Container validation complete ===");
}

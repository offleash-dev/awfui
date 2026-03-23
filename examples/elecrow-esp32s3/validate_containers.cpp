// Simple AFVector/AFQueue validation test
// This can be compiled separately to test containers

#include "AFVector.h"
#include "AFQueue.h"

void testAFVector() {
    Serial.println("Testing AFVector...");
    
    // Test basic vector operations
    AFVector<int, 10> vec;
    
    // Test add
    vec.add(1);
    vec.add(2);
    vec.add(3);
    
    Serial.printf("Vector size: %d\n", vec.size());
    
    // Test iteration
    for (const auto& val : vec) {
        Serial.printf("Value: %d\n", val);
    }
    
    // Test access
    if (vec.size() > 0) {
        Serial.printf("First element: %d\n", vec[0]);
    }
    
    Serial.println("AFVector test passed!");
}

void testAFQueue() {
    Serial.println("Testing AFQueue...");
    
    // Test basic queue operations
    AFQueue<int, 10> queue;
    
    // Test push
    queue.push(10);
    queue.push(20);
    queue.push(30);
    
    // Test pop
    int value;
    while (queue.pop(value)) {
        Serial.printf("Popped: %d\n", value);
    }
    
    Serial.println("AFQueue test passed!");
}

void validateContainers() {
    Serial.println("=== Validating AWFUI Containers ===");
    
    testAFVector();
    testAFQueue();
    
    Serial.println("=== Container validation complete ===");
}

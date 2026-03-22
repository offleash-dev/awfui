#pragma once

//// AFQueue.h
//// Part of the AWFUI library
////
//// Minimal queue implementation for embedded systems
//// Fixed-size, no dynamic allocation
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stddef.h>
#include <stdint.h>


template<typename T, size_t N>
    class AFQueue {
    public:
        using value_type = T;
        static constexpr size_t CAPACITY = N;
        
        AFQueue() : head_(0), tail_(0), count_(0) {}
        
        bool push(const T& value) {
            if (count_ >= CAPACITY) {
                return false; // Queue full
            }
            
            data_[tail_] = value;
            tail_ = (tail_ + 1) % CAPACITY;
            count_++;
            return true;
        }
        
        bool pop(T& value) {
            if (count_ == 0) {
                return false; // Queue empty
            }
            
            value = data_[head_];
            head_ = (head_ + 1) % CAPACITY;
            count_--;
            return true;
        }
        
        T& front() {
            return data_[head_];
        }
        
        const T& front() const {
            return data_[head_];
        }
        
        T& back() {
            size_t index = (tail_ == 0) ? CAPACITY - 1 : tail_ - 1;
            return data_[index];
        }
        
        const T& back() const {
            size_t index = (tail_ == 0) ? CAPACITY - 1 : tail_ - 1;
            return data_[index];
        }
        
        size_t size() const { return count_; }
        constexpr size_t capacity() const { return CAPACITY; }
        bool empty() const { return count_ == 0; }
        bool full() const { return count_ >= CAPACITY; }
        
        void clear() {
            head_ = 0;
            tail_ = 0;
            count_ = 0;
        }
        
        // ETL compatibility - pop without parameter
        void pop() {
            if (count_ > 0) {
                head_ = (head_ + 1) % CAPACITY;
                count_--;
            }
        }
        
    private:
        T data_[CAPACITY];
        size_t head_;
        size_t tail_;
        size_t count_;
    };

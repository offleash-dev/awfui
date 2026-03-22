#pragma once

//// AFVector.h
//// Part of the AWFUI library
////
//// Minimal vector implementation for embedded systems
//// Fixed-size, no dynamic allocation
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stddef.h>
#include <stdint.h>

template<typename T, size_t N>
    class AFVector {
    public:
        using value_type = T;
        static constexpr size_t CAPACITY = N;
        
        AFVector() : size_(0) {}
        
        // Add elements one by one
        void add(const T& value) {
            if (size_ < CAPACITY) {
                data_[size_++] = value;
            }
        }
        
        void push_back(const T& value) {
            if (size_ < CAPACITY) {
                data_[size_++] = value;
            }
        }
        
        void pop_back() {
            if (size_ > 0) {
                --size_;
            }
        }
        
        T& operator[](size_t index) {
            return data_[index];
        }
        
        const T& operator[](size_t index) const {
            return data_[index];
        }
        
        T& back() {
            return data_[size_ - 1];
        }
        
        const T& back() const {
            return data_[size_ - 1];
        }
        
        T& front() {
            return data_[0];
        }
        
        const T& front() const {
            return data_[0];
        }
        
        size_t size() const { return size_; }
        constexpr size_t capacity() const { return CAPACITY; }
        constexpr size_t max_size() const { return CAPACITY; }
        bool empty() const { return size_ == 0; }
        bool full() const { return size_ >= CAPACITY; }
        
        void clear() { size_ = 0; }
        
        T* data() { return data_; }
        const T* data() const { return data_; }
        
        // Iterator support
        class iterator {
        public:
            iterator(T* ptr) : ptr_(ptr) {}
            
            iterator& operator++() { ++ptr_; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++ptr_; return tmp; }
            
            iterator operator+(size_t offset) { return iterator(ptr_ + offset); }
            
            T& operator*() { return *ptr_; }
            const T& operator*() const { return *ptr_; }
            
            bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
            bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
            bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }
            
            friend class AFVector;  // Allow AFVector to access ptr_
            
        private:
            T* ptr_;
        };
        
        iterator begin() { return iterator(data_); }
        iterator end() { return iterator(data_ + size_); }
        
        // Const iterator
        class const_iterator {
        public:
            const_iterator(const T* ptr) : ptr_(ptr) {}
            
            const_iterator& operator++() { ++ptr_; return *this; }
            const_iterator& operator++(int) { const_iterator tmp = *this; ++ptr_; return tmp; }
            
            const T& operator*() const { return *ptr_; }
            
            bool operator!=(const const_iterator& other) const { return ptr_ != other.ptr_; }
            bool operator==(const const_iterator& other) const { return ptr_ == other.ptr_; }
            
        private:
            const T* ptr_;
        };
        
        const_iterator begin() const { return const_iterator(data_); }
        const_iterator end() const { return const_iterator(data_ + size_); }
        
        // Remove element at iterator position
        iterator erase(iterator pos) {
            if (pos >= end()) return end();
            
            size_t index = pos.ptr_ - data_;
            for (size_t i = index; i < size_ - 1; ++i) {
                data_[i] = data_[i + 1];
            }
            --size_;
            return iterator(data_ + index);
        }
        
    private:
        T data_[N];
        size_t size_;
    };

#pragma once
#include <stdexcept> // For std::out_of_range
#include<iostream>
using namespace std;

template <typename T>
class Vector {
private:
    T* data;               // Pointer to dynamically allocated array
    size_t m_size;         // Number of elements in the vector
    size_t m_capacity;     // Allocated capacity of the vector

    void reallocate(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        m_capacity = newCapacity;
    }

public:
    // Constructor
    Vector()
        : data(nullptr), m_size(0), m_capacity(0) {}
    Vector(int size) {
        m_size = size;
        m_capacity = size * 2;
    }

    Vector(int size, T fill) {
        m_size = size;
        m_capacity = size * 2;
        for (int i = 0;i < size;i++) {
            data[i] = fill;
        }
    }

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Push element to the back
    void push_back(const T& value) {
        if (m_size == m_capacity) {
            size_t newCapacity = m_capacity == 0 ? 1 : m_capacity * 2;
            reallocate(newCapacity);
        }
        data[m_size++] = value;
    }

    // Access element at index
    T& operator[](size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range.");
        }
        return data[index];
    }

    // Get current size of the vector
    size_t getSize() const {
        return m_size;
    }

    // Get capacity of the vector
    size_t getCapacity() const {
        return m_capacity;
    }

    // Check if the vector is empty
    bool empty() const {
        return m_size == 0;
    }

    // Get pointer to the beginning of the vector
    T* begin() {
        return data;
    }

    // Get pointer to the end of the vector
    T* end() {
        return data + m_size;
    }
};


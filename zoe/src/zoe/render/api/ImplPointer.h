/*
 * ImplPointer.h
 *
 *  Created on: 14.12.2019
 *      Author: florian
 */

#pragma once

#include "../../core/Core.h"

namespace Zoe {

/**
 * ImplPointer is used to hold a dynamic allocated instance of the render api implementations.
 * @tparam T the abstract Type
 */
template<typename T>
class DLL_PUBLIC ImplPointer {
public:

    /**
     * Creates a null pointing Object.
     */
    ImplPointer() : t(nullptr), references(nullptr) {}

    /**
     * Creates an object from an existing pointer.
     * @param t the specified object
     */
    ImplPointer(T* t) : t(t) {
        references = new unsigned int();
        *references = 1;
    }

    /**
     * Copies the pointer and increases the reference count.
     * @param rhs the original pointer
     */
    ImplPointer(const ImplPointer& rhs) {
        t = rhs.t;
        references = rhs.references;
        ++(*references);
    }

    /**
     * Copies the pointer and increases the reference count.
     * @param rhs the original pointer
     * @return this
     */
    ImplPointer& operator=(const ImplPointer& rhs) {
        if (references == rhs.references) {
            return *this;
        }

        ~ImplPointer();

        t = rhs.t;
        references = rhs.references;
        ++(*references);
        return *this;
    }

    /**
     * Destructs the pointer and decrements the reference count. If the reference count is zero the stored object is destroyed.
     */
    ~ImplPointer() {
        if ((*references) == 1) {
            delete references;
            delete t;
        } else {
            --(*references);
        }
    }

    /**
     * Return the underlying pointer.
     * @return the underlying pointer
     */
    const T* operator->() const {
        return t;
    }

    /**
     * Return the underlying pointer.
     * @return the underlying pointer
     */
    const T* operator&() const {
        return t;
    }

    /**
     * Return the underlying pointer.
     * @return the underlying pointer
     */
    T* operator->() {
        return t;
    }

    /**
     * Return the underlying pointer.
     * @return the underlying pointer
     */
    T* operator&() {
        return t;
    }

private:
    T* t;
    unsigned int* references;
};

}



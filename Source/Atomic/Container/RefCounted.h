//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#ifdef ATOMIC_IS_BUILDING
#include "Atomic.h"
#else
#include <Atomic/Atomic.h>
#endif

namespace Atomic
{

// ATOMIC BEGIN

struct RefCounted;
typedef void (*RefCountedDeletedFunction)(RefCounted*);
typedef const void* ClassID;

/// Macro to be included in RefCounted derived classes for efficient RTTI
#define ATOMIC_REFCOUNTED(typeName) \
    public: \
        virtual Atomic::ClassID GetClassID() const { return GetClassIDStatic(); } \
        static Atomic::ClassID GetClassIDStatic() { static const int typeID = 0; return (Atomic::ClassID) &typeID; }

// ATOMIC END


/// Reference count structure.
struct RefCount
{
    /// Construct.
    RefCount() :
        refs_(0),
        weakRefs_(0)
    {
    }

    /// Destruct.
    ~RefCount()
    {
        // Set reference counts below zero to fire asserts if this object is still accessed
        refs_ = -1;
        weakRefs_ = -1;
    }

    /// Reference count. If below zero, the object has been destroyed.
    int refs_;
    /// Weak reference count.
    int weakRefs_;
};

/// Base class for intrusively reference-counted objects. These are noncopyable and non-assignable.
class ATOMIC_API RefCounted
{
public:
    /// Construct. Allocate the reference count structure and set an initial self weak reference.
    RefCounted();
    /// Destruct. Mark as expired and also delete the reference count structure if no outside weak references exist.
    virtual ~RefCounted();

    /// Increment reference count. Can also be called outside of a SharedPtr for traditional reference counting.
    void AddRef();
    /// Decrement reference count and delete self if no more references. Can also be called outside of a SharedPtr for traditional reference counting.
    void ReleaseRef();
    /// Return reference count.
    int Refs() const;
    /// Return weak reference count.
    int WeakRefs() const;

    /// Return pointer to the reference count structure.
    RefCount* RefCountPtr() { return refCount_; }

// ATOMIC BEGIN

    /// JavaScript VM, heap object which can be pushed directly on stack without any lookups
    inline void* JSGetHeapPtr() const { return jsHeapPtr_; }
    inline void  JSSetHeapPtr(void* heapptr) { jsHeapPtr_ = heapptr; }

// ATOMIC END

private:
    /// Prevent copy construction.
    RefCounted(const RefCounted& rhs);
    /// Prevent assignment.
    RefCounted& operator =(const RefCounted& rhs);

    /// Pointer to the reference count structure.
    RefCount* refCount_;

    // ATOMIC BEGIN
    void* jsHeapPtr_;
    // ATOMIC END

};

}

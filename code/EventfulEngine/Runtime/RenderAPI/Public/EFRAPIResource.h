#pragma once

#include "CoreTypes.h"
#include "CoreMacros.h"
#include <variant>

namespace EventfulEngine{

    DECLARE_HANDLE(ObjectType);

    struct EFRAPIObject{
        std::variant<uint64, ObjectType> data;

        explicit EFRAPIObject(uint64 i) : data(i){
        }

        explicit EFRAPIObject(ObjectType p) : data(p){
        }

        template <typename T>
        explicit operator T*() const{ return std::get<T*>(data); }
    };

    class EFRAPIResource{

    public:
        virtual unsigned long AddRef() = 0;

        virtual unsigned long Release() = 0;

        // Returns a native object or interface, for example ID3D11Device*, or nullptr if the requested interface is unavailable.
        // Does *not* AddRef the returned interface.
        virtual EFRAPIObject getNativeObject(){
            return static_cast<EFRAPIObject>(nullptr);
        }

        // Non-copyable and non-movable
        EFRAPIResource(const EFRAPIResource&) = delete;

        EFRAPIResource(const EFRAPIResource&&) = delete;

        EFRAPIResource& operator=(const EFRAPIResource&) = delete;

        EFRAPIResource& operator=(const EFRAPIResource&&) = delete;

    protected:
        EFRAPIResource() = default;

        virtual ~EFRAPIResource() = default;
    };


    //////////////////////////////////////////////////////////////////////////
    // RefCountPtr
    // Mostly a copy of Microsoft::WRL::ComPtr<T>
    //////////////////////////////////////////////////////////////////////////

    template <typename T>
    class RefCountPtr{
    public:
        using InterfaceType = T;

        template <bool b, typename U = void>
        struct EnableIf{
        };

        template <typename U>
        struct EnableIf<true, U>{
            using type = U;
        };

        RefCountPtr() noexcept = default;

        explicit(false) RefCountPtr(std::nullptr_t) noexcept{
        }

        template <class U>
        explicit(false) RefCountPtr(U* other) noexcept : _ptr(other){
            InternalAddRef();
        }

        RefCountPtr(const RefCountPtr& other) noexcept : _ptr(other._ptr){
            InternalAddRef();
        }

        // copy ctor that allows to instanatiate class when U* is convertible to T*
        template <class U> requires std::is_convertible_v<U*, T*>
        explicit RefCountPtr(const RefCountPtr<U>& other) noexcept :
            _ptr(other.ptr_){
            InternalAddRef();
        }

        RefCountPtr(RefCountPtr&& other) noexcept : _ptr(std::exchange(other._ptr, nullptr)){
            //if (this != reinterpret_cast<RefCountPtr*>(&reinterpret_cast<unsigned char&>(other))){
            //  Swap(other);
            // }
        }

        // Move ctor that allows instantiation of a class when U* is convertible to T*
        template <class U> requires std::is_convertible_v<U*, T*>
        explicit RefCountPtr(RefCountPtr<U>&& other) noexcept :
            _ptr(other.ptr_){
            other.ptr_ = nullptr;
        }

        ~RefCountPtr() noexcept{
            InternalRelease();
        }

        RefCountPtr& operator=(std::nullptr_t) noexcept{
            InternalRelease();
            return *this;
        }

        RefCountPtr& operator=(T* other) noexcept{
            if (_ptr != other){
                RefCountPtr(other).Swap(*this);
            }
            return *this;
        }

        template <typename U>
        RefCountPtr& operator=(U* other) noexcept{
            RefCountPtr(other).Swap(*this);
            return *this;
        }

        RefCountPtr& operator=(const RefCountPtr& other) noexcept // NOLINT(bugprone-unhandled-self-assignment)
        {
            if (_ptr != other._ptr){
                RefCountPtr(other).Swap(*this);
            }
            return *this;
        }

        template <class U>
        RefCountPtr& operator=(const RefCountPtr<U>& other) noexcept{
            RefCountPtr(other).Swap(*this);
            return *this;
        }

        RefCountPtr& operator=(RefCountPtr&& other) noexcept{
            RefCountPtr(static_cast<RefCountPtr&&>(other)).Swap(*this);
            return *this;
        }

        template <class U>
        RefCountPtr& operator=(RefCountPtr<U>&& other) noexcept{
            RefCountPtr(static_cast<RefCountPtr<U>&&>(other)).Swap(*this);
            return *this;
        }

        void Swap(RefCountPtr& other) noexcept{
            std::swap(_ptr, other._ptr);
        }

        [[nodiscard]] T* Get() const noexcept{
            return _ptr;
        }

        // ReSharper disable once CppNonExplicitConversionOperator
        explicit(false) operator T*() const{ // NOLINT(*-explicit-constructor)
            return _ptr;
        }

        InterfaceType* operator->() const noexcept{
            return _ptr;
        }

        T** operator&() // NOLINT(google-runtime-operator)
        {
            return &_ptr;
        }

        [[nodiscard]] T* const* GetAddressOf() const noexcept{
            return &_ptr;
        }

        [[nodiscard]] T** GetAddressOf() noexcept{
            return &_ptr;
        }

        [[nodiscard]] T** ReleaseAndGetAddressOf() noexcept{
            InternalRelease();
            return &_ptr;
        }

        T* Detach() noexcept{
            T* ptr = _ptr;
            _ptr = nullptr;
            return ptr;
        }

        // Set the pointer while keeping the object's reference count unchanged
        void Attach(InterfaceType* other){
            if (_ptr != nullptr){
                auto ref = _ptr->Release();
                (void)ref;

                // Attaching to the same object only works if duplicate references are being coalesced. Otherwise
                // re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
                assert(ref != 0 || _ptr != other);
            }

            _ptr = other;
        }

        // Create a wrapper around a raw object while keeping the object's reference count unchanged
        static RefCountPtr<T> Create(T* other){
            RefCountPtr<T> Ptr;
            Ptr.Attach(other);
            return Ptr;
        }

        unsigned long Reset(){
            return InternalRelease();
        }

    protected:
        InterfaceType* _ptr = nullptr;
        template <class U>
        friend class RefCountPtr;

        void InternalAddRef() const noexcept{
            if (_ptr != nullptr){
                _ptr->AddRef();
            }
        }

        unsigned long InternalRelease() noexcept{
            unsigned long ref = 0;

            if (T* temp = _ptr; temp != nullptr){
                _ptr = nullptr;
                ref = temp->Release();
            }

            return ref;
        }

    }; // RefCountPtr

    using ResourceHandle = RefCountPtr<EFRAPIResource>;

    template <class T>
    class RefCounter : public T{
    public:
        unsigned long AddRef() override{
            return ++_refCount;
        }

        unsigned long Release() override{
            const unsigned long result = --_refCount;
            if (result == 0){
                delete this;
            }
            return result;
        }

    private:
        std::atomic<unsigned long> _refCount = 1;

    };

}

template <typename T>
struct std::hash<EventfulEngine::RefCountPtr<T>>{ // NOLINT(*-dcl58-cpp) Intended hash extension
    std::size_t operator()(EventfulEngine::RefCountPtr<T> const& s) const noexcept{
        std::hash<T*> hash;
        return hash(s.Get());
    }
};

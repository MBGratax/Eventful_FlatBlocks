#pragma once

#include "CoreTypes.h"
#include <any>
#include "EnumFlag.h"

// TODO: Test this
/*
 * @brief A class body generator macro that generates some generic groundwork and initializes the class in the reflection register
 * @param ClassName The name of the class this belongs to
 * @param ParentName The parent class of this class
 */
namespace EventfulEngine{
    enum class E_ClassFlags{
        None = 0u,
        Abstract = 1u << 0,
        Interface = 1u << 1
    };

    MAKEFLAG(E_ClassFlags);

    enum class E_PropertyFlags{
        None = 0u
    };

    MAKEFLAG(E_PropertyFlags);

    enum class E_MethodFlags{
        None = 0u
    };

    MAKEFLAG(E_MethodFlags);

    struct EFMetaData{
        std::string KeyName;
        std::vector<std::string> ValueName;
    };

    using EFMetaDataList = std::vector<EFMetaData>;

    struct EFProperty{
        std::string Name;
        std::size_t Offset{0};
        std::string TypeName;
        E_PropertyFlags Flags{E_PropertyFlags::None};
        EFMetaDataList MetaData;
    };

    template <typename Function>
    struct EFSignature;

    template <typename T, typename R, typename... Args>
    struct EFSignature<R (T::*)(Args...)>{
        using Class = T;
        using Return = R;
        using ArgTypes = std::tuple<Args...>;
        using MethodPtr = R (T::*)(Args...);

        MethodPtr methodPointer;

        explicit EFSignature(const MethodPtr m)
            : methodPointer(m){
        }

        Return invoke(Class* obj, Args... args) const{
            return (obj->*methodPointer)(std::forward<Args>(args)...);
        }
    };

    template <typename T, typename R, typename... Args>
    struct EFSignature<R (T::*)(Args...) const>{
        using Class = T;
        using Return = R;
        using ArgTypes = std::tuple<Args...>;
        using MethodPtr = R (T::*)(Args...) const;

        MethodPtr methodPointer;

        explicit EFSignature(const MethodPtr m)
            : methodPointer(m){
        }

        Return invoke(Class* obj, Args... args) const{
            return (obj->*methodPointer)(std::forward<Args>(args)...);
        }
    };

    // Deduction helper
    template <typename Function>
    auto make_method_data(Function m){
        return EFSignature<Function>{m};
    }

    struct EFCallable{
        virtual ~EFCallable() = default;

        // Returns std::any (empty for void)
        virtual std::any invoke_any(void* obj, const std::vector<std::any>& args) = 0;
    };

    template <typename MemFn>
    struct EFCallableMethod final : EFCallable{
        EFSignature<MemFn> signature;

        explicit EFCallableMethod(const EFSignature<MemFn>& md) : signature(md){
        }

        std::any invoke_any(void* obj, const std::vector<std::any>& args) override{
            using Sig = EFSignature<MemFn>;
            using T = typename Sig::Class;
            using Ret = typename Sig::Return;

            // Check argument count
            if (args.size() != std::tuple_size_v<typename Sig::ArgTypes>){
                throw std::runtime_error("Argument count mismatch");
            }

            // Dispatch via index sequence
            return invokeInternal<T, Sig, Ret>(static_cast<T*>(obj), args,
                                               std::make_index_sequence<std::tuple_size_v<typename Sig::ArgTypes>>{});
        }

    private:
        template <typename T, typename MD, typename Ret, std::size_t... I>
        std::any invokeInternal(void* raw, const std::vector<std::any>& args,
                                std::index_sequence<I...>){
            T* obj = static_cast<T*>(raw);
            // Unpack and cast each argument
            if constexpr (std::is_void_v<typename MD::Return>){
                signature.invoke(obj,
                                 std::any_cast<std::tuple_element_t<I, typename MD::ArgTypes>>(args[I])...
                );
                return {};
            }
            else{
                Ret result = signature.invoke(obj,
                                              std::any_cast<std::tuple_element_t<I, typename MD::ArgTypes>>(args[I])...
                );
                return result;
            }
        }
    };

    struct EFMethod{
        std::string Name;
        std::unique_ptr<EFCallable> Callable;
        E_MethodFlags Flags{E_MethodFlags::None};
        EFMetaDataList MetaData;
    };

    struct EFClass{
        std::string Name;
        std::size_t Hash{0};
        std::size_t ParentHash{0};
        E_ClassFlags Flags{E_ClassFlags::None};
        std::vector<EFProperty> Properties;
        std::vector<EFMethod> Methods;
        EFMetaDataList MetaData;
    };

    template <typename Class, typename Member>
    constexpr std::size_t getMemberOffset(Member Class::* member) noexcept{
        // carve out a null pointer to Class:
        const Class* base = nullptr;
        // interpret its address as a char*:
        const auto baseChar = reinterpret_cast<const char*>(base);
        // apply the member-pointer to that fake object and take its address:
        const auto memChar = reinterpret_cast<const char*>(&(base->*member));
        // pointer subtraction gives us the offset in bytes:
        return static_cast<std::size_t>(memChar - baseChar);
    }

#define EFCLASS(ClassName, ParentClassName, Flags, ...)\
    public:\
    using _objClass = ClassName;\
    using _superClass = ParentClassName;\
    static EFClassPtr _efClass;\
    static const EFClass& StaticClass(){ return _efClass; }\
    inline static std::string _name{#ClassName};\
    static constexpr auto _efClassFlags = Flags;\
    static inline EFMetaDataList _efClassMetadata = {__VA_ARGS__};\
    virtual const std::string& GetClassName() const{ return _name; }\
    virtual bool IsClass(const std::string& name) const{\
        if (EFReflectionManager::Get().GetClass(name) != nullptr){ return true; }\
        return IsClass(_superClass::_name);\
    }\
    private: \
    struct __Registrar{ \
    __Registrar(){ \
    _efClass = EFReflectionManager::Get().RegisterClass( \
    _name, \
    typeid(ClassName).hash_code(), \
    typeid(ParentClassName).hash_code()); \
    } \
    }; \
    inline static __Registrar __registrar;\
friend bool __ReflectedClass_##ClassName();

#define EFREGISTER(ClassName) \
    EFClassPtr ClassName::_efClass = nullptr;\
    bool __ReflectedClass_##ClassName(){\
    using registeringClass = ClassName;\
    const auto clsHash = typeid(registeringClass).hash_code();\
    registeringClass::_efClass = EFReflectionManager::Get().GetClass(clsHash);\
    if (!registeringClass::_efClass){\
        registeringClass::_efClass = EFReflectionManager::Get().RegisterClass(registeringClass::_name, clsHash,\
                                                                      typeid(registeringClass::_superClass).\
                                                                      hash_code());\
    }

#define EFPROPERTY(Property, Flags, ...)\
    EFReflectionManager::Get()\
    .RegisterProperty(clsHash,\
                      {\
                          #Property,\
                          getMemberOffset(&registeringClass::Property),\
                          typeid(decltype(registeringClass::Property)).name(),\
                          Flags,\
                          EFMetaDataList{__VA_ARGS__}\
                      }\
    );

#define EFMETHOD(Method, Flags, ...) \
    using MethodPointer = decltype(&registeringClass::Method);\
    using CallableT = EFCallableMethod<MethodPointer>;\
    auto methodData = make_method_data(&registeringClass::Method);\
    auto efCallable = std::make_unique<CallableT>(methodData);\
\
    EFReflectionManager::Get()\
    .RegisterMethod(clsHash,\
                    EFMethod{\
                        #Method,\
                        std::move(efCallable),\
                        Flags,\
                        EFMetaDataList{__VA_ARGS__}\
                    });

#define EFREGISTER_END(ClassName) \
    return true; \
    }

}

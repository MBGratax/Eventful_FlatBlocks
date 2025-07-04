#pragma once

#include "CoreTypes.h"
#include <any>
#include "EnumFlag.h"
#include <typeindex>

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
        EFString KeyName;
        std::vector<EFString> ValueName;
    };

    using EFMetaDataList = std::vector<EFMetaData>;
    // TODO: Move Property and Method stuff into their own files, this is getting bloated
    // TODO: REFACTOR: Change some void pointers to EFObject pointers, since only EFObjects and their children can even be reflected.
    template <typename Member>
    struct EFMemberPointer;

    template <typename Class, typename Member>
    struct EFMemberPointer<Member Class::*>{
        using MemberPtr = Member Class::*;
        using ClassType = Class;
        using MemberType = Member;

        MemberPtr varReference;

        explicit EFMemberPointer(const MemberPtr varReference): varReference(varReference){
        };

        void Set(Class* reference, Member value) const{
            reference->*varReference = value;
        }

        Member Get(const Class* reference) const{
            return reference->*varReference;
        };
    };

    // Deduction helper
    template <typename MemberPtr>
    auto MakeMemberData(MemberPtr m){
        return EFMemberPointer<MemberPtr>{m};
    }

    struct EFAutoProperty{
        virtual ~EFAutoProperty() = default;

        virtual std::any Get(const void* obj) const = 0;

        virtual void Set(void* obj, std::any value) = 0;
    };

    template <typename MemberPointer>
    struct EFAutoPropertyImp final : EFAutoProperty{
        using Traits = EFMemberPointer<MemberPointer>;
        using ClassType = typename Traits::ClassType;
        using MemberType = typename Traits::MemberType;

        EFMemberPointer<MemberPointer> memberPointer;

        explicit EFAutoPropertyImp(const EFMemberPointer<MemberPointer>& md) : memberPointer(md){
        }

        std::any Get(const void* obj) const override{
            auto* instance = static_cast<const ClassType*>(obj);
            MemberType value = memberPointer.Get(instance);
            return value;
        }

        void Set(void* obj, std::any value) override{
            auto* instance = static_cast<ClassType*>(obj);
            MemberType memberValue = std::any_cast<MemberType>(value);
            memberPointer.Set(instance, memberValue);
        }
    };

    struct EFProperty{
        EFString Name;
        std::shared_ptr<EFAutoProperty> AutoProperty;
        std::type_index Type{typeid(void)};
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
        template <typename T, typename Sig, typename Ret, std::size_t... I>
        std::any invokeInternal(T* obj, const std::vector<std::any>& args,
                                std::index_sequence<I...>){
            // Unpack and cast each argument
            if constexpr (std::is_void_v<typename Sig::Return>){
                signature.invoke(obj,
                                 std::any_cast<std::tuple_element_t<I, typename Sig::ArgTypes>>(args[I])...
                );
                return {};
            }
            else{
                Ret result = signature.invoke(obj,
                                              std::any_cast<std::tuple_element_t<I, typename Sig::ArgTypes>>(args[I])...
                );
                return result;
            }
        }
    };

    struct EFMethod{
        EFString Name;
        std::unique_ptr<EFCallable> Callable;
        E_MethodFlags Flags{E_MethodFlags::None};
        EFMetaDataList MetaData;
    };

    struct EFClass{
        EFString Name;
        std::size_t Hash{0};
        std::size_t ParentHash{0};
        std::type_index ClassType{typeid(void)};
        E_ClassFlags Flags{E_ClassFlags::None};
        std::vector<EFProperty> Properties;
        std::vector<EFMethod> Methods;
        EFMetaDataList MetaData;
    };

#define EFCLASS(ClassName, ParentClassName, Flags, ...)\
    public:\
    using _objClass = ClassName;\
    using _superClass = ParentClassName;\
    static EFClassPtr _efClass;\
    static const EFClass& StaticClass(){ return _efClass; }\
    inline static EFString _name{#ClassName};\
    static constexpr auto _efClassFlags = Flags;\
    static inline EFMetaDataList _efClassMetadata = {__VA_ARGS__};\
    virtual const EFString& GetClassName() const{ return _name; }\
    virtual bool IsClass(const EFString& name) const{\
        if (EFReflectionManager::Get().GetClass(name) != nullptr){ return true; }\
        return IsClass(_superClass::_name);\
    }\
    private: \
    struct _Registrar{ \
    _Registrar(){ \
    auto& efClassPtr = std::make_shared<EFClass>()\
    efClassPtr->Name = _name;\
    efClassPtr->Hash =typeid(ClassName).hash_code();\
    efClassPtr->ParentHash = typeid(ParentClassName).hash_code();\
    efClassPtr->ClassType = typeid(ClassName);\
    efClassPtr->Flags = Flags;\
    efClassPtr->MetaData = {__VA_ARGS__};\
    _efClass = EFReflectionManager::Get().RegisterClass(efClassPtr); \
    } \
    }; \
    inline static _Registrar _registrar;\
friend bool _ReflectedClass_##ClassName();

#define EFREGISTER(ClassName) \
    bool _ReflectedClass_##ClassName();\
    EFClassPtr ClassName::_efClass = nullptr;\
    bool _ReflectedClass_##ClassName(){\
    using registeringClass = ClassName;\
    const auto clsHash = typeid(registeringClass).hash_code();\
    registeringClass::_efClass = EFReflectionManager::Get().GetClass(clsHash);\
    if (!registeringClass::_efClass){\
        auto efClassPtr = std::make_shared<EFClass>();\
        efClassPtr->Name = registeringClass::_name;\
        efClassPtr->Hash = clsHash;\
        efClassPtr->ParentHash = typeid(registeringClass::_superClass).hash_code();\
        efClassPtr->ClassType = typeid(registeringClass);\
        efClassPtr->Flags = registeringClass::_efClassFlags;\
        efClassPtr->MetaData = registeringClass::_efClassMetadata;\
        registeringClass::_efClass = EFReflectionManager::Get().RegisterClass(efClassPtr);\
    }\
RegisterJsonType<ClassName>();

#define EFPROPERTY(Property, Flags, ...)\
using MemberPointer = decltype(&registeringClass::Property);\
using AutoPropertyT = EFAutoPropertyImp<MemberPointer>;\
auto memberData = MakeMemberData(&registeringClass::Property);\
auto efAutoProperty = std::make_unique<AutoPropertyT>(memberData);\
\
    EFReflectionManager::Get()\
    .RegisterProperty(clsHash,\
                      {\
                          #Property,\
                          std::move(efAutoProperty),\
                          std::type_index{typeid(decltype(registeringClass::Property))},\
                          Flags,\
                          EFMetaDataList{__VA_ARGS__}\
                      }\
    );\
RegisterJsonType<AutoPropertyT::MemberType>();

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

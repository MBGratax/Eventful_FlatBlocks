#pragma once

// TODO: Structs for ClassData, PropertyData and MethodData

/*! TODO: Improve this macro
 * TODO: Introduce EFPROPERTY and EFMETHOD macros to reflect those
 * TODO: Make DECLAREREFLECT macro to put into one C++ file to autoregister the whole class
 * @brief A class body generator macro that generates some generic groundwork and initializes the class in the reflection register
 * @param ClassName The name of the class this belongs to
 * @param ParentName The parent class of this class
 */
#define EFCLASS(ClassName, ParentName)                                   \
public:                                                                  \
    using _class = ClassName;                                            \
    using _superClass = ParentName;                                      \
                                                                         \
    static const std::string& StaticClassName() {                        \
        static const std::string name{#ClassName};                       \
        return name;                                                     \
    }                                                                    \
                                                                         \
    virtual const std::string& GetClassName() const override {           \
        return ClassName::StaticClassName();                             \
    }                                                                    \
\
virtual bool IsClass(const std::string &name)const override{\
return (name == (ClassName::StaticClassName())) ? true : ParentName::IsClass(name)\
\
private:                                                                 \
    struct __Registrar {                                                 \
        __Registrar() {                                                  \
            ReflectionRegisterManager::Get().RegisterClass(              \
                ClassName::StaticClassName(),                            \
                typeid(ClassName).hash_code(),                           \
                typeid(ParentName).hash_code()                           \
            );                                                           \
        }                                                                \
    };                                                                   \
                                                                         \
inline static __Registrar __registrar;

#pragma once

namespace EventfulEngine{

    /*!
     * @brief IManager provides a generic Manager/Singleton Interface. These objects only ever have one instance and can be accessed as long as you know the type.
     * @tparam T The template describes the type of the child class, meaning the manager is a manager of type T
     */
    template <typename T>
    class IManager{
    public:
        static T& Get(){
            static T instance;
            return instance;
        }

        IManager(const IManager&) = delete;

        IManager& operator=(const IManager&) = delete;

    protected:
        IManager() = default;

        ~IManager() = default;
    };

}

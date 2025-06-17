#pragma once
#include <EfMemory.h>
#include <cwchar>
#include <string>
#include <vector>

namespace EventfulEngine{
    /*!
     * @brief A base object class for all managed objects in Eventful.
     */
    class EFObject{
    public:
        EFObject() = default;

        EFObject(const EFObject&) = delete;

        EFObject& operator=(const EFObject&) = delete;

        virtual ~EFObject() = default;

        // Custom new/delete, so all objects go through EFMemory.
        void* operator new(const std::size_t size){ return EFMemory::Malloc(size); }
        void operator delete(void* ptr){ EFMemory::Free(ptr); }

        template <typename T, typename... Args>
        static EFSharedPtr<T> Create(EFObject* owner, const std::string& name, Args&&... args){
            auto obj = MakeShared<T>(std::forward<Args>(args)...);
            obj->_owner = owner;
            obj->_objectName = name;
            obj->_bIsValid = true;
            return obj;
        }

        template <typename T>
        static void Destroy(EFSharedPtr<T>& obj){ obj.reset(); }

        bool IsValid() const{ return _bIsValid; }

        EFObject* GetOuter() const{ return _owner; }
        void SetOuter(EFObject* outer){ _owner = outer; }

        const std::string& GetName() const{ return _objectName; }
        void Rename(const std::string& newName){ _objectName = newName; }

        std::string GetPath() const{
            if (_owner){ return _owner->GetPath() + "/" + _objectName; }
            return _objectName;
        }

        // Tagging -----------------------------------------------------------
        void AddTag(const std::string& tag){
            if (!HasTag(tag)) _tags.push_back(tag);
        }

        bool HasTag(const std::string& tag) const{
            return std::ranges::find(_tags, tag) != _tags.end();
        }

        void RemoveTag(const std::string& tag){
            if (const auto it = std::remove(_tags.begin(), _tags.end(), tag); it != _tags.end())
                _tags.erase(
                    it, _tags.end());
        }

        const std::vector<std::string>& GetTags() const{ return _tags; }

        // Duplication Hooks -------------------------------------------------
        virtual void PreDuplicate(const EFObject& source){
        }

        virtual void PostDuplicate(){
        }

        // Serialization & GC callbacks -------------------------------------
        virtual void Serialize(){
        }

        virtual void OnGC(){
        }

    protected:
        bool _bIsValid{true};
        EFObject* _owner{nullptr};
        std::string _objectName;
        std::vector<std::string> _tags;
    };
}

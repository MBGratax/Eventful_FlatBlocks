#pragma once
#include <EFGUID.h>

#include "CoreTypes.h"
#include "EFReflectionManager.h"
#include "EfMemory.h"
#include <cwchar>
#include <vector>
#include "EFClass.h"
#include <algorithm>

namespace EventfulEngine{
    class JsonArchive;
    /*!
     * @brief A base object class for all managed objects in Eventful.
     */
    class EFCORE_API EFObject{

    public:
        using _objClass = EFObject;
        using _superClass = EFObject;
        static EFClassPtr _efClass;
        static const EFClass& StaticClass(){ return *_efClass.get(); }

        inline static EFString _name{"EFObject"};

        static inline auto _efClassFlags = E_ClassFlags::None;
        static inline auto _efClassMetadata = EFMetaDataList{{"Category", {"Object"}}};
        virtual const EFString& GetClassName() const{ return _name; }

        virtual bool IsClass(const EFString& name) const{
            if (EFReflectionManager::Get().GetClass(name) != nullptr){ return true; }
            return false;
        }

        friend bool _ReflectedClass_EFObject();

        EFObject() = default;

        EFObject(const EFObject&) = delete;

        EFObject& operator=(const EFObject&) = delete;

        virtual ~EFObject() = default;

        template <typename T, typename... Args>
        static std::shared_ptr<T> Create(EFObject const* owner, const EFString& name, const int GUID, Args&&... args){
            auto obj = MakeShared<T>(std::forward<Args>(args)...);
            obj->_owner = owner;
            obj->_objectName = name;
            obj._guid = GUID;
            obj->_bIsValid = true;
            return obj;
        }

        template <typename T>
        static void Destroy(std::shared_ptr<T>& obj){ obj.reset(); }

        [[nodiscard]] bool IsValid() const{ return _bIsValid; }

        [[nodiscard]] EFObject* GetOwner() const{ return _owner; }
        void SetOuter(EFObject* outer){ _owner = outer; }

        EFGUID GetGUID() const{ return _guid; }

        [[nodiscard]] const EFString& GetName() const{ return _objectName; }
        void Rename(const std::string_view newName){ _objectName = newName; }

        [[nodiscard]] EFString GetPath() const{
            if (_owner){ return _owner->GetPath() + "/" + _objectName; }
            return _objectName;
        }

        // Tagging -----------------------------------------------------------
        void AddTag(const EFString& tag){
            if (!HasTag(tag)) _tags.push_back(tag);
        }

        [[nodiscard]] bool HasTag(const EFString& tag) const{
            return std::ranges::find(_tags, tag) != _tags.end();
        }

        void RemoveTag(const EFString& tag){
            if (const auto it = std::ranges::remove(_tags, tag).begin(); it != _tags.end())
                _tags.erase(
                    it, _tags.end());
        }

        [[nodiscard]] const std::vector<EFString>& GetTags() const{ return _tags; }

        // Duplication Hooks -------------------------------------------------
        virtual void PreDuplicate(const EFObject& source){
            //stub
            _bIsValid = false;
        }

        virtual void PostDuplicate(){
            //stub
            _bIsValid = true;
        }

        // Serialization & GC callbacks -------------------------------------
        virtual void Serialize(JsonArchive& ar) const;

        virtual void Deserialize(const JsonArchive& ar);

    protected:
        bool _bIsValid{true};
        EFObject* _owner{nullptr};
        EFString _objectName;
        // TODO: Make actual GUID
        EFGUID _guid{};
        std::vector<EFString> _tags;
    };
}

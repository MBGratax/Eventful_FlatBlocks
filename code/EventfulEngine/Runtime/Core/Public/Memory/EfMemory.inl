#pragma once

namespace EventfulEngine{

    template <typename T, typename... Args>
    inline EFUniquePtr<T> MakeUnique(Args&&... args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline EFSharedPtr<T> MakeShared(Args&&... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

} // namespace EventfulEngine

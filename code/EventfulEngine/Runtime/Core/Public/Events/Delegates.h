#pragma once
#include <functional>
#include <vector>
#include <ranges>

namespace EventfulEngine{
    // TODO: Create Delegate template to use with events
        /**
         * @brief Delegate allowing registration of multiple callables.
         *
         * Delegates store a list of functions with a common signature and
         * provide a simple broadcast interface.
         */
        template <typename Signature>
        struct Delegate;

        template <typename Ret, typename... Args>
        struct Delegate<Ret(Args...)>{
        public:
            using EFFunction = std::function<Ret(Args...)>;

            /** Add a new listener to this delegate. */
            void Add(EFFunction func){ listeners.emplace_back(std::move(func)); }

            /** Remove all registered listeners. */
            void Clear(){ listeners.clear(); }

            void Remove(EFFunction func){listeners.erase(std::remove(std::ranges::range(listeners.begin(),listeners.end()), func), listeners.end());}

            /** Invoke all registered listeners in order. */
            Ret Broadcast(Args... args) const{
                if constexpr (std::is_void_v<Ret>){
                    for (const auto& f : listeners){
                        f(args...);
                    }
                    return;
                }
                else{
                    Ret result{};
                    for (const auto& f : listeners){
                        result = f(args...);
                    }
                    return result;
                }
            }

            Delegate& operator+=(EFFunction func){
                Add(std::move(func));
                return *this;
            }
            Delegate& operator-=(EFFunction func){
                Remove(func);
                return *this;
            }
        private:
            std::vector<EFFunction> listeners;
        };
}

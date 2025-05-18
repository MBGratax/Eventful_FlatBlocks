#pragma once
#include <functional>

namespace EventfulEngine::Events{

    /**
     * @brief Generic delegate that stores callbacks
     * @param Ret The return type of the delegate
     * @param Args the arguments the delegate can take
     */
    template <typename Ret, typename... Args>
    class EFEvents<Ret(Args...)>{
    public:
        using CallbackType = std::function<Ret(Args...)>;

        /**
         * @brief Subscribe a callback to the delegate.
         *
         * @param callback The callback function.
         * @return int An ID that can be used to unsubscribe.
         */
        int Subscribe(CallbackType callback){
            int id = nextId++;
            subscribers[id] = callback;
            return id;
        }

        /**
         * @brief Unsubscribe a callback.
         *
         * @param id The subscription ID to remove.
         */
        void Unsubscribe(int id){
            subscribers.erase(id);
        }

        /**
         * @brief Invoke all subscriber callbacks with the provided arguments.
         *
         * @param args The arguments to pass to the callbacks.
         */
        void Invoke(Args... args){
            for (auto& [id, callback] : subscribers){
                callback(args...);
            }
        }

    private:
        int nextId = 0;
        std::unordered_map<int, CallbackType> subscribers;
    };

    /**
     * @brief A simple event bus interface.
     *
     * Subscribers can register callbacks for a given event type, and publishers
     * can dispatch events to all subscribers.
     *
     * Usage example:
     * @code
     *   struct MyEvent {
     *   int data;
     *   };
     *
     *   EventfulEngine::Events::EFEventHandler e_myEventHandler;
     *   int subId = e_myEventHandler.Subscribe<MyEvent>([](const MyEvent& e) {
     *       // Handle the event.
     *   };
     *
     *   MyEvent oneEvent{42};
     *   e_myEventHandler.Publish(oneEvent);
     *
     *   // To unsubscribe:
     *   bus.Unsubscribe<MyEvent>(subId);
     * @endcode
     */
    class EFEventHandler{
    public:
        /// Subscribe a callback to events of type EventType.
        template <typename EventType>
        int Subscribe(std::function<void(const EventType&)> callback){
            return GetDelegate<EventType>().Subscribe(callback);
        }

        /// Unsubscribe from events of type EventType.
        template <typename EventType>
        void Unsubscribe(int id){
            GetDelegate<EventType>().Unsubscribe(id);
        }

        /// Publish an event to all subscribers of that event type.
        template <typename EventType>
        void Publish(const EventType& event){
            GetDelegate<EventType>().Invoke(event);
        }

    private:
        /// Retrieve the delegate for a given event type.
        template <typename EventType>
        static EFEvents<void(const EventType&)>& GetDelegate(){
            // A static delegate instance is created for each event type.
            static EFEvents<void(const EventType&)> delegate;
            return delegate;
        }
    };

}

#pragma once
#include <functional>
#include <deque>
#include "EFCoreModuleAPI.h"

namespace EventfulEngine{


    /**
     * @struct UndoStep
     * @brief Encapsulates a single undo/redo operation.
     * An UndoStep holds two callable actions: one to undo a change and another to redo it.
     */
    struct UndoStep{
        std::function<void()> UndoAction;
        std::function<void()> RedoAction;
    };

    /**
     * @class UndoStack
     * @brief Manages a history of undo and redo steps.
     *
     * The UndoStack stores a fixed number of UndoStep entries. When you
     * perform an operation via Do(), it records how to undo and redo it.
     * You can then call Undo() or Redo() to traverse the history.
     *
     * @note Once the number of steps exceeds the maximum, the oldest
     *       entries are discarded.
     *
     * @example
     * @code
     * // Create an undo stack that holds up to 10 steps.
     * EFCore::UndoStack stack(10);
     *
     * // Example data we want to modify.
     * Int value = 0;
     *
     * // Perform an operation: increment value by 5.
     * // Record how to undo (subtract 5) and redo (add 5).
     * Stack.Do({
     *     [&value](){ value -= 5; },  // UndoAction
     *     [&value](){ value += 5; }   // RedoAction
     * });
     *
     * // Alternatively record the action via Stack.Record and perform it yourself:
     * Stack.Record({
     *     [&value](){ value -= 5; },  // UndoAction
     *     [&value](){ value += 5; }   // RedoAction
     * });
     * value += 5;
     *
     * // Both are functionally the same, Record and Do both add an UndoStep but Do also performs the RedoAction
     *
     * // Value is now at 10, and we have two UndoSteps recorded.
     *
     * // While you could check for CanUndo() here, Undo does so internally either way and simply doesn't do anything
     * stack.Undo();
     * // Value = 5
     * stack.Redo();
     * // Value is 10 again
     *
     *
     * // Clear all history.
     * Stack.Clear();
     * @endcode
     */
    class EFCORE_API UndoStack{
    public:
        explicit UndoStack(size_t maxUndoCount = 20);

        void Do(UndoStep step);

        void Record(UndoStep step);

        void Undo();

        void Redo();

        void Clear();

        bool CanUndo() const;

        bool CanRedo() const;

    private:
        size_t _maxUndoCount = 20;
        std::deque<UndoStep> _undos;
        std::deque<UndoStep> _redos;
    };

} // EventfulEngine

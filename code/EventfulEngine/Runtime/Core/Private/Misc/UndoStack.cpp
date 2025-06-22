#pragma once

#include "UndoStack.h"

namespace EventfulEngine{

    UndoStack::UndoStack(const size_t maxUndoCount) : _maxUndoCount(maxUndoCount){
    }

    void UndoStack::Do(UndoStep step){
        if (_undos.size() >= _maxUndoCount){
            _undos.pop_front();
        }
        step.RedoAction();
        _undos.push_back(std::move(step));
    }

    void UndoStack::Record(UndoStep step){
        if (_undos.size() >= _maxUndoCount){
            _undos.pop_front();
        }
        _undos.push_back(std::move(step));
    }

    void UndoStack::Undo(){
        if (!CanUndo()){
            return;
        }
        auto step = std::move(_undos.back());
        _undos.pop_back();
        step.UndoAction();
        if (_redos.size() >= _maxUndoCount){
            _redos.pop_front();
        }
        _redos.push_back(std::move(step));
    }

    void UndoStack::Redo(){
        if (!CanRedo()){
            return;
        }
        auto step = std::move(_redos.back());
        _redos.pop_back();
        step.RedoAction();
        if (_undos.size() >= _maxUndoCount){
            _undos.pop_front();
        }
        _undos.push_back(std::move(step));
    }

    void UndoStack::Clear(){
        _undos.clear();
        _redos.clear();
    }

    bool UndoStack::CanUndo() const{
        return !_undos.empty();
    }

    bool UndoStack::CanRedo() const{
        return !_redos.empty();
    }
} // EventfulEngine

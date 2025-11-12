#ifndef STATE_FACTORY_H
#define STATE_FACTORY_H

#include <unordered_map>
#include <memory>
#include "core/stateMachine/state.h"

using namespace StateMachine;

enum class StateId {
    StartUp,
    Idle,
    Cardholder
};

class StateFactory {
public:
    static std::shared_ptr<State> get(StateId id);
    // Optional: clear/reset all state instances
    static void reset() {
        getStateMap().clear();
    }

private:
    static std::unordered_map<StateId, std::unique_ptr<State>>& getStateMap() {
        static std::unordered_map<StateId, std::unique_ptr<State>> stateMap;
        return stateMap;
    }
};


#endif
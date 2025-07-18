#include "stateFactory.h"
#include "startup.h"
#include "idle.h"

std::shared_ptr<State> StateFactory::get(StateId id)
{
    static std::unordered_map<StateId, std::shared_ptr<State>> instances;

    auto it = instances.find(id);
    if (it != instances.end())
        return it->second;

    switch (id) {
        case StateId::StartUp:
            instances[id] = std::make_shared<StartUp>(nullptr);
            break;
        case StateId::Idle:
            instances[id] = std::make_shared<Idle>(instances[StateId::StartUp]);
            break;
        default:
            return nullptr;
    }

    return instances[id];
}
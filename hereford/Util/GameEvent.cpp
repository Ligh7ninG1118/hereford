#include "GameEvent.h"

std::unordered_map<std::type_index, std::vector<GameEvent::SubscriptionBase*>> GameEvent::mListenerMap;

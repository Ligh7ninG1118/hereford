#include "DelayedAction.h"

unsigned int DelayedActionManager::topIndex = 1;
std::unordered_map<unsigned int, std::unique_ptr<DelayedActionManager::DelayedAction>> DelayedActionManager::mDelayedActionMap;
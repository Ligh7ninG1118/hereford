#include "TimelineAction.h"

unsigned int TimelineActionManager::topIndex = 1;
std::unordered_map<unsigned int, std::unique_ptr<TimelineActionManager::TimelineAction>> TimelineActionManager::mTimelineActionMap;
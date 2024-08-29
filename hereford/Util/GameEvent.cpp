#include "GameEvent.h"

//TODO: C++ why u r like this 
std::vector<GameEvent::Subscription<void>*> GameEvent::mListenerMap;

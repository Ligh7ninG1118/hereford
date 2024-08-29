#pragma once

#include "Events.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <typeindex>

class GameEvent
{
public:
	struct Subscription
	{
		std::function<void(EventTest)> mCallback;

		Subscription(std::function<void(EventTest)> inCallback) : mCallback(inCallback) {}
		~Subscription()
		{
			GameEvent::Unsubscribe(this);
		}
	};


	static void Publish(EventTest inEvent)
	{
		for (auto& baseSub : mListenerMap)
		{
			//Subscription* sub = std::static_pointer_cast<Subscription<T>>(baseSub);
			baseSub->mCallback(inEvent);
		}
	}

	static Subscription* Subscribe(std::function<void(EventTest)> inCallback)
	{
		auto newSub = new Subscription(inCallback);
		mListenerMap.push_back(newSub);
		return newSub;
	}


	static void Unsubscribe(Subscription* inSubscription)
	{
		auto it = std::find(mListenerMap.begin(), mListenerMap.end(), inSubscription);
		if (it != mListenerMap.end())
			mListenerMap.erase(it);
		
	}

private:
	//std::unordered_map<std::type_index, std::vector<Subscription*>> mListenerMap;
	static std::vector<Subscription*> mListenerMap;

};



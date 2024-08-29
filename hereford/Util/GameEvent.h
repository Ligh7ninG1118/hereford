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
	template <typename TEvent>
	struct Subscription
	{
		std::function<void(TEvent)> mCallback;

		Subscription(std::function<void(TEvent)> inCallback) : mCallback(inCallback) {}
		~Subscription()
		{
			GameEvent::Unsubscribe(this);
		}
	};

	template <typename TEvent>
	static void Publish(TEvent inEvent)
	{
		for (auto& baseSub : mListenerMap)
		{
			Subscription<TEvent>* sub = (Subscription<TEvent>*)baseSub;
			sub->mCallback(inEvent);
		}
	}

	template <typename TEvent>
	static Subscription<TEvent>* Subscribe(std::function<void(TEvent)> inCallback)
	{
		auto newSub = new Subscription<TEvent>(inCallback);
		Subscription<void>* subVoid = (Subscription<void>*)newSub;
		mListenerMap.push_back(subVoid);
		return newSub;
	}

	template <typename TEvent>
	static void Unsubscribe(Subscription<TEvent>* inSubscription)
	{
		auto it = std::find(mListenerMap.begin(), mListenerMap.end(), inSubscription);
		if (it != mListenerMap.end())
			mListenerMap.erase(it);
		
	}

private:
	//std::unordered_map<std::type_index, std::vector<Subscription*>> mListenerMap;
	static std::vector<Subscription<void>*> mListenerMap;

};



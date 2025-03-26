#pragma once


#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

//TODO: Switch to use smart pointers
class GameEvent
{
private:
	class SubscriptionBase
	{
	public:
		virtual ~SubscriptionBase() = default;
	};

public:
	template <typename TEvent>
	class Subscription : public SubscriptionBase
	{
	public:
		Subscription(std::function<void(TEvent)> inCallback)
			: mCallback(inCallback)
		{
		}

		~Subscription()
		{
			GameEvent::Unsubscribe<TEvent>(this);
		}

		std::function<void(TEvent)> GetCallback() const { return mCallback; }
	private:
		std::function<void(TEvent)> mCallback;
	};

	template <typename TEvent>
	static void Publish(TEvent inEvent)
	{
		auto t = std::type_index(typeid(TEvent));
		if (mListenerMap.find(t) == mListenerMap.end())
			return;

		auto& listeners = mListenerMap[t];
		std::vector<Subscription<TEvent>*> orphanedSubscriptions;
		for (const auto& l_void : listeners)
		{
			auto l = static_cast<Subscription<TEvent>*>(l_void);
			if (l->GetCallback() == nullptr)
			{
				orphanedSubscriptions.push_back(l);
			}
			else
			{
				l->GetCallback()(inEvent);
			}
		}

		for (const auto& o : orphanedSubscriptions)
		{
			Unsubscribe<TEvent>(o);
		}
	}

	template <typename TEvent>
	static Subscription<TEvent>* Subscribe(std::function<void(TEvent)> inCallback)
	{
		auto t = std::type_index(typeid(TEvent)); 
		auto newSub = new Subscription<TEvent>(inCallback);
		auto subVoid = static_cast<SubscriptionBase*>(newSub);
		mListenerMap[t].push_back(subVoid);

		return newSub;
	}

	template <typename TEvent>
	static void Unsubscribe(Subscription<TEvent>* inSubscription)
	{
		auto t = std::type_index(typeid(TEvent));
		if (mListenerMap.find(t) == mListenerMap.end())
			return;

		auto& listeners = mListenerMap[t];
		auto inSubVoid = static_cast<SubscriptionBase*> (inSubscription);

		auto it = std::find(listeners.begin(), listeners.end(), inSubVoid);
		if (it != listeners.end())
			listeners.erase(it);
	}

private:
	static std::unordered_map<std::type_index, std::vector<SubscriptionBase*>> mListenerMap;
};



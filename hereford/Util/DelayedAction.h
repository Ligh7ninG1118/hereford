#pragma once
#include <functional>
#include <unordered_map>
#include <memory>

struct DAHandle
{
	DAHandle() : mIndex(0) {};
	unsigned int mIndex;
};



class DelayedActionManager
{
private:
	class DelayedAction
	{
	public:
		DelayedAction(float inTime, bool shouldLoop, std::function<void()> inCallback)
			: mTotalTime(inTime), mRemainingTime(inTime), mShouldLoop(shouldLoop), mPendingDelete(false), mCallback(inCallback)
		{}

		void Execute() const { mCallback(); }

		void Reset() { mRemainingTime = mTotalTime; }

		friend class DelayedActionManager;

	private:
		float mTotalTime;
		float mRemainingTime;
		bool mShouldLoop;
		bool mPendingDelete;

		std::function<void()> mCallback;
	};

public:
	static void AddAction(DAHandle& inHandle, const std::function<void()>& inCallback, float inPeriod, bool shouldLoop)
	{
		// If handle doesnt own any action before
		if (mDelayedActionMap.find(inHandle.mIndex) == mDelayedActionMap.end())
		{
			// Find next free index (useful if index went overbound and looped over) & skip 0
			while (mDelayedActionMap.find(topIndex) != mDelayedActionMap.end() || topIndex == 0)
				topIndex++;

			inHandle.mIndex = topIndex++;
		}

		// Previous action (if any) will be deleted automatically (unique ptr)
		mDelayedActionMap[inHandle.mIndex] = std::make_unique<DelayedAction>(DelayedAction(inPeriod, shouldLoop, inCallback));
	}

	static void RemoveAction(const DAHandle& inHandle)
	{
		if (mDelayedActionMap.find(inHandle.mIndex) != mDelayedActionMap.end())
		{
			mDelayedActionMap[inHandle.mIndex]->mPendingDelete = true;
		}
	}

	static void UpdateTimers(float deltaTime)
	{
		for (auto it = mDelayedActionMap.begin(); it!= mDelayedActionMap.end();)
		{
			auto& action = it->second;

			action->mRemainingTime -= deltaTime;
			if (action->mRemainingTime <= 0.0f)
			{
				action->Execute();

				// Additional guard if action is somehow marked for delete during execution
				if (!action->mPendingDelete)
				{
					if (!action->mShouldLoop)
						action->mPendingDelete = true;
					else
						action->Reset();
				}
			}
			++it;
		}

		// Remove all pending delete actions
		for (auto it = mDelayedActionMap.begin(); it != mDelayedActionMap.end();)
		{
			auto& action = it->second;

			if (action->mPendingDelete)
			{
				it = mDelayedActionMap.erase(it);
			}
			else
				++it;
		}
	}

private:
	static unsigned int topIndex;
	static std::unordered_map<unsigned int, std::unique_ptr<DelayedAction>> mDelayedActionMap;
};
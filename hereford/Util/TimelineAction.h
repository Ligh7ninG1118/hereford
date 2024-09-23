#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <Math/Math.h>

struct TAHandle
{
	TAHandle() : mIndex(0) {};
	unsigned int mIndex;
};


class TimelineActionManager
{
private:
	class TimelineAction
	{
	public:
		TimelineAction(float inTime, bool isReverse, std::function<void(float)> inCallback)
			: mTotalTime(inTime), mCurrentAlpha(0.0f), mIsReverse(isReverse), mPendingDelete(false), mCallback(inCallback)
		{
			mCurrentAlpha = isReverse ? 1.0f : 0.0f;
		}

		void Execute(float deltaTime) 
		{ 
			float diff = deltaTime / mTotalTime;
			if (mIsReverse)
				diff *= -1.0f;

			mCurrentAlpha += diff;
			if (mCurrentAlpha >= 1.0f || mCurrentAlpha <= 0.0f)
				mPendingDelete = true;

			mCurrentAlpha = Math::Clamp(mCurrentAlpha, 0.0f, 1.0f);

			mCallback(mCurrentAlpha);
		}

		friend class TimelineActionManager;

	private:
		float mTotalTime;
		float mCurrentAlpha;
		bool mIsReverse;
		bool mPendingDelete;

		std::function<void(float)> mCallback;
	};

public:
	static void PlayFromStart(TAHandle& inHandle, const std::function<void(float)>& inCallback, float inPeriod)
	{
		AddAction(inHandle, inCallback, inPeriod, false);
	}

	static void ReverseFromEnd(TAHandle& inHandle, const std::function<void(float)>& inCallback, float inPeriod)
	{
		AddAction(inHandle, inCallback, inPeriod, true);
	}

	static void RemoveAction(const TAHandle& inHandle)
	{
		if (mTimelineActionMap.find(inHandle.mIndex) != mTimelineActionMap.end())
		{
			mTimelineActionMap[inHandle.mIndex]->mPendingDelete = true;
		}
	}

	static void UpdateTimers(float deltaTime)
	{
		for (auto it = mTimelineActionMap.begin(); it != mTimelineActionMap.end();)
		{
			auto& action = it->second;
			action->Execute(deltaTime);
			++it;
		}

		// Remove all pending delete actions
		for (auto it = mTimelineActionMap.begin(); it != mTimelineActionMap.end();)
		{
			auto& action = it->second;

			if (action->mPendingDelete)
			{
				it = mTimelineActionMap.erase(it);
			}
			else
				++it;
		}
	}

private:
	static void AddAction(TAHandle& inHandle, const std::function<void(float)>& inCallback, float inPeriod, bool isReverse)
	{
		// If handle doesnt own any action before
		if (mTimelineActionMap.find(inHandle.mIndex) == mTimelineActionMap.end())
		{
			// Find next free index (useful if index went overbound and looped over) & skip 0
			while (mTimelineActionMap.find(topIndex) != mTimelineActionMap.end() || topIndex == 0)
				topIndex++;

			inHandle.mIndex = topIndex++;
		}

		// Previous action (if any) will be deleted automatically (unique ptr)
		mTimelineActionMap[inHandle.mIndex] = std::make_unique<TimelineAction>(TimelineAction(inPeriod, isReverse, inCallback));
	}


	static unsigned int topIndex;
	static std::unordered_map<unsigned int, std::unique_ptr<TimelineAction>> mTimelineActionMap;
};





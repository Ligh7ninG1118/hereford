#pragma once
#include "Core/Actor.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <memory>

#define REFLECT(__CLASS__) \
public: \
	static const std::string& StaticClassName() {static std::string name = #__CLASS__; return name; } \
	virtual const std::string& GetClassName() const override {return StaticClassName(); } \
	static std::unique_ptr<Actor> CreateInstance(class GameContext* gameCtx) {return std::make_unique<__CLASS__>(gameCtx);} \
	static void RegisterClass() {ReflectionRegistry::Instance().RegisterClass(StaticClassName(), CreateInstance); }\
private: \
	static inline const bool __registered = []() {__CLASS__::RegisterClass(); return true;}(); 

class ReflectionRegistry
{
public:
	using CreateInstanceFunc = std::unique_ptr<Actor> (*)(class GameContext*);
	static ReflectionRegistry& Instance()
	{
		static ReflectionRegistry instance;
		return instance;
	}

	void RegisterClass(const std::string& className, CreateInstanceFunc createFunc)
	{
		mRegistry[className] = createFunc;
	}

	std::unique_ptr<Actor> CreateInstance(const std::string& className, class GameContext* gameCtx) const
	{
		auto it = mRegistry.find(className);
		if (it != mRegistry.end())
			return it->second(gameCtx);
		return nullptr;
	}


private:
	std::unordered_map<std::string, CreateInstanceFunc> mRegistry;
};
#pragma once
#include "Core/Actor.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <typeinfo>

#define REFLECT(__CLASS__) \
public: \
	static const std::string& StaticClassName() {static std::string name = #__CLASS__; return name; } \
	virtual const std::string& GetClassName() const override {return StaticClassName(); } \
	static Actor* CreateInstance(class GameContext* gameCtx) {return new __CLASS__(gameCtx);} \
	static void RegisterClass() {ReflectionRegistry::Instance().RegisterClass(StaticClassName(), CreateInstance); }\
private: \
	static inline const bool __registered = []() {__CLASS__::RegisterClass(); return true;}(); 

class ReflectionRegistry
{
public:
	using CreateInstanceFunc = Actor * (*)(class GameContext*);
	static ReflectionRegistry& Instance()
	{
		static ReflectionRegistry instance;
		return instance;
	}

	void RegisterClass(const std::string& className, CreateInstanceFunc createFunc)
	{
		registry_[className] = createFunc;
	}

	Actor* CreateInstance(const std::string& className, class GameContext* gameCtx) const
	{
		auto it = registry_.find(className);
		if (it != registry_.end())
			return it->second(gameCtx);
		return nullptr;
	}


private:
	std::unordered_map<std::string, CreateInstanceFunc> registry_;
};
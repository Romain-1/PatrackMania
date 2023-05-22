#pragma once

#include "Modules/Graphics.h"

class Engine
{
private:
	static inline std::vector<GameObject*> m_gameObjects;
	static inline std::vector<IModule*> m_addedModules;
	static inline std::vector<IModule*> m_modules;
	static inline bool m_running;

public:
	static inline Module::Graphics* Graphics;

public:
	static void Run();
	static void Stop();

	static void Add(GameObject* gameObject) { m_gameObjects.push_back(gameObject); }
	static void Add(IModule* module) { m_addedModules.push_back(module); }
	static void Add(Module::Graphics* graphicModule) { m_addedModules.push_back(graphicModule); Graphics = graphicModule; }
};
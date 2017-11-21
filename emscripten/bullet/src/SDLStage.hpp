

#pragma once


#include <iostream>
#include <string>
#include <functional>

// #include "SDL/SDL.h"
// #include <SDL/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>


class SDLStage
{
private:
	std::function<void(const SDL_Event&)>	m_eventListener;
	std::function<void(int)>				m_updateCallback;
	std::function<void(const SDL_Window&)>	m_renderCallback;

	int m_time_prev = 0;
	bool m_paused = false;
	SDL_Window*		m_pWindow = nullptr;
	SDL_GLContext	m_glContextID = 0;

	int		m_ticksPerFrame = 0;

	bool	m_active = true;
	bool	m_visible = true;

public:

	SDLStage(int width, int height);
	~SDLStage();

	// void setCaption(const std::string& title);

	void step();

public:
	inline bool isActive() const { return m_active; }

	inline void setEventListener (std::function<void(const SDL_Event&)> cb)		{ m_eventListener = cb; }
	inline void setRenderCallback (std::function<void(const SDL_Window&)> cb)	{ m_renderCallback = cb; }
	inline void setUpdateCallback (std::function<void(int)> cb)					{ m_updateCallback = cb; }

public:
	void handleEvent(SDL_Event &event);
	void render();
	void update(int deltaTime);

	// static Uint32 timer_onComplete(Uint32 interval, void *param);
};


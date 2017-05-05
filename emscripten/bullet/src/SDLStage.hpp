
#ifndef D_SDLSTAGE_HPP
#define D_SDLSTAGE_HPP


#include <iostream>
#include <string>
#include <functional>

// #include "SDL/SDL.h"
#include <SDL/SDL.h>


class SDLStage
{
private:
	std::function<void(const SDL_Event&)>	m_eventListener;
	std::function<void(int)>				m_updateCallback;
	std::function<void(const SDL_Surface&)>	m_renderCallback;

	int m_time_prev;
	bool m_paused;
	SDL_Surface* m_pScreen;
	int m_ticksPerFrame;

	bool m_active;

public:

	SDLStage(int width, int height, int frameRate, int flags);
	~SDLStage();

	void setCaption(const std::string& title);

	void step();

public:
	inline bool isActive() const { return m_active; }

	inline void setEventListener (std::function<void(const SDL_Event&)> cb)		{ m_eventListener = cb; }
	inline void setRenderCallback (std::function<void(const SDL_Surface&)> cb)	{ m_renderCallback = cb; }
	inline void setUpdateCallback (std::function<void(int)> cb)					{ m_updateCallback = cb; }

private:
	void handleEvent(SDL_Event &event);
	void render();
	void update(int deltaTime);

	static Uint32 timer_onComplete(Uint32 interval, void *param);
};


#endif // D_SDLSTAGE_HPP
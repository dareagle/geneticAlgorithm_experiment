
#include "SDLStage.hpp"


SDLStage::SDLStage(int width, int height, int frameRate, int flags)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
		return;
	}

	m_pScreen = SDL_SetVideoMode(width, height, 0, flags);

	if (m_pScreen == nullptr)
	{
		std::cout << "Could not set video mode: " << SDL_GetError() << std::endl;
		return;
	}

	m_time_prev = 0;
	m_ticksPerFrame = (int)(1000 / frameRate);
	m_active = true;
	m_paused = false;
}

SDLStage::~SDLStage()
{
	SDL_Quit();
}

void SDLStage::handleEvent(SDL_Event &event)
{
	switch (event.type)
	{
		case SDL_ACTIVEEVENT:
			{
				if (event.active.state & SDL_APPACTIVE)
					m_paused = (event.active.gain == 0);
			}
			break;
		
		// case SDL_VIDEOEXPOSE:
		// 	{
		// 		render();
		// 	}
		// 	break;
		
		case SDL_QUIT:
			{
				m_active = false;
			}
			break;
	}
	
	if (m_eventListener)
		m_eventListener(event);
	
	event.type = -1;
}

void SDLStage::render()
{
	if (m_renderCallback)
		m_renderCallback(*m_pScreen);
}

void SDLStage::setCaption(const std::string& title)
{
	if (m_active)
		SDL_WM_SetCaption(title.c_str(), title.c_str());
}



void SDLStage::step()
{
	SDL_Event event;

#ifndef EMSCRIPTEN

	if (m_paused)
	{
		if (SDL_WaitEvent(&event))
			this->handleEvent(event);
	}
	else
	{

#endif

		while (SDL_PollEvent(&event))
		{
			this->handleEvent(event);

			if (!m_active)
				break;
		}
		
		if (m_active)
		{
			int currentTime = SDL_GetTicks();
			int deltaTime = currentTime - m_time_prev;
			
			this->update(deltaTime);
			this->render();

#ifndef EMSCRIPTEN

			while (deltaTime < m_ticksPerFrame)
			{
				SDL_TimerID timer = SDL_AddTimer(m_ticksPerFrame - deltaTime, SDLStage::timer_onComplete, nullptr);

				SDL_WaitEvent(&event);
				SDL_RemoveTimer(timer);

				if (event.type != SDL_USEREVENT)
				{
					this->handleEvent(event);
					deltaTime = SDL_GetTicks() - m_time_prev;
				}
				else
				{
					event.type = -1;
					break;
				}
			}

#endif

			m_time_prev = currentTime;
		}

#ifndef EMSCRIPTEN
	}
#endif
}


void SDLStage::update(int deltaTime)
{	
	if (m_updateCallback)
		m_updateCallback(deltaTime);
}




// Event Handlers




Uint32 SDLStage::timer_onComplete(Uint32 interval, void *param)
{
	static_cast<void>(interval);
	static_cast<void>(param);
	
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = nullptr;
	userevent.data2 = nullptr;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent (&event);

    return 0;
    
}

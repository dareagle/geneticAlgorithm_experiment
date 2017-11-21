

#pragma once


#include <SDL2/SDL.h>


class IState
{
public:
	virtual void	handleEvent(const SDL_Event&) = 0;
	virtual void	update(int) = 0;
	virtual void	render(const SDL_Window&) = 0;
};

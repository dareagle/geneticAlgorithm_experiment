

#ifndef	D_ISTATE_HPP
#define	D_ISTATE_HPP


#include <SDL/SDL.h>


class IState
{
public:
	virtual void	handleEvent(const SDL_Event&) = 0;
	virtual void	update(int) = 0;
	virtual void	render(const SDL_Surface&) = 0;
};



#endif // D_ISTATE_HPP


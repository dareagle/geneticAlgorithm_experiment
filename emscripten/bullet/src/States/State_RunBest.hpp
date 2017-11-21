

#ifndef D_STATE_RUNBEST_HPP
#define D_STATE_RUNBEST_HPP


#include "IState.hpp"


class State_RunBest : public IState
{
public:
	State_RunBest();
	~State_RunBest();

public:
	virtual void	handleEvent(const SDL_Event&) override;
	virtual void	update(int) override;
	virtual void	render(const SDL_Window&) override;
};


#endif // D_STATE_RUNBEST_HPP


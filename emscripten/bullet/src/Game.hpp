
#ifndef D_GAME_HPP
#define D_GAME_HPP


#include "SDLStage.hpp"


class Game
{
private:
	SDLStage*	m_pStage;

public:
	Game();
	~Game();
public:
	void	run();
};


#endif // D_GAME_HPP

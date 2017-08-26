

#include "Game.hpp"

#include "Logic/Data.hpp"
#include "States/StateManager.hpp"

#ifdef EMSCRIPTEN
#	include <emscripten.h>
#endif


Game::Game()
{
	m_pStage = new SDLStage(800, 600, 60, SDL_OPENGL);

	m_pStage->setCaption("OpenGL and Bullet");

	m_pStage->setEventListener([this](const SDL_Event &event)
	{
		StateManager::get()->handleEvent(event);
	});

	m_pStage->setUpdateCallback([this](int deltaTime)
	{
#ifdef EMSCRIPTEN
		EM_ASM(myFpsmeter_update.tickStart(););
#endif

		StateManager::get()->update(deltaTime);

#ifdef EMSCRIPTEN
		EM_ASM(myFpsmeter_update.tick(););
#endif
	});

	m_pStage->setRenderCallback([this](const SDL_Surface& screen)
	{
#ifdef EMSCRIPTEN
		EM_ASM(myFpsmeter_render.tickStart(););
#endif

		StateManager::get()->render(screen);

#ifdef EMSCRIPTEN
		EM_ASM(myFpsmeter_render.tick(););
#endif
	});

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );


	Data::create();
	StateManager::create();
}

Game::~Game()
{
	StateManager::destroy();
	Data::destroy();

	delete m_pStage;
}

#ifdef EMSCRIPTEN

void step(void* data)
{
	((SDLStage*)data)->step();
}

#endif

void	Game::run()
{
#ifdef EMSCRIPTEN

	emscripten_set_main_loop_arg(step, (void*)m_pStage, 0, true);

#else

	while (m_pStage->isActive())
		m_pStage->step();

#endif
}



#include "StateManager.hpp"

#include "State_Main.hpp"

//
//
// singleton

StateManager*	StateManager::m_pInstance = nullptr;

StateManager::StateManager()
{
	// allocate states

	m_States[e_States::eMain] = new State_Main();

	m_currentState = eMain;
}

StateManager::~StateManager()
{
	// deallocate states
}

//

void	StateManager::create()
{
	if (!m_pInstance)
		m_pInstance = new StateManager();
}

void	StateManager::destroy()
{
	delete m_pInstance, m_pInstance = nullptr;
}

StateManager*	StateManager::get()
{
	return m_pInstance;
}

// singleton
//
//



void	StateManager::handleEvent(const SDL_Event& event)
{
	m_States[m_currentState]->handleEvent(event);
}

void	StateManager::update(int delta)
{
	m_States[m_currentState]->update(delta);
}

void	StateManager::render(const SDL_Surface& screen)
{
	m_States[m_currentState]->render(screen);
}

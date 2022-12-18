
#include "demo/defines.hpp"

#if defined D_WEB_WEBWORKER_BUILD

#include "StateManager.hpp"
#include "State_WebWorkersLoading.hpp"

#include "demo/logic/Context.hpp"
#include "demo/logic/graphic/Scene.hpp"

void State_WebWorkersLoading::enter() {
  _countdownUntilNextState = 0;

  Context::get().logic.simulation->setOnWorkersReadyCallback([this]() -> void {
    // leave the "WEB WORKERS LOADING" message for at least 1 second
    _countdownUntilNextState = 1000;
  });
}

void State_WebWorkersLoading::handleEvent(const SDL_Event& event) {
  static_cast<void>(event); // <= unused
}

void State_WebWorkersLoading::update(int deltaTime) {
  if (_countdownUntilNextState == 0) {
    // only update to load the webworkers
    Context::get().logic.simulation->update(0.0f, 1);
  } else {
    // to ensure the message is visible (<= why the user waited)
    _countdownUntilNextState -= deltaTime;
    if (_countdownUntilNextState <= 0)
      StateManager::get()->changeState(StateManager::States::StartGeneration);
  }
}

void State_WebWorkersLoading::render(const SDL_Window& window) {
  static_cast<void>(window); // <= unused

  Scene::renderSimple();
}

void State_WebWorkersLoading::visibility(bool visible) {
  static_cast<void>(visible); // <= unused

  // do nothing, we do not want the parent's class logic
}

#endif

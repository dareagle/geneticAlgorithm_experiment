
#include "Scene.hpp"

#include "common.hpp"

#include "demo/states/StateManager.hpp"

#include "framework/graphic/GlContext.hpp"
#include "framework/graphic/ShaderProgram.hpp"
#include "framework/math/clamp.hpp"

void Scene::initialise() {
  GlContext::enable(GlContext::States::depthTest);
  GlContext::setDepthFunc(GlContext::DepthFuncs::less);

  GlContext::enable(GlContext::States::blend);
  GlContext::setBlendFunc(GlContext::BlendFuncs::srcAlpha,
                          GlContext::BlendFuncs::oneMinuxSrcAlpha);

  GlContext::disable(GlContext::States::cullFace);
}

void Scene::renderSimple() {
  Scene::_clear();

  auto& graphic = Context::get().graphic;

  graphic.hud.postProcess.startRecording();

  { // scene

    const Camera& camInstance = graphic.camera.main.scene;
    const auto& matricesData = camInstance.getMatricesData();
    graphic.scene.stackRenderers.wireframes.setMatricesData(matricesData);
    graphic.scene.stackRenderers.triangles.setMatricesData(matricesData);
    graphic.scene.particleManager.setMatricesData(matricesData);
    graphic.scene.animatedCircuitRenderer.setMatricesData(matricesData);

    // Scene::_renderFloor(camInstance);
    // graphic.scene.animatedCircuitRenderer.renderWireframe();
    // graphic.scene.animatedCircuitRenderer.renderWalls();
    // graphic.scene.animatedCircuitRenderer.renderGround();
  }

  graphic.hud.postProcess.stopRecording();

  { // HUD

    const auto& matricesData = graphic.camera.main.hud.getMatricesData();
    graphic.hud.postProcess.setMatricesData(matricesData);
    graphic.hud.stackRenderers.wireframes.setMatricesData(matricesData);
    graphic.hud.stackRenderers.triangles.setMatricesData(matricesData);
    graphic.scene.particleManager.setMatricesData(matricesData);
    graphic.hud.textRenderer.setMatricesData(matricesData);

    Scene::_renderHUD();
  }

  ShaderProgram::unbind();
}

void Scene::renderAll() {
  Scene::_clear();

  auto& context = Context::get();
  auto& graphic = context.graphic;

  graphic.hud.postProcess.startRecording();

  { // scene

    auto& logic = context.logic;
    auto& graphic = context.graphic;
    auto& camera = graphic.camera;
    const Camera& camInstance = camera.main.scene;
    const auto& matricesData = camInstance.getMatricesData();

    graphic.scene.stackRenderers.wireframes.setMatricesData(matricesData);
    graphic.scene.stackRenderers.triangles.setMatricesData(matricesData);
    graphic.scene.particleManager.setMatricesData(matricesData);
    graphic.scene.floorRenderer.setMatricesData(matricesData);
    graphic.scene.animatedCircuitRenderer.setMatricesData(matricesData);
    graphic.scene.flockingManager.setMatricesData(matricesData);
    graphic.scene.carTailsRenderer.setMatricesData(matricesData);

    Scene::_renderFloor(camInstance);
    graphic.scene.animatedCircuitRenderer.renderWireframe();
    graphic.scene.animatedCircuitRenderer.renderWalls();

    if (!logic.isAccelerated)
      Scene::_renderLeadingCarSensors();

    graphic.scene.flockingManager.render();
    graphic.scene.stackRenderers.triangles.flush();

    graphic.scene.particleManager.render();

    graphic.scene.stackRenderers.wireframes.flush();
    graphic.scene.stackRenderers.triangles.flush();

    graphic.scene.modelsRenderer.render(camInstance);
    graphic.scene.animatedCircuitRenderer.renderGround();
    graphic.scene.carTailsRenderer.render();
  }

  graphic.hud.postProcess.stopRecording();

  { // HUD

    const auto& matricesData = graphic.camera.main.hud.getMatricesData();
    graphic.hud.postProcess.setMatricesData(matricesData);
    graphic.hud.stackRenderers.wireframes.setMatricesData(matricesData);
    graphic.hud.stackRenderers.triangles.setMatricesData(matricesData);
    graphic.scene.particleManager.setMatricesData(matricesData);
    graphic.scene.floorRenderer.setMatricesData(matricesData);
    graphic.hud.textRenderer.setMatricesData(matricesData);

    Scene::_renderHUD();
  }

  ShaderProgram::unbind();
}

void Scene::updateMatrices(float elapsedTime) {
  auto& context = Context::get();
  const auto& logic = context.logic;
  auto& graphic = context.graphic;
  auto& camera = graphic.camera;

  { // scene

    // clamp vertical rotation [-70..+70]
    const float verticalLimit = glm::radians(70.0f);
    auto& rotations = camera.main.rotations;
    rotations.phi = glm::clamp(rotations.phi, -verticalLimit, verticalLimit);

    const float cosPhi = std::cos(rotations.phi);
    const glm::vec3 eye = {
      camera.main.center.x +
        camera.main.distance * cosPhi * std::cos(rotations.theta),
      camera.main.center.y +
        camera.main.distance * cosPhi * std::sin(rotations.theta),
      camera.main.center.z + camera.main.distance * std::sin(rotations.phi)};
    const glm::vec3 upAxis = {0.0f, 0.0f, 1.0f};

    camera.main.scene.setSize(camera.viewportSize.x, camera.viewportSize.y);
    camera.main.scene.lookAt(eye, camera.main.center, upAxis);
    camera.main.scene.computeMatrices();

    camera.main.hud.setOrthographic(0.0f, float(camera.viewportSize.x), 0.0f,
                                    float(camera.viewportSize.y), -10.0f,
                                    +10.0f);

    camera.main.hud.computeMatrices();

  } // scene

  { // third person

    if (auto leaderData = logic.leaderCar.leaderData()) {
      const glm::vec3 carOrigin =
        leaderData->liveTransforms.chassis * glm::vec4(0.0f, 0.0f, 2.5f, 1.0f);
      const glm::vec3 carUpAxis =
        leaderData->liveTransforms.chassis * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

      const StateManager::States currentState = StateManager::get()->getState();

      if (
        // do not update the third person camera if not in a correct state
        (currentState == StateManager::States::Running ||
         currentState == StateManager::States::StartGeneration) &&
        // do not update the third person camera if too close from the target
        glm::distance(carOrigin, camera.thirdPerson.eye) > 0.25f) {
        // simple lerp to setup the third person camera
        const float lerpRatio = 0.1f * 60.0f * elapsedTime;
        camera.thirdPerson.eye +=
          (carOrigin - camera.thirdPerson.eye) * lerpRatio;
        camera.thirdPerson.upAxis +=
          (carUpAxis - camera.thirdPerson.upAxis) * lerpRatio;
      }

      const glm::vec3 eye = camera.thirdPerson.eye;
      const glm::vec3 target = carOrigin;
      const glm::vec3 upAxis = camera.thirdPerson.upAxis;

      camera.thirdPerson.scene.setSize(scene::thirdPViewportWidth,
                                       scene::thirdPViewportHeight);
      camera.thirdPerson.scene.lookAt(eye, target, upAxis);
      camera.thirdPerson.scene.computeMatrices();
    }

  } // third person
}

void Scene::_clear() {
  const auto& viewportSize = Context::get().graphic.camera.viewportSize;

  GlContext::setViewport(0, 0, viewportSize.x, viewportSize.y);

  GlContext::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
  GlContext::clear(asValue(GlContext::Buffers::color) |
                   asValue(GlContext::Buffers::depth));
}

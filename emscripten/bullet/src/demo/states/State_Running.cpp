
#include "demo/defines.hpp"

#include "State_Running.hpp"

#include "StateManager.hpp"

#include "demo/logic/Data.hpp"

#include "thirdparty/GLMath.hpp"

#include <limits> // std::numeric_limits<T>::max();
#include <cmath> // std::ceil

void State_Running::update(int deltaTime)
{
    State_AbstractSimulation::update(deltaTime);

    float elapsedTime = float(deltaTime) / 1000.0f;

    auto& data = Data::get();
    auto& logic = data.logic;
    auto& simulation = *logic.simulation;
    auto& graphic = data.graphic;
    auto& camera = graphic.camera;

    { // simulation update

        const int simualtionSteps = (logic.isAccelerated ? 50 : 1);

        for (int ii = 0; ii < simualtionSteps; ++ii)
            simulation.update();

    } // simulation update

    { // camera tracking

        // done to avoid a spurious change of camera
        // -> true changing states: Running -> EndGeneration
        if (StateManager::get()->getState() == StateManager::States::Running)
        {
            glm::vec3   cameraNextCenter = logic.circuitAnimation.boundaries.center;
            float       cameraNextDistance = 200.0f;

            //
            //

            auto& leaderCar = logic.leaderCar;

            if (logic.isAccelerated)
            {
                leaderCar.timeoutUntilNewLeader = 0.0f;
                leaderCar.index = -1;
                leaderCar.totalTimeAsLeader = 0.0f;
            }
            else
            {
                cameraNextDistance = 40.0f;

                if (leaderCar.timeoutUntilNewLeader > 0)
                    leaderCar.timeoutUntilNewLeader -= elapsedTime;

                leaderCar.totalTimeAsLeader += elapsedTime;

                if (// no leader yet
                    leaderCar.index == -1 ||
                    // the current leader is dead
                    simulation.getCarResult(leaderCar.index).isAlive == false ||
                    // time to check for a potentially better leader
                    leaderCar.timeoutUntilNewLeader <= 0.0f)
                {
                    // reset the timeout
                    leaderCar.timeoutUntilNewLeader = 1.0f; // <= one second

                    // refresh the currently best car

                    unsigned int totalCars = simulation.getTotalCars();

                    float bestFitness = 0.0f;
                    int oldLeaderCarIndex = leaderCar.index;
                    leaderCar.index = -1;
                    for (unsigned int ii = 0; ii < totalCars; ++ii)
                    {
                        const auto& carData = simulation.getCarResult(ii);

                        if (!carData.isAlive)
                            continue;

                        if (leaderCar.index != -1 &&
                            !(carData.fitness > bestFitness + 2.0f))
                            continue;

                        bestFitness = carData.groundIndex;
                        leaderCar.index = ii;
                    }

                    if (leaderCar.index >= 0 && leaderCar.index != oldLeaderCarIndex)
                        leaderCar.totalTimeAsLeader = 0.0f; // new leader
                }

                // do we have a car to focus the camera on?
                if (leaderCar.index >= 0)
                {
                    const auto& carResult = simulation.getCarResult(leaderCar.index);

                    // this part elevate where the camera look along the up axis of the car
                    // => without it the camera look at the ground
                    // => mostly useful for a shoulder camera0
                    glm::vec4 carOrigin = carResult.transform * glm::vec4(0.0f, 0.0f, 2.0f, 1.0f);

                    cameraNextCenter = carOrigin;
                }
            }

            //
            //

            {
                const float lerpRatio = 0.1f;

                camera.center += (cameraNextCenter - camera.center) * lerpRatio;
                camera.distance += (cameraNextDistance - camera.distance) * lerpRatio;
            }
        }

    } // camera tracking

}

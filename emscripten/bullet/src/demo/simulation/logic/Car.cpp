
#include "Car.hpp"

#include "demo/logic/physic/PhysicTrimesh.hpp"
#include "demo/logic/physic/PhysicVehicle.hpp"

#include <cmath> // <= M_PI
#include <iostream>

namespace /*anonymous*/
{
    float   k_steeringMaxValue = M_PI / 8.0f;
    float   k_speedMaxValue = 20.0f;
    int     k_healthMaxValue = 200;
};

Car::Car(PhysicWorld& physicWorld,
         const glm::vec3& position,
         const glm::vec4& quaternion)
    : _physicWorld(physicWorld)
{
    _vehicle = _physicWorld.createVehicle();

    // _physicWorld.addVehicle(*_vehicle);

    reset(position, quaternion);
}

void	Car::update(const NeuralNetwork& neuralNetwork)
{
    if (!_isAlive)
        return;

    this->updateSensors();
    this->collideEyeSensors();
    this->collideGroundSensor();

    if (_health <= 0)
    {
        _isAlive = false;
        _physicWorld.removeVehicle(_vehicle);
        return;
    }

    // TODO: that might be slow, to profile and make as a static attribut?
    std::vector<float>	input;
    input.reserve(_eyeSensors.size());
    for (const auto& eyeSensor : _eyeSensors)
        input.push_back(eyeSensor.value);

    std::vector<float> output;

    neuralNetwork.process(input, output);

    _output.steer = output[0]; // the range is [0, 1]
    _output.speed = output[1]; // the range is [0, 1]

    // if (std::isnan(_output.steer))	_output.steer = 0.5f;
    // if (std::isnan(_output.speed))	_output.speed = 0.5f;

    _output.steer = std::min(1.0f, std::max(0.0f, _output.steer));
    _output.speed = std::min(1.0f, std::max(0.0f, _output.speed));

    _output.steer = _output.steer * 2.0f - 1.0f;
    _output.speed = _output.speed * 2.0f - 1.0f;

    _vehicle->setSteeringValue(_output.steer * k_steeringMaxValue);
    _vehicle->applyEngineForce(_output.speed * k_speedMaxValue);

    ++_totalUpdateNumber;
}

void	Car::updateSensors()
{
    glm::mat4 transform;
    _vehicle->getOpenGLMatrix(transform);

    { // eye sensor

        const float	eyeMaxRange = 50.0f;
        const float	eyeHeight = 1.0f;
        const float	eyeElevation = 6.0f;
        const float	eyeWidth = float(M_PI) / 8.0f;

        const std::array<float, 3> eyeElevations = {
            -eyeElevation,
            0.0f,
            +eyeElevation,
        };

        const std::array<float, 5> eyeAngles = {
            -eyeWidth * 2.0f,
            -eyeWidth,
            0.0f,
            +eyeWidth,
            +eyeWidth * 2.0f
        };

        glm::vec4 newNearValue = transform * glm::vec4(0, 0, eyeHeight, 1);

        int sensorIndex = 0;

        for (const auto& eyeElevation : eyeElevations)
            for (const auto& eyeAngle : eyeAngles)
            {
                auto& eyeSensor = _eyeSensors[sensorIndex++];

                glm::vec4 newFarValue = {
                    eyeMaxRange * sinf(eyeAngle),
                    eyeMaxRange * cosf(eyeAngle),
                    eyeHeight + eyeElevation,
                    1.0f
                };

                eyeSensor.near = newNearValue;
                eyeSensor.far = transform * newFarValue;
            }
    }

    { // ground sensor

        const float	groundMaxRange = 10.0f; // <= ground sensor
        const float	groundHeight = 1.0f;

        _groundSensor.near = transform * glm::vec4(0, 0, groundHeight, 1);
        _groundSensor.far = transform * glm::vec4(0, 0, groundHeight - groundMaxRange, 1);
    }
}


void	Car::collideEyeSensors()
{
    const float	sensorMaxRange = 50.0f;

    for (auto& sensor : _eyeSensors)
    {
        sensor.value = 1.0f;

        PhysicWorld::t_raycastParams params(sensor.near, sensor.far);
        params.collisionGroup = D_GROUP_SENSOR;
        params.collisionMask = D_GROUP_GROUND | D_GROUP_WALL; // collide ground and walls

        bool hasHit = _physicWorld.raycast(params);

        if (!hasHit)
            continue;

        sensor.far = params.result.impactPoint;

        sensor.value = glm::length(sensor.far - sensor.near) / sensorMaxRange;
    }
}

void	Car::collideGroundSensor()
{
    // raycast to get the checkpoints validation

    PhysicWorld::t_raycastParams	params(_groundSensor.near, _groundSensor.far);
    params.collisionGroup = D_GROUP_SENSOR;
    params.collisionMask = D_GROUP_GROUND; // collide with the ground only

    bool hasHitGround = _physicWorld.raycast(params);

    if (hasHitGround)
    {
        _groundSensor.far = params.result.impactPoint;
        int	hasHitGroundIndex = params.result.impactIndex;

        if (_groundIndex + 1 == hasHitGroundIndex)
        {
            _groundIndex = hasHitGroundIndex;

            // restore health to full
            _health = k_healthMaxValue;

            // reward the genome
            ++_fitness;
        }
        // else if (_groundIndex + 1 > hasHitGroundIndex)
        // {
        //     _vehicle->disableContactResponse();
        // }
    }

    // reduce the health over time
    // => reduce more if the car does not touch the ground
    // => faster discard of a most probably dying genome
    _health -= hasHitGround ? 1 : 2;
}

void	Car::reset(const glm::vec3& position, const glm::vec4& quaternion)
{
    _isAlive = true;
    _fitness = 0;
    _totalUpdateNumber = 0;
    _health = k_healthMaxValue;
    _groundIndex = 0;

    _output.steer = 0.0f;
    _output.speed = 0.0f;

    _vehicle->reset();
    _vehicle->setPosition({ position.x, position.y, position.z });
    _vehicle->setRotation({ quaternion.x, quaternion.y, quaternion.z, quaternion.w });

    updateSensors();

    // _physicWorld.removeVehicle(*_vehicle);
    _physicWorld.addVehicle(_vehicle); // ensure vehicle presence
}

const Car::t_sensors&	Car::getEyeSensors() const
{
    return _eyeSensors;
}

const Car::t_sensor&	Car::getGroundSensor() const
{
    return _groundSensor;
}

float	Car::getFitness() const
{
    return _fitness;
}

bool	Car::isAlive() const
{
    return _isAlive;
}

int		Car::getGroundIndex() const
{
    return _groundIndex;
}

const Car::t_neuralNetworkOutput&	Car::getNeuralNetworkOutput() const
{
    return _output;
}

const PhysicVehicle&	Car::getVehicle() const
{
    return *_vehicle;
}

float	Car::getLife() const
{
    return float(_health) / k_healthMaxValue;
}
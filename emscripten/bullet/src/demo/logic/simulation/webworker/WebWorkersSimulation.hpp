
#pragma once

#include "demo/defines.hpp"

#include "demo/logic/simulation/AbstactSimulation.hpp"

#include "demo/logic/simulation/logic/CircuitBuilder.hpp"

#include "demo/logic/simulation/machineLearning/GeneticAlgorithm.hpp"

#include "producer/WorkerProducer.hpp"
#include "common.hpp"

#include <list>
#include <vector>
#include <array>

class WebWorkersSimulation
    : public AbstactSimulation
{
private:
    enum class WorkerRequest : unsigned int
    {
        None = 0,
        WorkersLoading,
        ResetAndProcess,
        Process,
    } _currentRequest = WorkerRequest::None;

    glm::vec3 _startPosition;

    std::vector<WorkerProducer> _workerProducers;

    unsigned int _totalCores = 0;
    unsigned int _genomesPerCore = 0;
    unsigned int _totalGenomes = 0;

    std::vector<bool> _carLiveStatus;

private:
    NeuralNetworkTopology _neuralNetworkTopology;
    GeneticAlgorithm _geneticAlgorithm;

    struct t_callbacks
    {
        AbstactSimulation::SimpleCallback onWorkersReady;
        AbstactSimulation::SimpleCallback onGenerationReset;
        AbstactSimulation::SimpleCallback onGenerationStep;
        AbstactSimulation::GenomeDieCallback onGenomeDie;
        AbstactSimulation::GenerationEndCallback onGenerationEnd;
    } _callbacks;

public:
    WebWorkersSimulation() = default;
    virtual ~WebWorkersSimulation() = default;

public:
    virtual void initialise(const Definition &def) override;

public:
    virtual void update() override;

private:
    void _processSimulation();
    void _resetAndProcessSimulation();

public:
    virtual unsigned int getTotalCores() const override;
    virtual const AbstactSimulation::CoreState &getCoreState(unsigned int index) const override;
    virtual const CarData &getCarResult(unsigned int index) const override;
    virtual unsigned int getTotalCars() const override;

public:
    virtual void setOnWorkersReadyCallback(AbstactSimulation::SimpleCallback callback) override;
    virtual void setOnGenerationResetCallback(AbstactSimulation::SimpleCallback callback) override;
    virtual void setOnGenerationStepCallback(AbstactSimulation::SimpleCallback callback) override;
    virtual void setOnGenomeDieCallback(AbstactSimulation::GenomeDieCallback callback) override;
    virtual void setOnGenerationEndCallback(AbstactSimulation::GenerationEndCallback callback) override;

public:
    virtual const Genomes &getGenomes() const override;
    virtual const Genome &getBestGenome() const override;
    virtual unsigned int getGenerationNumber() const override;
    virtual const glm::vec3 &getStartPosition() const override;
};

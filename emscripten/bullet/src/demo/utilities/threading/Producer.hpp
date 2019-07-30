
#pragma once

#include "demo/defines.hpp"

#if defined D_WEB_WEBWORKER_BUILD
#	error "exclude this file to build natively or with multi thread support"
#endif

#include "Consumer.hpp"
#include "TaskSynchroniser.hpp"
#include "t_task.hpp"

#include <list>
#include <vector>
#include <functional>

#include <thread>

class Producer
{
	friend class Consumer; 

private:
    std::thread     _thread;
    TaskSynchroniser      _waitOneTask;
    TaskSynchroniser      _waitAllTask;

    bool _running = true;

    std::vector<Consumer*> _consumers;

    std::list<t_task>	_plannedTasks;
    std::list<t_task>	_runningTasks;
    std::list<t_task>	_completedTasks;

public:
	Producer();
	~Producer();

public:
	void push(const t_task::t_work& work);
	void push(const t_task::t_work& work, const t_task::t_work& complete);
	void update();
	void quit();
	void waitUntilAllCompleted();

private:
	void notifyWorkDone(Consumer* in_consumer);
    void run();
};

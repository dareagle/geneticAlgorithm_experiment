

#pragma once


#include "IPhysicWrapper.hpp"

#include <vector>
#include <functional>


class Circuit
{
public:
	typedef std::function<void(std::vector<float>& buffer, std::vector<int>& indices, bool ground)> t_geometry_callback;

public:

	void	initialise(
		IPhysicWrapper* pPhysicWrapper,
		const std::vector<float>& left,
		const std::vector<float>& right,
		t_geometry_callback callback
	);
};



#pragma once


#include "OpenGLES.hpp"


class	ShaderColor;

class	GeometryColor
{
private:
	static ShaderColor*	m_pShaderColor;
public:
	static void	initialise(ShaderColor*);

private:
	GLuint			m_obj_buffer;
	unsigned int	m_numItems;

public:
	GeometryColor();
	~GeometryColor();

public:
	void	initialise();
	void	deinitialise();

public:
	void	update(float* pData, unsigned int length);
	void	render(GLint primitive, float* matrix, float alpha = 1.0f);
};

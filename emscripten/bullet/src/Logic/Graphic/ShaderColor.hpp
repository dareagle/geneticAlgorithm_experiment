

#ifndef D_SHADERCOLOR_HPP
#define D_SHADERCOLOR_HPP


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


class	GeometryColor;

class	ShaderColor
{
private:
	friend GeometryColor;

private:
public:
	GLuint	m_obj_program;
	GLint	m_attr_Position;
	GLint	m_attr_Color;
	GLint	m_unif_ComposedMatrix;
	GLint	m_unif_Alpha;

public:
	ShaderColor();
	~ShaderColor();

public:
	bool	initialise();
	void	deinitialise();

public:
	static void	bind(ShaderColor* pShader);
};


#endif // D_SHADERCOLOR_HPP


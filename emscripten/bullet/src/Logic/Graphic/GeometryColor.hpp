

#ifndef D_GEOMETRYCOLOR_CPP
#define D_GEOMETRYCOLOR_CPP


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


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


#endif // D_GEOMETRYCOLOR_CPP


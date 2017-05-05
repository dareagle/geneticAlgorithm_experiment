

#include "GeometryColor.hpp"

#include "ShaderColor.hpp"

#include <cstdio>

//


ShaderColor*	GeometryColor::m_pShaderColor = nullptr;

void	GeometryColor::initialise(ShaderColor* pShaderColor)
{
	m_pShaderColor = pShaderColor;
}

//

GeometryColor::GeometryColor()
	: m_obj_buffer(0) // not initialised
	, m_numItems(0)
{
}

GeometryColor::~GeometryColor()
{
	if (m_obj_buffer) // initialised?
		deinitialise();
}

//

void	GeometryColor::initialise()
{
	if (m_obj_buffer) // already initialised?
		return;

	glGenBuffers(1, &m_obj_buffer);
}

void	GeometryColor::deinitialise()
{
	if (!m_obj_buffer)
		return;

    glDeleteBuffers(1, &m_obj_buffer);
    m_obj_buffer = 0;
}

//

void	GeometryColor::update(float* pData, unsigned int length)
{
	if (!m_obj_buffer)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, m_obj_buffer);

	glBufferData(GL_ARRAY_BUFFER, length, pData, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, length, pData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_numItems = length / 6 / sizeof(float);
}

void	GeometryColor::render(GLint primitive, float* matrix, float alpha /*= 1.0f*/)
{
	if (!m_obj_buffer ||
		!m_pShaderColor)
		return;

	glEnableVertexAttribArray(m_pShaderColor->m_attr_Position);
	glEnableVertexAttribArray(m_pShaderColor->m_attr_Color);

		// glUniformMatrix4fv(m_pShaderColor->m_unif_ComposedMatrix, 1, GL_FALSE, glm::value_ptr(composedMatrix));

		glUniformMatrix4fv( m_pShaderColor->m_unif_ComposedMatrix, 1, GL_FALSE, matrix );
		glUniform1f( m_pShaderColor->m_unif_Alpha, alpha );

		std::size_t		bpp = sizeof(float);
		std::size_t 	stride = 6 * bpp;
		void*			index_pos = (void*)(0 * bpp);
		void* 			index_color = (void*)(3 * bpp);

		glBindBuffer(GL_ARRAY_BUFFER, m_obj_buffer);

			glVertexAttribPointer(m_pShaderColor->m_attr_Position, 3, GL_FLOAT, GL_FALSE, stride, index_pos);
			glVertexAttribPointer(m_pShaderColor->m_attr_Color, 3, GL_FLOAT, GL_FALSE, stride, index_color);

			glDrawArrays(primitive, 0, m_numItems);

		glBindBuffer(GL_ARRAY_BUFFER, 0);



	glDisableVertexAttribArray(m_pShaderColor->m_attr_Position);
	glDisableVertexAttribArray(m_pShaderColor->m_attr_Color);
}


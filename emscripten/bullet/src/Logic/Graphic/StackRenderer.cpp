

#include "StackRenderer.hpp"


#include <iostream>
#include <cassert>


namespace
{
	void sCheckGLError()
	{
		GLenum errCode = glGetError();
		if (errCode != GL_NO_ERROR)
		{
			std::cout << "OpenGL error = " << errCode << std::endl;
			assert(false);
		}
	}

	// Prints shader compilation errors
	void sPrintLog(GLuint object)
	{
		GLint log_length = 0;
		if (glIsShader(object))
			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
		else if (glIsProgram(object))
			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
		else
		{
			std::cout << "printlog: Not a shader or a program" << std::endl;
			return;
		}

		char* log = (char*)malloc(log_length);

		if (glIsShader(object))
			glGetShaderInfoLog(object, log_length, NULL, log);
		else if (glIsProgram(object))
			glGetProgramInfoLog(object, log_length, NULL, log);

		fprintf(stderr, "%s", log);
		free(log);
	}


	//
	GLuint sCreateShaderFromString(const char* source, GLenum type)
	{
		GLuint res = glCreateShader(type);
		const char* sources[] = { source };
		glShaderSource(res, 1, sources, NULL);
		glCompileShader(res);
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

		if (compile_ok == GL_FALSE)
		{
			std::cout << "Error compiling shader of type " << type << std::endl;
			sPrintLog(res);
			glDeleteShader(res);
			return 0;
		}

		return res;
	}

	// 
	GLuint sCreateShaderProgram(const char* vs, const char* fs)
	{
		GLuint vsId = sCreateShaderFromString(vs, GL_VERTEX_SHADER);
		GLuint fsId = sCreateShaderFromString(fs, GL_FRAGMENT_SHADER);
		assert(vsId != 0 && fsId != 0);

		GLuint programId = glCreateProgram();
		glAttachShader(programId, vsId);
		glAttachShader(programId, fsId);
		glLinkProgram(programId);

		glDeleteShader(vsId);
		glDeleteShader(fsId);

		GLint status = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);
		assert(status != GL_FALSE);
		
		return programId;
	}
};



StackRenderer::StackRenderer()
	: m_vboId(0)
	, m_programId(0)
	, m_pMatrix(nullptr)
{
}

//

void	StackRenderer::create()
{
	const char vs[] =
		"attribute vec3 a_Position;\n"
		"attribute vec4 a_Color;\n"
		"\n"
		"uniform mat4 u_ComposedMatrix;\n"
		"\n"
		"varying vec4 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   v_Color = a_Color;\n"
		"   gl_Position = u_ComposedMatrix * vec4(a_Position, 1.0);\n"
		"}\n";

	const char fs[] =
		"precision mediump float;\n"
		"\n"
		"varying vec4 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = v_Color;\n"
		"}\n";

	m_programId = sCreateShaderProgram(vs, fs);
	m_projectionUniform = glGetUniformLocation(m_programId, "u_ComposedMatrix");
	m_vertexAttribute = glGetAttribLocation(m_programId, "a_Position");
	m_colorAttribute = glGetAttribLocation(m_programId, "a_Color");
	m_colorAttribute = glGetAttribLocation(m_programId, "a_Color");

	glGenBuffers(1, &m_vboId);
	m_vertices.reserve( 2 * 512 );

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.capacity() * sizeof(t_vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	StackRenderer::destroy()
{
	if (m_vboId)
	{
		glDeleteBuffers(1, &m_vboId);
		m_vboId = 0;
	}

	if (m_programId)
	{
		glDeleteProgram(m_programId);
		m_programId = 0;
	}
}

//

void	StackRenderer::push_vertex(const t_pos& v, const t_color& c)
{
	if (m_vertices.size() == m_vertices.capacity())
		flush();

	m_vertices.push_back(t_vertex(v, c));
}
    
void	StackRenderer::flush()
{
	if (m_vertices.empty() ||
		!m_pMatrix ||
		!m_programId)
		return;

	glUseProgram(m_programId);

	glEnableVertexAttribArray(m_vertexAttribute);
	glEnableVertexAttribArray(m_colorAttribute);

	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, m_pMatrix);


	std::size_t	bpp = sizeof(float);
	std::size_t	stride = sizeof(t_vertex);
	const void*	pVertex_index	= reinterpret_cast<const void*>(0 * bpp);
	const void*	pColor_index	= reinterpret_cast<const void*>(3 * bpp);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * stride, &m_vertices[0]);
	glVertexAttribPointer(m_vertexAttribute, 3, GL_FLOAT, GL_FALSE, stride, pVertex_index);
	glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, stride, pColor_index);



	glDrawArrays(GL_LINES, 0, m_vertices.size());

	sCheckGLError();

	glDisableVertexAttribArray(m_vertexAttribute);
	glDisableVertexAttribArray(m_colorAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	m_vertices.clear();
}



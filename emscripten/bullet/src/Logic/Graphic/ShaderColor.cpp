

#include "ShaderColor.hpp"

#include "GeometryColor.hpp"

#include <iostream>


ShaderColor::ShaderColor()
	: m_obj_program(0)
{
}

ShaderColor::~ShaderColor()
{
	deinitialise();
}

//

namespace
{
	GLuint	loadShader(GLenum type, const char *source)
	{
		GLuint shader = glCreateShader(type);
		
		if (shader == 0)
		{
			std::cout << "Error creating shader" << std::endl;
			return 0;
		}
		
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			
			if (infoLen > 1)
			{
				char* infoLog = new char[infoLen];
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

				std::cout << "Error compiling shader: " << infoLog << std::endl;

				delete[] infoLog;
			}
			
			glDeleteShader (shader);
			return 0;
			
		}
		
		return shader;
	}
};

//

bool	ShaderColor::initialise()
{
	const char vertexShaderString[] =
		"attribute vec3 a_Position;\n"
		"attribute vec3 a_Color;\n"
		"\n"
		"uniform mat4 u_ComposedMatrix;\n"
		"\n"
		"varying vec3 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   gl_Position = u_ComposedMatrix * vec4(a_Position, 1.0);\n"
		"   v_Color = a_Color;\n"
		"}\n";

	const char fragmentShaderString[] =  
		"precision mediump float;\n"
		"\n"
		"uniform float u_Alpha;\n"
		"\n"
		"varying vec3 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = vec4(v_Color, u_Alpha);\n"
		"}\n";

	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderString);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderString);

	m_obj_program = glCreateProgram();

	if (!m_obj_program)
	{
		std::cout << "Could not create OpenGL program" << std::endl;
		return false;
	}

	glAttachShader(m_obj_program, vertexShader);
	glAttachShader(m_obj_program, fragmentShader);
	glLinkProgram(m_obj_program);

	GLint linked;
	glGetProgramiv(m_obj_program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(m_obj_program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char [infoLen];
			glGetProgramInfoLog (m_obj_program, infoLen, NULL, infoLog);

			std::cout << "Error linking program: " << infoLog << std::endl;

			delete[] infoLog;
		}

		glDeleteProgram(m_obj_program);
		return false;
	}


	m_attr_Position = glGetAttribLocation(m_obj_program, "a_Position");
	m_attr_Color = glGetAttribLocation(m_obj_program, "a_Color");

	m_unif_ComposedMatrix = glGetUniformLocation(m_obj_program, "u_ComposedMatrix");
	m_unif_Alpha = glGetUniformLocation(m_obj_program, "u_Alpha");

	return true;
}

void	ShaderColor::deinitialise()
{
	if (!m_obj_program)
		return;

	glDeleteProgram(m_obj_program);
	m_obj_program = 0;
}

//

void	ShaderColor::bind(ShaderColor* pShader)
{
	if (pShader)
		glUseProgram(pShader->m_obj_program);
	else
		glUseProgram(0);
}

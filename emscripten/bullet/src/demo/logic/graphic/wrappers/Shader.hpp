
#pragma once

#include "thirdparty/OpenGLES.hpp"

#include <string>
#include <vector>
#include <map>

class UniformBufferObject;

class Shader
{
private:
	friend UniformBufferObject;

public:
	struct t_def
	{
		struct t_filenames
		{
			std::string	vertex;
			std::string	fragment;
		}
		filenames;

		std::vector<std::string> attributes;
		std::vector<std::string> uniforms;
	};

	struct t_uniformBlockData
	{
		GLint	id;
		GLuint	layout;
	};

private:
	GLuint	_programId = 0;
	std::map<std::string, GLint>	_attributesMap;
	std::map<std::string, GLint>	_uniformsMap;

public:
	Shader(const t_def& def);
	~Shader();

public:
	void	bind() const;

public:
	static void	unbind();

public:
	GLint	getAttribute(const std::string& name) const;
	GLint	getUniform(const std::string& name) const;
	bool	hasAttribute(const std::string& name) const;
	bool	hasUniform(const std::string& name) const;

};
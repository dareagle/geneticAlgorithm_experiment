

#include "ShaderColor.hpp"

#include "../Shader.hpp"

#include "Utility/TraceLogger.hpp"


ShaderColor::~ShaderColor()
{
	delete m_pShader, m_pShader = nullptr;
}

//

bool	ShaderColor::initialise()
{
	delete m_pShader, m_pShader = nullptr;

	//

	Shader::t_def	def;
	def.vertex_source =
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
	def.fragment_source =
		"uniform float u_Alpha;\n"
		"\n"
		"varying vec3 v_Color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = vec4(v_Color, u_Alpha);\n"
		"}\n";

	def.attributes.push_back("a_Position");
	def.attributes.push_back("a_Color");
	def.uniforms.push_back("u_ComposedMatrix");
	def.uniforms.push_back("u_Alpha");


	m_pShader = Shader::build(def);

	if (!m_pShader)
		D_MYLOG("Failed to build the shader");

	return (m_pShader != nullptr);
}

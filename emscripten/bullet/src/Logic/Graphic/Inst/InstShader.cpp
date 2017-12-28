

#include "InstShader.hpp"

#include "Utility/TraceLogger.hpp"


bool InstShader::initialise()
{
    Shader::t_def	def;
    def.vertex_source =
        "attribute vec3 a_Position;\n"
        "attribute vec3 a_Color;\n"
        "attribute mat4 a_Transform;\n"
        "\n"
        "uniform mat4 u_ComposedMatrix;\n"
        "\n"
        "varying vec4 v_Color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = u_ComposedMatrix * a_Transform * vec4(a_Position, 1.0);\n"
        "   v_Color = vec4(a_Color, 1.0);\n"
        "}\n";
    def.fragment_source =
        "varying vec4 v_Color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_FragColor = v_Color;\n"
        "}\n";

    def.attributes.push_back("a_Position");
    def.attributes.push_back("a_Color");
    def.attributes.push_back("a_Transform");
    def.uniforms.push_back("u_ComposedMatrix");

    m_pShader = Shader::build(def);

    if (!m_pShader)
        D_MYLOG("Failed to build the shader");

    return (m_pShader != nullptr);
}

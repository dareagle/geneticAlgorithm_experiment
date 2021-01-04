
#include "Data.hpp"

void Data::initialiseShaders()
{
    { // shaders

        { // create the stackRenderer shader

            Shader::Definition def;
            def.filenames.vertex = "assets/shaders/stackRenderer.vert.glsl";
            def.filenames.fragment = "assets/shaders/stackRenderer.frag.glsl";
            def.attributes = { "a_position", "a_color" };
            def.uniforms = { "u_composedMatrix" };

            graphic.shaders.stackRenderer = std::make_unique<Shader>(def);
        }

        { // create the wireframes shader

            Shader::Definition def;

            def.filenames.vertex = "assets/shaders/wireframes.vert.glsl";
            def.filenames.fragment = "assets/shaders/wireframes.frag.glsl";
            def.attributes = { "a_position" };
            def.uniforms = { "u_composedMatrix", "u_color" };

            graphic.shaders.wireframes = std::make_unique<Shader>(def);
        }

        { // create the animated circuit shader

            Shader::Definition def;

            def.filenames.vertex = "assets/shaders/animatedCircuit.vert.glsl";
            def.filenames.fragment = "assets/shaders/animatedCircuit.frag.glsl";
            def.attributes = { "a_position", "a_color", "a_normal", "a_index" };
            def.uniforms = {
                "u_composedMatrix", "u_alpha", "u_lowerLimit", "u_upperLimit"
            };

            graphic.shaders.animatedCircuit = std::make_unique<Shader>(def);
        }

        { // create the hud text shader

            Shader::Definition def;

            def.filenames.vertex = "assets/shaders/hudText.vert.glsl";
            def.filenames.fragment = "assets/shaders/hudText.frag.glsl";
            def.attributes = {
                "a_position", "a_texCoord",
                "a_offsetPosition", "a_offsetTexCoord", "a_offsetScale"
            };
            def.uniforms = { "u_composedMatrix", "u_texture" };

            graphic.shaders.hudText = std::make_unique<Shader>(def);
        }

        { // particles

            Shader::Definition def;
            def.filenames.vertex = "assets/shaders/particles.vert.glsl";
            def.filenames.fragment = "assets/shaders/particles.frag.glsl";
            def.attributes = {
                "a_position",
                "a_offsetPosition", "a_offsetScale", "a_offsetColor"
            };
            def.uniforms = { "u_composedMatrix" };

            graphic.shaders.particles = std::make_unique<Shader>(def);
        }

        { // model (chassis + wheels)

            Shader::Definition def;
            def.filenames.vertex = "assets/shaders/model.vert.glsl";
            def.filenames.fragment = "assets/shaders/model.frag.glsl";
            def.attributes = {
                "a_position", "a_color",
                "a_offsetTransform", "a_offsetColor"
            };
            def.uniforms = {
                "u_composedMatrix"
            };

            graphic.shaders.model = std::make_unique<Shader>(def);
        }

    } // shaders

    { // textures

        bool pixelated = true;
        graphic.textures.textFont.load("assets/textures/ascii_font.png", pixelated);

    } // textures
}

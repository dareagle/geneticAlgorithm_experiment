#version 300 es

precision lowp float;

in vec4 v_color;
in vec4 v_outlineColor;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_outline;

void main(void)
{
  out_color = v_color;
  out_outline = v_outlineColor;
}



#ifndef D_STACKRENDERER_HPP
#define D_STACKRENDERER_HPP


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <vector>


struct StackRenderer
{
public:

	struct t_pos
	{
		float x, y, z;

		t_pos(float in_x, float in_y, float in_z)
			: x(in_x)
			, y(in_y)
			, z(in_z)
		{}
	};

	struct t_color
	{
		float r, g, b, a;

		t_color(float in_r, float in_g, float in_b, float in_a)
			: r(in_r)
			, g(in_g)
			, b(in_b)
			, a(in_a)
		{}
	};

	struct t_vertex
	{
		t_pos	pos;
		t_color	color;

		t_vertex(const t_pos& p, const t_color& c)
			: pos(p)
			, color(c)
		{}
	};

public:
	GLuint m_vboId;
	std::vector<t_vertex>	m_vertices;

	GLuint m_programId;
	GLint m_projectionUniform;
	GLint m_vertexAttribute;
	GLint m_colorAttribute;

	float*	m_pMatrix;

public:
	StackRenderer();

public:
	void create();
	void destroy();

public:
	inline void	setMatrix(float* pMatrix) { m_pMatrix = pMatrix; }

	void push_vertex(const t_pos& v, const t_color& c);
	// inline void push_vertex(const t_vertex& v) { push_vertex(v.pos, v.color); }
    void flush();
};


#endif // D_STACKRENDERER_HPP


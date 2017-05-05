

#ifndef D_TRIMESH_HPP
#define D_TRIMESH_HPP



#include <vector>


class	btTriangleIndexVertexArray;
class	btBvhTriangleMeshShape;
class	btDefaultMotionState;
class	btRigidBody;


namespace	Physic
{


class World;

class Trimesh
{
private:
	friend World;

private:
	float*	m_pArr_Vertices;
	int*	m_pArr_Indices;

	int 	m_id;

	btTriangleIndexVertexArray*	m_pIndexVertexArrays;
	btBvhTriangleMeshShape*		m_pShape;
	btDefaultMotionState*	m_pMotionState;
	btRigidBody*	m_pBbody;

private:
	Trimesh(const std::vector<float>& b, const std::vector<int>& i, int id);
	~Trimesh();

public:
	inline int	getID() const { return m_id; }

};


};


#endif // D_TRIMESH_HPP


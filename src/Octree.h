#ifndef OCTREE_H_
#define OCTREE_H_

#include "Main.h"
#include "Vector3.h"
#include <vector>

using namespace std;

extern int g_MaxTriangles;

extern int g_MaxSubdivisions;

extern int g_EndNodeCount;

extern int g_bLighting;

extern int g_bRenderMode;

enum eOctreeNodes {
	TOP_LEFT_FRONT,
	TOP_LEFT_BACK,
	TOP_RIGHT_BACK,
	TOP_RIGHT_FRONT,
	BOTTOM_LEFT_FRONT,
	BOTTOM_LEFT_BACK,
	BOTTOM_RIGHT_BACK,
	BOTTOM_RIGHT_FRONT
};

class Debug
{
public:
	void AddDebugLine(Vector3 vPoint1, Vector3 vPoint2);
	void AddDebugRectangle(Vector3 vCenter, float width, float height, float depth);
	void RenderDebugLines();
	void Clear();

private:
	vector<Vector3> m_vLines;
};

class Octree
{
public:
	Octree();
	~Octree();

	Vector3 GetCenter() { return m_vCenter; }

	int GetTriangleCount() { return m_TriangleCount; }

	float GetWidth() { return m_Width; }

	bool IsSubDivided() { return m_bSubDivided; }

	void GetSceneDimensions(Vector3 *pVertices, int numberOfVerts);

	Vector3 GetNewNodeCenter(Vector3 vCenter, float width, int nodeID);

	void CreateNode(Vector3 *pVertices, int numberOfVerts, Vector3 vCenter, float width);

	void CreateNewNode(Vector3 *pVertices, vector<bool> pList, int numberOfVerts, Vector3 vCenter, float width, int triangleCount, int nodeID);

	void AssignVerticesToNode(Vector3 *pVertices, int numberOfVerts);

	void DrawOctree(Octree *pNode);

	void DestroyOctree();

private:
	void InitOctree();

	bool m_bSubDivided;
	float m_Width;
	int m_TriangleCount;
	Vector3 m_vCenter;
	Vector3 *m_pVertices;
	Octree *m_pOctreeNodes[8];
};

Vector3 Cross(Vector3 vVector1, Vector3 vVector2);

float Magnitude(Vector3 vNormal);

Vector3 Normalize(Vector3 vVector);

#endif
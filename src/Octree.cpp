#include "Octree.h"

extern Debug g_Debug;

int g_CurrentSubdivision = 0;

void 
Debug::RenderDebugLines() {
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

	glColor3ub(255, 255, 0);

	for (unsigned int i = 0; i < m_vLines.size(); i++) {
		glVertex3f(m_vLines[i].x, m_vLines[i].y, m_vLines[i].z);
	}

	glEnd();

	if (g_bLighting)
		glEnable(GL_LIGHTING);
}

void 
Debug::AddDebugLine(Vector3 vPoint1, Vector3 vPoint2)
{
	m_vLines.push_back(vPoint1);
	m_vLines.push_back(vPoint2);
}

void
Debug::AddDebugRectangle(Vector3 vCenter, float width, float height, float depth)
{
	width /= 2.0f; height /= 2.0f; depth /= 2.0f;

	Vector3 vTopLeftFront(vCenter.x - width, vCenter.y + height, vCenter.z + depth);
	Vector3 vTopLeftBack(vCenter.x - width, vCenter.y + height, vCenter.z - depth);
	Vector3 vTopRightBack(vCenter.x + width, vCenter.y + height, vCenter.z - depth);
	Vector3 vTopRightFront(vCenter.x + width, vCenter.y + height, vCenter.z + depth);

	Vector3 vBottom_LeftFront(vCenter.x - width, vCenter.y - height, vCenter.z + depth);
	Vector3 vBottom_LeftBack(vCenter.x - width, vCenter.y - height, vCenter.z - depth);
	Vector3 vBottomRightBack(vCenter.x + width, vCenter.y - height, vCenter.z - depth);
	Vector3 vBottomRightFront(vCenter.x + width, vCenter.y - height, vCenter.z + depth);


	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopRightFront);
	
	m_vLines.push_back(vTopLeftBack);  		m_vLines.push_back(vTopRightBack);
	
	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopLeftBack);
	
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vTopRightBack);



	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottomRightFront);

	m_vLines.push_back(vBottom_LeftBack);	m_vLines.push_back(vBottomRightBack);

	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottom_LeftBack);

	m_vLines.push_back(vBottomRightFront); m_vLines.push_back(vBottomRightBack);



	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vBottom_LeftFront);

	m_vLines.push_back(vTopLeftBack);		m_vLines.push_back(vBottom_LeftBack);

	m_vLines.push_back(vTopRightBack);		m_vLines.push_back(vBottomRightBack);

	m_vLines.push_back(vTopRightFront); m_vLines.push_back(vBottomRightFront);
}

void
Debug::Clear()
{
	m_vLines.clear();
}


Octree::Octree()
{
	InitOctree();
}


Octree::~Octree()
{
	DestroyOctree();
}

void
Octree::InitOctree()
{
	m_bSubDivided = false;

	m_Width = 0;

	m_TriangleCount = 0;

	m_vCenter = Vector3(0, 0, 0);

	m_pVertices = NULL;

	// set the sub nodes to NULL
	memset(m_pOctreeNodes, 0, sizeof(m_pOctreeNodes));
}

void
Octree::GetSceneDimensions(Vector3 *pVertices, int numberOfVerts)
{
	float maxWidth = 0, maxHeight = 0, maxDepth = 0;

	if (!pVertices || numberOfVerts <= 0) return;

	for (int i = 0; i < numberOfVerts; i++) {
		m_vCenter = m_vCenter + pVertices[i];
	}

	m_vCenter.x /= numberOfVerts;
	m_vCenter.y /= numberOfVerts;
	m_vCenter.z /= numberOfVerts;

	for (int i = 0; i < numberOfVerts; i++) {
		float currentWidth  = fabsf(pVertices[i].x - m_vCenter.x);
		float currentHeight = fabsf(pVertices[i].y - m_vCenter.y);
		float currentDepth  = fabsf(pVertices[i].z - m_vCenter.z);

		if (currentWidth > maxWidth)   maxWidth = currentWidth;
		if (currentHeight > maxHeight) maxDepth = currentHeight;
		if (currentDepth > maxDepth)   maxDepth = currentDepth;
	}

	maxWidth *= 2; maxHeight *= 2; maxDepth *= 2;

	if (maxWidth > maxHeight && maxWidth > maxDepth)
		m_Width = maxWidth;

	else if (maxHeight > maxWidth && maxHeight > maxDepth)
		m_Width = maxHeight;

	else
		m_Width = maxDepth;
}

Vector3
Octree::GetNewNodeCenter(Vector3 vCenter, float width, int nodeID)
{
	Vector3 vNodeCenter(0, 0, 0);
	Vector3 vCtr = vCenter;

	switch (nodeID)
	{
	case TOP_LEFT_FRONT:
		vNodeCenter = Vector3(vCtr.x - width / 4, vCtr.y + width / 4, vCtr.z + width / 4);
		break;

	case TOP_LEFT_BACK:
		vNodeCenter = Vector3(vCtr.x - width / 4, vCtr.y + width / 4, vCtr.z - width / 4);
		break;

	case TOP_RIGHT_BACK:
		vNodeCenter = Vector3(vCtr.x + width / 4, vCtr.y + width / 4, vCtr.z - width / 4);
		break;

	case TOP_RIGHT_FRONT:
		vNodeCenter = Vector3(vCtr.x + width / 4, vCtr.y + width / 4, vCtr.z + width / 4);
		break;

	case BOTTOM_LEFT_FRONT:
		vNodeCenter = Vector3(vCtr.x - width / 4, vCtr.y - width / 4, vCtr.z + width / 4);
		break;

	case BOTTOM_LEFT_BACK:
		vNodeCenter = Vector3(vCtr.x - width / 4, vCtr.y - width / 4, vCtr.z - width / 4);
		break;

	case BOTTOM_RIGHT_BACK:
		vNodeCenter = Vector3(vCtr.x + width / 4, vCtr.y - width / 4, vCtr.z - width / 4);
		break;

	case BOTTOM_RIGHT_FRONT:
		vNodeCenter = Vector3(vCtr.x + width / 4, vCtr.y - width / 4, vCtr.z + width / 4);
		break;
	}

	return vNodeCenter;
}

void
Octree::CreateNewNode(Vector3 *pVertices, vector<bool> pList, int numberOfVerts, Vector3 vCenter, float width, int triangleCount, int nodeID)
{
	if (triangleCount) {
		Vector3 *pNodeVertices = new Vector3[triangleCount * 3];
		
		int index = 0;

		for (int i = 0; i < numberOfVerts; i++) {
			if (pList[i / 3]) {
				pNodeVertices[index] = pVertices[i];
				index++;
			}
		}

		m_pOctreeNodes[nodeID] = new Octree;

		Vector3 vNodeCenter = GetNewNodeCenter(vCenter, width, nodeID);

		g_CurrentSubdivision++;

		m_pOctreeNodes[nodeID]->CreateNode(pNodeVertices, triangleCount * 3, vNodeCenter, width / 2);

		g_CurrentSubdivision++;

		delete[] pNodeVertices;

	}
}


void
Octree::CreateNode(Vector3 *pVertices, int numberOfVerts, Vector3 vCenter, float width)
{
	int numberOfTriangles = numberOfVerts / 3;

	m_vCenter = vCenter;

	m_Width = width;

	g_Debug.AddDebugRectangle(vCenter, width, width, width);

	if ((numberOfTriangles > g_MaxTriangles) && (g_CurrentSubdivision < g_MaxSubdivisions)) {
		m_bSubDivided = true;

		vector<bool> pList1(numberOfTriangles);
		vector<bool> pList2(numberOfTriangles);
		vector<bool> pList3(numberOfTriangles);
		vector<bool> pList4(numberOfTriangles);
		vector<bool> pList5(numberOfTriangles);
		vector<bool> pList6(numberOfTriangles);
		vector<bool> pList7(numberOfTriangles);
		vector<bool> pList8(numberOfTriangles);
		
		Vector3 vCtr = vCenter;

		for (int i = 0; i < numberOfVerts; i++) {
			Vector3 vPoint = pVertices[i];

			if ((vPoint.x <= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z >= vCtr.z))
				pList1[i / 3] = true;

			if ((vPoint.x <= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z <= vCtr.z))
				pList2[i / 3] = true;

			if ((vPoint.x >= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z <= vCtr.z))
				pList3[i / 3] = true;

			if ((vPoint.x >= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z >= vCtr.z))
				pList4[i / 3] = true;

			if ((vPoint.x <= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z >= vCtr.z))
				pList5[i / 3] = true;

			if ((vPoint.x <= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z <= vCtr.z))
				pList6[i / 3] = true;

			if ((vPoint.x >= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z <= vCtr.z))
				pList7[i / 3] = true;

			if ((vPoint.x >= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z >= vCtr.z))
				pList8[i / 3] = true;
		}

		int triCount1 = 0;		int triCount2 = 0;		int triCount3 = 0;		int triCount4 = 0;
		int triCount5 = 0;		int triCount6 = 0;		int triCount7 = 0;		int triCount8 = 0;

		for (int i = 0; i < numberOfTriangles; i++) {
			if (pList1[i]) triCount1++;		if (pList2[i]) triCount2++;
			if (pList3[i]) triCount3++;		if (pList4[i]) triCount4++;
			if (pList5[i]) triCount5++;		if (pList6[i]) triCount6++;
			if (pList7[i]) triCount7++;		if (pList8[i]) triCount8++;
		}

		CreateNewNode(pVertices, pList1, numberOfVerts, vCenter, width, triCount1, TOP_LEFT_FRONT);
		CreateNewNode(pVertices, pList2, numberOfVerts, vCenter, width, triCount2, TOP_LEFT_BACK);
		CreateNewNode(pVertices, pList3, numberOfVerts, vCenter, width, triCount3, TOP_RIGHT_BACK);
		CreateNewNode(pVertices, pList4, numberOfVerts, vCenter, width, triCount4, TOP_RIGHT_FRONT);
		CreateNewNode(pVertices, pList5, numberOfVerts, vCenter, width, triCount5, BOTTOM_LEFT_FRONT);
		CreateNewNode(pVertices, pList6, numberOfVerts, vCenter, width, triCount6, BOTTOM_LEFT_BACK);
		CreateNewNode(pVertices, pList7, numberOfVerts, vCenter, width, triCount7, BOTTOM_RIGHT_BACK);
		CreateNewNode(pVertices, pList8, numberOfVerts, vCenter, width, triCount8, BOTTOM_RIGHT_FRONT);
	}
	else {
		AssignVerticesToNode(pVertices, numberOfVerts);
	}
}

void
Octree::AssignVerticesToNode(Vector3 *pVertices, int numberOfVerts)
{
	m_bSubDivided = false;

	m_TriangleCount = numberOfVerts / 3;

	m_pVertices = new Vector3[numberOfVerts];

	memset(m_pVertices, 0, sizeof(Vector3) * numberOfVerts);

	memcpy(m_pVertices, pVertices, sizeof(Vector3) * numberOfVerts);

	g_EndNodeCount++;
}

void
Octree::DrawOctree(Octree *pNode)
{
	if (!pNode) return;

	if (pNode->IsSubDivided()) {
		DrawOctree(pNode->m_pOctreeNodes[TOP_LEFT_FRONT]);
		DrawOctree(pNode->m_pOctreeNodes[TOP_LEFT_BACK]);
		DrawOctree(pNode->m_pOctreeNodes[TOP_RIGHT_BACK]);
		DrawOctree(pNode->m_pOctreeNodes[TOP_RIGHT_FRONT]);

		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_LEFT_FRONT]);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_LEFT_BACK]);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_RIGHT_BACK]);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_RIGHT_FRONT]);
	}
	else {
		if (!pNode->m_pVertices) return;

		glBegin(GL_TRIANGLES);

		glColor3ub(0, 255, 0);

		Vector3 *pVertices = pNode->m_pVertices;

		for (int i = 0; i < pNode->GetTriangleCount() * 3; i += 3) {
			Vector3 vVector1 = pVertices[i + 1] - pVertices[i];
			Vector3 vVector2 = pVertices[i + 2] - pVertices[i];

			Vector3 vNormal = Cross(vVector1, vVector2);

			vNormal = Normalize(vNormal);

			glNormal3f(vNormal.x, vNormal.y, vNormal.z);

			glVertex3f(pVertices[i].x, pVertices[i].y, pVertices[i].z);

			glVertex3f(pVertices[i + 1].x, pVertices[i + 1].y, pVertices[i + 1].z);

			glVertex3f(pVertices[i + 2].x, pVertices[i + 2].y, pVertices[i + 2].z);
		}

		glEnd();
	}
}

void
Octree::DestroyOctree()
{
	if (m_pVertices) {
		delete[] m_pVertices;
		m_pVertices = NULL;
	}

	for (int i = 0; i < 8; i++) {
		if (m_pOctreeNodes[i]) {
			delete m_pOctreeNodes[i];
			m_pOctreeNodes[i] = NULL;
		}
	}

	InitOctree();
}
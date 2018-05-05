#ifndef VECTOR_H_
#define VECTOR_H_

#define PI 3.1415926535897932	

#define BEHIND		0
#define INTERSECTS  1
#define FRONT		2

class Vector3
{
public:
	Vector3() {}

	Vector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	Vector3 operator + (Vector3 vVector)
	{
		return Vector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	Vector3 operator - (Vector3 vVector)
	{
		return Vector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	Vector3 operator * (float num)
	{
		return Vector3(x * num, y * num, z * num);
	}

	Vector3 operator / (float num)
	{
		return Vector3(x / num, y / num, z / num);
	}

	float x, y, z;
};

float Absolute(float num);

Vector3 Cross(Vector3 vVector1, Vector3 vVector2);

float Magnitude(Vector3 vNormal);

Vector3 Normalize(Vector3 vNormal);

Vector3 Normal(Vector3 vPolyon[]);

float Distance(Vector3 vPoint1, Vector3 vPoint2);

Vector3 ClosestPointOnLine(Vector3 vA, Vector3 vB, Vector3 vPoint);

float PlaneDistance(Vector3 Normal, Vector3 Point);

bool IntersectedPlane(Vector3 vPoly[], Vector3 vLine[], Vector3 &vNormal, float &originDistance);

float Dot(Vector3 vVector1, Vector3 vVector2);

double AngleBetweenVectors(Vector3 Vector1, Vector3 Vector2);

Vector3 IntersectionPoint(Vector3 vNormal, Vector3 vLine[], double distance);

bool InsidePolygon(Vector3 vIntersection, Vector3 Poly[], long verticeCount);

bool IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int verticeCount);

int ClassifyingSphere(Vector3 &vCenter, Vector3 &vNormal, Vector3 &vPoint, float radius, float &distance);

bool EdgeSphereCollision(Vector3 &vCenter, Vector3 vPolygon[], int vertexCount, float radius);

bool SpherePolygonCollision(Vector3 vPolygon[], Vector3 &vCenter, int vertexCount, float radius);

Vector3 GetCollisionOffset(Vector3 &vNormal, float radius, float distance);


#endif
#include "Vector3.h"

#include <math.h>
#include <float.h>

float 
Absolute(float num)
{
	if (num < 0)
		return (0 - num);

	return num;
}

Vector3 
Cross(Vector3 vVector1, Vector3 vVector2)
{
	Vector3 vNormal;

	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;
}

float
Magnitude(Vector3 vNormal)
{
	return (float)sqrt((vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z));
}

Vector3 Normalize(Vector3 vNormal)
{
	float magnitue = Magnitude(vNormal);

	vNormal.x /= magnitue;
	vNormal.y /= magnitue;
	vNormal.z /= magnitue;

	return vNormal;
}

Vector3 
Normal(Vector3 vPolygon[])
{
	Vector3 vVector1 = vPolygon[2] - vPolygon[0];
	Vector3 vVector2 = vPolygon[2] - vPolygon[0];

	Vector3 vNormal = Cross(vVector1, vVector2);

	vNormal = Normalize(vNormal);

	return vNormal;
}

float
Distance(Vector3 vPoint1, Vector3 vPoint2)
{
	double distance = sqrt((vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
			   			   (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						   (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z));
	return (float)distance;
}

Vector3
ClosestPointOnLine(Vector3 vA, Vector3 vB, Vector3 vPoint)
{
	Vector3 vVector1 = vPoint - vA;

	Vector3 vVector2 = Normalize(vB - vA);

	float d = Distance(vA, vB);

	float t = Dot(vVector1, vVector2);

	if (t <= 0)
		return vA;

	if (t >= d)
		return vB;

	Vector3 vVector3 = vVector2 * t;

	Vector3 vClosestPoint = vA + vVector3;

	return vClosestPoint;
}

float
PlaneDistance(Vector3 Normal, Vector3 Point)
{
	float distance = 0;

	distance = -((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));

	return distance;
}

bool 
IntersectedPlane(Vector3 vPoly[], Vector3 vLine[], Vector3 &vNormal, float &originDistance)
{
	float distance1 = 0, distance2 = 0;

	vNormal = Normal(vPoly);

	originDistance = PlaneDistance(vNormal, vPoly[0]);

	distance1 = ((vNormal.x * vLine[0].x) +
				 (vNormal.y * vLine[0].y) +
				 (vNormal.z * vLine[0].z)) + originDistance;

	distance1 = ((vNormal.x * vLine[1].x) +
				 (vNormal.y * vLine[1].y) +
				 (vNormal.z * vLine[1].z)) + originDistance;

	if (distance1 * distance2 >= 0)
		return false;

	return true;
}

float
Dot(Vector3 vVector1, Vector3 vVector2)
{
	return ((vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z));
}

double
AngleBetweenVectors(Vector3 vVector1, Vector3 vVector2)
{
	float dotProduct = Dot(vVector1, vVector2);

	float vectorsMagnitude = Magnitude(vVector1) * Magnitude(vVector2);

	double angle = acos(dotProduct / vectorsMagnitude);

	if (_isnan(angle))
		return 0;

	return (angle);
}

Vector3
IntersectionPoint(Vector3 vNormal, Vector3 vLine[], double distance)
{
	Vector3 vPoint, vLineDir;
	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;
	
	vLineDir = vLine[1] - vLine[0];
	vLineDir = Normalize(vLineDir);

	Numerator = - ((vNormal.x * vLine[0].x) +
				   (vNormal.x * vLine[0].x) +
				   (vNormal.x * vLine[0].x) + distance);

	Denominator = Dot(vNormal, vLineDir);

	if (Denominator == 0.0)
		return vLine[0];

	dist = Numerator / Denominator;

	vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (float)(vLine[0].x + (vLineDir.y * dist));
	vPoint.z = (float)(vLine[0].x + (vLineDir.z * dist));

	return vPoint;		
}

bool 
InsidePolygon(Vector3 vIntersection, Vector3 Poly[], long verticeCount)
{
	const double MATCH_FLOOR = 0.99;
	double Angle = 0.0;
	Vector3 vA, vB;

	for (int i = 0; i < verticeCount; i++) {
		vA = Poly[i] - vIntersection;
		vB = Poly[(i + 1) % verticeCount] - vIntersection;
		Angle += AngleBetweenVectors(vA, vB);
	}

	if (Angle >= (MATCH_FLOOR * (2.0 * PI)))
		return true;

	return false;
}

bool 
IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int verticeCount)
{
	Vector3 vNormal;
	float originDistance = 0.0;

	if (!IntersectedPlane(vPoly, vLine, vNormal, originDistance))
		return false;

	Vector3 vIntersection = IntersectionPoint(vNormal, vLine, originDistance);

	if (InsidePolygon(vIntersection, vPoly, verticeCount))
		return true;

	return false;
}

int
ClassifyingSphere(Vector3 &vCenter, Vector3 &vNormal, Vector3 &vPoint, float radius, float &distance)
{
	float d = (float)PlaneDistance(vNormal, vPoint);

	distance = (vNormal.x * vCenter.x + vNormal.y * vCenter.y + vNormal.z * vCenter.z + d);

	if (Absolute(distance) < radius)
		return INTERSECTS;
	else if (distance >= radius)
		return FRONT;

	return BEHIND;
}

bool
EdgeSphereCollision(Vector3 &vCenter, Vector3 vPolygon[], int vertexCount, float radius)
{
	Vector3 vPoint;

	for (int i = 0; i < vertexCount; i++) {
		vPoint = ClosestPointOnLine(vPolygon[i], vPolygon[i + 1 % vertexCount], vCenter);
		float distance = Distance(vPoint, vCenter);

		if (distance < radius)
			return true;
	}

	return false;
}

bool
SpherePolygonCollision(Vector3 vPolygon[], Vector3 &vCenter, int vertexCount, float radius)
{
	Vector3 vNormal = Normal(vPolygon);

	float distance = 0.0;

	int classification = ClassifySphere(vCenter, vNormal, vPolygon[0], radius, distance);

	if (classification == INTERSECTS) {
		Vector3 vOffset = vNormal * distance;

		Vector3 vPosition = vCenter - vOffset;

		if (InsidePolygon(vPosition, vPolygon, 3))
			return true;

		else {
			if (EdgeSphereCollision(vCenter, vPolygon, vertexCount, radius))
				return true;
		}
	}
	return false;
}

Vector3
GetCollisionOffset(Vector3 &vNormal, float radius, float distance)
{
	Vector3 vOffset = Vector3(0, 0, 0);

	if (distance > 0) {
		float distanceOver = radius - distance;
		vOffset = vNormal * distanceOver;
	}
	else {
		float distanceOver = radius + distance;
		vOffset = vNormal * -distanceOver;
	}

	return vOffset;
}
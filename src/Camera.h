#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector3.h"

class Camera
{
public:
	Camera();

	Vector3 Position() { return m_vPosition; }
	Vector3 View() { return m_vView; }
	Vector3 UpVector() { return m_vUpVector; }
	Vector3 Strafe() { return m_vStrafe; }

	void PositionCamera(float positionX, float positionY, float positionZ,
		float viewX, float viewY, float viewZ,
		float upVectorX, float upVectorY, float upVectorZ);

	void RotateCamera(float angle, float X, float Y, float Z);

	void SetViewByMouse();

	void RotateAroundPoint(Vector3 vCenter, float X, float Y, float Z);

	void StrafeCamera(float speed);

	void MoveCamera(float speed);

	void CheckForMovement();

	void Update();

	void Lock();

private:
	Vector3 m_vPosition;

	Vector3 m_vView;

	Vector3 m_vUpVector;

	Vector3 m_vStrafe;
};

#endif
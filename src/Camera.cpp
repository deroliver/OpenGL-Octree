#include "Main.h"
#include "Camera.h"

#define kSpeed 50.0f

float g_FrameInterval = 0.0f;


void CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;
	static float strFrameRate[50] = { 0 };
	static float frameTime = 0.0f;

	float currentTime = timeGetTime() * 0.001f;

	g_FrameInterval = currentTime - frameTime;
	frameTime = currentTime;

	++framesPerSecond;

	if (currentTime - lastTime > 1.0f) {
		lastTime = currentTime;

		sprintf(strFrameRate, "Current Frames Per Second: %d", int(framesPerSecond));

		framesPerSecond = 0;
	}
}

Camera::Camera()
{
	Vector3 vZero = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 vView = Vector3(0.0f, 1.0f, 0.5f);
	Vector3 vUp = Vector3(0.0f, 0.0f, 1.0f);

	m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}

void
Camera::PositionCamera(float positionX, float positionY, float positionZ,
	float viewX, float viewY, float viewZ,
	float upVectorX, float upVectorY, float upVectorZ)
{
	Vector3 vPosition = Vector3(positionX, positionY, positionZ);
	Vector3 vView = Vector3(viewX, viewY, viewZ);
	Vector3 vUpVector = Vector3(upVectorX, upVectorY, upVectorZ);

	m_vPosition = vPosition;
	m_vView = vView;
	m_vUpVector = vUpVector;
}

void
Camera::SetViewByMouse()
{
	POINT mousePos;
	int middleX = SCREEN_WIDTH >> 1;
	int middleY = SCREEN_HEIGHT >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	angleY = (float)((middleX - mousePos.x)) / 500.0f;
	angleZ = (float)((middleY - mousePos.y)) / 500.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;

	currentRotX += angleZ;

	if (currentRotX > 1.0f) {
		currentRotX = 1.0f;

		if (lastRotX != 1.0f) {
			Vector3 vAxis = Cross()
		}
	}

}
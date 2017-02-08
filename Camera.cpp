#include "Camera.h"

Camera::Camera(XMFLOAT3 _eye, XMFLOAT3 _at, XMFLOAT3 _up, FLOAT _windowWidth, FLOAT _windowHeight, FLOAT _nearDepth, FLOAT _farDepth)
{
	position = _eye;

	eye = XMVectorSet(_eye.x, _eye.y, _eye.z, 0.0f);
	at = XMVectorSet(_at.x, _at.y, _at.z, 0.0f);
	up = XMVectorSet(_up.x, _up.y, _up.z, 0.0f);

	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	nearDepth = _nearDepth;
	farDepth = _farDepth;
}

Camera::~Camera()
{
}

void Camera::Input()
{

}

void Camera::CalculateViewProjection()
{
	// Initiliase the view matrix
	XMStoreFloat4x4(&view, XMMatrixLookAtLH(eye, at, up));

	// Initialize the projection matrix
	XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, windowWidth / windowHeight, nearDepth, farDepth));
}

void Camera::SetEye(XMFLOAT3 _eye)
{
	position = _eye;
	eye = XMVectorSet(_eye.x, _eye.y, _eye.z, 0.0f);
}

void Camera::SetAt(XMFLOAT3 _at)
{
	at = XMVectorSet(_at.x, _at.y, _at.z, 0.0f);
}

void Camera::SetRotation(float x, float y, float z)
{
	XMStoreFloat4x4(&rotate, XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z));
}

void Camera::Reshape(FLOAT _windowWidth, FLOAT _windowHeight, FLOAT _nearDepth, FLOAT _farDepth)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
	nearDepth = _nearDepth;
	farDepth = _farDepth;
}

XMFLOAT4X4 Camera::GetViewProjection() const
{
	XMMATRIX _view = XMLoadFloat4x4(&view);
	XMMATRIX _projection = XMLoadFloat4x4(&projection);

	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, _view * _projection);

	return viewProj;
}
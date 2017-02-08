#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

using namespace DirectX;

class Camera
{
public:
	Camera(XMFLOAT3 _eye, XMFLOAT3 _at, XMFLOAT3 _up, FLOAT _windowWidth, FLOAT _windowHeight, FLOAT _nearDepth, FLOAT _farDepth);
	~Camera();

	// Check Camera Input
	void Input();

	// Calculate the View Projection
	void CalculateViewProjection();

	// Get View
	XMFLOAT4X4 GetView() const { return view; }

	// Get Projection
	XMFLOAT4X4 GetProjection() const { return projection; }

	// Get View/Projection
	XMFLOAT4X4 GetViewProjection() const;

	// Get/Set Eye
	XMVECTOR GetEye() const { return eye; }
	void SetEye(XMFLOAT3 _eye);

	// Get/Set At
	XMVECTOR GetAt() const { return at; }
	void SetAt(XMFLOAT3 _at);

	// Get/Set Up
	XMVECTOR GetUp() const { return up; }
	void SetUp(XMVECTOR _up) { up = _up; }

	// Get/Set Position
	XMFLOAT3 GetPosition() const { return position; }
	void SetPosition(XMFLOAT3 _position) { position = _position; };

	// Get/Set Rotation
	void SetRotation(float x, float y, float z);

	// Alter Window height and width values. As well as distance sight
	void Reshape(FLOAT _windowWidth, FLOAT _windowHeight, FLOAT _nearDepth, FLOAT _farDepth);

private:
	// Camera Properties
	XMVECTOR eye;
	XMVECTOR at;
	XMVECTOR up;

	FLOAT windowWidth;
	FLOAT windowHeight;

	FLOAT nearDepth;
	FLOAT farDepth;

	XMFLOAT4X4 rotate;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	XMFLOAT3 position;
};


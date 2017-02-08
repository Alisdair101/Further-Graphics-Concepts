#pragma once

#include "CarParticleModel.h"
#include "GameObject.h"

class Car
{
public:
	Car(GameObject* _carBody, vector < GameObject* > _carWheels);
	~Car();

	virtual void Update(float t);
	void Draw(ID3D11DeviceContext* _pImmediateContext);

	// Car Input
	void Input();

	// Get Car Body and Wheels
	GameObject* GetCarBody() const { return carBody; };
	vector< GameObject* > GetCarWheels() const { carWheels; };

	// Get/Set Car Position
	XMFLOAT3 GetCarPosition() const { return carPos; };
	void SetCarPosition(XMFLOAT3 _carPos) { carPos = _carPos; };
	void SetCarPosition(float x, float y, float z) { carPos.x = x, carPos.y = y, carPos.z = z; };

	// Get/Calculate Car Forward Vector
	XMFLOAT3 GetForwardVector() const { return carForwardVector; };
	void CalculateForwardVector();

	// Get/Set Car Rotation
	float GetCarRotation() const { return carRotation; };
	void SetCarRotation(float _carRotation) { carRotation = _carRotation; carRotation = carRotation * (XM_PI / 180);
											  carBody->GetTransform()->SetRotation(0.0f, carRotation, 0.0f); };
	void AddCarRotation(float _carRotation) { carRotation = _carRotation; carRotation = carRotation * (XM_PI / 180);
											  carBody->GetTransform()->SetRotation(0.0f, carRotation, 0.0f); };

	// Get/Set Car Rotation Speed
	float GetCarRotationSpeed() const { return carRotationSpeed; };
	void SetCarRotationSpeed(float _carRotationSpeed) { carRotationSpeed = carRotationSpeed; };

	// Get/Set Car Wheel Rotation
	float GetCarWheelRotation() const { return carWheelRotation; };
	void SetCarWheelRotation(float _carWheelRotation) { carWheelRotation = _carWheelRotation; };
	void AddCarWheelRotation(float _carWheelRotation) { carWheelRotation += _carWheelRotation; };

private:
	// Car Object
	GameObject* carBody;
	vector < GameObject* > carWheels;

	// Car Properties
	XMFLOAT3 carPos;
	XMFLOAT3 carForwardVector;

	float carRotation;
	float carRotationSpeed;
	float carWheelRotation;

	float engineSpeedAdd;
};
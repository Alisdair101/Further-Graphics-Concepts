#pragma once

#include "ParticleModel.h"

class CarParticleModel : public ParticleModel
{
public:
	CarParticleModel(Transform* transform, float _mass);
	~CarParticleModel();

	void CalculateWheelSpeed();
	void CalculateThrust(float t);
	void CalculateVelocity();

	void TruncateSpeed();

	void Update(float t);


	// --------------- Get/Set ------------------ //

	// Get/Set Car Direction
	XMFLOAT3 GetCarDirection() const { return carDirection; }
	void SetCarDirection(XMFLOAT3 _carDirection) { carDirection = _carDirection; }

	// Get/Set Car Velocity
	XMFLOAT3 GetCarVelocity() const { return carVelocity; }
	void SetEngineSpeed(XMFLOAT3 _carVelocity) { carVelocity = _carVelocity; }

	// Get/Set/Add Engine Speed
	float GetEngineSpeed() const { return engineSpeed; }
	void SetEngineSpeed(float _engineSpeed) { engineSpeed = _engineSpeed;}
	void AddEngineSpeed(float _engineSpeed);

	// Get/Set/Add Engine Speed Limit
	float GetEngineSpeedLimit() const { return engineSpeedLimit; }
	void SetEngineSpeedLimit(float _engineSpeedLimit) { engineSpeedLimit = _engineSpeedLimit; }

private:
	float wheelRadius;
	float wheelSpeed;
	int	gearRatio;

	float engineSpeed;
	float engineSpeedLimit;

	float thrust;

	XMFLOAT3 carVelocity;
	XMFLOAT3 carDirection;
};


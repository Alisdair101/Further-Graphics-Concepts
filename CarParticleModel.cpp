#include "CarParticleModel.h"


CarParticleModel::CarParticleModel(Transform* transform, float _mass) : ParticleModel(transform, _mass)
{
	gearRatio = 10.0f;
	wheelSpeed = 1.0f;
	engineSpeed = 0.0f;
	wheelRadius = 10.0f;

	engineSpeedLimit = 0.3f;

	carVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CarParticleModel::~CarParticleModel()
{
}

void CarParticleModel::AddEngineSpeed(float _engineSpeed)
{
	engineSpeed += _engineSpeed;
	TruncateSpeed();
}

void CarParticleModel::CalculateWheelSpeed()
{
	TruncateSpeed();

	wheelSpeed = engineSpeed / gearRatio;
}

void CarParticleModel::TruncateSpeed()
{
	if (engineSpeed >= engineSpeedLimit)
	{
		engineSpeed = engineSpeedLimit;
	}
}

void CarParticleModel::CalculateThrust(float t)
{
	thrust = ((wheelSpeed * 60) * 2 * XM_PI * wheelRadius) / t;
}

void CarParticleModel::CalculateVelocity()
{
	carVelocity.x = carDirection.x * thrust;
	carVelocity.y = carDirection.y * thrust;
	carVelocity.z = carDirection.z * thrust;
}

void CarParticleModel::Update(float t)
{
	CalculateWheelSpeed();
	CalculateThrust(t);
	CalculateVelocity();

	// Update Particle Model
	ParticleModel::Update(t);
}
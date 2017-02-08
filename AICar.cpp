#include "AICar.h"

AICar::AICar(GameObject* _carBody, vector < GameObject* > _carWheels, vector < GameObject* > _waypoints) : Car(_carBody, _carWheels)
{
	waypoints = _waypoints;
	targetPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	targetIndex = 0;

	CarParticleModel* carBodyModel = (CarParticleModel*)GetCarBody()->GetParticleModel();
	carBodyModel->SetEngineSpeedLimit(0.2f);
}

AICar::~AICar()
{

}

void AICar::Update(float t)
{
	Pathfinding();
	GoToTarget();

	Car::Update(t);
}

// ----------------- PATH FINDING -------------------- //

void AICar::Pathfinding()
{
	targetPosition = waypoints.at(targetIndex)->GetTransform()->GetPosition();
	XMFLOAT3 carPosition = GetCarBody()->GetTransform()->GetPosition();

	// Distance to Target Position
	XMFLOAT3 targetDistance;
	targetDistance.x = targetPosition.x - carPosition.x;
	targetDistance.y = targetPosition.y - carPosition.y;
	targetDistance.z = targetPosition.z - carPosition.z;

	float targetDistanceMagnitude = sqrt((targetDistance.x * targetDistance.x) + (targetDistance.y * targetDistance.y) + (targetDistance.z * targetDistance.z));

	if (targetDistanceMagnitude <= 30.0f)
	{
		targetIndex += 1;
	}

	if (targetIndex >= waypoints.size())
	{
		targetIndex = 0;
	}
}

// --------------- STEERING BEHAVIOUR ---------------- //

void AICar::GoToTarget()
{
	XMFLOAT3 carForwardVector = GetForwardVector();
	XMFLOAT3 carPos = GetCarBody()->GetTransform()->GetPosition();

	XMFLOAT3 upDirection = XMFLOAT3(carPos.x, carPos.y + 10.0f, carPos.z);
	upDirection.x = upDirection.x - carPos.x;
	upDirection.y = upDirection.y - carPos.y;
	upDirection.z = upDirection.z - carPos.z;

	XMFLOAT3 forwardDirection;
	forwardDirection.x = carPos.x - (carPos.x + carForwardVector.x);
	forwardDirection.y = carPos.y - (carPos.y + carForwardVector.y);
	forwardDirection.z = carPos.z - (carPos.z + carForwardVector.z);

	XMFLOAT3 targetDirection;
	targetDirection.x = targetPosition.x - carPos.x;
	targetDirection.y = targetPosition.y - carPos.y;
	targetDirection.z = targetPosition.z - carPos.z;

	XMFLOAT3 crossProduct;
	crossProduct.x = (forwardDirection.y * targetDirection.z) - (forwardDirection.z * targetDirection.y);
	crossProduct.y = (forwardDirection.z * targetDirection.x) - (forwardDirection.x * targetDirection.z);
	crossProduct.z = (forwardDirection.x * targetDirection.y) - (forwardDirection.y * targetDirection.x);

	float dotProduct = (crossProduct.x + upDirection.x) + (crossProduct.y + upDirection.y) + (crossProduct.z + upDirection.z);

	CarParticleModel* carBodyTemp = (CarParticleModel*)GetCarBody()->GetParticleModel();
	float engineSpeed = carBodyTemp->GetEngineSpeed();

	if (dotProduct < 5.0f && dotProduct > -5.0f)
	{
		carBodyTemp->AddEngineSpeed(0.0008f);
		SetCarWheelRotation(0.0f);
	}
	else if (dotProduct > 5.0f)
	{
		AddCarWheelRotation(-0.005f);

		if (engineSpeed < 0.1)
		{
			carBodyTemp->AddEngineSpeed(0.0002f);
		}
		else if (engineSpeed > 0.1)
		{
			carBodyTemp->AddEngineSpeed(-0.0002f);
		}
	}
	else if (dotProduct < 5.0f)
	{
		AddCarWheelRotation(0.005f);

		if (engineSpeed < 0.1)
		{
			carBodyTemp->AddEngineSpeed(0.0002f);
		}
		else if (engineSpeed > 0.1)
		{
			carBodyTemp->AddEngineSpeed(-0.0002f);
		}
	}

	engineSpeed = carBodyTemp->GetEngineSpeed();

	GameObject* cartemp = GetCarBody();
	float carRotation = GetCarBody()->GetTransform()->GetRotation().y;
	float carWheelRotation = GetCarWheelRotation();
	float carRotationSpeed = 0.1f;

	//// Rotate Car in consideration to the Wheels Rotation
	//if (engineSpeed > 0)
	//{
	//	carRotation += (carWheelRotation * 100) * (engineSpeed * 20);
	//}
	//else if (engineSpeed < 0)
	//{
	//	engineSpeed *= -1;
	//	carRotation -= (carWheelRotation * 100) * (engineSpeed * 20);
	//}
}
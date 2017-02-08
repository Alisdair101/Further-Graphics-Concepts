#include "Car.h"
#include <iostream>

Car::Car(GameObject* _carBody, vector < GameObject* > _carWheels)
{
	carBody = _carBody;
	carWheels = _carWheels;

	carPos = carBody->GetTransform()->GetPosition();

	carRotation = -350.0f;
	carRotationSpeed = 0.001f;
	carWheelRotation = 0.0f;

	engineSpeedAdd = 0.0008f;
}

Car::~Car()
{

}

void Car::Input()
{
	CarParticleModel* carBodyModel = (CarParticleModel*)carBody->GetParticleModel();
	float engineSpeed = carBodyModel->GetEngineSpeed();

	carPos = carBody->GetTransform()->GetPosition();

	if (GetAsyncKeyState('B'))
	{
		carPos = XMFLOAT3(carPos.x, 20.0f, carPos.z);
		carBody->GetTransform()->SetPosition(carPos);
	}

	if (GetAsyncKeyState('W'))
	{
		// Set Car Engine Speed
		carBodyModel->AddEngineSpeed((engineSpeedAdd));
	}
	else if (GetAsyncKeyState('S'))
	{
		if (engineSpeed > 0)
		{
			carBodyModel->AddEngineSpeed(-engineSpeedAdd * 2.0f);
		}
		else
		{
			carBodyModel->AddEngineSpeed((-engineSpeedAdd / 1.5f));
		}
	}
	else
	{
		float engineSpeed = carBodyModel->GetEngineSpeed();

		if (engineSpeed > 0)
		{
			carBodyModel->AddEngineSpeed(-engineSpeedAdd);

			if (engineSpeed < 0.0008f && engineSpeed != 0.0f)
			{
				carBodyModel->SetEngineSpeed(0.0f);
			}
		}
		else if (engineSpeed < 0)
		{
			carBodyModel->AddEngineSpeed(engineSpeedAdd);
		}
	}

	// Car Rotation Check
	if (GetAsyncKeyState('A'))
	{
		carWheelRotation -= 0.01f;
	}
	else if (GetAsyncKeyState('D'))
	{
		carWheelRotation += 0.01f;
	}
	else
	{
		if (carWheelRotation < 0)
		{
			carWheelRotation += 0.02f;
		}
		else if (carWheelRotation > 0)
		{
			carWheelRotation -= 0.02f;
		}

		if (carWheelRotation < 0.01f && carWheelRotation > -0.01f)
		{
			carWheelRotation = 0;
		}
	}
}

void Car::CalculateForwardVector()
{
	carBody->GetTransform()->GetRotation();

	carForwardVector.x = sin((carRotation / 17.425) * (XM_PI / 180));
	carForwardVector.y = 0.0f;
	carForwardVector.z = cos((carRotation / 17.425) * (XM_PI / 180));

	float carDirectionMag = sqrt((carForwardVector.x * carForwardVector.x) + (carForwardVector.y * carForwardVector.y) + (carForwardVector.z * carForwardVector.z));
	carForwardVector = XMFLOAT3((carForwardVector.x / carDirectionMag), (carForwardVector.y / carDirectionMag), (carForwardVector.z / carDirectionMag));

	CarParticleModel* carBodyModel = (CarParticleModel*)carBody->GetParticleModel();
	carBodyModel->SetCarDirection(carForwardVector);

	// Reset Car Rotation if over 360 or 0 degrees
	if (carRotation <= -6247 || carRotation >= 6247)
	{
		carRotation = 0;
	}
}

void Car::Update(float t)
{
	CalculateForwardVector();

	CarParticleModel* carBodyModel = (CarParticleModel*)carBody->GetParticleModel();
	float engineSpeed = carBodyModel->GetEngineSpeed();

	// Update Car Wheels Rotation
	carBody->GetChildren().at(0)->GetTransform()->SetRotation(engineSpeed * 50, carWheelRotation, 0.0f);
	carBody->GetChildren().at(1)->GetTransform()->SetRotation(engineSpeed * 50, carWheelRotation, 0.0f);
	carBody->GetChildren().at(2)->GetTransform()->SetRotation(engineSpeed * 50, 0.0f, 0.0f);
	carBody->GetChildren().at(3)->GetTransform()->SetRotation(engineSpeed * 50, 0.0f, 0.0f);

	// Check Car type
	string type = carBody->GetType();

	if (type == "Car")
	{
		XMFLOAT3 velTemp = carBody->GetParticleModel()->GetVelocity();
		XMFLOAT3 carVelTemp = carBodyModel->GetCarVelocity();

		velTemp.x += carVelTemp.x;
		velTemp.y += carVelTemp.y;
		velTemp.z += carVelTemp.z;

		carBodyModel->SetVelocity(velTemp);

		// Limit Wheel Rotation
		float carlimit = 0.1f;

		if (carWheelRotation <= -carlimit)
		{
			carWheelRotation = -carlimit;
		}
		else if (carWheelRotation >= carlimit)
		{
			carWheelRotation = carlimit;
		}
	}
	else
	{
		carPos = carBody->GetTransform()->GetPosition();
		XMFLOAT3 velTemp = carBody->GetParticleModel()->GetVelocity();

		// AI positioning
		XMFLOAT3 carVelocity = carBodyModel->GetCarVelocity();
		carPos = XMFLOAT3((carPos.x + carVelocity.x), (carPos.y + carVelocity.y), (carPos.z + carVelocity.z));
		carBody->GetTransform()->SetPosition(carPos);

		// Limit Wheel Rotation
		float carlimit = 0.1f;

		if (carWheelRotation <= -carlimit)
		{
			carWheelRotation = -carlimit;
		}
		else if (carWheelRotation >= carlimit)
		{
			carWheelRotation = carlimit;
		}
	}


	// Rotate Car in consideration to the Wheels Rotation
	if (engineSpeed > 0)
	{
		carRotation += (carWheelRotation * 100) * (engineSpeed * 20);
	}
	else if (engineSpeed < 0)
	{
		engineSpeed *= -1;
		carRotation -= (carWheelRotation * 100) * (engineSpeed * 20);
	}

	carBody->GetTransform()->SetRotation(0.0f, carRotation * carRotationSpeed, 0.0f);

	// Update Transform
	carBody->Update(t);

	// Update Particle Model
	carBody->GetParticleModel()->Update(t);
}

void Car::Draw(ID3D11DeviceContext* _pImmediateContext)
{
	carBody->Draw(_pImmediateContext);
}

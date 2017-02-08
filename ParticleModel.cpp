#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform* _transform, float _mass)
{
	transform = _transform;
	acceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	velocity = XMFLOAT3(0.0f, 0.0f, -0.1f);
	netForce = XMFLOAT3(0.0f, 0.0f, 0.0f);
	slidingForce = XMFLOAT3(0.0f, 0.0f, 0.0f);

	dragFactor = 1.0f;
	forceMag = 0.0f;
	mass = _mass;

	slidingOn = false;

	useConstAcc = true;
	laminar = true;

	collisionRadius = 1.5f;

	theta = 90.0f;
}

ParticleModel::~ParticleModel()
{

}

// ----------------------- Movement ------------------------- //

void ParticleModel::Move(float amountX, float amountY, float amountZ)
{
	XMFLOAT3 position = transform->GetPosition();

	position.x += amountX;
	position.y += amountY;
	position.z += amountZ;

	transform->SetPrevPosition(transform->GetPosition());

	XMFLOAT3 PREV = transform->GetPrevPosition();

	transform->SetPosition(position);
}

void ParticleModel::MoveConstAcc(float t)
{
	t /= 1000;

	XMFLOAT3 position = transform->GetPosition();

	constAccel.x = velocity.x * t + 0.5f * acceleration.x * t * t;
	constAccel.y = velocity.y * t + 0.5f * acceleration.y * t * t;
	constAccel.z = velocity.z * t + 0.5f * acceleration.z * t * t;

	// Update object position
	position.x += constAccel.x;
	position.y += constAccel.y;
	position.z += constAccel.z;

	// Set velocity
	velocity.x += (acceleration.x * 1.4f) * t;
	velocity.y += (acceleration.y * 1.4f) * t;
	velocity.z += (acceleration.z * 1.4f) * t;

	transform->SetPosition(position);
}

void ParticleModel::MoveConstVel(float t)
{
	XMFLOAT3 prevPosition = transform->GetPosition();

	XMFLOAT3 position;
	position.x = prevPosition.x + velocity.x * t;
	position.y = prevPosition.y + velocity.y * t;
	position.z = prevPosition.z + velocity.z * t;

	transform->SetPosition(position);
}

// -------------------- Sliding -------------------- //

void ParticleModel::SlidingForce(float theta, float frCoef)
{
	forceMag = mass * gravity * (sin(theta) - frCoef * cos(theta));

	slidingForce.x = forceMag * cos(theta);
	slidingForce.y = forceMag * sin(theta);
	slidingForce.z = forceMag * cos(theta);

	if (slidingForce.y > 0.0f)
	{
		slidingForce.x = 0.0f;
		slidingForce.y = 0.0f;
	}
}

// ------------------- Drag Force ------------------ //

void ParticleModel::DragForce()
{
	if (laminar)
	{
		// Calculate drag force for laminar flow
		DragLamFlow();
	}
	else
	{
		// Calculate drag force for turbulent flow
		TurbulantFlow();
	}
}

void ParticleModel::DragLamFlow()
{
	dragForce.x = -dragFactor * velocity.x;
	dragForce.y = -dragFactor * velocity.y;
	dragForce.z = -dragFactor * velocity.z;
}

void ParticleModel::MotionInFluid(float t)
{
	DragForce();
	MoveConstAcc(t);
}

void ParticleModel::TurbulantFlow()
{
	float velMag;
	// Calculate magnitude of velocity
	velMag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.y * velocity.y));
	
	// Calculate unit vector of velocity
	XMFLOAT3 unitVel;
	unitVel.x = (velocity.x / velMag);
	unitVel.y = (velocity.y / velMag);
	unitVel.z = (velocity.z / velMag);

	// Calculate magnitude of drag force
	float dragMag;
	dragMag = dragFactor * velMag * velMag;

	// Calculate of x- and y-components of drag force
	dragForce.x = -dragMag * unitVel.x;
	dragForce.y = -dragMag * unitVel.y;
}

// ----------------- Collision Detection -------------- //

void ParticleModel::BaseCollisionCheck(XMFLOAT3 floorPos)
{
	XMFLOAT3 objectPos = transform->GetPosition();

	XMFLOAT3 difference;
	difference.y = objectPos.y - floorPos.y;

	float differenceMag = sqrt(difference.y * difference.y);
	float velocityMag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z));

	if (differenceMag <= collisionRadius)
	{
		velocity.y = -velocity.y / mass;
		slidingOn = true;
	}
	else
	{
		slidingOn = false;
	}
}

bool ParticleModel::CollisionCheck(XMFLOAT3 object2Pos, float object2Radius)
{
	XMFLOAT3 object1Pos = transform->GetPosition();

	XMFLOAT3 difference;
	difference.x = object1Pos.x - object2Pos.x;
	difference.y = object1Pos.y - object2Pos.y;
	difference.z = object1Pos.z - object2Pos.z;

	float differenceMag = sqrt((difference.x * difference.x) + (difference.y * difference.y) + (difference.z * difference.z));

	float radiusDistance = collisionRadius + object2Radius;

	if (differenceMag <= radiusDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ParticleModel::ResolveCollision(ParticleModel* object2)
{
	// Resolve Interpenertrations
	XMFLOAT3 obj1PrevPos = transform->GetPrevPosition();
	XMFLOAT3 obj2PrevPos = object2->transform->GetPrevPosition();

	XMFLOAT3 obj1Pos = transform->GetPosition();
	XMFLOAT3 obj2Pos = object2->transform->GetPosition();

	transform->SetPosition(obj1PrevPos);
	object2->transform->SetPosition(obj2PrevPos);

	// Resulting velocities
	float object1Mass = mass;
	float object2Mass = object2->GetMass();

	XMFLOAT3 object1Vel = velocity;
	XMFLOAT3 object2Vel = object2->GetVelocity();

	float cR = 1.0f;

	XMFLOAT3 object1NewVel;
	object1NewVel.x = (((object1Mass * object1Vel.x) + (object2Mass * object2Vel.x) + (object2Mass * cR * (object2Vel.x - object1Vel.x))) / (object1Mass + object2Mass));
	object1NewVel.y = (((object1Mass * object1Vel.y) + (object2Mass * object2Vel.y) + (object2Mass * cR * (object2Vel.y - object1Vel.y))) / (object1Mass + object2Mass));
	object1NewVel.z = (((object1Mass * object1Vel.z) + (object2Mass * object2Vel.z) + (object2Mass * cR * (object2Vel.z - object1Vel.z))) / (object1Mass + object2Mass));

	XMFLOAT3 object2NewVel;
	object2NewVel.x = (((object2Mass * object2Vel.x) + (object1Mass * object1Vel.x) + (object1Mass * cR * (object1Vel.x - object2Vel.x))) / (object1Mass + object2Mass));
	object2NewVel.y = (((object2Mass * object2Vel.y) + (object1Mass * object1Vel.y) + (object1Mass * cR * (object1Vel.y - object2Vel.y))) / (object1Mass + object2Mass));
	object2NewVel.z = (((object2Mass * object2Vel.z) + (object1Mass * object1Vel.z) + (object1Mass * cR * (object1Vel.z - object2Vel.z))) / (object1Mass + object2Mass));

	velocity = object1NewVel;
	object2->SetVelocity(object2NewVel);
}

// --------------------- Update ----------------------- //

void ParticleModel::UpdateAccel()
{
	acceleration.x = netForce.x / mass;
	acceleration.y = netForce.y / mass;
	acceleration.z = netForce.z / mass;
}

void ParticleModel::UpdateNetForce()
{
	netForce.x = dragForce.x + slidingForce.x;
	netForce.y = dragForce.y + slidingForce.y;
	netForce.z = dragForce.z + slidingForce.z;
}

void ParticleModel::UpdateState()
{
	UpdateNetForce();
	UpdateAccel();
}

void ParticleModel::Update(float t)
{
	if (useConstAcc)
	{
		if (slidingOn)
		{
			SlidingForce(XMConvertToRadians(theta), 0.2f);
		}
		else if(!slidingOn)
		{
			slidingForce.x = 0.0f;
			slidingForce.y = mass * gravity;
			slidingForce.z = 0.0f;
		}
		
		DragForce();
		UpdateState();
		MoveConstAcc(t);
	}
	else
	{
		MoveConstVel(t);
	}
}
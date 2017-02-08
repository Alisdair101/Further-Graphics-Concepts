#pragma once

#include <directxmath.h>
#include <d3d11_1.h>

#include "Transform.h"

using namespace DirectX;
using namespace std;

class ParticleModel
{
public:
	ParticleModel(Transform* _transform, float _mass);
	~ParticleModel();

	// --------------- Movement ----------------

	void Move(float amountX, float amountY, float amountZ);
	void MoveConstVel(float t);
	void MoveConstAcc(float t);
	
	// -------------- Drag Force ---------------- //

	void DragForce();
	void DragLamFlow();
	void MotionInFluid(float t);
	void TurbulantFlow();

	// --------------- Sliding ------------------ //

	void SlidingMotion();
	void SlidingForce(float theta, float frCoef);

	// ------------ Collision Check ------------- //

	void BaseCollisionCheck(XMFLOAT3 floorPos);
	bool CollisionCheck(XMFLOAT3 object2Pos, float object2Radius);
	void ResolveCollision(ParticleModel* object2);

	// --------------- Updates ------------------ //

	void UpdateNetForce();
	void UpdateAccel();
	void UpdateState();
	virtual void Update(float t);

	// --------------- Get/Set ------------------ //

	// Get/Set Collision Radius
	float GetCollisionRadius() const { return collisionRadius; }
	void SetCollisionRadius(float _collisionRadius) { collisionRadius = _collisionRadius; }

	// Get/Set Mass
	float GetMass() const { return mass; };
	void SetMass(float _mass) { mass = _mass; }

	// Get/Set Velocity
	XMFLOAT3 GetVelocity() const { return velocity; }
	void SetVelocity(XMFLOAT3 _velocity) { velocity = _velocity; }

	void SetTheta(float _theta) { _theta = theta; }

	// ----------- CarParticleModel ------------- //

	

protected:
	Transform* transform;

private:
	// Movement
	XMFLOAT3 acceleration;
	XMFLOAT3 velocity;

	XMFLOAT3 constAccel;

	bool useConstAcc;

	// Forces
	XMFLOAT3 netForce;
	float forceMag;

	// Drag Force
	XMFLOAT3 dragForce;
	float dragFactor;
	bool laminar;

	// Sliding
	XMFLOAT3 slidingForce;
	bool slidingOn;

	// Collision Check
	float collisionRadius;

	// Other
	const float gravity = -9.81;
	float mass;

	float theta;
};
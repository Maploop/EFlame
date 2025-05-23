#pragma once

#include <PxPhysicsAPI.h>
#include <iostream>

class PhysicsHandler {
public:
	PhysicsHandler();
	~PhysicsHandler();

	bool initPhysics();
	void cleanupPhysX();

protected:
	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;

	physx::PxFoundation* gFoundation = nullptr;
	physx::PxPhysics* gPhysics = nullptr;
	physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	physx::PxScene* gScene = nullptr;
	physx::PxMaterial* gMaterial = nullptr;
	physx::PxPvd* gPvd = nullptr;
};


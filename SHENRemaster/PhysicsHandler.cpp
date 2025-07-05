#include "PhysicsHandler.h"
#include "logger.hpp"

PhysicsHandler::PhysicsHandler() {
}

PhysicsHandler::~PhysicsHandler() {
}

// using namespace physx;
bool PhysicsHandler::initPhysics() {
	/*
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation) {
		std::cerr << "PhysicsHandler > [ERROR] PxCreateFoundation failure." << std::endl;
		return false;
	}

	gPvd = physx::PxCreatePvd(*gFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	if (transport) {
		gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	}

	// Finally initializing physx
	PxTolerancesScale toleranceScale;
	toleranceScale.length = 1.0f; // 1 meter
	toleranceScale.speed = 10.0f; // 10 m/s

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, toleranceScale, true, gPvd);
	if (!gPhysics) {
		SHERROR("PhysicsHandler > PxCreatePhysics failure.");
		return false;
	}

	if (!PxInitExtensions(*gPhysics, gPvd)) {
		SHERROR("PhysicsHandler > PxInitExtensions failure.");
		return false;
	}

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 9.81f, 0.0f); // standard gravity for now

	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// for performance & improved phase algo
	sceneDesc.flags |= PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM; // persistant contact manifold
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eABP;

	sceneDesc.solverType = PxSolverType::eTGS; // gauss sidel (recommended by docs)
	sceneDesc.bounceThresholdVelocity = 2.0f;
	sceneDesc.frictionOffsetThreshold = 0.04f;
	sceneDesc.frictionCorrelationDistance = 0.025f;

	gScene = gPhysics->createScene(sceneDesc);
	if (!gScene) {
		SHFATAL("PhysicsHandler > createScene failure.");
		return false;
	}

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient) {
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	if (!gMaterial) {
		SHFATAL("PhysicsHandler > create material screen failure.");
		return false;
	}

	SHINFO("PhysicsHandler > PhysX 5.0 was successfully initialized.");
	aaa
	return true;
}

void PhysicsHandler::cleanupPhysX() {
	PX_RELEASE(gMaterial);
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);

	PxCloseExtensions();
	PX_RELEASE(gPhysics);

	if (gPvd) {
		PxPvdTransport* transport = gPvd->getTransport();
		if (transport) {
			gPvd->disconnect();
		}
		gPvd->release();
		gPvd = nullptr;
		PX_RELEASE(transport);
	}

	PX_RELEASE(gFoundation);

	SHINFO("PhysicsHandler > PhysX 5.0 cleanup successful.");*/

	return false;
}

void PhysicsHandler::cleanupPhysX() {
}

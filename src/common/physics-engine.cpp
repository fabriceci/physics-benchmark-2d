#include "physics-engine.h"

#include "box2d/physics-engine-box2d.h"
#include "rapier2d/physics-engine-rapier2d.h"

#include "common/utils.h"

PhysicsEngine* PhysicsEngine::create_engine(EPhysicsEngine engine_type) {
	switch (engine_type) {
	case EPhysicsEngine::Box2D:
		return new PhysicsEngineBox2D();
	case EPhysicsEngine::Rapier2D:
		return new PhysicsEngineRapier2D();
	default:
		Assert(false);
		return nullptr;
	}
}

void PhysicsEngine::destroy_engine(PhysicsEngine* engine) {
	delete engine;
}

#pragma once

#include "physics-body.h"

enum class EPhysicsEngine {
	Box2D,
	Rapier2D,
	Count
};

EPhysicsEngine const DEFAULT_PHYSICS_ENGINE = EPhysicsEngine::Box2D;

class PhysicsEngine {
public:
	virtual ~PhysicsEngine() = default; 
	static PhysicsEngine* create_engine(EPhysicsEngine engine_type);
	static void destroy_engine(PhysicsEngine* engine);

	virtual char const* const get_name() const = 0;

	virtual bool initialize(Vector2D const& gravity) = 0;
	virtual bool release() = 0;

	virtual void step(float delta) = 0;

	virtual PhysicsBody* create_body(EBodyType type) = 0;
	virtual void destroy_body(PhysicsBody* body) = 0;

	virtual void insert_body(PhysicsBody* body) = 0;
	virtual void remove_body(PhysicsBody* body) = 0;
};

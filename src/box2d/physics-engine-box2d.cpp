#include "physics-engine-box2d.h"

#include "physics-body-box2d.h"

#include "box2d-include.h"

#include "common/utils.h"

bool PhysicsEngineBox2D::initialize(Vector2D const& gravity) {
	Assert(m_b2World == nullptr);
	b2Vec2 b2Gravity(gravity.x, gravity.y);
	m_b2World = new b2World(b2Gravity);
	return true;
}

bool PhysicsEngineBox2D::release() {
	Assert(m_b2World != nullptr);
	delete m_b2World;
	return true;
}

void PhysicsEngineBox2D::step(float delta) {
	Assert(m_b2World != nullptr);
	int32 velocityIterations = 8;
	int32 positionIterations = 4;
	m_b2World->Step(delta, velocityIterations, positionIterations);
}

PhysicsBody* PhysicsEngineBox2D::create_body(EBodyType type) {
	return new PhysicsBodyBox2D(type);
}

void PhysicsEngineBox2D::destroy_body(PhysicsBody* body) {
	delete body;
}

void PhysicsEngineBox2D::insert_body(PhysicsBody* body) {
	Assert(m_b2World != nullptr);
	PhysicsBodyBox2D* b2Body = (PhysicsBodyBox2D*)body;
	b2Body->insert(m_b2World);
}

void PhysicsEngineBox2D::remove_body(PhysicsBody* body) {
	Assert(m_b2World != nullptr);
	PhysicsBodyBox2D* b2Body = (PhysicsBodyBox2D*)body;
	b2Body->remove(m_b2World);
}

#include "physics-engine-rapier2d.h"

#include "physics-body-rapier2d.h"
#include "rapier-include.h"

// Libs needed to link Rapier
#pragma comment(lib, "WS2_32")
#pragma comment(lib, "BCrypt")
#pragma comment(lib, "userenv")

bool PhysicsEngineRapier2D::initialize(Vector2D const& gravity) {
	m_Gravity = gravity;
	rapier_init();
	return true;
}

bool PhysicsEngineRapier2D::release() {
	// TODO
	return true;
}

void PhysicsEngineRapier2D::step(float delta) {
	SimulationSettings settings;
	settings.delta_time = delta;
	settings.gravity.x = m_Gravity.x;
	settings.gravity.y = m_Gravity.y;
	settings.max_velocity_iterations = 8;
	settings.max_velocity_friction_iterations = 8;
	settings.max_stabilization_iterations = 1;
	rapier_step(&settings);
}

PhysicsBody* PhysicsEngineRapier2D::create_body(EBodyType type) {
	return new PhysicsBodyRapier2D(type);
}

void PhysicsEngineRapier2D::destroy_body(PhysicsBody* body) {
	delete body;
}

void PhysicsEngineRapier2D::insert_body(PhysicsBody* body) {
	PhysicsBodyRapier2D* rapider_body = (PhysicsBodyRapier2D*)body;
	rapider_body->insert();
}

void PhysicsEngineRapier2D::remove_body(PhysicsBody* body) {
	PhysicsBodyRapier2D* rapider_body = (PhysicsBodyRapier2D*)body;
	rapider_body->remove();
}

#include "Scene.h"

Scene::Scene(PhysicsEngine* p_physics_engine) {
    Assert(p_physics_engine != nullptr);
    physics_engine = p_physics_engine;
}

Scene::~Scene() {
    for (PhysicsBody* body : bodies) {
		physics_engine->remove_body(body);
		physics_engine->destroy_body(body);
	}

	for (PhysicsShape* shape : shapes) {
		PhysicsShape::destroy_shape(shape);
	}

	for (PhysicsMaterial* material : materials) {
		PhysicsMaterial::destroy_material(material);
	}
}

void Scene::addGround() {
	// Setup Ground
    PhysicsBody* ground_body = physics_engine->create_body(EBodyType::Static);
	ground_body->set_position(Vector2D(100.0f, 0.2f));
	PhysicsShape* ground_shape = PhysicsShape::create_box(Vector2D(200.0f, 0.4f));
	shapes.push_back(ground_shape);
	ground_body->set_shape(ground_shape);
	bodies.push_back(ground_body);
}
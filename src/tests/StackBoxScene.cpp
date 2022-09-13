#include "StackBoxScene.h"

StackBoxScene::StackBoxScene(PhysicsEngine* physics_engine) : Scene(physics_engine) {
}

PhysicsBody* ref;

void StackBoxScene::Setup() {
    
    addGround();

	// Test Polygon
	PhysicsBody* polygon_body = physics_engine->create_body(EBodyType::Dynamic);
	polygon_body->set_position(Vector2D(18.0f, 20.0f));
	//polygon_body->set_rotation(DEG2RAD(45.0f));
	PhysicsMaterial* polygon_mat = PhysicsMaterial::create_default_material();
	materials.push_back(polygon_mat);
	polygon_mat->m_Restitution = 0.7f;
	polygon_body->set_material(polygon_mat);
	std::vector<Vector2D> triangleVertices = {Vector2D(0.6f, 0.6f), Vector2D(-0.6f, 0.6f), Vector2D(0, -0.6f)};
	PhysicsShape* polygon_shape = PhysicsShape::create_polygon(triangleVertices);
	shapes.push_back(polygon_shape);
	polygon_body->set_shape(polygon_shape);
	bodies.push_back(polygon_body);

	// Create a bouncing ball
	PhysicsBody* ball_body = physics_engine->create_body(EBodyType::Dynamic);
	ref = ball_body;
	ball_body->set_position(Vector2D(22.0f, 20.0f));
	PhysicsMaterial* ball_mat = PhysicsMaterial::create_default_material();
	materials.push_back(ball_mat);
	ball_mat->m_Restitution = 0.7f;
	ball_body->set_material(ball_mat);
	PhysicsShape* ball_shape = PhysicsShape::create_circle(1.0f);
	shapes.push_back(ball_shape);
	ball_body->set_shape(ball_shape);
	bodies.push_back(ball_body);
	
    // Create a pile of boxes
	PhysicsShape* box_shape = PhysicsShape::create_box(Vector2D(1.0f, 1.0f));
	shapes.push_back(box_shape);

	int const pileSize = 20;
	for (int i = 0; i < pileSize; ++i) {
		PhysicsBody* box_body = physics_engine->create_body(EBodyType::Dynamic);
		box_body->set_position(Vector2D(30.0f, 1.0f * (i + 1)));
		//box_body->set_rotation(DEG2RAD(45.0f));
		box_body->set_shape(box_shape);
		bodies.push_back(box_body);

		PhysicsBody* box_body2 = physics_engine->create_body(EBodyType::Dynamic);
		box_body2->set_position(Vector2D(10.0f, 1.0f * (i + 1)));
		//box_body2->set_rotation(DEG2RAD(45.0f));
		box_body2->set_shape(box_shape);
		bodies.push_back(box_body2);
	}
}

void StackBoxScene::Update(float delta) {
	// printf("Position %f , %f \n", ref->get_position().x, ref->get_position().y);
	// fflush(stdout);
}
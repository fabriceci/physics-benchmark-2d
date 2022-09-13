#include "PerfScene.h"
#define BENCHMARK_ADD_BOX_DELAY 0.35f

#include <chrono>

BoxPerfScene::BoxPerfScene(PhysicsEngine* physics_engine, PerfSceneShape p_shape1, PerfSceneShape p_shape2) : Scene(physics_engine) {
	shape1 = p_shape1;
	shape2 = p_shape2;
}

void BoxPerfScene::Setup() {
    addGround();
	if (shape1 == PerfSceneShape::Box || shape2 == PerfSceneShape::Box) {
		box_shape = PhysicsShape::create_box(Vector2D(.6f, .6f));
		shapes.push_back(box_shape);
	}
	if (shape1 == PerfSceneShape::Circle || shape2 == PerfSceneShape::Circle) {
		circle_shape = PhysicsShape::create_circle(.3f);
		shapes.push_back(circle_shape);
	}
	if (shape1 == PerfSceneShape::Polygon || shape2 == PerfSceneShape::Polygon) {
		std::vector<Vector2D> vertices = {
			Vector2D(0.0f, -0.3f),
			Vector2D(0.1f, -0.3f),
			Vector2D(0.3f, -0.1f),
			Vector2D(0.3f, 0.2f),
			Vector2D(0.2f, 0.3f),
			Vector2D(-0.2f, 0.3f),
			Vector2D(-0.3f, 0.2f),
			Vector2D(-0.4f, 0.2f),
			Vector2D(-0.3f, -0.3f),
			Vector2D(-0.1f, -0.3f),
		};
		polygon_shape = PhysicsShape::create_polygon(vertices);
		shapes.push_back(polygon_shape);
	}
}

PhysicsShape* BoxPerfScene::get_shape(PerfSceneShape p_shape) {
	switch (p_shape)
	{
	case PerfSceneShape::Box:
		return box_shape;
	case PerfSceneShape::Circle:
		return circle_shape;
	case PerfSceneShape::Polygon:
		return polygon_shape;
	default:
		Assert(false);
		return nullptr;
	}
}

float offset = -0.05;
void BoxPerfScene::Update(float delta) {
	total_delta += delta;
	frame_count += 1.0f;

	static auto begin = std::chrono::high_resolution_clock::now();

	if (pause_time > 0.0f) {
		pause_time -= delta;

		if (pause_time <= 0.0f) {
			auto end = std::chrono::high_resolution_clock::now();
			auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			float avg_fps = frame_count / ((float)time_ms * 0.001f);

			begin = end;
			frame_count = 0.0f;

			printf("  pause FPS: %.1f\n", avg_fps);
		}

		return;
	}

	if (total_delta >= BENCHMARK_ADD_BOX_DELAY) {
		offset = -offset;
		total_delta = 0;
		float step = (800 / 20) / 40.0f;
		
		for (int i = 0; i< 50; i++){
			bool even = (i % 2) == 0;
			++body_count;
			PhysicsBody* box_body = physics_engine->create_body(EBodyType::Dynamic);
			box_body->set_position(Vector2D(step * i + 0.6f + offset, 50));

			box_body->set_shape(get_shape(even ? shape1 : shape2));
			physics_engine->insert_body(box_body);
			bodies.push_back(box_body);
		}

		if (body_count % body_print_step == 0) {
			body_print_step = 1000;
			auto end = std::chrono::high_resolution_clock::now();
			auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			float avg_fps = frame_count / ((float)time_ms * 0.001f);

			begin = end;
			frame_count = 0.0f;

			printf("- Bodies: %u, avg FPS: %.1f\n", body_count, avg_fps);

			// Uncomment to test results without adding objects
			//pause_time = 1.0f;
		}
	}
}
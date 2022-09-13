#pragma once

#include "Scene.h"

enum class PerfSceneShape {
	Box,
	Circle,
    Polygon
};

class BoxPerfScene : public Scene {
    public:
        virtual char const* const get_name() const override { return "Performance Scene"; }
        PerfSceneShape shape1;
        PerfSceneShape shape2;
        BoxPerfScene(PhysicsEngine* physics_engine, PerfSceneShape p_shape1, PerfSceneShape p_shape2);
        virtual ~BoxPerfScene() = default; 

        virtual void Setup() override;
        virtual void Update(float delta) override;

private:
    PhysicsShape* get_shape(PerfSceneShape p_shape);
    PhysicsShape* box_shape = nullptr;
    PhysicsShape* circle_shape = nullptr;
    PhysicsShape* polygon_shape = nullptr;

    float total_delta = 0.0f;
    float pause_time = 0.0f;
    int body_count = 0;
    float frame_count = 0;
    int body_print_step = 500;
};
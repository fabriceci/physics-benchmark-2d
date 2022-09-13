#pragma once

#include "Scene.h"

class StackBoxScene : public Scene {
    public:
        virtual char const* const get_name() const override { return "Stack Box Scene"; };

        StackBoxScene(PhysicsEngine* physics_engine);
        virtual ~StackBoxScene() = default; 

        virtual void Setup() override;
        virtual void Update(float delta) override;
};
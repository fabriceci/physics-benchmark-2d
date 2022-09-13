#pragma once

#include "Graphics.h"
#include "common/Vector2D.h"
#include "common/Constant.h"
#include "common/physics-body.h"
#include "common/physics-engine.h"
#include "common/physics-material.h"
#include "common/physics-shape.h"
#include "common/utils.h"

#include <vector>

class Scene {
    public:
        std::vector<PhysicsBody*> bodies;
        std::vector<PhysicsShape*> shapes;
        std::vector<PhysicsMaterial*> materials;
        PhysicsEngine* physics_engine;

        Scene(PhysicsEngine* physics_engine);
        virtual ~Scene();
        virtual void Setup() = 0;
        virtual void Input() {};
        virtual void Update(float delta) {};
        virtual void Render() {
            for (auto body: bodies) {
                body->draw();
            }
        };
        void addGround();
        virtual char const* const get_name() const = 0;
};
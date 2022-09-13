#pragma once

#include "common/physics-engine.h"
#include "common/Vector2D.h"

class b2World;

class PhysicsEngineBox2D : public PhysicsEngine {
public:
	virtual char const* const get_name() const override { return "Box2D"; };

	virtual bool initialize(Vector2D const& gravity) override;
	virtual bool release() override;

	virtual void step(float delta) override;

	virtual PhysicsBody* create_body(EBodyType type) override;
	virtual void destroy_body(PhysicsBody* body) override;

	virtual void insert_body(PhysicsBody* body) override;
	virtual void remove_body(PhysicsBody* body) override;

private:
	b2World* m_b2World = nullptr;
};

#pragma once

#include "common/physics-engine.h"
#include "common/Vector2D.h"

class PhysicsEngineRapier2D : public PhysicsEngine {
public:
	virtual char const* const get_name() const override { return "Rapier2D"; };

	virtual bool initialize(Vector2D const& gravity) override;
	virtual bool release() override;

	virtual void step(float delta) override;

	virtual PhysicsBody* create_body(EBodyType type) override;
	virtual void destroy_body(PhysicsBody* body) override;

	virtual void insert_body(PhysicsBody* body) override;
	virtual void remove_body(PhysicsBody* body) override;

private:
	Vector2D m_Gravity;
};

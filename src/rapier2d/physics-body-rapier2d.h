#pragma once

#include "common/physics-body.h"
#include "rapier-include.h"
#include "common/Vector2D.h"

class PhysicsBodyRapier2D : public PhysicsBody {
public:
	PhysicsBodyRapier2D(EBodyType type);
	~PhysicsBodyRapier2D();

	virtual Vector2D get_position() const override;
	virtual void set_position(Vector2D const& pos) override;

	virtual float get_rotation() const override;
	virtual void set_rotation(float rot) override;

	void insert();
	void remove();

private:
	Handle m_BodyHandle;
	Handle m_ColliderHandle;
	Vector2D m_Position;
	float m_Rotation = 0.0;
};

#pragma once

#include "common/physics-body.h"
#include "common/Vector2D.h"

class b2Body;
class b2World;

class PhysicsBodyBox2D : public PhysicsBody {
public:
	PhysicsBodyBox2D(EBodyType type);
	~PhysicsBodyBox2D();

	virtual Vector2D get_position() const override;
	virtual void set_position(Vector2D const& pos) override;

	virtual float get_rotation() const override;
	virtual void set_rotation(float rot) override;

	void insert(b2World* b2World);
	void remove(b2World* b2World);

private:
	b2Body* m_b2Body = nullptr;
	Vector2D m_Position;
	float m_Rotation = 0.0;
};

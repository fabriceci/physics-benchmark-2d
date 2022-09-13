#pragma once

#include "math.h"
#include "common/Vector2D.h"

struct PhysicsMaterial;
class PhysicsShape;

enum class EBodyType {
	Static,
	Dynamic,
};

class PhysicsBody {
public:
	virtual ~PhysicsBody() = default; 
	PhysicsBody(EBodyType type) : m_Type(type) {}

	PhysicsMaterial* get_material() { return m_Material; }
	const PhysicsMaterial* get_material() const { return m_Material; }
	void set_material(PhysicsMaterial* material) { m_Material = material; }

	PhysicsShape* get_shape() { return m_Shape; }
	const PhysicsShape* get_shape() const { return m_Shape; }
	void set_shape(PhysicsShape* shape) { m_Shape = shape; }

	EBodyType get_type() const { return m_Type; }

	virtual Vector2D get_position() const = 0;
	virtual void set_position(Vector2D const& pos) = 0;

	virtual float get_rotation() const = 0;
	virtual void set_rotation(float rot) = 0;

	void draw();

protected:
	PhysicsMaterial* m_Material = nullptr;
	PhysicsShape* m_Shape = nullptr;
	EBodyType m_Type;
};

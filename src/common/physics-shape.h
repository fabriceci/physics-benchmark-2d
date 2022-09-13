#pragma once

#include "common/Vector2D.h"
#include "common/Color.h"
#include <vector>

enum class EShapeType {
	Circle,
	Box,
	Polygon
};

class PhysicsShape_Circle;
class PhysicsShape_Box;
class PhysicsShape_Polygon;

class PhysicsShape {
public:
	virtual ~PhysicsShape() = default; 
	static PhysicsShape_Circle* create_circle(float radius);
	static PhysicsShape_Box* create_box(Vector2D const& size);
	static PhysicsShape_Polygon* create_polygon(std::vector<Vector2D> const& vertices) ;

	static void destroy_shape(PhysicsShape* shape);

	virtual EShapeType get_type() const = 0;

	virtual void draw_wireframe(Vector2D const& pos, float rotation, Color const& color) {}
	virtual void draw_solid(Vector2D const& pos, float rotation,  Color const& color) {}
};

class PhysicsShape_Circle : public PhysicsShape {
public:
	virtual EShapeType get_type() const override { return EShapeType::Circle; }

	virtual void draw_wireframe(Vector2D const& position, float rotation, Color const& color) override;
	virtual void draw_solid(Vector2D const& position, float rotation, Color const& color) override;

	float m_Radius;
};

class PhysicsShape_Box : public PhysicsShape {
public:
	virtual EShapeType get_type() const override { return EShapeType::Box; }

	virtual void draw_wireframe(Vector2D const& position, float rotation, Color const& color) override;
	virtual void draw_solid(Vector2D const& position, float rotation, Color const& color) override;

	Vector2D m_Size;
};

class PhysicsShape_Polygon : public PhysicsShape {
public:
	virtual EShapeType get_type() const override { return EShapeType::Polygon; }

	virtual void draw_wireframe(Vector2D const& position, float rotation, Color const& color) override;
	virtual void draw_solid(Vector2D const& position, float rotation, Color const& color) override;

	std::vector<Vector2D> m_Vertices;
};

#include "physics-shape.h"

#include "Graphics.h"

PhysicsShape_Circle* PhysicsShape::create_circle(float radius) {
	PhysicsShape_Circle* shape = new PhysicsShape_Circle();
	shape->m_Radius = radius;
	return shape;
}

void PhysicsShape_Circle::draw_wireframe(Vector2D const& pos, float rotation, Color const& color) {
	Graphics::DrawCircle(pos.x, pos.y, m_Radius, rotation, color.to_uint32());
}

void PhysicsShape_Circle::draw_solid(Vector2D const& pos, float rotation, Color const& color) {
	Graphics::DrawFillCircle(pos.x, pos.y, m_Radius, color.to_uint32());
}

PhysicsShape_Box* PhysicsShape::create_box(Vector2D const& size) {
	PhysicsShape_Box* shape = new PhysicsShape_Box();
	shape->m_Size = size;
	return shape;
}

void PhysicsShape_Box::draw_wireframe(Vector2D const& pos, float rotation, Color const& color) {
	std::vector<Vector2D> localVertices;
	localVertices.reserve(4);
	localVertices.push_back(Vector2D(-m_Size.x / 2.0f, -m_Size.y / 2.0f));
    localVertices.push_back(Vector2D(+m_Size.x / 2.0f, -m_Size.y / 2.0f));
    localVertices.push_back(Vector2D(+m_Size.x / 2.0f, +m_Size.y / 2.0f));
    localVertices.push_back(Vector2D(-m_Size.x / 2.0f, +m_Size.y / 2.0f));

	std::vector<Vector2D> rotatedVertices;
	rotatedVertices.reserve(4);
	for (Vector2D v : localVertices) {
		float x =  v.x * (float)cos(rotation) - v.y * (float)sin(rotation);
		float y = v.x * (float)sin(rotation) + v.y * (float)cos(rotation);
		rotatedVertices.push_back(Vector2D(x, y));
	}
	Graphics::DrawPolygon(pos.x, pos.y, rotatedVertices, color.to_uint32());
	// Graphics::DrawRect(pos.x, pos.y, m_Size.x, m_Size.y, color.to_uint32());
}

void PhysicsShape_Box::draw_solid(Vector2D const& pos, float rotation, Color const& color) {
	Graphics::DrawFillRect(pos.x, pos.y, m_Size.x, m_Size.y, color.to_uint32());
}

PhysicsShape_Polygon* PhysicsShape::create_polygon(std::vector<Vector2D> const& vertices) {
	PhysicsShape_Polygon* shape = new PhysicsShape_Polygon();
	shape->m_Vertices = vertices;
	return shape;
}

void PhysicsShape_Polygon::draw_wireframe(Vector2D const& pos, float rotation, Color const& color) {
	std::vector<Vector2D> rotatedVertices;
	rotatedVertices.reserve(m_Vertices.size());
	for (Vector2D v : m_Vertices) {
		float x = v.x * (float)cos(rotation) - v.y * (float)sin(rotation);
		float y = v.x * (float)sin(rotation) + v.y * (float)cos(rotation);
		rotatedVertices.push_back(Vector2D(x, y));
	}
	
	Graphics::DrawPolygon(pos.x, pos.y, rotatedVertices, color.to_uint32());
	//Graphics::DrawRect(pos.x, pos.y, 0.5f, 0.5f, color.to_uint32());
}

void PhysicsShape_Polygon::draw_solid(Vector2D const& pos, float rotation, Color const& color) {
	//Graphics::DrawFillPolygon(pos.x, pos.y, m_Vertices, color.to_uint32());
}

void PhysicsShape::destroy_shape(PhysicsShape* shape) {
	delete shape;
}
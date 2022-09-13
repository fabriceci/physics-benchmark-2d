#include "physics-body-box2d.h"

#include "common/physics-material.h"
#include "common/physics-shape.h"

#include "box2d-include.h"

#include "common/utils.h"

#include <stdlib.h>

PhysicsBodyBox2D::PhysicsBodyBox2D(EBodyType type) : PhysicsBody(type) {
}

PhysicsBodyBox2D::~PhysicsBodyBox2D() {
	Assert(m_b2Body == nullptr);
}

Vector2D PhysicsBodyBox2D::get_position() const {
	if (m_b2Body != nullptr) {
		b2Vec2 const& b2Pos = m_b2Body->GetTransform().p;
		return Vector2D(b2Pos.x, b2Pos.y);
	} 
	return m_Position;
}

void PhysicsBodyBox2D::set_position(Vector2D const& pos) {
	m_Position = pos;
	if (m_b2Body != nullptr) {
		m_b2Body->SetTransform(b2Vec2(m_Position.x, m_Position.y), m_Rotation);
	}
}

float PhysicsBodyBox2D::get_rotation() const {
	if (m_b2Body != nullptr) {
		return m_b2Body->GetAngle();
	}
	return m_Rotation;
}

void PhysicsBodyBox2D::set_rotation(float rot) {
	m_Rotation = rot;
	if (m_b2Body != nullptr) {
		m_b2Body->SetTransform(b2Vec2(m_Position.x, m_Position.y), m_Rotation);
	}
}

void PhysicsBodyBox2D::insert(b2World* b2World) {
	Assert(m_b2Body == nullptr);

	b2BodyDef bodyDef;
	bodyDef.position.Set(m_Position.x, m_Position.y);
	bodyDef.angle = m_Rotation;

	switch (m_Type) {
	case EBodyType::Static:
		bodyDef.type = b2_staticBody;
		break;
	case EBodyType::Dynamic:
		bodyDef.type = b2_dynamicBody;
		break;
	default:
		Assert(false);
	}

	b2Shape* shape;

	Assert(m_Shape != nullptr);
	switch (m_Shape->get_type()) {
	case EShapeType::Circle: {
		const PhysicsShape_Circle* circleShape = (const PhysicsShape_Circle*)m_Shape;
		b2CircleShape* circle_shape = new b2CircleShape();
		circle_shape->m_radius = circleShape->m_Radius;
		shape = circle_shape;
		break;
	}
	case EShapeType::Box: {
		const PhysicsShape_Box* boxShape = (const PhysicsShape_Box*)m_Shape;
		b2PolygonShape* polygon_shape = new b2PolygonShape();
		polygon_shape->SetAsBox(boxShape->m_Size.x * 0.5f, boxShape->m_Size.y * 0.5f);
		shape = polygon_shape;
		break;
	}
	case EShapeType::Polygon: {
		const PhysicsShape_Polygon* polygon = (const PhysicsShape_Polygon*)m_Shape;
		int num_vertices = (int)polygon->m_Vertices.size();

		static_assert(sizeof(b2Vec2) == sizeof(Vector2D), "Can't use Vector2D in place of b2Vec2");

		b2PolygonShape* polygon_shape = new b2PolygonShape();
		polygon_shape->Set((b2Vec2*)polygon->m_Vertices.data(), num_vertices);

		shape = polygon_shape;
		break;
	}
	default:
		Assert(false);
		return;
	}

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1.0f;

	if (m_Material != nullptr) {
		fixtureDef.friction = m_Material->m_Friction;
		fixtureDef.restitution = m_Material->m_Restitution;
	} else {
		fixtureDef.friction = DEFAULT_FRICTION;
		fixtureDef.restitution = DEFAULT_RESTITUTION;
	}

	m_b2Body = b2World->CreateBody(&bodyDef);

	Assert(m_b2Body != nullptr);

	m_b2Body->CreateFixture(&fixtureDef);
}

void PhysicsBodyBox2D::remove(b2World* b2World) {
	Assert(m_b2Body != nullptr);
	b2World->DestroyBody(m_b2Body);

	m_b2Body = nullptr;
}

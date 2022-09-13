#include "physics-body-rapier2d.h"

#include "common/physics-material.h"
#include "common/physics-shape.h"

#include "common/utils.h"

PhysicsBodyRapier2D::PhysicsBodyRapier2D(EBodyType type) : PhysicsBody(type) {
	m_BodyHandle = invalid_handle();
	m_ColliderHandle = invalid_handle();
}

PhysicsBodyRapier2D::~PhysicsBodyRapier2D() {
	Assert(!is_handle_valid(m_BodyHandle));
	Assert(!is_handle_valid(m_ColliderHandle));
}

Vector2D PhysicsBodyRapier2D::get_position() const {
	if (is_handle_valid(m_BodyHandle)) {
		Vector pos = rapier_body_get_position(m_BodyHandle);
		return Vector2D(pos.x, pos.y);
	}
	return m_Position;
}

void PhysicsBodyRapier2D::set_position(Vector2D const& pos) {
	m_Position = pos;
	if (is_handle_valid(m_BodyHandle)) {
		rapier_body_set_transform(m_BodyHandle, make_vector(m_Position.x, m_Position.y), m_Rotation);
	}
}

float PhysicsBodyRapier2D::get_rotation() const {
	if (is_handle_valid(m_BodyHandle)) {
		return rapier_body_get_angle(m_BodyHandle);
	}
	return m_Rotation;
}

void PhysicsBodyRapier2D::set_rotation(float rot) {
	m_Rotation = rot;
	if (is_handle_valid(m_BodyHandle)) {
		rapier_body_set_transform(m_BodyHandle, make_vector(m_Position.x, m_Position.y), m_Rotation);
	}
}

void PhysicsBodyRapier2D::insert() {
	Assert(!is_handle_valid(m_BodyHandle));
	Assert(!is_handle_valid(m_ColliderHandle));

	switch (m_Type) {
	case EBodyType::Static:
		m_BodyHandle = rapier_body_create_fixed(make_vector(m_Position.x, m_Position.y), m_Rotation);
		break;
	case EBodyType::Dynamic:
		m_BodyHandle = rapier_body_create_dynamic(make_vector(m_Position.x, m_Position.y), m_Rotation);
		break;
	default:
		Assert(false);
	}

	Material mat = default_material();
	if (m_Material != nullptr) {
		mat.friction = m_Material->m_Friction;
		mat.restitution = m_Material->m_Restitution;
	} else {
		mat.friction = DEFAULT_FRICTION;
		mat.restitution = DEFAULT_RESTITUTION;
	}

	Assert(m_Shape != nullptr);
	switch (m_Shape->get_type()) {
	case EShapeType::Circle: {
		const PhysicsShape_Circle* circleShape = (const PhysicsShape_Circle*)m_Shape;
		m_ColliderHandle = rapier_collider_create_circle(circleShape->m_Radius, &mat, m_BodyHandle);
		break;
	}
	case EShapeType::Box: {
		const PhysicsShape_Box* boxShape = (const PhysicsShape_Box*)m_Shape;
		m_ColliderHandle = rapier_collider_create_box(make_vector(boxShape->m_Size.x, boxShape->m_Size.y), &mat, m_BodyHandle);
		break;
	}
	case EShapeType::Polygon: {
		const PhysicsShape_Polygon* polygonShape = (const PhysicsShape_Polygon*)m_Shape;

		static_assert(sizeof(Vector) == sizeof(Vector2D), "Can't use Vector2D in place of rapier Vector");

		m_ColliderHandle = rapier_collider_create_polygon((Vector*)polygonShape->m_Vertices.data(), polygonShape->m_Vertices.size(), &mat, m_BodyHandle);
		break;
	}
	default:
		Assert(false);
	}
}

void PhysicsBodyRapier2D::remove() {
	Assert(is_handle_valid(m_BodyHandle));
	Assert(is_handle_valid(m_ColliderHandle));

	// This call also destroys the collider
	rapier_body_destroy(m_BodyHandle);

	m_BodyHandle = invalid_handle();
	m_ColliderHandle = invalid_handle();
}

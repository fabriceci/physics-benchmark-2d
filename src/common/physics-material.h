#pragma once

#define DEFAULT_FRICTION 1.0f
#define DEFAULT_RESTITUTION 0.0f

struct PhysicsMaterial {
	static PhysicsMaterial* create_default_material();

	static void destroy_material(PhysicsMaterial* material);

	float m_Friction = DEFAULT_FRICTION;
	float m_Restitution = DEFAULT_RESTITUTION;
};

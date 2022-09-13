#include "physics-material.h"

PhysicsMaterial* PhysicsMaterial::create_default_material() {
	return new PhysicsMaterial();
}

void PhysicsMaterial::destroy_material(PhysicsMaterial* material) {
	delete material;
}

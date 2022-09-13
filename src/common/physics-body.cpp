#include "physics-body.h"
#include "physics-shape.h"

#include "common/utils.h"
#include "common/Color.h"

void PhysicsBody::draw() {
	if (m_Shape != nullptr) {
		Vector2D pos = get_position();
		float rotation = get_rotation();

		Color color;
		switch (m_Type) {
		case EBodyType::Static:
			color = Color::WHITE;
			break;
		case EBodyType::Dynamic:
			color = Color::BLUE;
			break;
		default:
			Assert(false);
		}

		Color fillColor(color);
		fillColor *= 0.5f;

		// m_Shape->draw_solid(pos, rotation, fillColor);

		m_Shape->draw_wireframe(pos, rotation, color);
	}
}

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>


struct Material {
  float friction;
  float restitution;
};

struct Vector {
  float x;
  float y;
};

struct SimulationSettings {
  float delta_time;
  size_t max_velocity_iterations;
  size_t max_velocity_friction_iterations;
  size_t max_stabilization_iterations;
  Vector gravity;
};

struct Handle {
  uint32_t id;
  uint32_t generation;
};


extern "C" {

Material default_material();

SimulationSettings default_simulation_settings();

Handle invalid_handle();

bool is_handle_valid(Handle handle);

Vector make_vector(float x, float y);

Handle rapier_body_create_dynamic(Vector pos, float rot);

Handle rapier_body_create_fixed(Vector pos, float rot);

void rapier_body_destroy(Handle body_handle);

float rapier_body_get_angle(Handle body_handle);

Vector rapier_body_get_position(Handle body_handle);

void rapier_body_set_transform(Handle body_handle, Vector pos, float rot);

Handle rapier_collider_create_box(Vector size, const Material *mat, Handle body_handle);

Handle rapier_collider_create_circle(float radius, const Material *mat, Handle body_handle);

Handle rapier_collider_create_polygon(const Vector *points,
                                      size_t point_count,
                                      const Material *mat,
                                      Handle body_handle);

void rapier_collider_destroy(Handle handle);

float rapier_collider_get_angle(Handle handle);

Vector rapier_collider_get_position(Handle handle);

void rapier_collider_set_transform(Handle handle, Vector pos, float rot);

bool rapier_init();

bool rapier_step(const SimulationSettings *settings);

} // extern "C"

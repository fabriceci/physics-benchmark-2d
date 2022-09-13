use rapier2d::prelude::*;

#[repr(C)]
pub struct Handle {
    id : u32,
    generation : u32,
}

impl Handle {
    pub fn is_valid(&self) -> bool {
        return (self.id != u32::MAX) && (self.generation != u32::MAX);
    }
}

#[no_mangle]
pub extern "C" fn invalid_handle() -> Handle {
    Handle {
        id : u32::MAX,
        generation : u32::MAX,
    }
}

#[no_mangle]
pub extern "C" fn is_handle_valid(handle : Handle) -> bool {
    return handle.is_valid();
}

#[repr(C)]
pub struct SimulationSettings {
    delta_time : f32,
    max_velocity_iterations : usize,
    max_velocity_friction_iterations : usize,
    max_stabilization_iterations : usize,
    gravity : Vector,
}

#[no_mangle]
pub extern "C" fn default_simulation_settings() -> SimulationSettings {
    SimulationSettings {
        delta_time : 1.0 / 60.0,
        max_velocity_iterations : 4,
        max_velocity_friction_iterations : 8,
        max_stabilization_iterations : 1,
        gravity : Vector { x : 0.0, y : -9.81 },
    }
}

#[repr(C)]
pub struct Material {
    friction : f32,
    restitution : f32,
}

#[no_mangle]
pub extern "C" fn default_material() -> Material {
    Material {
        friction : 1.0,
        restitution : 0.0,
    }
}

#[repr(C)]
pub struct Vector {
    x : f32,
    y : f32,
}

#[no_mangle]
pub extern "C" fn make_vector(x : f32, y : f32) -> Vector {
    Vector {
        x : x,
        y : y,
    }
}

fn point_array_to_vec(data : &Vector, data_count : usize) -> Vec::<Point::<Real>> {
    let mut vec = Vec::<Point::<Real>>::with_capacity(data_count);
    unsafe {
        let data_raw = std::slice::from_raw_parts(data, data_count);
        for point in data_raw {
            vec.push(Point::<Real> { coords : vector![point.x, point.y] });
        }
    }
    return vec;
}

fn collider_handle_to_handle(collider_handle : ColliderHandle) -> Handle {
    let raw_parts = collider_handle.into_raw_parts();
    return Handle {
        id : raw_parts.0,
        generation : raw_parts.1,
    }
}

fn handle_to_collider_handle(handle : Handle) -> ColliderHandle {
    return ColliderHandle::from_raw_parts(handle.id, handle.generation);
}

fn rigid_body_handle_to_handle(rigid_body_handle : RigidBodyHandle) -> Handle {
    let raw_parts = rigid_body_handle.into_raw_parts();
    return Handle {
        id : raw_parts.0,
        generation : raw_parts.1,
    }
}

fn handle_to_rigid_body_handle(handle : Handle) -> RigidBodyHandle {
    return RigidBodyHandle::from_raw_parts(handle.id, handle.generation);
}

struct PhysicsWorld {
    physics_pipeline : PhysicsPipeline,
    island_manager : IslandManager,
    broad_phase : BroadPhase,
    narrow_phase : NarrowPhase,
    impulse_joint_set : ImpulseJointSet,
    multibody_joint_set : MultibodyJointSet,
    ccd_solver : CCDSolver,
    
    collider_set : ColliderSet,
    rigid_body_set : RigidBodySet,
}

impl PhysicsWorld {
    fn new() -> PhysicsWorld {
        PhysicsWorld {
            physics_pipeline : PhysicsPipeline::new(),
	        island_manager : IslandManager::new(),
	        broad_phase : BroadPhase::new(),
	        narrow_phase : NarrowPhase::new(),
	        impulse_joint_set : ImpulseJointSet::new(),
	        multibody_joint_set : MultibodyJointSet::new(),
	        ccd_solver : CCDSolver::new(),
    
            rigid_body_set : RigidBodySet::new(),
            collider_set : ColliderSet::new(),
        }
    }
    
    fn init(&mut self) {
    }

    fn step(&mut self, settings : &SimulationSettings) {
        let mut integration_parameters = IntegrationParameters::default();
        integration_parameters.dt = settings.delta_time;
        integration_parameters.max_velocity_iterations = settings.max_velocity_iterations;
        integration_parameters.max_velocity_friction_iterations = settings.max_velocity_friction_iterations;
        integration_parameters.max_stabilization_iterations = settings.max_stabilization_iterations;

        let gravity = vector![settings.gravity.x, settings.gravity.y];

        let physics_hooks = ();
        let event_handler = ();

        self.physics_pipeline.step(
          &gravity,
          &integration_parameters,
          &mut self.island_manager,
          &mut self.broad_phase,
          &mut self.narrow_phase,
          &mut self.rigid_body_set,
          &mut self.collider_set,
          &mut self.impulse_joint_set,
          &mut self.multibody_joint_set,
          &mut self.ccd_solver,
          &physics_hooks,
          &event_handler,
        );
    }

    fn insert_collider(&mut self, collider : Collider, body_handle : Handle) -> Handle {
        if body_handle.is_valid()
        {
            let rigid_body_handle = handle_to_rigid_body_handle(body_handle);
            let collider_handle = self.collider_set.insert_with_parent(collider, rigid_body_handle, &mut self.rigid_body_set);
            return collider_handle_to_handle(collider_handle);
        }
        else
        {
            let collider_handle = self.collider_set.insert(collider);
            return collider_handle_to_handle(collider_handle);
        }
    }

    fn remove_collider(&mut self, handle : Handle) {
        let collider_handle = handle_to_collider_handle(handle);
        self.collider_set.remove(collider_handle
            , &mut self.island_manager
            , &mut self.rigid_body_set
            , false
        );
    }
    
    fn insert_rigid_body(&mut self, rigid_body : RigidBody) -> Handle {
        let body_handle = self.rigid_body_set.insert(rigid_body);
        return rigid_body_handle_to_handle(body_handle);
    }
    
    fn remove_rigid_body(&mut self, body_handle : Handle) {
        let rigid_body_handle = handle_to_rigid_body_handle(body_handle);
        self.rigid_body_set.remove(rigid_body_handle
            , &mut self.island_manager
            , &mut self.collider_set
            , &mut self.impulse_joint_set
            , &mut self.multibody_joint_set
            , true
        );
    }
}

#[macro_use]
extern crate lazy_static;
use std::sync::Mutex;

lazy_static! {
    static ref SINGLETON: Mutex<PhysicsWorld> = Mutex::new(PhysicsWorld::new());
}

#[no_mangle]
pub extern "C" fn rapier_init() -> bool {
    let mut physics_world = SINGLETON.lock().unwrap();
    physics_world.init();
    return true;
}

#[no_mangle]
pub extern "C" fn rapier_step(settings : &SimulationSettings) -> bool {
    let mut physics_world = SINGLETON.lock().unwrap();
    physics_world.step(settings);
    return true;
}

fn set_collider_properties_internal(collider : &mut Collider, mat : &Material) {
    collider.set_friction(mat.friction);
    collider.set_restitution(mat.restitution);
    collider.set_friction_combine_rule(CoefficientCombineRule::Multiply);
    collider.set_restitution_combine_rule(CoefficientCombineRule::Max);
}

#[no_mangle]
pub extern "C" fn rapier_collider_create_box(size : Vector, mat : &Material, body_handle : Handle) -> Handle {
    let mut collider = ColliderBuilder::cuboid(0.5 * size.x, 0.5 * size.y).build();
    set_collider_properties_internal(&mut collider, mat);
    let mut physics_world = SINGLETON.lock().unwrap();
    return physics_world.insert_collider(collider, body_handle);
}

#[no_mangle]
pub extern "C" fn rapier_collider_create_circle(radius : f32, mat : &Material, body_handle : Handle) -> Handle {
    let mut collider = ColliderBuilder::ball(radius).build();
    set_collider_properties_internal(&mut collider, mat);
    let mut physics_world = SINGLETON.lock().unwrap();
    return physics_world.insert_collider(collider, body_handle);
}

#[no_mangle]
pub extern "C" fn rapier_collider_create_polygon(points : &Vector, point_count : usize, mat : &Material, body_handle : Handle) -> Handle {
    let points_vec = point_array_to_vec(points, point_count);
    let collider_data = ColliderBuilder::convex_polyline(points_vec);
    assert!(collider_data.is_some());
    let mut collider = collider_data.unwrap().build();
    set_collider_properties_internal(&mut collider, mat);
    let mut physics_world = SINGLETON.lock().unwrap();
    return physics_world.insert_collider(collider, body_handle);
}

#[no_mangle]
pub extern "C" fn rapier_collider_destroy(handle : Handle) {
    let mut physics_world = SINGLETON.lock().unwrap();
    return physics_world.remove_collider(handle);
}

#[no_mangle]
pub extern "C" fn rapier_collider_get_position(handle : Handle) -> Vector {
    let physics_world = SINGLETON.lock().unwrap();
    let collider_handle = handle_to_collider_handle(handle);
    let collider = physics_world.collider_set.get(collider_handle);
    assert!(collider.is_some());
    let collider_vector = collider.unwrap().translation();
    return Vector { x : collider_vector.x, y : collider_vector.y };
}

#[no_mangle]
pub extern "C" fn rapier_collider_get_angle(handle : Handle) -> f32 {
    let physics_world = SINGLETON.lock().unwrap();
    let collider_handle = handle_to_collider_handle(handle);
    let collider = physics_world.collider_set.get(collider_handle);
    assert!(collider.is_some());
    return collider.unwrap().rotation().angle();
}

#[no_mangle]
pub extern "C" fn rapier_collider_set_transform(handle : Handle, pos : Vector, rot : f32) {
    let mut physics_world = SINGLETON.lock().unwrap();
    let collider_handle = handle_to_collider_handle(handle);
    let collider = physics_world.collider_set.get_mut(collider_handle);
    assert!(collider.is_some());
    collider.unwrap().set_position(Isometry::new(vector![pos.x, pos.y], rot));
}

fn set_rigid_body_properties_internal(rigid_body : &mut RigidBody, pos : Vector, rot : f32) {
    rigid_body.set_rotation(rot, false);
    rigid_body.set_translation(vector![pos.x, pos.y], false);
}

#[no_mangle]
pub extern "C" fn rapier_body_create_fixed(pos : Vector, rot : f32) -> Handle {
    let mut physics_world = SINGLETON.lock().unwrap();
    let mut rigid_body = RigidBodyBuilder::fixed().build();
    set_rigid_body_properties_internal(&mut rigid_body, pos, rot);
    let body_handle = physics_world.rigid_body_set.insert(rigid_body);
    return rigid_body_handle_to_handle(body_handle);
}

#[no_mangle]
pub extern "C" fn rapier_body_create_dynamic(pos : Vector, rot : f32) -> Handle {
    let mut physics_world = SINGLETON.lock().unwrap();
    let mut rigid_body = RigidBodyBuilder::dynamic().build();
    set_rigid_body_properties_internal(&mut rigid_body, pos, rot);
    return physics_world.insert_rigid_body(rigid_body);
}

#[no_mangle]
pub extern "C" fn rapier_body_destroy(body_handle : Handle) {
    let mut physics_world = SINGLETON.lock().unwrap();
    physics_world.remove_rigid_body(body_handle);
}

#[no_mangle]
pub extern "C" fn rapier_body_get_position(body_handle : Handle) -> Vector {
    let physics_world = SINGLETON.lock().unwrap();
    let rigid_body_handle = handle_to_rigid_body_handle(body_handle);
    let body = physics_world.rigid_body_set.get(rigid_body_handle);
    assert!(body.is_some());
    let body_vector = body.unwrap().translation();
    return Vector { x : body_vector.x, y : body_vector.y };
}

#[no_mangle]
pub extern "C" fn rapier_body_get_angle(body_handle : Handle) -> f32 {
    let physics_world = SINGLETON.lock().unwrap();
    let rigid_body_handle = handle_to_rigid_body_handle(body_handle);
    let body = physics_world.rigid_body_set.get(rigid_body_handle);
    assert!(body.is_some());
    return body.unwrap().rotation().angle();
}

#[no_mangle]
pub extern "C" fn rapier_body_set_transform(body_handle : Handle, pos : Vector, rot : f32) {
    let mut physics_world = SINGLETON.lock().unwrap();
    let rigid_body_handle = handle_to_rigid_body_handle(body_handle);
    let body = physics_world.rigid_body_set.get_mut(rigid_body_handle);
    assert!(body.is_some());
    body.unwrap().set_position(Isometry::new(vector![pos.x, pos.y], rot), false);
}

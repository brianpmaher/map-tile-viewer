#include "base.h"
#include "rl.h"

#include <stdlib.h>
#include <assert.h>

#define CAMERA_PAN_SPEED 0.01f
#define CAMERA_ZOOM_SPEED 1.7f
#define CAMERA_MIN_DISTANCE 1.0f
#define CAMERA_MAX_DISTANCE 20.0f

typedef struct Map_Camera {
	rl_Camera rl_camera;
	bool is_orbiting;
	bool is_panning;
} Map_Camera;

typedef struct State {
	float delta_time;
	rl_Vector2 window_size;
	Map_Camera camera;
	rl_Model tile_model;
} State;

func rl_Vector3 get_camera_direction(const Map_Camera* camera) {
	return rl_vector3_normalize(rl_vector3_subtract(camera->rl_camera.target, camera->rl_camera.position));
}

func void init_window(rl_Vector2* window_size) {
	*window_size = (rl_Vector2){1280, 720};
	rl_set_config_flags(rl_FLAG_WINDOW_RESIZABLE);
	rl_init_window((int)window_size->x, (int)window_size->y, "Map Tile Viewer");
}

func void init_camera(Map_Camera* camera) {
	rl_Vector3 target = {0.0f, 0.0f, 0.0f};
	rl_Vector3 direction = rl_vector3_normalize((rl_Vector3){0.0f, 1.0f, 1.0f});
	float half_distance = (CAMERA_MIN_DISTANCE + CAMERA_MAX_DISTANCE) / 2.0f;
	rl_Vector3 position = rl_vector3_add(direction, rl_vector3_scale(direction, half_distance));
	*camera = (Map_Camera){
		.rl_camera = {
			.position = position,
			.target = target,
			.up = {0.0f, 1.0f, 0.0f},
			.fovy = 45.0f,
			.projection = rl_CAMERA_PERSPECTIVE,
		},
		.is_orbiting = false,
		.is_panning = false,
	};
}

func void init_tile(rl_Model* tile_model) {
	Mesh mesh = rl_gen_mesh_plane(10.0f, 10.0f, 1, 1);
	*tile_model = rl_load_model_from_mesh(mesh);
	Texture texture = rl_load_texture("assets/tile_albedo.png");
	tile_model->materials[0].maps[rl_MATERIAL_MAP_DIFFUSE].texture = texture;
}

func State* init() {
	State* state = malloc(sizeof(State));
	assert(state != NULL);
	init_window(&state->window_size);
	init_camera(&state->camera);
	init_tile(&state->tile_model);
	return state;
}

func void shutdown(State* state) {
	rl_close_window();
	free(state);
}

func void update_delta_time(float* delta_time) {
	*delta_time = rl_get_frame_time();
}

func void update_window_size(rl_Vector2* window_size) {
	window_size->x = (float)rl_get_screen_width();
	window_size->y = (float)rl_get_screen_height();
}

func void update_camera_orbit(Map_Camera* camera) {
	if (rl_is_mouse_button_down(rl_MOUSE_BUTTON_RIGHT)) {
		rl_update_camera(&camera->rl_camera, rl_CAMERA_THIRD_PERSON);
		camera->is_orbiting = true;
	} else {
		camera->is_orbiting = false;
	}
}

func void update_camera_zoom(Map_Camera* camera) {
	rl_Vector3 position_to_target = rl_vector3_subtract(camera->rl_camera.target, camera->rl_camera.position);
	rl_Vector3 direction = rl_vector3_normalize(position_to_target);
	float distance = rl_vector3_length(position_to_target);
	float mouse_wheel_move = rl_get_mouse_wheel_move();
	float zoom_speed_by_distance = CAMERA_ZOOM_SPEED * rl_clamp(distance / CAMERA_MAX_DISTANCE, 0.1f, 1.0f);
	float zoom_movement = mouse_wheel_move * zoom_speed_by_distance;
	float new_distance = rl_clamp(distance - zoom_movement, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE);
	rl_Vector3 target_to_position = rl_vector3_negate(direction);
	camera->rl_camera.position = rl_vector3_add(camera->rl_camera.target, rl_vector3_scale(target_to_position, new_distance));
}

func void update_camera_pan_position(Map_Camera* camera) {
	rl_Vector2 mouse_delta = rl_get_mouse_delta();
	rl_Vector3 direction = get_camera_direction(camera);
	rl_Vector3 right = rl_vector3_cross_product(camera->rl_camera.up, direction);
	rl_Vector3 forward = rl_vector3_cross_product(camera->rl_camera.up, right);
	rl_Vector3 forward_movement = rl_vector3_scale(forward, -mouse_delta.y);
	rl_Vector3 right_movement = rl_vector3_scale(right, mouse_delta.x);
	rl_Vector3 movement = rl_vector3_add(right_movement, forward_movement);
	rl_Vector3 scaled_movement = rl_vector3_scale(movement, CAMERA_PAN_SPEED);
	camera->rl_camera.target = rl_vector3_add(camera->rl_camera.target, scaled_movement);
	camera->rl_camera.position = rl_vector3_add(camera->rl_camera.position, scaled_movement);
}

func void update_camera_pan(Map_Camera* camera) {
	if (rl_is_mouse_button_down(rl_MOUSE_BUTTON_LEFT)) {
		camera->is_panning = true;
		update_camera_pan_position(camera);
	} else {
		camera->is_panning = false;
	}
}

func void update_camera_cursor(const Map_Camera* camera) {
	if (camera->is_orbiting || camera->is_panning) {
		rl_hide_cursor();
	} else {
		rl_show_cursor();
	}
}

func void update_camera(Map_Camera* camera) {
	update_camera_orbit(camera);
	update_camera_zoom(camera);
	update_camera_pan(camera);
	update_camera_cursor(camera);
}

func void update(State* state) {
	update_delta_time(&state->delta_time);
	update_window_size(&state->window_size);
	update_camera(&state->camera);
}

func void render_tile(const rl_Model* tile_model) {
	rl_Vector3 position = {0.0f, 0.0f, 0.0f};
	float scale = 1.0f;
	rl_draw_model(*tile_model, position, scale, rl_RAYWHITE);
}

func void render_3d(const State* state) {
	rl_begin_mode3d(state->camera.rl_camera);
	render_tile(&state->tile_model);
	rl_end_mode3d();
}

func void render(const State* state) {
	rl_begin_drawing();
	rl_clear_background(rl_BLACK);
	render_3d(state);
	rl_end_drawing();
}

int main(void) {
	State* state = init();
	while (!rl_window_should_close()) {
		update(state);
		render(state);
	}
	shutdown(state);
	return EXIT_SUCCESS;
}

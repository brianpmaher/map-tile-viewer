#include "base.h"
#include "rl.h"

#include <stdlib.h>
#include <assert.h>

typedef struct State {
	rl_Vector2 window_size;
	rl_Camera camera;
	rl_Model tile_model;
} State;

func void init_window(rl_Vector2* window_size) {
	*window_size = (rl_Vector2){1280, 720};
	rl_set_config_flags(rl_FLAG_WINDOW_RESIZABLE);
	rl_init_window((int)window_size->x, (int)window_size->y, "Map Tile Viewer");
}

func void init_camera(rl_Camera* camera) {
	*camera = (rl_Camera){
		.position = {0.0f, 10.0f, 10.0f},
		.target = {0.0f, 0.0f, 0.0f},
		.up = {0.0f, 1.0f, 0.0f},
		.fovy = 45.0f,
		.projection = rl_CAMERA_PERSPECTIVE,
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

func void update_window_size(rl_Vector2* window_size) {
	window_size->x = (float)rl_get_screen_width();
	window_size->y = (float)rl_get_screen_height();
}

func void update_camera(rl_Camera* camera) {
	if (rl_is_mouse_button_down(rl_MOUSE_BUTTON_RIGHT)) {
		rl_hide_cursor();
		rl_update_camera(camera, rl_CAMERA_THIRD_PERSON);
	} else {
		rl_show_cursor();
	}
}

func void update(State* state) {
	update_window_size(&state->window_size);
	update_camera(&state->camera);
}

func void render_tile(const rl_Model* tile_model) {
	const rl_Vector3 position = {0.0f, 0.0f, 0.0f};
	const float scale = 1.0f;
	rl_draw_model(*tile_model, position, scale, rl_RAYWHITE);
}

func void render_3d(const State* state) {
	rl_begin_mode3d(state->camera);
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

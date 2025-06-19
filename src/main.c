#include "base.h"
#include "rl.h"

#include <stdlib.h>
#include <assert.h>

#define VECTOR3_UP (rl_Vector3){0.0f, 1.0f, 0.0f}

typedef struct State {
	rl_Vector2 window_size;
	rl_Camera camera;
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
		.up = VECTOR3_UP,
		.fovy = 45.0f,
		.projection = rl_CAMERA_PERSPECTIVE,
	};
}

func State* init() {
	State* state = (State*)malloc(sizeof(State));
	assert(state != NULL);
	init_window(&state->window_size);
	init_camera(&state->camera);
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

func void render_3d(const State* state) {
	rl_begin_mode3d(state->camera);
	rl_draw_grid(10, 1.0f);
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

#include "base.h"
#include "rl.h"

#include <stdlib.h>
#include <assert.h>

typedef struct State {
	rl_Vector2 window_size;
} State;

func State* init() {
	State* state = (State*)malloc(sizeof(State));
	assert(state != NULL);
	state->window_size = (rl_Vector2){1280, 720};
	rl_set_config_flags(rl_FLAG_WINDOW_RESIZABLE);
	rl_init_window((int)state->window_size.x, (int)state->window_size.y, "Raylib Window");
	return state;
}

func void shutdown(State* state) {
	rl_close_window();
	free(state);
}

func void update(State* state) {
	rl_Vector2* window_size = &state->window_size;
	window_size->x = (float)rl_get_screen_width();
	window_size->y = (float)rl_get_screen_height();
}

func void render_text(const State* state) {
	const char* text = "Hello, Raylib!";
	rl_Vector2 text_size;
	{
		text_size.y = 40;
		text_size.x = (float)rl_measure_text(text, (int)text_size.y);
	}
	rl_Vector2 text_position = rl_vector2_add(rl_vector2_scale(state->window_size, 0.5), rl_vector2_scale(text_size, -0.5));
	rl_draw_text(text, (int)text_position.x, (int)text_position.y, (int)text_size.y, rl_DARKGRAY);
}

func void render(const State* state) {
	rl_begin_drawing();
	rl_clear_background(rl_RAYWHITE);
	render_text(state);
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

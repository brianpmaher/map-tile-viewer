/**
 * @file rl.h
 * 
 * @brief Nitpicky wrapper for Raylib.
 * 
 * This header file provides a consistent interface for Raylib that prefixes all symbols with `rl_` and uses snake_case naming conventions.
 * I have only added the functions that I use in this template, but you can add more as needed.
 */

#include <raylib.h>
#include <raymath.h>

// Math
#define rl_Vector2 Vector2
#define rl_vector2_add Vector2Add
#define rl_vector2_scale Vector2Scale

// Window
#define rl_FLAG_WINDOW_RESIZABLE FLAG_WINDOW_RESIZABLE
#define rl_set_config_flags SetConfigFlags
#define rl_init_window InitWindow
#define rl_close_window CloseWindow
#define rl_window_should_close WindowShouldClose
#define rl_get_screen_width GetScreenWidth
#define rl_get_screen_height GetScreenHeight

// Drawing
#define rl_RAYWHITE RAYWHITE
#define rl_DARKGRAY DARKGRAY
#define rl_begin_drawing BeginDrawing
#define rl_end_drawing EndDrawing
#define rl_clear_background ClearBackground
#define rl_draw_text DrawText

// Text
#define rl_measure_text MeasureText

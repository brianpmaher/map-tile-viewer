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
#define rl_Vector3 Vector3

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
#define rl_BLACK BLACK
#define rl_begin_drawing BeginDrawing
#define rl_end_drawing EndDrawing
#define rl_begin_mode3d BeginMode3D
#define rl_end_mode3d EndMode3D
#define rl_clear_background ClearBackground
#define rl_draw_text DrawText
#define rl_draw_grid DrawGrid

// Camera
#define rl_CAMERA_PERSPECTIVE CAMERA_PERSPECTIVE
#define rl_CAMERA_ORBITAL CAMERA_ORBITAL
#define rl_CAMERA_THIRD_PERSON CAMERA_THIRD_PERSON
#define rl_Camera3D Camera3D
#define rl_Camera Camera
#define rl_update_camera UpdateCamera

// I/O
#define rl_MOUSE_BUTTON_LEFT MOUSE_BUTTON_LEFT
#define rl_MOUSE_BUTTON_RIGHT MOUSE_BUTTON_RIGHT
#define rl_is_mouse_button_down IsMouseButtonDown
#define rl_hide_cursor HideCursor
#define rl_show_cursor ShowCursor

// Text
#define rl_measure_text MeasureText

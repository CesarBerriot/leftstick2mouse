//
// Created by César B. on 1/30/2024.
//

#include "logic.h"
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include <unistd.h>
#include "ui/ui.h"

struct g_logic_struct g_logic;

struct PointF { double x, y; };

__forceinline void wait(clock_t ms);

void logic_initialize() { memset(&g_logic, 0, sizeof(struct g_logic_struct)); }

void logic_run()
{	register DWORD i;
	XINPUT_STATE state;
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	struct PointF subpixel_movement = { 0, 0 };
	// runs until the ui gets closed
	while(!g_ui.closed)
	{	for(i = 0; i < XUSER_MAX_COUNT; ++i)
		{	// if this is a valid controller
			if(XInputGetState(i, &state) != ERROR_SUCCESS)
				break;

			// dead zone
			if(sqrt(pow(state.Gamepad.sThumbLX, 2) + pow(state.Gamepad.sThumbLY, 2)) < (SHRT_MAX * g_logic.dead_zone / 100))
				break;

			// sensitivity calculations
			float sensibility_division_factor = (101 - g_logic.sensitivity) * 50;
			float f = sensibility_division_factor; // alias
			// gather movement input & apply previous calculations
			struct PointF movement = { state.Gamepad.sThumbLX / f, -state.Gamepad.sThumbLY / f };

			// ready yourself for some autistic bullshit
			// I won't even try to explain this shit
			input.mi.dx = movement.x;
			input.mi.dy = movement.y;
			subpixel_movement.x += movement.x - input.mi.dx;
			subpixel_movement.y += movement.y - input.mi.dy;
			if(abs(subpixel_movement.x) >= 1)
			{	int sub_x_int = subpixel_movement.x;
				input.mi.dx += sub_x_int;
				subpixel_movement.x -= sub_x_int;
			}
			if(abs(subpixel_movement.y) >= 1)
			{	int sub_y_int = subpixel_movement.y;
				input.mi.dy += sub_y_int;
				subpixel_movement.y -= sub_y_int;
			}

			// send mouse input
			SendInput(1, &input, sizeof(INPUT));
		}
		// refresh rate
		wait(205 - g_logic.refresh_rate);
	}
}

__forceinline void wait(clock_t ms)
{	clock_t target = clock() + ms;
	while(clock() < target);
}

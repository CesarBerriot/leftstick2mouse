//
// Created by César B. on 1/30/2024.
//

#pragma once

#include <stdbool.h>
#define UI_WINDOWS
#include <luigi/luigi.h>

extern struct g_ui_struct
{	UISlider * sensitivity_slider;
	UISlider * dead_zone_slider;
	UISlider * refresh_rate_slider;
	bool closed;
} g_ui;

void ui_initialize();

void ui_run();

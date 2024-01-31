//
// Created by César B. on 1/30/2024.
//

#pragma once

#include <stdint.h>

extern struct g_logic_struct
{	volatile float sensitivity; // 0.f to 100.f
	volatile uint8_t dead_zone; // 5 to 40
	volatile uint16_t refresh_rate; // 5 to 200

} g_logic;

void logic_initialize();

void logic_run();

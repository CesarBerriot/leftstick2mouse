#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <xinput.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "ui/ui.h"
#include "logic/logic.h"

volatile bool ui_initialized;
volatile bool logic_initialized;

void * ui_thread_proc(void*)
{	ui_initialize();
	ui_initialized = true;
	while(!logic_initialized);
	ui_run();
}

int main()
{	// init locks
	ui_initialized = logic_initialized = false;

	// run ui thread
	pthread_t ui_thread;
	pthread_create(&ui_thread, NULL, ui_thread_proc, NULL);

	// run logic
	logic_initialize();
	logic_initialized = true;
	while(!ui_initialized);
	logic_run();

	return 0;
}

//
// Created by César B. on 1/30/2024.
//

#define UI_IMPLEMENTATION

#include "ui.h"
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include "logic/logic.h"

char * splash_messages[10] =
{	"le c# c'est pour les gays unreal c juste mieux nik les graphs",
	"pyerre ntr",
	"oui je me suis fait chier a coder des messages random",
	"windows 11 c'est giga gay",
	"les manettes xbox en main c'est un peu comme la peste",
	"sami mange moi l'ognon",
	"terasse moi le terreau",
	"pete moi l'ananas",
	"aaaaaaaaaaa saivainiaaaaaa manawi chiwanaaaaaaaaaaaaa oumnaaa omnaiiiii",
	"zebi bobo ? zebi malade ! donne du sirop a zebi. un jeu hasbro"
};

struct g_ui_struct g_ui;

#define MAKE_UI_SLIDER(prefix, slider, slider_pos, suffix)													\
do                                                         													\
{	panel = UIPanelCreate(main_panel, UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL | UI_ELEMENT_H_FILL);	\
	UILabelCreate(panel, 0, prefix, -1);																	\
	g_ui.slider = UISliderCreate(panel, UI_ELEMENT_H_FILL);													\
	g_ui.slider->position = slider_pos;                                   									\
	g_ui.slider->e.messageUser = sensitivity_slider_message_cb;												\
	UILabelCreate(panel, 0, suffix, -1);																	\
} while(0)

int sensitivity_slider_message_cb(UIElement *element, UIMessage message, int di, void *dp);

void ui_initialize()
{	memset(&g_ui, 0, sizeof(struct g_ui_struct));
	UIInitialise();
	UIWindow * window = UIWindowCreate(NULL, 0, "leftstick2mouse", 700, 200);
	UIPanel * main_panel = UIPanelCreate(window, UI_PANEL_MEDIUM_SPACING | UI_PANEL_GRAY);

	// splash message
	struct timeval time;
	gettimeofday(&time, NULL);
	UILabelCreate(main_panel, 0, splash_messages[time.tv_sec % 10], -1);

	// sliders
	UIPanel * panel;
	MAKE_UI_SLIDER("sensitivity    1", sensitivity_slider, .6f, "100");
	MAKE_UI_SLIDER("dead zone      5%", dead_zone_slider, .2f, "40%");
	MAKE_UI_SLIDER("refresh rate   5tps", refresh_rate_slider, 1, "200tps");

	// todo fix - doesn't work fsr
	// note : after taking a look at it it seems like
	// the library's math about steps is broken in a way
	// that makes them ineffective
	g_ui.dead_zone_slider->steps = 40 - 5;
	g_ui.dead_zone_slider->steps = 200 - 5;

	// removing the window style flag that allows resizing
	LONG style = GetWindowLongA(window->hwnd, GWL_STYLE);
	style &= ~WS_SIZEBOX;
	SetWindowLongA(window->hwnd, GWL_STYLE, style);
	// due to windows being a pile of shit and bugs in this exact situation
	// a large bugged frame will be drawn around the window as soon as it gets moved
	// and only disappear when maximized, so here we're "unbugging" it ourselves
	ShowWindow(window->hwnd, SW_MAXIMIZE);
	ShowWindow(window->hwnd, SW_SHOWNORMAL);
}

void ui_run()
{	// refresh logic
	sensitivity_slider_message_cb(g_ui.sensitivity_slider, UI_MSG_MOUSE_DRAG, 0, NULL);
	sensitivity_slider_message_cb(g_ui.dead_zone_slider, UI_MSG_MOUSE_DRAG, 0, NULL);
	sensitivity_slider_message_cb(g_ui.refresh_rate_slider, UI_MSG_MOUSE_DRAG, 0, NULL);

	UIMessageLoop();

	g_ui.closed = true;
}

int sensitivity_slider_message_cb(UIElement *element, UIMessage message, int di, void *dp)
{
	if(message == UI_MSG_LEFT_DOWN || message == UI_MSG_MOUSE_DRAG || element->window->pressedButton == 1)
		if(element == g_ui.sensitivity_slider)
			g_logic.sensitivity = g_ui.sensitivity_slider->position * 100;
		else if(element == g_ui.dead_zone_slider)
			g_logic.dead_zone = 5 + roundf(g_ui.dead_zone_slider->position * 35);
		else
			g_logic.refresh_rate = 5 + roundf(g_ui.refresh_rate_slider->position * 195);
	return 0;
}

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <sstream>
#include <iostream>

int main(int argc, char **argv){

	std::cout << "wheat";

	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	//Change working directory to location of resources
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "res");
	al_change_directory(al_path_cstr(path, '/'));
	al_destroy_path(path);

	ALLEGRO_DISPLAY* display = al_create_display(800, 600);
	ALLEGRO_COLOR black = al_map_rgba(0, 0, 0, 255);
	ALLEGRO_COLOR red = al_map_rgba(255, 0, 0, 255);
	ALLEGRO_BITMAP* ink = al_load_bitmap("img/homeIcon.png");
	//ALLEGRO_BITMAP* pencil = al_load_bitmap("img/pencil.png");
	//ALLEGRO_FONT* myriad_pro = al_load_ttf_font("fnt/myriad_pro.otf", 22, 0);

	ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
	ALLEGRO_EVENT event;

	//Prepare to receive exit events
	al_register_event_source(eq, al_get_display_event_source(display));
	bool exit = false;

	//Prepare to receive mouse events
	al_install_mouse();
	al_register_event_source(eq, al_get_mouse_event_source());
	int mx = 0, my = 0;
	char* msg = "Whee!";

	//Prepare to receive joystick events
	al_install_joystick();
	al_register_event_source(eq, al_get_joystick_event_source());
	std::stringstream jstr;

	while (!exit){
		al_clear_to_color(black);
		al_draw_bitmap(ink, 0, 0, 0);
		//al_draw_scaled_bitmap(pencil, 0, 0, 3124, 2248, 100, 100, 400, 250, 0);
		//al_draw_text(myriad_pro, red, mx + 15, my, 0, msg); //TODO get zlib?
		//al_draw_text(myriad_pro, red, 0, 0, 0, jstr.str().c_str());
		al_flip_display();

		al_wait_for_event(eq, &event);
		if (event.type == ALLEGRO_EVENT_JOYSTICK_AXIS){
			jstr.clear();
			jstr.str(std::string());
			jstr << event.joystick.pos;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES ||
			event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY){
			mx = event.mouse.x;
			my = event.mouse.y;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			msg = "Boop!";
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
			msg = "Whee!";
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			exit = true;
		}
	}

	al_destroy_event_queue(eq);
	al_destroy_bitmap(ink);
	//al_destroy_bitmap(pencil);
	//al_destroy_font(myriad_pro);
	al_destroy_display(display);
	return 0;
}

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main(int argc, char **argv){
	
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
	ALLEGRO_BITMAP* ink = al_load_bitmap("img/ink.png");
	ALLEGRO_BITMAP* pencil = al_load_bitmap("img/pencil.png");
	ALLEGRO_FONT* verdana = al_load_ttf_font("fnt/verdana.ttf", 12, 0);
	
	//Prepare to receive exit event
	ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
	al_register_event_source(eq, al_get_display_event_source(display));
	ALLEGRO_EVENT event;
	bool exit = false;

	while (!exit){
		al_clear_to_color(black);
		al_draw_bitmap(ink, 0, 0, 0);
		al_draw_scaled_bitmap(pencil, 0, 0, 3124, 2248, 100, 100, 400, 250, 0);
		al_draw_text(verdana, red, 200, 20, 0, "Hello, World!");
		al_flip_display();
		al_wait_for_event(eq, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			exit = true;
		}
	}
	
	al_destroy_event_queue(eq);
	al_destroy_bitmap(ink);
	al_destroy_bitmap(pencil);
	al_destroy_font(verdana);
	al_destroy_display(display);
	return 0;
}
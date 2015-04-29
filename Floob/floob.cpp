#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

struct Point{
	float x, y;
};

void genAddProb(int&, int&, int&, std::stringstream&, float&, float&, int, int, ALLEGRO_FONT*);
void genAddAns(int[], const int, int);

void genSubProb(int&, int&, int&, std::stringstream&, float&, float&, int, int, ALLEGRO_FONT*);
void genSubAns(int[], const int, int);

int main(int argc, char** argv){
	
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	
	//Switch working directory to location of res
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "res");
	al_change_directory(al_path_cstr(path, '/'));
	al_destroy_path(path);

	// Enable Antialiasing (1 buffer, 4x MSAA)
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);

	const int SCREEN_W = 320, SCREEN_H = 480, DIM = 3;
	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
	
	ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
	ALLEGRO_EVENT event;
	al_register_event_source(eq, al_get_display_event_source(display));
	bool exit = false;
	al_install_mouse();
	al_register_event_source(eq, al_get_mouse_event_source());
	//TODO: use below if system supports touch
	/*al_install_touch_input();
	//al_set_mouse_emulation_mode(ALLEGRO_MOUSE_EMULATION_5_0_x); //compatible with 5.0
	al_set_mouse_emulation_mode(ALLEGRO_MOUSE_EMULATION_TRANSPARENT);
	al_register_event_source(eq, al_get_touch_input_mouse_emulation_event_source());
	*/

	const float UPDATE_TIME = 0.005f;
	double last_update = al_get_time(), elapsed_time;
	
	//TODO: make const
	ALLEGRO_FONT* myriad = al_load_ttf_font("fnt/myriad_pro.OTF", 32, 0);
	ALLEGRO_FONT* myriad_bold = al_load_ttf_font("fnt/myriad_pro_bold.OTF", 70, 0);
	
	//TODO: make const
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR cream = al_map_rgb(249, 243, 221);
	ALLEGRO_COLOR red = al_map_rgb(237, 28, 36);
	ALLEGRO_COLOR orange = al_map_rgb(255, 127, 39);
	ALLEGRO_COLOR yellow = al_map_rgb(255, 201, 14);
	ALLEGRO_COLOR pink = al_map_rgb(255, 174, 201);
	ALLEGRO_COLOR purple = al_map_rgb(163, 73, 164);
	ALLEGRO_COLOR indigo = al_map_rgb(63, 72, 204);
	ALLEGRO_COLOR lime = al_map_rgb(181, 230, 29);
	ALLEGRO_COLOR green = al_map_rgb(34, 177, 76);
	ALLEGRO_COLOR blue = al_map_rgb(0, 162, 232);
	
	float progress = 0; //0-1
	const float BAR_H = 5;
	ALLEGRO_COLOR bar_color = green;
	const float DEPLETE_RATE = 0.0006;
	
	int level = 0;
	const int MAX_LEVEL = 1;
	
	float r = 30; //circle radius
	Point circle[DIM * DIM]; //circle locations
	ALLEGRO_COLOR color[DIM * DIM] = {red, orange, yellow, pink, purple, indigo, lime, green, blue}; //circle colors

	//Init circle locations
	float buf = (SCREEN_W - 2 * r * DIM) / (DIM + 1);
	float cx, cy = SCREEN_H - DIM * (2 * r + buf) + r;
	for (int y = 0; y < DIM; y++){
		cx = buf + r;
		for (int x = 0; x < DIM; x++){
			circle[y * DIM + x].x = cx;
			circle[y * DIM + x].y = cy;
			cx += 2 * r + buf;
		}
		cy += 2 * r + buf;
	}

	bool error = false, winner = false;
	const char* ERROR_MSG = "ERROR";
	const char* WINNER_MSG = "WINNER";
	float ex, ey, wx, wy; //message locations
	ey = wy = (circle[0].y - r) / 2 - al_get_font_ascent(myriad_bold) / 2;
	ex = SCREEN_W / 2 - al_get_text_width(myriad_bold, ERROR_MSG) / 2;
	wx = SCREEN_W / 2 - al_get_text_width(myriad_bold, WINNER_MSG) / 2;
	
	float px, py; //problem location
	std::stringstream problem;
	const int NUM_ANS = DIM * DIM;
	int ans[NUM_ANS];
	float ax, ay; //answer location
	std::stringstream ansstr;

	int a, b, c;
	srand(time(0));
	genAddProb(a, b, c, problem, px, py, SCREEN_W, circle[0].y - r, myriad_bold);
	genAddAns(ans, NUM_ANS, c);

	int mx, my, mdist; //mouse location
	while (!exit){
		al_clear_to_color(cream);
		al_draw_filled_rectangle(0, 0, SCREEN_W * progress, BAR_H, bar_color);
		for (int i = 0; i < NUM_ANS; i++){
			al_draw_filled_circle(circle[i].x, circle[i].y, r, color[i]);
		}
		if (error) al_draw_text(myriad_bold, red, ex, ey, 0, ERROR_MSG);
		else if (winner) al_draw_text(myriad_bold, green, wx, wy, 0, WINNER_MSG);
		else{
			al_draw_text(myriad_bold, black, px, py, 0, problem.str().c_str());
			for (int i = 0; i < NUM_ANS; i++){
				ansstr.str("");
				ansstr << ans[i];
				ay = circle[i].y - al_get_font_ascent(myriad) / 2;
				ax = circle[i].x - al_get_text_width(myriad, ansstr.str().c_str()) / 2;
				al_draw_text(myriad, white, ax, ay, 0, ansstr.str().c_str());
			}
		}
		al_flip_display();
		if (al_wait_for_event_timed(eq, &event, UPDATE_TIME)){
			//TODO: use below if system supports touch
			/*if (event.type == ALLEGRO_EVENT_TOUCH_BEGIN){
				int x = event.touch.x, y = event.touch.y;
				//guts
			}*/
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				if (!(error || winner)){
					mx = event.mouse.x;
					my = event.mouse.y;
					for (int i = 0; i < NUM_ANS; i++){
						mdist = std::sqrt(std::pow(circle[i].x - mx, 2) + std::pow(circle[i].y - my, 2));
						if (mdist <= r){
							if (ans[i] == c){
								progress += 0.25;
								if (progress >= 1){
									if (level == MAX_LEVEL) winner = true;
									else{
										++level;
										progress = 0;
									}
								}
								switch (level){
								case 0:
									genAddProb(a, b, c, problem, px, py, SCREEN_W, circle[0].y - r, myriad_bold);
									genAddAns(ans, NUM_ANS, c);
									break;
								case 1:
									genSubProb(a, b, c, problem, px, py, SCREEN_W, circle[0].y - r, myriad_bold);
									genSubAns(ans, NUM_ANS, c);
									break;
								}
								
							}
							else{
								error = true;
								bar_color = red;
							}
						}
					}
				}
				else{ //TODO: click on problem?
					bar_color = green;
					progress = 0;
					winner = error = false;
					level = 0;
					genAddProb(a, b, c, problem, px, py, SCREEN_W, circle[0].y - r, myriad_bold);
					genAddAns(ans, NUM_ANS, c);
				}
			}
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
				exit = true;
			}
		}
		if (!(error || winner) && progress > 0) progress -= DEPLETE_RATE;
		
		elapsed_time = al_get_time() - last_update;
		if (elapsed_time < UPDATE_TIME) al_rest(UPDATE_TIME - elapsed_time);
		last_update = al_get_time();
	}

	al_destroy_event_queue(eq);
	al_destroy_font(myriad);
	al_destroy_font(myriad_bold);
	al_destroy_display(display);
	return 0;
}

void genAddProb(int& a, int& b, int& c, std::stringstream& problem, float& px, float& py, int width, int height, ALLEGRO_FONT* font){
	a = rand()%10;
	b = rand()%10;
	c = a + b;
	problem.str("");
	problem << a << " + " << b;
	px = width / 2 - al_get_text_width(font, problem.str().c_str()) / 2;
	py = height / 2 - al_get_font_ascent(font) / 2;
}

void genAddAns(int ans[], const int size, int correct_ans){
	for (int i = 0; i < size; i++){
		ans[i] = correct_ans - size / 2 + i;
	}
	std::random_shuffle(&ans[0], &ans[size-1]);
}

void genSubProb(int& a, int& b, int& c, std::stringstream& problem, float& px, float& py, int width, int height, ALLEGRO_FONT* font){
	a = rand()%10;
	b = rand()%10;
	c = a - b;
	problem.str("");
	problem << a << " - " << b;
	px = width / 2 - al_get_text_width(font, problem.str().c_str()) / 2;
	py = height / 2 - al_get_font_ascent(font) / 2;
}

void genSubAns(int ans[], const int size, int correct_ans){ //No difference between genAddAns
	for (int i = 0; i < size; i++){
		ans[i] = correct_ans - size / 2 + i;
	}
	std::random_shuffle(&ans[0], &ans[size-1]);
}

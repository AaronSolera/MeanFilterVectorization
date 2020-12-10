#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "../include/serial_mean_filter.h"

const float FPS = 60;

//define bitmaps
ALLEGRO_BITMAP *image;
int ptr_height, ptr_width;
unsigned char *** input_img, *** output_img;

void filter_analysis(char * input_path, char * output_path, char * csv_path){
	//Read the bitmap from the image .png
	image = al_load_bitmap(input_path);

	ptr_width  = al_get_bitmap_width(image);
	ptr_height = al_get_bitmap_height(image);

	printf("Generating pixels matrix from image\n");
	input_img  = createMatrix(image);
	printf("\nAllocating memory for output image");
	output_img = allocateMemorySpaceForImage(&ptr_height, &ptr_width);
	printf("\nExecuting mean filter\n");

	clock_t begin, end;
	float   serial_time, parallel_time;

	FILE *fptr;
	fptr = fopen(csv_path,"w");
	fprintf(fptr,"width,serial_time,parallel_time");

	if(fptr == NULL)
	{
	  printf("Error!");   
	  exit(1);             
	}

	for (int temp_width = 1; temp_width <= ptr_width; ++temp_width)
	{
		begin = clock();
		medianFilter(&ptr_height, &temp_width, input_img, output_img);
		end = clock();

		serial_time = (float)(end - begin) / CLOCKS_PER_SEC;

		begin = clock();
		// medianFilterNeon(&ptr_height, &temp_width, input_img, output_img);
		end = clock();

		parallel_time = (float)(end - begin) / CLOCKS_PER_SEC;

		fprintf(fptr, "\n%i,%f,%f", temp_width, serial_time, parallel_time);
	}
	printf("\nGenerating .bmp filtered output image");
	generateBitmapImage(ptr_height, ptr_width, output_img, output_path);
	printf("\nDeallocating image memory\n");
	deallocateMemorySpaceForImage(ptr_height, ptr_width, input_img);
	deallocateMemorySpaceForImage(ptr_height, ptr_width, output_img);

	al_destroy_bitmap(image);
	fclose(fptr);
}


int main(int argc, void *argv[]) {
	/*
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_MOUSE_STATE state;

	bool running = true;
	bool redraw = true;
	*/
	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}
	//init the mouse driver
	//al_install_mouse();

	//Init image reader
	al_init_image_addon();
	al_init_primitives_addon();

	filter_analysis("images/butterfly.png", "images/butterfly.bmp", "butterfly_execution_time.csv");

	/*
	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	display = al_create_display(ptr_width, ptr_height);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}
	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}
	//configure the window
	al_set_window_title(display,"Mean filter");

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	
	// Start the timer
	al_start_timer(timer);

	// Game loop *********************************************************************************************
	while (running) { 
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;
		ALLEGRO_BITMAP *bitmap;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event) {
			switch (event.type) {
				case ALLEGRO_EVENT_TIMER:
					redraw = true;
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				default:
					fprintf(stderr, "Unsupported event received: %d\n", event.type);
					break;
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(150, 120, 170));


			al_draw_bitmap(image, 0, 0, 0);

			
			al_flip_display();
			redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	*/

	return 0;
}
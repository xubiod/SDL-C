#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "palettes.h"
#include "project.h"

const double EMU_SCREENW = 192;
const double EMU_SCREENH = 128;

int frame = 0;
int quit = 0;

SDL_Renderer *ren;

int execute(void* data);

int main() {
	// VARIABLE INITIALIZATION
	int color = 0;
	int mouse_x, mouse_y;
	int inClick = 0;

	double multi = 0;

	SDL_Event e;

	// CODE STARTS
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return 1;
	}
	if (TTF_Init() == -1) {
		return 1;
	}
	printf("SDL + SDL_tff initialized\n");

	SDL_Window *win = SDL_CreateWindow("EmulatedScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 4 * EMU_SCREENW, 4 * EMU_SCREENH, 0);
	if (win == NULL) {
		SDL_Quit();
		return 3;
	}
	printf("Console window created\n");

	SDL_Window *control = SDL_CreateWindow("Controller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2 * EMU_SCREENW, EMU_SCREENH/2, 0);
	if (control == NULL) {
		SDL_Quit();
		return 3;
	}
	printf("Controller window created\n");

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL) {
		SDL_Quit();
		return 4;
	}
	printf("Console renderer created\n");

	SDL_Renderer *controlren = SDL_CreateRenderer(control, -1, SDL_RENDERER_ACCELERATED);
	if (controlren == NULL) {
		SDL_Quit();
		return 4;
	}
	printf("Controller renderer created\n");

	SDL_Thread *consoleThread = SDL_CreateThread(execute, "ConsoleThread", (void *)NULL);

	atexit(SDL_Quit);

	/*
	for (int i = 0; i < 600; i++) {
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 255,255,255, 255);
		//SDL_RenderDrawLine(ren, rand() % 192, rand() % 128, rand() % 192, rand() % 128);
		SDL_Rect rect;

		rect.x = rand() % 192;
		rect.y = rand() % 128;
		rect.w = rand() % 192;
		rect.h = rand() % 128;

		SDL_RenderDrawRect(ren, &rect);

		//SDL_RenderDrawRect(ren, );
		
		printf("Rect %d\n", i);

		SDL_RenderPresent(ren);
	}*/

	SDL_RenderSetScale(ren, 4.0, 4.0);

	while (quit == 0) {
		SDL_GetMouseState(&mouse_x, &mouse_y);
		inClick = 0;

		//printf("mx:%d", mouse_x);
		//printf(", my:%d", mouse_y);

		// EVENTS
		while (SDL_PollEvent(&e)) {
			// on clicking of "x" on top right
			if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE)) {
				quit = 1;
			}

			if ((e.type == SDL_MOUSEBUTTONDOWN) && (e.window.windowID == SDL_GetWindowID(control))) {
				//consolePalette = (consolePalette + 1) % palettesCount;
				inClick = 1;
			}
		}

		//consoleThread = SDL_CreateThread(execute, "ConsoleThread", (void *)NULL);

		// CONTROLLER
		SDL_Surface *_palSurf = SDL_GetWindowSurface(control);
		
		SDL_Color *fillColor = 0x0;
		SDL_FillRect(_palSurf, NULL, fillColor);

		for (int color = 0; color < palettesCount; color++) {
			SDL_Rect contPalette;

			// i is rendered palette
			// point_distance(window_mouse_get_x(),0,scr_w * (i / array_height_2d(obj_code.colors)),0)/scr_w * -15 + 1.5;
			//double multi = 0.5 * abs((mouse_x - (double)color) / EMU_SCREENW);

			int con_w = EMU_SCREENW * 2;

			if (SDL_GetMouseFocus() == control) {
				multi = ((abs((double)mouse_x - (float)con_w * ((float)color / (float)palettesCount))) / (float)con_w) * ((EMU_SCREENH * -0.2)/(double)mouse_y) + 0.75;
			} else {
				multi = 0;
			}
			
			if (color == consolePalette) multi = 1;

			for (int cc = 0; cc < 4; cc++) {
				contPalette.x = round(EMU_SCREENW / (palettesCount) * color * 2);
				contPalette.y = round((EMU_SCREENH / 8) * cc * multi);

				contPalette.w = round(EMU_SCREENW / palettesCount * 2);
				contPalette.h = round(EMU_SCREENH / 8 * multi);

				if (inClick == 1) {
					//printf("%d\n", round(multi * 100) / 100);
					if (mouse_x > contPalette.x && mouse_x < (contPalette.x + contPalette.w)) {
						consolePalette = color;
					}
				}

				//SDL_SetRenderDrawColor(controlren, getRed(palettes[color][cc]), getGreen(palettes[color][cc]), getBlue(palettes[color][cc]), 255);
				//SDL_RenderDrawLine(controlren, contPalette.x, contPalette.y, contPalette.x + contPalette.w, contPalette.y + contPalette.h);
				//SDL_RenderDrawRect(controlren, &contPalette);

				SDL_Color *fillColor = palettes[color][cc];
				SDL_FillRect(_palSurf, &contPalette, fillColor);
			}
		}

		SDL_Texture *_palText = SDL_CreateTextureFromSurface(controlren, _palSurf);
		SDL_FreeSurface(_palSurf);

		//SDL_SetRenderDrawColor(controlren, 0, 0, 0, 255);
		//SDL_RenderClear(controlren);

		SDL_RenderCopy(controlren, _palText, NULL, NULL);
		SDL_DestroyTexture(_palText);

		SDL_UpdateWindowSurface(control);
		SDL_RenderPresent(controlren);

		//frame++;
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	TTF_Quit();
	SDL_Quit();
	return 0;
}

int execute(void* data) {
	while (quit == 0) {
		for (int x = 0; x < EMU_SCREENW; x++) {
			for (int y = 0; y < EMU_SCREENH; y++) {
				if ((x + y + frame) % 2 == 0) {
					int colorInPalette = ((x + y + frame) / 16) % 4;
					SDL_SetRenderDrawColor(ren, getRed(palettes[consolePalette][colorInPalette]), getGreen(palettes[consolePalette][colorInPalette]), getBlue(palettes[consolePalette][colorInPalette]), 255);

					SDL_RenderDrawPoint(ren, x, y);
				}
			}
		}

		//RENDER ITs
		SDL_RenderPresent(ren);
		frame++;
	}

	return 0;
}
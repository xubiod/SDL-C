#include <SDL.h>
#include "palettes.h"

//definitions
void projectDrawNoise(SDL_Renderer *ren, int color, int EMU_SCREENW, int EMU_SCREENH);
void lineTest(SDL_Renderer *ren, int color, int EMU_SCREENW, int EMU_SCREENH);

//void* data

/*
void execute(SDL_Renderer *ren, int color, int frame, int EMU_SCREENW, int EMU_SCREENH, int quit) {
	while (quit == 1) {
		for (int x = 0; x < EMU_SCREENW; x++) {
			for (int y = 0; y < EMU_SCREENH; y++) {
				int colorInPalette = ((x + y + frame) / 16) % 4;
				SDL_SetRenderDrawColor(ren, getRed(palettes[consolePalette][colorInPalette]), getGreen(palettes[consolePalette][colorInPalette]), getBlue(palettes[consolePalette][colorInPalette]), 255);

				SDL_RenderDrawPoint(ren, x, y);
			}
		}

		//RENDER ITs
		SDL_RenderPresent(ren);
	}
}
*/
void lineTest(SDL_Renderer *ren, int color, int EMU_SCREENW, int EMU_SCREENH) {
	SDL_SetRenderDrawColor(ren, getRed(palettes[consolePalette][color]), getGreen(palettes[consolePalette][color]), getBlue(palettes[consolePalette][color]), 255);
	SDL_RenderClear(ren);

	double q = 1;

	while (q < EMU_SCREENH) {
		SDL_SetRenderDrawColor(ren, getRed(palettes[consolePalette][3 - color]), getGreen(palettes[consolePalette][3 - color]), getBlue(palettes[consolePalette][3 - color]), 255);
		SDL_RenderDrawLine(ren, 0, q, 192, q);

		q = q * 1.5;
	}
}

void projectDrawNoise(SDL_Renderer *ren, int color, int EMU_SCREENW, int EMU_SCREENH) {
	for (int x = 0; x < EMU_SCREENW; x++) {
		for (int y = 0; y < EMU_SCREENH; y++) {
			int colorInPalette = rand() % 4;
			SDL_SetRenderDrawColor(ren, getRed(palettes[consolePalette][colorInPalette]), getGreen(palettes[consolePalette][colorInPalette]), getBlue(palettes[consolePalette][colorInPalette]),255);

			SDL_RenderDrawPoint(ren, x, y);
		}
	}
}
#include <SDL.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread> 
#include "Spring.h"

#include "Vector2D.h"
#include "Particle.h"

SDL_Window* window;
SDL_Surface* surface;
SDL_Surface* pixelSpace;
SDL_Renderer* renderer;
SDL_Texture* texture;
unsigned int* pixels;

const int width = 640;
const int height = 480;

bool quit = false;

int initSDL() {
	window = NULL;
	surface = NULL;
	renderer = NULL;
	texture = NULL;
	pixelSpace = NULL;

	window = SDL_CreateWindow("Powder Toy Recreation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);
	pixelSpace = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	pixels = (unsigned int*)pixelSpace->pixels;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	return 0;
}

void cleanSDL() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void updateSDL() {
	SDL_UpdateTexture(texture, NULL, pixels, pixelSpace->pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void drawBackground() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pixels[y * width + x] = int(0 << 16) | int(0 << 8) | int(0);
		}
	}
}

void drawParticle(const int& x, const int& y, const Particle& p) {
	int r, g, b;
	switch (p.type) {
	case 0:
		r = g = b = 225;
		break;
	case 1:
		r = 225;
		g = 20;
		b = 20;
		break;
	case 2:
		r = p.heat;
		g = 0;
		b = 0;
		if(r > 255) {r = 255;}
		break;
	case 3:
		r = 0;
		g = p.heat;
		b = 0;
		if(g > 255) {g = 255;}
		break;
	}
	if (y > 0 && y < 480 - 1) {
		if (x > 0 && x < 640 - 1) {
			pixels[y* width + x] = int(b << 16) | int(g << 8) | int(r);
		}
	}
}

int main(int argc, char* argv[]) {

	SDL_Event event;

	initSDL();

	long last = 0;
	double deltaTime = 0.001;
	float timeScale = 1;

	std::vector<Particle> p1s;
	std::vector<Particle> p2s;

	float angle = 360/120;
	float totalA = 0;

	/*for (int i = 0; i < 120; i++) {		
		float x = 10 * sinf(totalA);
		float y = 10 * cosf(totalA);
		p1s.push_back(Particle(Vector2D(x + 320, y + 240), Vector2D(0, 0), Vector2D(0, 0), 0, false, rand() % 15 + 5));
		totalA += angle;
	}*/

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			p1s.push_back(Particle(Vector2D(j + 250, i + 235), Vector2D(200, 65), Vector2D(0, 0), 1, false, 1, 10, 200));
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 200; j++) {
			p2s.push_back(Particle(Vector2D(j + 250, i + 255), Vector2D(0, 0), Vector2D(0, 0), 0, false, 3, 10, 10));
		}
	}

	//Spring temp = Spring(0, 0, 0, 0, 0, 0);
	//Spring temp2 = Spring(0, 0, 0, 0, 0, 0);

	//std::vector<Spring> springs;
	//std::vector<Spring> springs2;

	//temp.spawnSprings(p1s, springs, 2, 0.85, 1, 250, 235, 1, 0.8, 0.6, 2, 2);

	//temp2.spawnSprings(p2s, springs2, 200, 0.85, 1, 200, 300, 1, 0.8, 0.6, 2, 15);

	/*std::vector<Particle> BHs = { Particle(Vector2D(430, 240), Vector2D(0, 0), Vector2D(0, 0), 1, true, 2400),
								  Particle(Vector2D(130, 240), Vector2D(0, 0), Vector2D(0, 0), 1, true, 2400)};*/

	bool k = false;
	bool j = false;
	bool space = false;


	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_k:
						k = true;
						break;
					case SDLK_SPACE:
						space = !space;
						break;
					case SDLK_j:
						j = true;
						break;
					}
				}
				break;
			case SDL_KEYUP:
				if (event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_k:
						k = false;
						break;
					case SDLK_j:
						j = false;
						break;
					}
				}
				break;
			}	
		}

		if (k == true) {
			for (int i = 0; i < p1s.size(); i++) {
				p1s[i].heat += 0.5;
			}
		}

		if (space) {

			for (int i = 0; i < p1s.size(); i++) {
				p1s[i].newVel = p1s[i].velocity;
				p1s[i].newPos = p1s[i].position;
			}

			for (int i = 0; i < p2s.size(); i++) {
				p2s[i].newVel = p2s[i].velocity;
				p2s[i].newPos = p2s[i].position;
			}

			for (int i = 0; i < p1s.size(); i++) {
				p1s[i].acceleration = p1s[i].acceleration.Add(Vector2D(0, 9.81));
				p1s[i].update(deltaTime, timeScale);	
			}

			for (int i = 0; i < p2s.size(); i++) {
				p2s[i].update(deltaTime, timeScale);	
			}

			for (int i = 0; i < p1s.size(); i++) {
				p1s[i].testCollision(deltaTime, timeScale, p1s, i);
				p1s[i].testCollision(deltaTime, timeScale, p2s, i);
				p1s[i].molecularBond(p1s, i, 10);
			}

			for (int i = 0; i < p2s.size(); i++) {
				p2s[i].testCollision(deltaTime, timeScale, p2s, i);
				p2s[i].testCollision(deltaTime, timeScale, p1s, i);
				p2s[i].molecularBond(p2s, i, 10);
			}

			for (int i = 0; i < p1s.size(); i++) {
				p1s[i].position = p1s[i].newPos;
				p1s[i].velocity = p1s[i].newVel;
			}

			for (int i = 0; i < p2s.size(); i++) {
				p2s[i].position = p2s[i].newPos;
				p2s[i].velocity = p2s[i].newVel;
			}
		
		}

		drawBackground();

		//drawParticle(BHs[0].position.x, BHs[0].position.y, BHs[0]);
		//drawParticle(BHs[1].position.x, BHs[1].position.y, BHs[1]);

		//temp.drawSprings(p1s, springs, pixels);
		//temp2.drawSprings(p2s, springs2, pixels);

		/*long now = SDL_GetTicks();

		if (now > last) {
			deltaTime = ((float)(now - last)) / (float)1000;
			last = now;
		}*/


		for (int i = 0; i < p1s.size(); i++) {
			drawParticle(p1s[i].position.x, p1s[i].position.y, p1s[i]);	
		}

		for (int i = 0; i < p2s.size(); i++) {
			drawParticle(p2s[i].position.x, p2s[i].position.y, p2s[i]);	
		}

		updateSDL();

	}

	

	cleanSDL();

	return 0;
}
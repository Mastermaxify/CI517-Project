#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"
#include "../GraphicsEngine.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <memory>
#include <iostream>


class MyEngineSystem {
	friend class XCube2Engine;

	//creating private variables that we don't want the user to be able to access from outside this script
	private:
		static SDL_Renderer * renderer;
		int size;
		TTF_Font * font;
		SDL_Color textColor;
		SDL_Color boxColor;
		int x;
		int y;
		int textSpeed;
		int life;
		int w;
		int h;
		std::string text;

		
	//making public classes to be later defined in the .cpp file
	public:
		MyEngineSystem::MyEngineSystem();

		std::shared_ptr<GraphicsEngine> gfx;

		void useFont(TTF_Font * font);

		void useColors(SDL_Color textColor, SDL_Color boxColor);

		void useFontSize(TTF_Font);

		void setTextSpeed(int textSpeed);

		void setLife(int life);

		void drawTextBox(std::string text, int x, int y, int w, int h, int textSpeed, int life);

		void drawBox(const int& x, const int& y, const int& w, const int& h);
#
		void renderTextBox();


};

#endif
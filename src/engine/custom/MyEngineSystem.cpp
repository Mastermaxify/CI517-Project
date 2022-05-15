#include "MyEngineSystem.h"
#include "../GraphicsEngine.h"
#include "../XCube2d.h"

//giving the textbox empty text to use before the program runs to avoid a crash
MyEngineSystem::MyEngineSystem() {
	text = "";
}

//class to set the font before writing anything in the textbox
void MyEngineSystem::useFont(TTF_Font* font2) {
	if (nullptr == font2) {
		return;
	}

	font = font2;
}

//setting the color for both the text box and the text
void MyEngineSystem::useColors(SDL_Color textColor2, SDL_Color boxColor2) {
	textColor = textColor2;
	boxColor = boxColor2;
}

//setting text scroll speed
void MyEngineSystem::setTextSpeed(int textSpeed2) {
	textSpeed = textSpeed2;

}

//setting the life of the text box
void MyEngineSystem::setLife(int life2) {
	life = life2;

}

//class to set all of the parameters for whatever text box the user wants to draw, which is later drawn with the renderTextBox class
void MyEngineSystem::drawTextBox(std::string text2, int x2, int y2, int w2, int h2, int textSpeed2, int life2) {
	text = text2;
	x = x2;
	y = y2;
	w = w2;
	h = h2;
	textSpeed = textSpeed2;
	life = life2;

}

//the main function which handles all the drawing of the text box and text
void MyEngineSystem::renderTextBox() {

	//using the existing graphics engine to draw the rectangle and set font
	gfx->setDrawColor(boxColor);
	gfx->fillRect(x, y, w, h);
	gfx->setDrawColor(textColor);
	gfx->useFont(font);

	int w2;
	int h2;
	int count = 0;

	//getting the length of the string in x and y size
	TTF_SizeText(font, text.c_str(), &w2, &h2);

	int stringLength = text.length();

	//entire logic for screenwrapping on the text box if the text is too long, which only occurs on spaces so words are never wrapped mid way through
	while (w2 > w - 20) {

		for (int i = stringLength - 1; i >= 0; i--) {

			char currentChar = text[i];

			if (currentChar == ' ') {
				TTF_SizeText(font, text.substr(0, i).c_str(), &w2, &h2);

				if (w2 < w - 20) {
					//text[i] = '\n';
					gfx->drawText(text.substr(0, i), x + 10, (y + 5) + ((h2 + 5) * count));
					text = text.substr(i + 1, stringLength - 1);
					TTF_SizeText(font, text.c_str(), &w2, &h2);
					stringLength = text.length();

					count += 1;
					break;
				}
			}
		}
	}

	gfx->drawText(text, x + 10, (y + 5) + ((h2 + 5) * count));
}

//drawing an empty box
void MyEngineSystem::drawBox(const int& x, const int& y, const int& w, const int& h) {
	gfx->fillRect(x, y, 785, 130);
}
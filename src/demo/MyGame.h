#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

//creating the various structs for the game objects
struct GameKey {
	Point2 pos;
	bool isAlive;
};

struct GameWall {
	Point2 pos;
};

struct GameBullet {
	int x;
	int y;
	int xspeed;
	int yspeed;
	Point2 pos;
};

struct GameEnemy {
	int x;
	int y;
	int startx;
	int starty;
	int timer;
	int radius;
	Point2 pos;
};

//the main class for initialising variables used in MyGame.cpp
class MyGame : public AbstractGame {
	private:
		Rect box;

		Vector2i velocity;

		TTF_Font * defaultFont;

		std::vector<std::shared_ptr<GameKey>> gameKeys;

		std::vector<std::shared_ptr<GameWall>> walls;

		std::vector<std::shared_ptr<GameBullet>> bullets;

		std::vector<std::shared_ptr<GameEnemy>> enemies;

		/* GAMEPLAY */
		int score, numKeys, lives, numWalls, numBullets, numEnemies, cooldown, t;
		bool gameWon;

		SDL_Texture* texture;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
        MyGame();
		~MyGame();
};

#endif
#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(15), numWalls(50), numBullets(30), numEnemies(10), gameWon(false), box(30, 80, 30, 30){

	//initialising fonts so they dont load every time text is written
	defaultFont = ResourceManager::loadFont("res/fonts/arial.ttf", 32);
	mySystem->useFont(ResourceManager::loadFont("res/fonts/arial.ttf", 24));

	//setting the font to what we initialised
	gfx->useFont(defaultFont);
	gfx->setVerticalSync(true);
	mySystem->gfx = gfx;

	//creating instances of the gamekey struct
    for (int i = 0; i < numKeys; i++) {
        std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
        k->isAlive = true;
        k->pos = Point2(getRandom(0, 750), getRandom(0, 420));
        gameKeys.push_back(k);
    }

	//creating instances of the gamewall struct
	for (int j = 0; j < numWalls; j++) {
		std::shared_ptr<GameWall> l = std::make_shared<GameWall>();
		l->pos = Point2(50 + j*10, 250);
		walls.push_back(l);
	}

	//creating instances of the gameenemy struct
	for (int j = 0; j < numEnemies; j++) {
		std::shared_ptr<GameEnemy> l = std::make_shared<GameEnemy>();
		l->x = 150 + j * 50;
		l->y = 330;
		l->startx = 100 + j * 50;
		l->starty = 330;
		l->pos = Point2(l->x, l->y);
		l->radius = 50;
		l->timer = 0;
		enemies.push_back(l);
	}

	//initialising misceallaneous variables
	cooldown = 0;
	t = 0;

	//drawing the first text box when the game begins and making it only happen once
	mySystem->useColors(SDL_COLOR_BLACK, SDL_COLOR_RED);
	mySystem->drawTextBox("WELCOME TO MY TEST DEMO!", 7, 460, 785, 130, 5, 5);

	//playing music
	sfx->setSoundVolume(100);
	Mix_Chunk* music = Mix_LoadWAV("res/sounds/carefree.wav");
	Mix_PlayChannel(1, music, -1);
	Mix_Volume(1, 1);

	SDL_Surface* image = IMG_Load("res/images/space.jpg");
	texture = gfx->createTextureFromSurface(image);
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	int speed = 3;
	
	//user key input detection for movement
	if (eventSystem->isPressed(Key::W)) {
		velocity.y = -speed;
	}

	if (eventSystem->isPressed(Key::S)) {
		velocity.y = speed;
	}

	if (eventSystem->isPressed(Key::A)) {
		velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::D)) {
		velocity.x = speed;
	}

	//user key input detection for firing bullets in all 4 directions
	if (eventSystem->isPressed(Key::RIGHT)) {
		if (cooldown < 1) {

			if (numBullets > 0) {
				std::shared_ptr<GameBullet> l = std::make_shared<GameBullet>();
				l->x = box.x+15;
				l->y = box.y+15;
				l->xspeed = 8;
				l->yspeed = 0;
				l->pos = Point2(l->x, l->y);
				numBullets--;
				bullets.push_back(l);

				cooldown = 25;
			}
		}
	}

	if (eventSystem->isPressed(Key::LEFT)) {
		if (cooldown < 1) {

			if (numBullets > 0) {
				std::shared_ptr<GameBullet> l = std::make_shared<GameBullet>();
				l->x = box.x + 15;
				l->y = box.y + 15;
				l->xspeed = -8;
				l->yspeed = 0;
				l->pos = Point2(l->x, l->y);
				numBullets--;
				bullets.push_back(l);

				cooldown = 25;
			}
		}
	}

	if (eventSystem->isPressed(Key::UP)) {
		if (cooldown < 1) {

			if (numBullets > 0) {
				std::shared_ptr<GameBullet> l = std::make_shared<GameBullet>();
				l->x = box.x + 15;
				l->y = box.y + 15;
				l->xspeed = 0;
				l->yspeed = -8;
				l->pos = Point2(l->x, l->y);
				numBullets--;
				bullets.push_back(l);

				cooldown = 25;
			}
		}
	}

	if (eventSystem->isPressed(Key::DOWN)) {
		if (cooldown < 1) {

			if (numBullets > 0) {
				std::shared_ptr<GameBullet> l = std::make_shared<GameBullet>();
				l->x = box.x + 15;
				l->y = box.y + 15;
				l->xspeed = 0;
				l->yspeed = 8;
				l->pos = Point2(l->x, l->y);
				numBullets--;
				bullets.push_back(l);

				cooldown = 25;
			}
		}
	}
}

void MyGame::update() {

	//updating time and the cooldown for bullet firing
	t++;
	cooldown -= 1;

	//easter egg
	if (box.x > 650 && box.y < 60) {
		mySystem->useColors(SDL_COLOR_BLACK, SDL_COLOR_RED);
		mySystem->drawTextBox("MAX GORKA-CI517-2021/2022", 7, 460, 785, 130, 5, 5);
	}
	
	//moving the box x and y
	box.x += velocity.x;
	box.y += velocity.y;

	//behaviour for the yellow coins
	for (auto key : gameKeys) {
		if (key->isAlive && box.contains(key->pos)) {
			score += 200;
			key->isAlive = false;
			numKeys--;
		}
	}
	
	//behaviour for the red walls
	for (auto wall : walls) {
		if (box.contains(wall->pos)) {
			box.x = 30;
			box.y = 80;

			lives -= 1;

			std::string livesStr = std::to_string(lives);

			mySystem->useColors(SDL_COLOR_BLACK, SDL_COLOR_RED);
			mySystem->drawTextBox("YOU DIED TO A RED ENEMY. YOU HAVE " + livesStr + ((lives > 1) ? " LIVES" : " LIFE") + " LEFT.", 7, 460, 785, 130, 5, 5);

		}
	}

	//behaviour for the green bullets
	for (auto bullet : bullets) {
		bullet->x += bullet->xspeed;
		bullet->y += bullet->yspeed;
		bullet->pos = Point2(bullet->x, bullet->y);
	}

	//behaviour for the purple enemies
	for (auto enemy : enemies) {
		enemy->x += 0;
		
		//sine wave function to smoothly move up and down
		enemy->y = enemy->starty + (sin(enemy->timer * (M_PI / 30)) * enemy->radius);
		enemy->pos = Point2(enemy->x, enemy->y);
		enemy->timer++;

		if (box.contains(enemy->pos)) {
			box.x = 30;
			box.y = 80;

			lives -= 1;

			std::string livesStr = std::to_string(lives);

			mySystem->useColors(SDL_COLOR_BLACK, SDL_COLOR_RED);
			mySystem->drawTextBox("YOU DIED TO A PURPLE ENEMY. YOU HAVE " + livesStr + ((lives > 1) ? " LIVES" : " LIFE") + " LEFT.", 7, 460, 785, 130, 5, 5);
		}
	}

	//setting the velocity to 0 by default
	velocity.x = 0;
    velocity.y = 0;

	//making the game won when yellow coin count is 0
	if (numKeys == 0) {
		gameWon = true;
	}

	//making the box unable to go offscreen
	if (box.x < 0)
	{
		box.x = 0;
	}

	if (box.y < 0)
	{
		box.y = 0;
	}

	if (box.x > 770)
	{
		box.x = 770;
	}

	if (box.y > 420)
	{
		box.y = 420;
	}


}
//the main render function where everything is drawn on screen
void MyGame::render() {

	//drawing the backround
	Rect rectangle(0, 0, 800, 600);
	gfx->drawTexture(texture, &rectangle.getSDLRect());

	if (lives > 0 && gameWon == false)
	{
		gfx->setDrawColor(SDL_COLOR_BLUE);
		gfx->drawRect(box);

		gfx->setDrawColor(SDL_COLOR_YELLOW);
		for (auto key : gameKeys)
			if (key->isAlive)
				gfx->drawCircle(key->pos, 5);

		gfx->setDrawColor(SDL_COLOR_GREEN);
		for (auto bullet : bullets)
			gfx->drawCircle(bullet->pos, 5);

		gfx->setDrawColor(SDL_COLOR_PURPLE);
		for (auto enemy : enemies)
			gfx->drawCircle(enemy->pos, 5);

		gfx->setDrawColor(SDL_COLOR_RED);
		for (auto wall : walls)
			gfx->drawCircle(wall->pos, 5);

		
		
	}

	

}


void MyGame::renderUI() {

	//using the default font as default, unless specified otherwise
	gfx->useFont(defaultFont);

	if (gameWon == false) {

		if (lives > 0)
		{
			//drawing the text at the top with the score, lives and bullet variables
			gfx->setDrawColor(SDL_COLOR_RED);
			gfx->drawLine(Point2(0, 450), Point2(800, 450));

			gfx->setDrawColor(SDL_COLOR_AQUA);
			std::string scoreStr = std::to_string(score);
			gfx->drawText("Score: " + scoreStr, 650 - scoreStr.length() * 50, 25);

			std::string livesStr = std::to_string(lives);
			gfx->drawText("Lives: " + livesStr, 80 - livesStr.length() * 50, 25);

			std::string bulletsStr = std::to_string(numBullets);
			gfx->drawText("Bullets: " + bulletsStr, 300 - bulletsStr.length() * 50, 25);

		}

		//making the game over screen
		if (lives == 0)
		{
			mySystem->useColors(SDL_COLOR_BLACK, SDL_COLOR_RED);
			mySystem->drawTextBox("YOU LOST. ", 7, 460, 785, 130, 5, 5);

		}
	}

	//game win screen being rendered only when the game is over
	if (gameWon == true) {
		mySystem->useColors(SDL_COLOR_BLACK, SDL_COLOR_RED);
		mySystem->drawTextBox("YOU WON CONGRATULATIONS. ", 7, 460, 785, 130, 5, 5);
	}

	//the rendertextbox function is called as the very last thing in each frame so that the text box has all necessary details done before drawing
	mySystem->renderTextBox();
}
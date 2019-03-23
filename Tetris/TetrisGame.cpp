#include "TetrisGame.h"


//constructor
TetrisGame::TetrisGame(sf::RenderWindow *window, sf::Sprite *blockSprite, sf::Sprite *shadowSprite, Point gameboardOffset, Point nextShapeOffset, int player) {
	//setup pointers
	pWindow = window;
	pBlockSprite = blockSprite;
	pShadowSprite = shadowSprite;
	//setup offsets
	this->gameboardOffset = gameboardOffset;
	this->nextShapeOffset = nextShapeOffset;
	//setup text
	scoreFont.loadFromFile("res/RedOctober.ttf");
	setupText();
	//set the player
	this->player = player;
	//setup sound
	backroundMusic.setVolume(20.f);
	removeRowSound.setVolume(70.f);
	setupSound();
	currentShape.setShape(Tetramino::getRandomShape());
	//initial shape spawn
	currentShape.setGridLoc(board.getSpawnLoc());
	//reset variables
	reset();

	//setup key bindings for player 1 and 2
	switch (player)
	{
	case 1:
		leftKey = sf::Keyboard::Left;
		rightKey = sf::Keyboard::Right;
		rotateKey = sf::Keyboard::Up;
		dropKey = sf::Keyboard::Down;
		break;
	case 2:
		leftKey = sf::Keyboard::A;
		rightKey = sf::Keyboard::D;
		rotateKey = sf::Keyboard::W;
		dropKey = sf::Keyboard::S;
		break;
	default:
		break;
	}
}
//de-constructor
TetrisGame::~TetrisGame() {
	pWindow = NULL;
	pBlockSprite = NULL;
	pShadowSprite = NULL;
}

//draws the whole game
void TetrisGame::draw() {
	drawGameboard();
	drawShadow(currentShape, gameboardOffset);
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, Point(nextShapeOffset.getX(), nextShapeOffset.getY() - BLOCK_HEIGHT));
	//draw on the window
	pWindow->draw(scoreText);
}

//-------------------User Input-----------------------
void TetrisGame::onKeyPressed(sf::Event input) {
	//left
	if (input.key.code == leftKey) {
		attemptMove(currentShape, -1, 0);
	}
	//right
	else if (input.key.code == rightKey) {
		attemptMove(currentShape, 1, 0);
	}
	//rotate
	else if (input.key.code == rotateKey) {
		attemptRotate(currentShape);
	}
	//drop
	else if (input.key.code == dropKey) {
		drop(currentShape);
	}
}
//---------------------------------Game Logic----------------------------
void TetrisGame::processGameLoop(float gameSeconds) {
	//keeps track of tick timing
	secondsSinceLastTick += gameSeconds;
	//if a whole tick is complete
	if (secondsSinceLastTick >= secsPerTick && !shapePlacedSinceLastGameLoop) {
		tick();
	}
	//only want one music if 2 player
	if (player == 1) {
		//set new song every X seconds
		soundTime = soundClock.getElapsedTime().asSeconds();
		if ((int)soundTime >= 45) {
			setRandomSong();
			soundClock.restart();
		}
	}
	shapePlacedSinceLastGameLoop = false;
}
//makes the shape move on a set tick pace
void TetrisGame::tick() {
	determineSecsPerTick();
	//moves shape down
	if (!attemptMove(currentShape, 0, 1)) {
		//lock the shape to the board
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
		//clears rows and updates score
		setScore(board.removeCompletedRows());
		//board printed to console
		//system("cls");
		//board.printToConsole();/**/

		//game over
		if (isGameOver >= 3) {
			//menu game check
			if (player != -1) {
				drawGameOver();
			}
			reset();
		}
		//the game is not over
		else if (isGameOver == 0) {
			//spawns next shape
			if (!spawnNextShape()) {
				//spawn collision, game start ending sequence
				isGameOver++;
			}
		//the game ending sequence has begun
		}else {
			spawnNextShape();
			isGameOver++;
		}
	}
	//reset tick seconds
	secondsSinceLastTick = 0.0;
}

//-----------------------------------------Private Functions--------------------------------
//Resets the game
void TetrisGame::reset() {
	score = 0;
	scoreText.setString("Points: 0");
	isGameOver = 0;
	determineSecsPerTick();
	board.empty();
	pickNextShape();
	//only want one music if two player
	if (player == 1) {
		setRandomSong();
	}
	soundClock.restart();
}

//sets the next shape to a random one
void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetramino::getRandomShape());
}

//sets a new shape at spawn
bool TetrisGame::spawnNextShape() {
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	pickNextShape();
	return isPositionLegal(currentShape);
}

//rotates shape if possible
bool TetrisGame::attemptRotate(GridTetramino &shape) {
	GridTetramino temp = shape;
	temp.rotateCW();
	//checks if shape can rotate
	if (isPositionLegal(temp)) {
		shape.rotateCW();
		return true;
	}
	return false;
}

//moves shape in requested direction if possible
bool TetrisGame::attemptMove(GridTetramino &shape, int x, int y) {
	GridTetramino temp = shape;
	temp.move(x, y);
	//checks if the shape can move
	if (isPositionLegal(temp)) {
		shape.move(x, y);
		return true;
	}
	return false;
}

//instant drop shape to lowest available position
void TetrisGame::drop(GridTetramino &shape) {
	while(attemptMove(shape, 0, 1));
}

//saves the landed shape to the board
void TetrisGame::lock(const GridTetramino &shape) {
	lockSound.play();
	board.setContent(shape.getBlockLocsMappedToGrid(), currentShape.getColor());
}

//draws a single block
void TetrisGame::drawBlock(int x, int y, TetColor color, Point origin) {
	//selects portion of the texture to be used
	pBlockSprite->setTextureRect(sf::IntRect(color * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	//translates board position to the window
	pBlockSprite->setPosition((float)(x * BLOCK_WIDTH + origin.getX()), (float)(y * BLOCK_HEIGHT + origin.getY()));
	pWindow->draw(*pBlockSprite);
}

//loops through the board and draws blocks that are filled
void TetrisGame::drawGameboard() {
	for (int y = 0; y < board.MAX_Y; y++) {
		for (int x = 0; x < board.MAX_X; x++) {
			int color = board.getContent(x, y);
			if (color != board.EMPTY_BLOCK) {
				drawBlock(x, y, TetColor(color), gameboardOffset);
			}
		}
	}
}

//draws the blocks a shape
void TetrisGame::drawTetromino(GridTetramino tetromino, Point origin) {
	for (Point point : tetromino.getBlockLocsMappedToGrid()) {
		drawBlock(point.getX(), point.getY(), tetromino.getColor(), origin);
	}
}

//draws a shadow that is at the current shapes lowest possible position
void TetrisGame::drawShadow(GridTetramino tetromino, Point origin) {
	GridTetramino shadow = tetromino;
	//shape moves to bottom
	drop(shadow);
	//same as drawBlock() but with darkened sprite
	for (Point point : shadow.getBlockLocsMappedToGrid()) {
		pShadowSprite->setTextureRect(sf::IntRect(shadow.getColor() * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		pShadowSprite->setPosition((float)(point.getX() * BLOCK_WIDTH + origin.getX()), (float)(point.getY() * BLOCK_HEIGHT + origin.getY()));
		pWindow->draw(*pShadowSprite);
	}
}

//random blocks fill the screen over time
void TetrisGame::drawGameOver() {
	backroundMusic.stop();
	gameOverSound.play();
	for (int y = 0; y < board.MAX_Y; y++) {
		for (int x = 0; x < board.MAX_X; x++) {
			board.setContent(x, y, rand()%7);
			drawGameboard();
			sf::sleep(sf::Time(sf::milliseconds(10)));	//Side effect of using sleep instead of tick mechanic pauses both players in 2p
			pWindow->display();
		}
		sf::sleep(sf::Time(sf::milliseconds(30)));
	}
	saveScore();
}
void TetrisGame::saveScore() {
	sf::RenderWindow inputWindow(sf::VideoMode(400, 219), "Save High Score");
	//background
	sf::Texture inputTxtr;
	if (!inputTxtr.loadFromFile("res/nameInputBg.jpg")) {
		system("pause");
	}
	sf::Sprite inputSp;
	inputSp.setTexture(inputTxtr);
	
	//the text that shows what the user has entered
	sf::Text inputText;
	inputText.setFont(scoreFont);
	std::string inputString = "";
	inputText.setString(inputString);
	inputText.setFillColor(sf::Color::White);
	inputText.setPosition(inputText.getPosition().x + 40, inputText.getPosition().y + 60);

	//The request for user to input name and press enter with a points display
	sf::Text request;
	request.setFont(scoreFont);
	request.setString("Please enter your name: \n\n\n\nPoints: " + std::to_string(score)+"\n\n" + "Press Enter to Save...");
	request.setFillColor(sf::Color::White);
	request.scale(0.6f,0.6f);
	request.setPosition(request.getPosition().x + 40, request.getPosition().y + 30);
	while (inputWindow.isOpen())
	{
		// handle any window or keyboard events that have occured since the last game loop
		sf::Event event;
		while (inputWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				inputWindow.close();
			}
			//any key pressed
			else if (event.type == sf::Event::KeyPressed)
			{
				//Backspace key
				if (event.key.code == sf::Keyboard::BackSpace) {
					//remove one character from string and update
					inputString = inputString.substr(0, inputString.size() - 1);;
					inputText.setString(inputString);
				}
				//Enter key
				else if (event.key.code == sf::Keyboard::Enter) {
					//open file to append to
					std::ofstream leaderboard;
					leaderboard.open("data/leaderboard.txt", std::ios_base::app);
					if (!leaderboard) {
						std::cout << "unable to save score" << std::endl;
					}
					//save the name and score to the file
					leaderboard << inputString << "," << std::to_string(score) + "\n";
					leaderboard.close();
					inputWindow.close();
				}
				//all other keys
				else {
					//only letters work, other keys result in bad output
					inputString += static_cast<char>('A' + event.key.code);
					inputText.setString(inputString);
				}
			}
		}
		inputWindow.clear(sf::Color::White);	// clear the entire window
		inputWindow.draw(inputSp);
		inputWindow.draw(request);				//draw
		inputWindow.draw(inputText);
		inputWindow.display();					// re-display the entire window
	}
}

//sets the song to a new random song
void TetrisGame::setRandomSong() {
	static std::vector<int> randomSong;
	static std::string file;
	if (score < 100) {
		if (randomSong.size() <= 0) {
			//for each song in the res/audio folder
			for (int i = 0; i <= 30; i++) {
				randomSong.push_back(i);
			}
			//random generator
			auto rng2 = std::default_random_engine((unsigned)time(NULL));
			//shuffle vector
			std::shuffle(std::begin(randomSong), std::end(randomSong), rng2);
		}
		//sets to a song from the shuffled vector
		file = "res/audio/" + std::to_string(randomSong[randomSong.size() - 1]) + ".WAV";
		randomSong.pop_back();

		//opens the file to be played
		if (!backroundMusic.openFromFile(file)) {
			system("pause");
		}
		//change song being played
		backroundMusic.stop();
		backroundMusic.setLoop(true);
		backroundMusic.play();
	}
	//--------------------BONUS SONG!!!--------------- 
	//must have 100 pts or more
	else {
		if (file != "res/audio/31.WAV") {
			file = "res/audio/31.WAV";
			//opens the file to be played
			if (!backroundMusic.openFromFile(file)) {
				system("pause");
			}
			//change song being played
			backroundMusic.stop();
			backroundMusic.setLoop(true);
			backroundMusic.play();
		}
	}
}

//called for initial setup of text
void TetrisGame::setupText() {
	scoreText.setString("Points: 0");
	//set font
	scoreText.setFont(scoreFont);
	//set size
	scoreText.scale(0.7f, 0.7f);
	//set position
	scoreText.setPosition((float)(nextShapeOffset.getX() - BLOCK_WIDTH * 2), (float)(nextShapeOffset.getY() + BLOCK_HEIGHT * 3));
}

//loads sounds to buffers
void TetrisGame::setupSound() {
	//buffer sound effects
	if (!lockBuffer.loadFromFile("land.wav")) {
		system("pause");
	}
	lockSound.setBuffer(lockBuffer);

	if (!gameOverBuffer.loadFromFile("gameOver.wav")) {
		system("pause");
	}
	gameOverSound.setBuffer(gameOverBuffer);

	if (!removeRowBuffer.loadFromFile("removeRow.wav")) {
		system("pause");
	}
	removeRowSound.setBuffer(removeRowBuffer);
}

//increments and sets score text
void TetrisGame::setScore(int score) {
	//if a row was removed
	if (score > 0) {
		//play remove row sound
		removeRowSound.play();
		this->score += score;
		scoreText.setString("Points: " + std::to_string(this->score));
	}
}

//check collision with wall or other block
bool TetrisGame::isPositionLegal(const GridTetramino &shape) {
	return (isShapeWithinBorders(shape) && !doesShapeIntersectLockedBlocks(shape));
}

//checks wall collision
bool TetrisGame::isShapeWithinBorders(const GridTetramino &shape) {
	std::vector<Point> theShape = shape.getBlockLocsMappedToGrid();
	for (Point point : theShape) {
		//bottom collided
		if (point.getY() >= board.MAX_Y ) {
			return false;
		}
		//walls collided
		else if (point.getX() < 0 || point.getX() >= board.MAX_X) {
			return false;
		}
	}
	//no collision with borders
	return true;
}

//check collision with other blocks
bool TetrisGame::doesShapeIntersectLockedBlocks(const GridTetramino &shape) {
	if (board.areLocsEmpty(shape.getBlockLocsMappedToGrid())) {
		return false;
	}
	return true;
}

//tracks how fast the game is running based on points ----- 120 points is max speed
void TetrisGame::determineSecsPerTick() {
	//how much less time per tick
	double increment = 0.05;
	//each step reduces tick by the increment
	int step = 10;
	double speed = MAX_SECS_PER_TICK - (increment * (score/step));
	//make sure tick is within MIN
	if (speed > MIN_SECS_PER_TICK) {
		secsPerTick = speed;
	}
	//else tick is set to MIN
	else {
		secsPerTick = MIN_SECS_PER_TICK;
	}
}

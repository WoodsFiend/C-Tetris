
//---------------Curtis Naples 2019---------
//opengameart.com - audio assets

#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <functional>
#include <fstream>
#include <sstream>
#include "TetrisGame.h"
#include "TestSuite.h"

void playGame();
void playGame2p();
void openLeaderboard();

sf::Image iconTexture;

//Runs main Menu
int main()
{
	//hide the console
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	srand((unsigned)time(NULL));	// seed our randomizer

	//variables needed for all games
	sf::Sprite blockSprite;			// the tetromino block sprite
	sf::Texture blockTexture;		// the tetromino block texture
	sf::Sprite shadowSprite;		// the shadow block sprite
	sf::Texture shadowTexture;		// the shadow block texture
	//main menu
	sf::RenderWindow mainMenu(sf::VideoMode(1704, 959), "Tetris");
	
	mainMenu.setSize(sf::Vector2u::Vector2(mainMenu.getSize().x, mainMenu.getSize().y));
	mainMenu.setPosition(sf::Vector2i(mainMenu.getPosition().x, 0));
	
	//-------------------------------------Load Assets------------------------------------------
	if (!iconTexture.loadFromFile("res/icon.png")) {
		system("pause");
	}
	mainMenu.setIcon(200,184,iconTexture.getPixelsPtr());
	if (!blockTexture.loadFromFile("res/tiles.png")) {			// load the tetris block texture
		system("pause");
	}
	blockSprite.setTexture(blockTexture);

	if (!shadowTexture.loadFromFile("res/shadowTiles.png")) {	//load shadow block texture
		system("pause");
	}
	shadowSprite.setTexture(shadowTexture);

	sf::Music themeSong;										//Music
	if (!themeSong.openFromFile("res/audio/theme.wav")) {
		system("pause");
	}
	themeSong.setLoop(true);
	themeSong.play();

	sf::Texture backgroundImg;									//Background
	if (!backgroundImg.loadFromFile("res/mainMenu.jpg")) {
		system("pause");
	}
	sf::Sprite backgroundSp;
	backgroundSp.setTexture(backgroundImg);
	sf::Texture titleImage;										//Title
	if (!titleImage.loadFromFile("res/title.png")) {
		system("pause");
	}
	sf::Sprite titleSp;
	titleSp.setTexture(titleImage);
	titleSp.setPosition(titleSp.getPosition().x + 520, titleSp.getPosition().y + 75);
	
	sf::Texture buttonTexture;									//Button
	if (!buttonTexture.loadFromFile("res/button.png")) {
		system("pause");
	}
	sf::Texture buttonRollOver;
	if (!buttonRollOver.loadFromFile("res/buttonRollOver.png")) {
		system("pause");
	}
	sf::Font font;												//font
	if (!font.loadFromFile("res/RedOctober.ttf")) {		
		system("pause");
	}		
	//----------------------------------Setup buttons------------------------------------
	//button backgrounds
	sf::Sprite onePlayerBg;
	sf::Sprite twoPlayerBg;
	sf::Sprite leaderboardBg;
	onePlayerBg.setTexture(buttonTexture);
	twoPlayerBg.setTexture(buttonTexture);
	leaderboardBg.setTexture(buttonTexture);
	onePlayerBg.setPosition(550, 788);
	twoPlayerBg.setPosition(972, 788);
	leaderboardBg.setPosition(762, 850);

	//button text
	sf::Text onePlayer;
	sf::Text twoPlayer;
	sf::Text leaderboard;
	onePlayer.setFont(font);
	twoPlayer.setFont(font);
	leaderboard.setFont(font);
	leaderboard.scale(0.68f, 0.68f);
	onePlayer.setString("1 Player");
	twoPlayer.setString("2 Player");
	leaderboard.setString("Leaderboard");
	onePlayer.setFillColor(sf::Color::White);
	twoPlayer.setFillColor(sf::Color::White);
	leaderboard.setFillColor(sf::Color::White);
	onePlayer.setPosition(onePlayerBg.getPosition().x + 10, onePlayerBg.getPosition().y + 5);
	twoPlayer.setPosition(twoPlayerBg.getPosition().x + 5, twoPlayerBg.getPosition().y + 5);
	leaderboard.setPosition(leaderboardBg.getPosition().x + 3, leaderboardBg.getPosition().y + 12);

	//----------------------------------Setup Background Games------------------------------
	TetrisGame menuBgGame2(&mainMenu, &blockSprite, &shadowSprite, Point(468, 180), Point(2200, 100), -1);
	TetrisGame menuBgGame3(&mainMenu, &blockSprite, &shadowSprite, Point(890, 180), Point(2200, 100), -1);
	//background game clock
	sf::Clock clock;	// set up a clock so we can determine seconds per game loop

	//---------------------------------------Menu Loop----------------------------------------
	while (mainMenu.isOpen()) {
		// Clock
		float gameLoopSeconds = clock.getElapsedTime().asSeconds();
		clock.restart();
		//--------------Event Loop-------------
		sf::Event event;
		while (mainMenu.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mainMenu.close();
			}
			//-------------Mouse Move Event---------------
			else if (event.type == sf::Event::MouseMoved) {
				//tracks mouse position on window
				sf::Vector2i mousePos = sf::Mouse::getPosition(mainMenu);
				sf::Vector2f mousePosF = mainMenu.mapPixelToCoords(mousePos);
				//button 1 rollover
				if (onePlayerBg.getGlobalBounds().contains(mousePosF)) {
					onePlayerBg.setTexture(buttonRollOver);
					onePlayer.setFillColor(sf::Color::Black);
				}
				else {
					onePlayerBg.setTexture(buttonTexture);
					onePlayer.setFillColor(sf::Color::White);
				}
				//button 2 rollover
				if (twoPlayerBg.getGlobalBounds().contains(mousePosF)) {
					twoPlayerBg.setTexture(buttonRollOver);
					twoPlayer.setFillColor(sf::Color::Black);
				}
				else {
					twoPlayerBg.setTexture(buttonTexture);
					twoPlayer.setFillColor(sf::Color::White);
				}
				//leaderboard rollover
				if (leaderboardBg.getGlobalBounds().contains(mousePosF)) {
					leaderboardBg.setTexture(buttonRollOver);
					leaderboard.setFillColor(sf::Color::Black);
				}
				else {
					leaderboardBg.setTexture(buttonTexture);
					leaderboard.setFillColor(sf::Color::White);
				}
			}
			//-----------------Click Mouse Event---------------------
			else if (event.type == sf::Event::MouseButtonPressed) {
				//gets mouse position when click occured
				sf::Vector2i mousePos = sf::Mouse::getPosition(mainMenu);
				sf::Vector2f mousePosF = mainMenu.mapPixelToCoords(mousePos);
				//click button 1
				if (onePlayerBg.getGlobalBounds().contains(mousePosF)) {
					mainMenu.setVisible(false);
					themeSong.stop();
					playGame();
					mainMenu.setVisible(true);
					themeSong.play();
				}
				//click button 2
				else if (twoPlayerBg.getGlobalBounds().contains(mousePosF)) {
					mainMenu.setVisible(false);
					themeSong.stop();
					playGame2p();
					mainMenu.setVisible(true);
					themeSong.play();
				}
				else if (leaderboardBg.getGlobalBounds().contains(mousePosF)) {
					openLeaderboard();
				}
				else {
					leaderboardBg.setTexture(buttonTexture);
					leaderboard.setFillColor(sf::Color::White);
				}
			}
		}
		//Background Games Logic
		menuBgGame2.processGameLoop(gameLoopSeconds);
		menuBgGame3.processGameLoop(gameLoopSeconds);

		mainMenu.clear();
		//----------Draw-----------
		mainMenu.draw(backgroundSp);
		menuBgGame2.draw();
		menuBgGame3.draw();
		mainMenu.draw(titleSp);
		mainMenu.draw(onePlayerBg);
		mainMenu.draw(twoPlayerBg);
		mainMenu.draw(leaderboardBg);
		mainMenu.draw(onePlayer);
		mainMenu.draw(twoPlayer);
		mainMenu.draw(leaderboard);
		mainMenu.display();
	}
	return 0;
}

//------------------------------------------------------------------One Player Game----------------------------------------------
void playGame() {
	// run some sanity tests on our classes to ensure they're working as expected.
	assert(TestSuite::runTestSuite());

	sf::Sprite blockSprite;			// the tetromino block sprite
	sf::Texture blockTexture;		// the tetromino block texture
	sf::Sprite shadowSprite;		// the shadow block sprite
	sf::Texture shadowTexture;		// the shadow block texture
	sf::Sprite backgroundSprite;	// the background sprite
	sf::Texture backgroundTexture;	// the background texture

	//------------------------------Load Assets-----------------------------
	if (!backgroundTexture.loadFromFile("res/background.png")) {	// load the background texture
		system("pause");
	}
	backgroundSprite.setTexture(backgroundTexture);

	if (!blockTexture.loadFromFile("res/tiles.png")) {			// load the tetris block texture
		system("pause");
	}
	blockSprite.setTexture(blockTexture);

	if (!shadowTexture.loadFromFile("res/shadowTiles.png")) {		//load shadow block texture
		system("pause");
	}
	shadowSprite.setTexture(shadowTexture);

	//---------------------------------Game Setup----------------------------
	// create the game window
	sf::RenderWindow window(sf::VideoMode(640, 800), "Tetris Game Window");
	window.setIcon(200, 184, iconTexture.getPixelsPtr());
	window.setPosition(sf::Vector2i(window.getPosition().x - 100, 0));
	window.setSize(sf::Vector2u::Vector2(window.getSize().x + 200, window.getSize().y + 175));
	// set up a tetris game
	TetrisGame game(&window, &blockSprite, &shadowSprite, Point(54, 125), Point(490, 210));
	sf::Clock clock;	// set up a clock so we can determine seconds per game loop

	// ----------------------------Game Loop---------------------------------
	while (window.isOpen())
	{
		// how long since the last loop (fraction of a second)
		float gameLoopSeconds = clock.getElapsedTime().asSeconds();
		clock.restart();

		// handle any window or keyboard events that have occured since the last game loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				//sends events to the game
				game.onKeyPressed(event);
			}
		}
		
		game.processGameLoop(gameLoopSeconds);	// handle tetris game logic in here.
		window.clear(sf::Color::White); // clear the entire window
		window.draw(backgroundSprite);	// draw the background (onto the window) 
		game.draw();					// draw the game (onto the window)
		window.display();				// re-display the entire window
	}
}

//------------------------------------------------------------------Two Player Game-------------------------------------------------
void playGame2p() {
	// run some sanity tests on our classes to ensure they're working as expected.
	//assert(TestSuite::runTestSuite());

	sf::Sprite blockSprite;			// the tetromino block sprite
	sf::Texture blockTexture;		// the tetromino block texture
	sf::Sprite shadowSprite;		// the shadow block sprite
	sf::Texture shadowTexture;		// the shadow block texture
	sf::Sprite backgroundSprite;	// the background sprite
	sf::Sprite backgroundSprite2;	// the background sprite
	sf::Texture backgroundTexture;	// the background texture

	//-----------------Load Assets----------------------
	if (!backgroundTexture.loadFromFile("res/background.png")) {// load the background texture
		system("pause");
	}
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite2.setTexture(backgroundTexture);
	backgroundSprite2.setPosition(backgroundSprite2.getPosition().x + 640, backgroundSprite2.getPosition().y);
	if (!blockTexture.loadFromFile("res/tiles.png")) {		// load the tetris block texture
		system("pause");
	}
	blockSprite.setTexture(blockTexture);

	if (!shadowTexture.loadFromFile("res/shadowTiles.png")) {	// load the shadow block texture
		system("pause");
	}
	shadowSprite.setTexture(shadowTexture);

	//---------------------------------Setup Window------------------
	// create the game window
	sf::RenderWindow window(sf::VideoMode(1280, 800), "Tetris Game Window");
	window.setIcon(200, 184, iconTexture.getPixelsPtr());
	window.setPosition(sf::Vector2i(window.getPosition().x - 200, 0));
	window.setSize(sf::Vector2u::Vector2(window.getSize().x + 350, window.getSize().y + 175));

	//---------------------------------Setup 2 Games--------------------------------
	TetrisGame game(&window, &blockSprite, &shadowSprite, Point(54 + 640, 125), Point(490 + 640, 210));
	TetrisGame game2(&window, &blockSprite, &shadowSprite, Point(54, 125), Point(490, 210), 2);

	sf::Clock clock;	// set up a clock so we can determine seconds per game loop
	//--------------------------------------Game Loop----------------------------------
	while (window.isOpen())
	{
		// how long since the last loop (fraction of a second)
		float gameLoopSeconds = clock.getElapsedTime().asSeconds();
		clock.restart();
		// handle any window or keyboard events that have occured since the last game loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				//send keypress to the game
				game.onKeyPressed(event);
				game2.onKeyPressed(event);
			}
		}

		game.processGameLoop(gameLoopSeconds);	// handle tetris game logic in here.
		game2.processGameLoop(gameLoopSeconds);
		window.clear(sf::Color::White);			// clear the entire window
		window.draw(backgroundSprite);			// draw the background (onto the window) 
		window.draw(backgroundSprite2);	
		game.draw();					// draw the game (onto the window)
		game2.draw();	
		window.display();				// re-display the entire window
	}
}

//-------------------------------------------------------------Leaderboard Window----------------------------------------------------------------------
//reads scores from a file to display in a new window
void openLeaderboard() {
	//struct to hold a score with associated name
	struct highScore
	{
		int score ;
		std::string name;
		//override the comparison for sorting by score only
		bool operator<(const highScore &a) const
		{
			return score < a.score;
		}
	};

	//vector to hold all highScores
	std::vector<highScore> scores;

	//string to be printed to leaderboard
	std::string scoreStr = "";
	std::string nameStr = "";

	//---------------------------Read scores from file--------------------------
	//load leaderboard
	std::ifstream leaderboardFile;
	leaderboardFile.open("data/leaderboard.txt");
	if (!leaderboardFile) {
		std::cout << "unable to load scores" << std::endl;
	}
	//read line from file
	for (std::string line; std::getline(leaderboardFile, line);) {
		std::stringstream ss(line);
		int x = 0;
		std::string name = "";
		int score;
		//separate by comma
		while (std::getline(ss, line, ',')) {
			if (x == 0) {
				name = line;
				x++;
			}
			else {
				score = std::stoi(line);
				x = 0;
			}
		}
		//create and push the highscore to vector
		highScore theScore = {score,name};
		scores.push_back(theScore);
	}
	leaderboardFile.close();
	//sort by score number using struct < override
	std::sort(std::begin(scores),std::end(scores));
	
	//update the strings to be displayed
	for (int i = scores.size() - 1; i >= 0; i--) {
		scoreStr += std::to_string(scores[i].score) + "\n";
		nameStr += scores[i].name + "\n";
	}
	//------------------------------Setup Window components--------------------------
	//background
	sf::Texture backgroundTxtr;
	sf::Sprite backgroundSp;
	if (!backgroundTxtr.loadFromFile("res/leaderboardBg.jpg")) {
		system("pause");
	}
	backgroundSp.setTexture(backgroundTxtr);
	//font
	sf::Font font;
	if (!font.loadFromFile("res/RedOctober.ttf")) {
		system("pause");
	}
	//name
	sf::Text nameTxt;
	nameTxt.setFont(font);
	nameTxt.setString(nameStr);
	nameTxt.setFillColor(sf::Color::White);
	nameTxt.setPosition(nameTxt.getPosition().x + 97, nameTxt.getPosition().y + 40);
	//score
	sf::Text scoreTxt;
	scoreTxt.setFont(font);
	scoreTxt.setString(scoreStr);
	scoreTxt.setFillColor(sf::Color::White);
	scoreTxt.setPosition(nameTxt.getPosition().x+300, nameTxt.getPosition().y);
	//widow
	sf::RenderWindow window(sf::VideoMode(547, 1000), "Leaderboard");
	window.setPosition(sf::Vector2i(window.getPosition().x - 600, 0));
	window.setIcon(200, 184, iconTexture.getPixelsPtr());
	//draw loop
	while (window.isOpen())
	{
		// handle any window or keyboard events that have occured since the last game loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::White);			// clear the entire window
		window.draw(backgroundSp);
		window.draw(scoreTxt);
		window.draw(nameTxt);
		
		window.display();				// re-display the entire window
	}

}
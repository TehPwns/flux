/* 900 Squares flux Demo
 *
 * Uses flux to create a show of 900 small squares that change position,
 * size, and alpha values.
 *
 * Public Domain
 */
#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <iostream>
#include <math.h>
#include "flux.hpp"

const char* windowTitle = "900 Tiny Squares";

/********************************************************************/
/** Square + Squares definition **/
/********************************************************************/

/* Squares definition
 */
struct square {
	square()
		: x(0), y(0), size(10), alpha(255)
	{}
	int x, y;
	float size;
	float alpha;
};


/* Squares900
 * Contains all the data for 900 Tiny Squares. Including the
 * squares themselves, and the beginning text.
 */
namespace Sq900
{
	std::array<square, 900> squares;	//900 Squares

	/* Introduction Text Information
	 */
	sf::Font sansation;
	sf::Text introText;
	float introText_alpha;

	/* Flag indicating if the current tween is the intro
	 * text, or the squares.
	 */
	bool isOnIntroText;

	/* We need to define a dummy variable to
	 * utilize flux::to with to make an "oncomplete" that
	 * can advance to the next scene automatically
	 */
	int timeDummmy;
}



/********************************************************************/
/** Utility Functions definition **/
/********************************************************************/

/* Small random float function
 * Adapted from Haaf's Game Engine 1.8
 */
unsigned int g_seed = 0;

float random_float(float min, float max)
{
	g_seed=214013*g_seed+2531011;
	return min+(g_seed>>16)*(1.0f/65535.0f)*(max-min);
}


/* drawSquare
 * Function used to draw a square on the screen
 */
void drawSquare(sf::RenderWindow& win, const square& s)
{
	sf::RectangleShape shape(sf::Vector2f(s.size, s.size));

	shape.setPosition(s.x, s.y);
	shape.setFillColor(sf::Color(255, 255, 255, s.alpha));

	win.draw(shape);
}


/* drawAllSquares
 * Function that draws all 900 squares
 */
inline void drawAllSquares(sf::RenderWindow& win)
{
	for(const square& sq : Sq900::squares)
		drawSquare(win, sq);
}


/********************************************************************/
/** Tween initialization and scene definitions using Flux **/
/********************************************************************/

void initializeTween1();

/* initializeSquarePositions
 * Sets all square position to where they need to be initially.
 * That is, starting at 71, -360 and moving outwards
 */
void initializeSquarePositions()
{
	for(int i=0; i!=30; ++i) {
		int yscale = i * (10 + 2);
	for(int j=0; j!=30; ++j) {
		/* 10 : Initial size of each square
		 * 2  : Each square separated by 2 px
		 * -360 starts them outside the screen
		 */
		Sq900::squares[30*i + j].x =   71 + j*(10 + 2);
		Sq900::squares[30*i + j].y = -360 + yscale;
	}
	}
}


/* =======================================================================
 * 10. With a still large delay, and also long duration, each square "slides"
 *	 down out the bottom of the screen. (easein / easinout function) around 9 seconds
 */
void initializeTween10()
{
	for(square& sq : Sq900::squares)
	{
		flux::to(3, &sq.y, 600).ease("circin").delay(random_float(0, 7));
	}

	//And now we loop back to the start
	flux::to(7 + 3 + 1, &Sq900::timeDummmy, 1).oncomplete(initializeTween1);
}


/* =======================================================================
 * 9. Over a large random delay, each of the squares "explode" (inout?) to
 *	random positions on the screen Over about 4.25 sec
 */
void initializeTween9()
{
	for(square& sq : Sq900::squares)
	{
		flux::to(0.60, {&sq.x, &sq.y}, {random_float(10, 490), random_float(10, 490)})
			.ease("expoinout").delay(random_float(0, 4));
	}

	flux::to(4 + 0.60, &Sq900::timeDummmy, 1).oncomplete(initializeTween10);
}


/* =======================================================================
 * 8. The squares reconstruct; they end up in the same positions.
 *	The 1st index at zero start to go to the new top-left corner of the square,
 *	and others follow. At the same time The square's half-length index
 *	fills the other half of the square left-to-right. Around 5 seconds
 */
void initializeTween8()
{
	for(int i=0; i!=15; ++i) {
		int xscale = i * (1 + 1);
	for(int j=0; j!=30; ++j) {
		square& sq1 = Sq900::squares.at(30*i + j);	 //Square first half
		square& sq2 = Sq900::squares.at(30*(i+15) + j);  //Square second half

		/* 8.5/30 works better here (about 5 sec) instead of something like 5.0/30
		 */
		flux::to(1.25, {&sq1.x, &sq1.y}, {221 + xscale, 226 + j*(1 + 1)})
			.delay((8.5/30)*i + (0.25/30)*j).ease("expoout");

		flux::to(1.25, {&sq2.x, &sq2.y}, {251 + xscale, 226 + j*(1 + 1)})
			.delay((8.5/30)*i + (0.25/30)*j).ease("expoout");
	}
	}

	flux::to((5.0 + 1.25 + 0.33) , &Sq900::timeDummmy, 1).oncomplete(initializeTween9);
}


/* =======================================================================
 * 7. A "fuse" going around the circle clockwise, making each square first
 *	big and size then quickly small.
 *	Almost exactly 4 seconds. About 3/4 of the way around, 8 starts.
 */
void initializeTween7()
{
	for(auto it = Sq900::squares.begin(); it != Sq900::squares.end(); ++it)
	{
		flux::to(0.10, &it->size, 8).ease("expoout")
			.delay( (4.0/900.0) * std::distance(Sq900::squares.begin(), it) )
			.after(0.75, 1).ease("cubicout");
	}

	//This tween does not start another, because both 7 and 8 are started by 6.
}


/* =======================================================================
 * 6. Starting from the top row, each square is placed row-by-row along a circle,
 *	whose diameter is the screen (500 px). This starts with square 0 being placed
 *	at angle 0, and the rest follow clockwise below it along a negative angle.
 *	About 3/4 of the way, 7 starts.
 *	About 9.5 seconds
 */
void initializeTween6()
{
	float theta = 0;

	for(int i=0; i!=30; ++i) {
	for(int j=0; j!=30; ++j) {
		square& sq = Sq900::squares[30*i + j];

		float x_pos = 250*cos(theta) + 250;
		float y_pos = 250*sin(theta) + 250;

		flux::to(2, {&sq.x, &sq.y}, {x_pos, y_pos})
			.delay((8.60/30)*i + (0.33/30)*j + 1 ).ease("quintout");

		theta += (2*M_PI) / 900;
	}
	}

	//Tween 7 starts about 3/4 of the way
	flux::to((8.6 + 2) * (3.0/4), &Sq900::timeDummmy, 1).oncomplete(initializeTween7);

	//Tween 8 starts when this completes
	flux::to((8.6 + 0.33 + 2), &Sq900::timeDummmy, 1).oncomplete(initializeTween8);
}


/* =======================================================================
 * 5. About half way though the previous tween (#4), starting from the top left
 *	corner each square is reduced to 1 size. This is done diagonally though
 *	the squares, like a wave
 */
void initializeTween5()
{
	/* Top half triangle of square (Across first row) */
	for(int col = 0; col != 30; ++col) {
		int n = col;
		int row = 0;
		do {
			flux::to(1.25, &Sq900::squares[30*row + n].size, 1.0).ease("quartinout")
				.delay((2.0 / 30) * col);
			--n;
			++row;
		}
		while(n >= 0);
	}

	/* Bottom half triangle of square (Down right-most column) */
	for(int row = 1; row != 30; ++row) {
		int n = row;
		int col = 29;
		do {
			flux::to(1.25, &Sq900::squares[30*n + col].size, 1).ease("quartinout")
				.delay((2.0 / 30) * (30 + row));
			--col;
			++n;
		}
		while(n < 30);
	}

	flux::to(2 + 1.25, &Sq900::timeDummmy, 1).oncomplete(initializeTween6);
}


/* =======================================================================
 * 4. Starting from the top left, each square comes back to 255 alpha.
 *	Happens all over about 7-8 seconds
 */
void initializeTween4()
{
	for(auto it = Sq900::squares.begin(); it != Sq900::squares.end(); ++it)
	{
		flux::to(3, &it->alpha, 255).ease("circout")
			.delay( (7.0/900.0) * std::distance(Sq900::squares.begin(), it) );
	}

	//Set next (#5) to start about halfway though this one
	flux::to(3.5, &Sq900::timeDummmy, 1).oncomplete(initializeTween5);
}


/* =======================================================================
 * 3. Each square randomly fades to 0 alpha with the same duration
 */
void initializeTween3()
{
	float max_delay = 0;

	for(square& sq : Sq900::squares)
	{
		float new_delay = random_float(0.0, 8);
		max_delay = std::max(max_delay, new_delay);

		flux::to(0.75, &sq.alpha, 0).delay(new_delay);
	}

	flux::to(max_delay + 0.5 + 0.5, &Sq900::timeDummmy, 1).oncomplete(initializeTween4);
}


/* =======================================================================
 * 2. Each square comes in from the top with "backout" with a random delay
 */
void initializeTween2()
{
	/* Each square has to come down a total of 360 + 66 px.
	 * -360 is where each square starts (see initializeSquarePositions)
	 * and 66 is to have them in the middle of the screen after passing Y=0.
	 * Each square takes about 1.25 seconds to fall down.
	 */
	static const int move_length =  360 + 66;

	/* Now moving out of introText, set flag */
	Sq900::isOnIntroText = false;

	/* We need to keep track of the maximum delay to start the next tween
	 * right on time each time
	 */
	float max_delay = 0;

	for(square& sq : Sq900::squares)
	{
		float new_delay = random_float(0.0, 6);
		max_delay = std::max(max_delay, new_delay);

		flux::to(1.25, &sq.y, sq.y + move_length).ease("backinout").delay(new_delay);
	}

	flux::to(max_delay + 1.25 + 0.5, &Sq900::timeDummmy, 1).oncomplete(initializeTween3);
}


/* =======================================================================
 * 1. The text "900 Tiny Squares" fades in, stays for three seconds,
 *	then fades out. Afterwards the squares start.
 * Custom draw function that draws text instead of drawAllSquares
 */
void initializeTween1()
{
	Sq900::introText_alpha = 0; //Start out fully transparent
	Sq900::isOnIntroText = true;

	initializeSquarePositions();

	for(square& sq : Sq900::squares)
		sq.size = 10;

	/* Use Flux to create the tweens. Simple!
	 * When this one finishes, start the next
	 */
	flux::to(1, &Sq900::introText_alpha, 255.0).delay(1).after(1, 0.0).delay(3)
	  .oncomplete(initializeTween2);
}

void drawIntroText(sf::RenderWindow& win)
{
	Sq900::introText.setColor(
		sf::Color(255, 255, 255, Sq900::introText_alpha));

	win.draw(Sq900::introText);
}


/********************************************************************/
/** Main Function **/
/********************************************************************/

void initialize()
{
	if(!Sq900::sansation.loadFromFile("sansation.ttf")) {
		std::cin.get();
		exit(1);
	}

	//"900 Tiny Squares"
	Sq900::introText = sf::Text(windowTitle, Sq900::sansation, 14);
	Sq900::introText.setPosition(190, 245);

	g_seed = time(NULL);
}

int main()
{
	initialize();
	initializeTween1();

	sf::RenderWindow win(sf::VideoMode(500, 500, 32), windowTitle);
	win.setFramerateLimit(60);

	sf::Clock myClock;

	/* Main Loop
	 */
	while (win.isOpen())
	{
		/* Update flux. Flux handles all scene logic
		 * like switching tweens automatically
		 */
		float dt = myClock.restart().asSeconds();
		flux::update(dt);

		sf::Event event;
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				win.close();
		}

		win.clear();

		if(Sq900::isOnIntroText) {
			drawIntroText(win);
		} else {
			drawAllSquares(win);
		}

		win.display();
	}

	return 0;
}

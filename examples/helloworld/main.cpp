/* Hello World
 *
 * Creates a sample tween on a double variable
 * over five seconds.
 *
 * Public Domain
 */
#include <iostream>
#include <chrono>
#include "flux.hpp"

/* Value to Tween */
double myValue = 0;
bool isTweenFinished = false;

void printMyValue() 
{
	std::cout << myValue << std::endl;
}

void finishTween() 
{
	isTweenFinished = true;
}

int main()
{
	/* Create a flux tween over 5 seconds on myValue from 0-100 */
    flux::to(5, &myValue, 100).ease("backinout").onupdate(printMyValue).oncomplete(finishTween);

	auto now = std::chrono::system_clock::now();
	auto prev = now;

	while(!isTweenFinished)
	{
		//Determines the number of seconds since last update
		prev = now;
		now = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = now - prev;

		flux::update(elapsed.count());
	}
	
	return 0;
}


/* Hello World
 *
 * Creates a sample tween on a double variable
 * over five seconds.
 *
 * Public Domain
 */
#include <iostream>
#include <time.h>
#include "flux.hpp"

/* Value to Tween */
double myValue = 0;
bool isTweenFinished = false;

void printMyValue() {
	std::cout << myValue << std::endl;
}
void finishTween() {
	isTweenFinished = true;
}

int main()
{
	/* Create a flux tween on myValue from 0-100
	 */
	flux::to(5, &myValue, 100).ease("backinout")
		.onupdate(printMyValue)
		.oncomplete(finishTween);

	
	/* Let's use C's time functions because they are simple.
	 */
	clock_t now, prev;
	now = clock();
	
	while(!isTweenFinished)
	{
		prev = now;
		now = clock();
		
		flux::update((float)(now - prev) / CLOCKS_PER_SEC);
	}
	
	return 0;
}
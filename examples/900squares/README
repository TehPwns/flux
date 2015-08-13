The "900 Tiny Squares" demo demonstrates flux's capability to manage a large amount of tweens, and also shows some of the neat effects possible by the library. It uses SFML 2.1 for graphics. Based off the "900 Squares" example at
https://love2d.org/forums/viewtopic.php?f=5&p=167633

The interesting part about this example is that it shows how to use flux as a state manager that uses tweens to invoke the next scene after a delay. All logic is handled by flux; it is started with only one function, and continues and loops automatically with flux::update.

You can compile this example with something like:
g++ -std=c++11 flux.cpp main.cpp -o 900squares -lsfml-graphics-s -lsfml-window-s -lsfml-system-s

==========================================================================================

1. Text "900 Tiny Squares" fades in, stays, and fades out

2. Each squares comes in from the top with "backout" with a random delay

3. Each square randomly fades to 0 alpha with the same delay

4. Starting from the top left, each square comes back to 255 alpha.
   About half way though, start from the top left corner each square is reduced to 1 size.
   This is done diagonally though the squares
   
5. About half way though the previous tween (#4), starting from the top left
   corner each square is reduced to 1 size. This is done diagonally though
   the squares. About 9/10 of the way though, 6 starts.
   
6. Starting from the top row, each square is placed row-by-row along a circle,
   whose diameter is the screen (500 px). This starts with square 0 being placed
   at angle 0, and the rest follow clockwise below it along a negative angle

   
7. A "fuse" going around the circle clockwise, making each square first big and size then quickly small.
   Almost exactly 4 seconds. About 3/4 of the way around, 8 starts.

8. The squares reconstruct; they end up in the same positions. The 1st index at zero start to go to
   the new top-left corner of the square, and others follow. At the same time The square's half-length index
   fills the other half of the square left-to-right. Around 5 seconds
   
9. Over a large random delay, each of the squares "exlpode" (inout?) to random positions on the screen
   Over about 4.25 sec

10. With a still large delay, and also long duration, each square "slides" down out the bottom of the screen.
	(easein / easinout function) around 9 seconds





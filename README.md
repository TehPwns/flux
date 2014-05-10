flux
====
A C++ port and adaption of the flux tweening library for Lua. This project assumes the ISO C++11 standard and will not compile otherwise. For additional (better) documentation, see the original Lua documentation for flux at:
https://github.com/rxi/flux

Functionality
=============
Version 0.6

Big Fixes:
 * Fixed the sine tween being incorrect (Missing minus sign)
 
 * Correctly initialized tween before start when using "after"
 
 * Tween correctly takes delay into account when using "after"

This port of flux is incomplete. Most of the features are available, including:
* All variation of the supplied tween curves (See below)
* All tween options, including `onstart`, `onupdate`, `oncomplete`, `delay`, and `after`


How to use
==========
`flux.h` and `flux.cpp` should both be added to your project's directory, in the same folder. Then add flux.cpp to your build options or command line. Then #include "flux.hpp"

###Initializing a tween
All functions and classes reside in the `flux` namespace.
A tween is created using the `flux::to` function. Currently there is one version:
```c
to<T>(float seconds, std::initializer_list<T*> ptrs, std::initializer_list<T> vals);
```
Where `T` is the type of the variable to be tweened.

* `seconds` is the amount of time the tween will last.
* `ptrs` is a {list}  of pointers to the values to be tweened.
* `vals` is a {list} corresponding to the values to tween each pointer to (the ending value).

The initializer_list versions allow multiple variables to be tweened to multiple values.

An example creation of a tween using the `to`:
```c++
double value1, value2, value3;
flux::to<double>(5, {&value1, &value2, &value3}, {200.0f, 400.0f, 600.0f});
```

This creates three simultaneous tweens for value1 to 200, value2 to 400, and value3 to 600.

By default, the easing function is `quadout`

###Updating Tweens
Currently, flux::update<T>(double deltaTime) must be called seperately for each type that is being tweened. For example, if you have `float` and `double` tweens, update<float> and update<double> need to be called seperately. All updates should be called once per main loop. `deltaTime` is the time in *seconds* since last call to the function (in other words, since last frame).
```c++
flux::update<double>(myDeltaTime);
flux::update<float>(myDeltaTime);
```


###Setting Options
Either one of the `to` functions return a reference to the new tween object. Additional options can be set by using the `.` operator on the object `to` returns. All of these options chain. These options have the same functionality as the original flux options. Here are some example of Lua and C++ correspondents.

*Lua:*
```lua
flux.to(t, 4, { x = 10 }):ease("linear"):delay(1)
```
*C++:*
```c++
flux::to<double>(4, {&t.x}, {10.0f}).ease("linear").delay(1)
```
The available `ease` options are as follows, as per original [documentation](https://github.com/rxi/flux):

  `linear`
  `quadin`       `quadout`       `quadinout`
  `cubicin`      `cubicout`      `cubicinout`
  `quartin`      `quartout`      `quartinout`
  `quintin`      `quintout`      `quintinout`
  `expoin`       `expoout`       `expoinout`
  `sinein`       `sineout`       `sineinout`
  `circin`       `circout`       `circinout`
  `backin`       `backout`       `backinout`
  `elasticin`    `elasticout`    `elasticinout`





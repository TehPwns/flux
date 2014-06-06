flux
====
A C++ port and adaption of the flux tweening library for Lua. This project assumes the ISO C++11 standard and will not compile otherwise. For additional (better) documentation, see the original Lua documentation for flux at:
https://github.com/rxi/flux

Functionality
=============
Version 1.2

**More type-generic; flux now works with different types as one might expect.**

 * The pointer and value in `to` and `after` don't have to explicitly be the same type.
 * ptrs in `after` don't have to be the same type as the previous tweens
 * Ensured all instances of using `to` use the correct one in context of possible scope errors
 * Using delay after `after` correctly adds delay instead of overwriting
 * typedef of callbackFn changed to std::function for flexibility
 * Added more friendly comments


Version 1.1
 * Implemented tween::stop() 
 * scoped flux::easing enum
 
Version 1.0
 * Arrival of generalized `update` and `to` functions
 * Implemented tween groups

Version 0.7
 * Corrected the single-variable functions.
 
Version 0.6
 * Fixed the sine tween being incorrect (Missing minus sign)
 * Correctly initialized tween before start when using "after"
 * Tween correctly takes delay into account when using "after"


How to use
==========
`flux.hpp` and `flux.cpp` should both be added to your project's directory, in the same folder. Then add flux.cpp to your build options or command line. Then #include "flux.hpp"

###Initializing a tween
All functions and classes reside in the `flux` namespace.
A tween is created using the `flux::to` function. Currently there are two  versions:
```c
to(float seconds, std::initializer_list<T1*> ptrs, std::initializer_list<T2> vals);
to(float seconds, T1* ptr, T2 val);
```
Where T1 and T2 are types such that T2 is convertible to T1. The type of the tween becomes of T1. You do not need template arguments on the `to` functions as of 1.0.

* `seconds` is the amount of time the tween will last.
* `ptrs` is a {list}  of pointers to the values to be tweened.
* `vals` is a {list} corresponding to the values to tween each pointer to (the ending value).
* For the first version, only one variable is specified instead of a list.

The initializer_list versions allow multiple variables to be tweened to multiple values.

An example creation of some tweens using `to`:
```c++
double value1=0, value2=0, value3=0;
flux::to(5, {&value1, &value2, &value3}, {200.0, 400.0, 600.0});
flux::to(5, &value1, 300);

//The following is also valid (1.2):
flux::to(5, {&value1, &value2, &value3}, {100, 200, 300});
```

The first version creates three simultaneous tweens for value1 to 200, value2 to 400, and value3 to 600. The second only creates one tween for value1.

By default, the easing function is `quadout`

###Updating Tweens
To update all tweens held by the flux namespace, make a single call to `flux::update(float dt)`, with the time since the last frame in seconds. This will now update all current tweens.
```c++
flux::update(myDeltaTime);
```


###Setting Options
Either one of the `to` functions return a reference to the new tween object. Additional options can be set by using the `.` operator on the object `to` returns. All of these options chain. These options have the same functionality as the original flux options. *The syntax for a callbackFn is a function returning void taking no arguments.*

`ease(const char* type)` `ease(flux::easing type)`: Sets the easing curve for the set of tweens being defined.

`after(...)`: Register a tween to take place after the previous one finishes. All options after this apply to the new tween. This option uses the same syntax as `to`.

`onstart(callbackFn)`: Add a function to be called when the tween starts. Call more than once to add multiple functions. See format for a *callbackFn* above.

`onupdate(callbackFn)`: Add functions to be called each time the tween updates.

`oncomplete(callbackFn)`: Add functions to be called when the tween completes.

`delay(float sec)`: Do not start the tween immediately; *add* this amount of delay. Multiple calls to `delay` add onto previous delay time.

`stop(void)`: Immediately stops a tween. The `oncomplete` functions are not called. To use this function, keep a reference to the tween created from `to`.

```c++
auto myTween = flux::to(...).ease(...).oncomplete(myfunction);
...
myTween.stop();  //Tween stopped and myfunction not called
```

An example of Lua and C++ correspondents:

*Lua:*
```lua
flux.to(t, 4, { x = 10 }):ease("linear"):delay(1)
```
*C++:*
```c++
flux::to(4, &t.x, 10.0).ease("linear").delay(1)
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
  
###Tween groups
Flux allows tween objects separate from the flux namespace called groups. Groups contain their own tweens and must have their own `update` and `to` methods called separately. Groups are useful to single out and own a set of tweens that may not be updated with the main flux namesapce.

As with the flux namespace, `to` returns a reference to a chainable tween object, and all other settings apply.

Usage of groups:
```c++
flux::group MyGroup = flux::group();	//flux::group MyGroup;
MyGroup.to(...);
MyGroup.to(...);
...
MyGroup.update(dt);
```

==========================================================================================

###C++ Caveats

I want to outline some internal library behavior that might be of importance when using C++.

* The callback functions are called backwards, because use of a forward_list and push_front.

* When using one of the T1 and T2 functions, ie, `to` or `after`, the tween object returned is always of type `tween<T1>` (Of the same type of the targets of the pointers).

* tween::stop() is a linear lookup in a list. Wasn't meant for performance in mind.

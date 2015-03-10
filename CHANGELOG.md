Version 1.2.7
 * Fixed a compilation error on some Linux machines
 * Project is now single-folder

Version 1.2.6
 * Fixed mysterious errors that went undetected with `after` functions
 * Makefiles added for examples

Version 1.2.5
 * Functionality: Added ability to omit pointers from `after`. Doing so will create a tween on the previous variable. All `after` functions are also now inline.
 * Fixed a possibility for memory leaks

Version 1.2
 * More type-generic; flux now works with different types as one might expect.
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


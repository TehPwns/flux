/*
 Copyright (c) 2014 TehPwns
 Orignal Flux Lua library Copyright (c) 2014 rxi

 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use
 of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not claim that
    you wrote the original software. If you use this software in a product, an
    acknowledgment in the product documentation would be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be misrepresented as
    being the original software.

 3. This notice may not be removed or altered from any source distribution.

 contact: p00n3dj002@yahoo.com
*/
#ifndef FLUX_CPP_CPP
#define FLUX_CPP_CPP

#include <utility>
#include <type_traits>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>
#include "flux.hpp"

namespace flux
{
namespace impl
{
	typedef double (*TweenFunc)(double t);
	typedef double (*TweenModidyFunc)(TweenFunc& fn, double t);

	static double TweenFunc_Linear(double t) {
		return t;
	}
	static double TweenFunc_Quad(double t) {
		return t * t;
	}
	static double TweenFunc_Cubic(double t) {
		return t * t * t;
	}
	static double TweenFunc_Quart(double t) {
		return t * t * t * t;
	}
	static double TweenFunc_Quint(double t) {
		return t * t * t * t * t;
	}
	static double TweenFunc_Expo(double t) {
		return pow(2, 10*(t - 1));
	}
	static double TweenFunc_Sine(double t) {
		return -cos(t * (M_PI_2)) + 1;
	}
	static double TweenFunc_Circ(double t) {
		return -sqrt(1 - (t * t)) + 1;
	}
	static double TweenFunc_Back(double t) {
		return t * t * (2.7 * t - 1.7);
	}
	static double TweenFunc_Elastic(double t) {
		return -pow(2, 10 * (t - 1)) * sin((t - 1.075) * (2*M_PI) / .3);
	}

	static double TweenModidyFunc_easeIn(TweenFunc& fn, double t) {
		return fn(t);
	}
	static double TweenModidyFunc_easeOut(TweenFunc& fn, double t) {
		t = 1 - t;
		return 1 - fn(t);
	}
	static double TweenModidyFunc_easeInOut(TweenFunc& fn, double t) {
		t *= 2;
		if(t < 1) {
			return 0.5f * fn(t);
		} else {
			t = 2 - t;
			return 0.5f * (1 - fn(t)) + 0.5f;
		}
	}

	static TweenFunc easingTable[] = {
		TweenFunc_Linear, 	TweenFunc_Quad,
		TweenFunc_Cubic,	TweenFunc_Quart,
		TweenFunc_Quint,	TweenFunc_Expo,
		TweenFunc_Sine,		TweenFunc_Circ,
		TweenFunc_Back,		TweenFunc_Elastic,
	};

	static TweenModidyFunc modifyTable[] = {
		TweenModidyFunc_easeIn,		TweenModidyFunc_easeOut,
		TweenModidyFunc_easeInOut
	};

	/********************************************************************/

	static std::unordered_map<const char*, easing> stringToEasingMap = {
		{"linear", easing::linear},
		{"quadin", easing::quadin},  {"quadout", easing::quadout},  {"quadinout", easing::quadinout},
		{"cubicin", easing::cubicin},{"cubicout", easing::cubicout},{"cubicinout", easing::cubicinout},
		{"quartin", easing::quartin},{"quartout", easing::quartout},{"quartinout", easing::quartinout},
		{"quintin", easing::quintin},{"quintout", easing::quintout},{"quintinout", easing::quintinout},
		{"expoin", easing::expoin},  {"expoout", easing::expoout}, 	{"expoinout", easing::expoinout},
		{"sinein", easing::sinein},  {"sineout", easing::sineout},  {"sineinout", easing::sineinout},
		{"circin", easing::circin},  {"circout", easing::circout},  {"circinout", easing::circinout},
		{"backin", easing::backin},  {"backout", easing::backout},  {"backinout", easing::backinout},
		{"elasticin", easing::elasticin}, {"elasticout", easing::elasticout}, {"elasticinout", easing::elasticinout}
	};
} //namespace impl

	/********************************************************************/

	template<typename T>
	void tween<T>::initialize()
	{
		//Initialize the tween.
		vars.reserve(my_initPtrs.size());
		auto it_p = my_initPtrs.begin();
		auto it_v = my_initVals.begin();

		for(; it_p != my_initPtrs.end(); ++it_p, ++it_v)
		{
			T start = *(*it_p); //Dereferencing original pointer to tweened variable
			T end   = *it_v;
			T diff  = end - start;

			vars.emplace_back(start, diff, *it_p);
		}
	}

	template<typename T>
	tween<T>& tween<T>::ease(easing type)
	{
		this->easeFuncIndex = type / 10; 	//Integer division
		this->modFuncIndex = type % 10;
		return *this;
	}

	template<typename T>
	tween<T>& tween<T>::ease(const char* type)
	{
		auto it = impl::stringToEasingMap.find(type);
		if(it != impl::stringToEasingMap.end())
			return ease(it->second);
		else {
			return ease(easing::quadout);	// Invalid input string - return default?
		}
	}

	template<typename T>
	tween<T>& tween<T>::onstart(void (*fn)(void))
	{
		callbacks_onstart.push_front(fn);
		return *this;
	}

	template<typename T>
	tween<T>& tween<T>::onupdate(void (*fn)(void))
	{
		callbacks_onupdate.push_front(fn);
		return *this;
	}

	template<typename T>
	tween<T>& tween<T>::oncomplete(void (*fn)(void))
	{
		callbacks_oncomplete.push_front(fn);
		return *this;
	}

	template<typename T>
	tween<T>& tween<T>::delay(float sec)
	{
		this->start_delay = sec;
		return *this;
	}

	template<typename T>
	tween<T>& tween<T>::after(float seconds, T* ptr, T val)
	{
		return after(seconds, {ptrs}, {vals});
	}

	template<typename T>
	tween<T>& tween<T>::after(float seconds, std::initializer_list<T*> ptrs, std::initializer_list<T> vals)
	{
		return to(seconds, ptrs, vals).delay(start_delay + ((rate != 0) ? (1 / rate) : 0));
	}

	/********************************************************************/

	template<typename T>
	tween<T>& to(float seconds, T* ptr, T val)
	{
		return to(seconds, {ptr}, {val});
	}

	template<typename T>
	tween<T>& to(float seconds, std::initializer_list<T*> ptrs, std::initializer_list<T> vals)
	{
		//Tween initialization. The tween's "constructor" outside of the class.
		tween<T> New;
		New.inited = false;
		New.rate  = (seconds > 0) ? (1 / seconds) : 0;
		New.time  = (New.rate > 0) ? 0 : 1;
		New.start_delay = 0;
		New.easeFuncIndex = New.modFuncIndex  = 1; //Quadout is default tween
		New.my_initPtrs = ptrs;
		New.my_initVals = vals;

		tween<T>::tweens.push_back(New);
		return tween<T>::tweens.back();
	}

	template<typename T>
	void update(double deltaTime)
	{
		for(auto it = tween<T>::tweens.begin(); it != tween<T>::tweens.end(); ++it)
		{
			auto& t = *it;

			if(t.start_delay > 0) {
				t.start_delay -= deltaTime;
				continue;
			}

			if(t.inited == false) {
			    t.inited = true;
			    t.initialize();

				for(callbackFn& fn : t.callbacks_onstart) fn();
				t.callbacks_onstart.clear();
			}

			t.time = t.time + (t.rate * deltaTime);
			double p = t.time;
			double x = (p >= 1) ? 1 :
			    impl::modifyTable[t.modFuncIndex](impl::easingTable[t.easeFuncIndex], p);

			for(auto& var : t.vars)
				*(var.variable) = var.start + (x * var.diff);

			if(!t.callbacks_onupdate.empty())
				for(callbackFn& fn : t.callbacks_onupdate) fn();

			if(p >= 1) {
				if(!t.callbacks_oncomplete.empty())
					for(callbackFn& fn : t.callbacks_oncomplete) fn();

				auto remove_item = it; it++;
				tween<T>::tweens.erase(remove_item);
			}
		}
	}
}

#endif

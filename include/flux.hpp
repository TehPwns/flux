/*
 Copyright (c) 2014 TehPwns
 Original Flux Lua library Copyright (c) 2014 rxi

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
#ifndef FLUX_CPP_H
#define FLUX_CPP_H

#include <vector>
#include <list>
#include <map>
#include <forward_list>
#include <initializer_list>
#include <typeindex>

namespace flux
{
	enum easing {
		linear = 0,
		quadin = 10,   quadout,   quadinout,
		cubicin = 20,  cubicout,  cubicinout,
		quartin = 30,  quartout,  quartinout,
		quintin = 40,  quintout,  quintinout,
		expoin = 50,   expoout,   expoinout,
		sinein = 60,   sineout,   sineinout,
		circin = 70,   circout,   circinout,
		backin = 80,   backout,   backinout,
		elasticin = 90,elasticout,elasticinout
	};

	/********************************************************************/
	/** Forward declarations and general namespace functions
	/********************************************************************/

	typedef void (*callbackFn)(void);

	template<typename T> class tween;   //Some forward declarations
	class group;

	template<typename T>
	inline tween<T>& to(float seconds, std::initializer_list<T*> ptrs, std::initializer_list<T> vals);

	template<typename T>
	inline auto to(float seconds, T* ptr, T val) -> decltype(to(seconds, {ptr}, {val}));

	inline void update(double deltaTime);


	/********************************************************************/
	/** Tween class definition
	/********************************************************************/

	template<typename T> class tween
	{
	public:
		tween<T>& ease(easing type);
		tween<T>& ease(const char* type);
		tween<T>& onstart(void (*fn)(void));
		tween<T>& onupdate(void (*fn)(void));
		tween<T>& oncomplete(void (*fn)(void));
		tween<T>& delay(float sec);
		tween<T>& after(float seconds, std::initializer_list<T*> ptrs, std::initializer_list<T> vals);
		tween<T>& after(float seconds, T* ptr, T val);
		bool update(double deltaTime);
	private:
		void initialize();
		bool    inited;
		bool	finished;
		double 	time;
		float 	rate;
		float 	start_delay;
		short 	easeFuncIndex;
		short 	modFuncIndex;

		struct var_info
		{
			var_info(float s, float d, T* var) : start(s), diff(d), variable(var) {}
			float start, diff;
			T* variable;
		};
		std::vector<var_info> vars;
		std::list<T*> my_initPtrs;
		std::list<T>  my_initVals;
		std::forward_list<callbackFn> callbacks_onstart;
		std::forward_list<callbackFn> callbacks_onupdate;
		std::forward_list<callbackFn> callbacks_oncomplete;
		
		friend class group;
	};


	/********************************************************************/
	/** Internal implementation-namespace abstraction items
	/** (Allows one update() for all types, other badassery)
	/********************************************************************/

namespace impl
{
    	class GenericTweenList
	{
	public:
		virtual ~GenericTweenList() {}
		virtual bool update(double dt) = 0;
	};

	template<typename T> class TweenList : GenericTweenList
	{
	public:
		bool update(double deltaTime) override;
	private:
		std::list<tween<T>> mTweens;
		friend class flux::group;
	};

}   //namespace impl

	/********************************************************************/
	/** flux::group definition
	/********************************************************************/

	class group
	{
	public:
		template<typename T>
		tween<T>& to(float seconds, std::initializer_list<T*> ptrs, std::initializer_list<T> vals);

		template<typename T>
		tween<T>& to(float seconds, T* ptr, T val);

		inline void update(double deltaTime);
	private:
		template<typename T>
		impl::TweenList<T>* getTweens();

		std::map<std::type_index, impl::GenericTweenList*> mTweensLists;
	};
}

#include "flux.cpp"

#endif

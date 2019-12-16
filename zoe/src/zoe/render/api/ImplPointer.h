/*
 * ImplPointer.h
 *
 *  Created on: 14.12.2019
 *      Author: florian
 */

#pragma once

#include "../../Core.h"

namespace Zoe{

template<typename T>
class DLL_PUBLIC ImplPointer{
public:

	ImplPointer(): t(nullptr), references(nullptr){}
	ImplPointer(T* t): t(t){
		references = new unsigned int();
		*references = 1;
	}
	ImplPointer(const ImplPointer& copy){
		t = copy.t;
		references = copy.references;
		++(*references);
	}
	ImplPointer& operator=(const ImplPointer& assign){
		if(references == assign.references){ //if same object, why copy?
			return *this;
		}

		//~ImplPointer()
		if((*references) == 1){
			delete references;
			delete t;
		}else{
			--(*references);
		}

		//ImplPointer(assign)
		t = assign.t;
		references = assign.references;
		++(*references);
		return *this;
	}
	~ImplPointer(){
		if((*references) == 1){
			delete references;
			delete t;
		}else{
			--(*references);
		}
	}
	const T* operator->() const{
		return t;
	}

	const T* operator&() const{
		return t;
	}

	T* operator->(){
		return t;
	}

	T* operator&(){
		return t;
	}

private:
	T* t;
	unsigned int* references;
};

}



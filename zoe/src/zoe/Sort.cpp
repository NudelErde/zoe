/*
 * Sort.cpp
 *
 *  Created on: 03.05.2019
 *      Author: florian
 */

#include "Sort.h"

template<typename numType>
void Zoe::bubbleSort(numType* array,unsigned int length){
	bool working;
	unsigned int iteration = 1;
	do{
		working = false;
		for (unsigned int i = 0; i < length-iteration; ++i) {
			if(array[i]>array[i+1]){
				numType dummy = array[i];
				array[i] = array[i+1];
				array[i+1] = dummy;
				working = true;
			}
		}
		iteration++;
	}while(working);
}

template<typename numType>
void Zoe::selectionSort(numType* array,unsigned int length){
	unsigned int index = 0;
	for (unsigned int iter = 0; iter < length; ++iter) {
		index = iter;
		for (unsigned int i = iter; i < length; ++i) {
			if(array[i]<array[index]){
				index = i;
			}
		}
		numType dummy=array[index];
		array[index] = array[iter];
		array[iter]=dummy;
	}
}


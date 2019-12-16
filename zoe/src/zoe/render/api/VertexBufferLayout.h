/*
 * VertexBufferLayout.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "VertexBuffer.h"
#include "ImplPointer.h"

namespace Zoe{

class VertexBufferLayoutImpl{
public:
	VertexBufferLayoutImpl(){}
	virtual ~VertexBufferLayoutImpl(){}

	virtual void push_float(unsigned int count)=0;
	virtual void push_uint(unsigned int count)=0;
	virtual void push_uchar(unsigned int count)=0;
};

class DLL_PUBLIC VertexBufferLayout{
public:
	VertexBufferLayout(VertexBufferLayoutImpl* impl): impl(impl){}
	~VertexBufferLayout(){}

	inline void push_float(unsigned int count){impl->push_float(count);}
	inline void push_uint(unsigned int count){impl->push_uint(count);}
	inline void push_uchar(unsigned int count){impl->push_uchar(count);}

	inline VertexBufferLayoutImpl* getImpl() {return &impl;}
private:
	ImplPointer<VertexBufferLayoutImpl> impl;
};

}

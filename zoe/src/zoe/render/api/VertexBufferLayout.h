/*
 * VertexBufferLayout.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "VertexBuffer.h"
#include "ImplPointer.h"
#include "../GraphicsContext.h"

namespace Zoe{

class VertexBufferLayoutImpl{
public:
	VertexBufferLayoutImpl(): id(GraphicsContext::generateID()){}
	virtual ~VertexBufferLayoutImpl(){}

	virtual void push_float(unsigned int count)=0;
	virtual void push_uint(unsigned int count)=0;
	virtual void push_uchar(unsigned int count)=0;

    inline const int& getID() const{ return id;};

private:
    int id;
};

class DLL_PUBLIC VertexBufferLayout{
public:
	VertexBufferLayout(VertexBufferLayoutImpl* impl): impl(impl){}
	~VertexBufferLayout(){}

	inline void push_float(unsigned int count){impl->push_float(count);}
	inline void push_uint(unsigned int count){impl->push_uint(count);}
	inline void push_uchar(unsigned int count){impl->push_uchar(count);}

	inline VertexBufferLayoutImpl* getImpl() {return &impl;}

    inline const int& getID() const { return impl->getID();}
private:
	ImplPointer<VertexBufferLayoutImpl> impl;
};

}

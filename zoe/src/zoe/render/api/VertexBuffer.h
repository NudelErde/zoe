/*
 * VertexBuffer.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../Core.h"
#include "ImplPointer.h"
#include "../GraphicsContext.h"

namespace Zoe{

class VertexBufferImpl{
public:
	VertexBufferImpl(GraphicsContext* context): context(context), id(GraphicsContext::generateID()){}
	virtual ~VertexBufferImpl(){}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void setData(const void* data,unsigned int size) = 0;
	virtual void* getData(unsigned int offset,unsigned int size) = 0;

    inline const int& getID() const{ return id;};

protected:
	GraphicsContext* context;

	int id;
};

class DLL_PUBLIC VertexBuffer{
public:
	VertexBuffer(VertexBufferImpl* impl): impl(impl){}
	~VertexBuffer(){}

	inline void bind(){impl->bind();}
	inline void unbind(){impl->unbind();}

	inline void setData(const void* data,unsigned int size){impl->setData(data,size);}
	inline void* getData(unsigned int offset,unsigned int size){return impl->getData(offset, size);}

	inline VertexBufferImpl* getImpl(){return &impl;}

    inline const int& getID() const { return impl->getID();}
private:
	ImplPointer<VertexBufferImpl> impl;
};

}

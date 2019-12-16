/*
 * IndexBuffer.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../Core.h"
#include "ImplPointer.h"

namespace Zoe{

class GraphicsContext;

class IndexBufferImpl{
public:
	IndexBufferImpl(GraphicsContext* context): context(context){}
	virtual ~IndexBufferImpl(){}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void setData(unsigned int* data,unsigned int count) = 0;
	virtual unsigned int* getData(unsigned int offset,unsigned int count) = 0;

	virtual const unsigned int& getCount() const = 0;

protected:
	GraphicsContext* context;
};

class DLL_PUBLIC IndexBuffer{
public:
	IndexBuffer(IndexBufferImpl* impl): impl(impl){}
	~IndexBuffer(){}

	inline void bind() {impl->bind();}
	inline void unbind() {impl->unbind();}

	inline void setData(unsigned int* data,unsigned int count) {impl->setData(data, count);};
	inline unsigned int* getData(unsigned int offset,unsigned int count) {return impl->getData(offset, count);};

	inline const unsigned int& getCount() const{return impl->getCount();}

	inline IndexBufferImpl* getImpl() {return &impl;}
private:
	ImplPointer<IndexBufferImpl> impl;
};

}

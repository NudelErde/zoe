/*
 * VertexArray.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "ImplPointer.h"

namespace Zoe{

class GraphicsContext;

class VertexArrayImpl{
public:

	VertexArrayImpl(GraphicsContext* context): context(context){}
	virtual ~VertexArrayImpl(){}

	virtual void set(VertexBuffer& vb,IndexBuffer& ib,VertexBufferLayout& layout) = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual IndexBufferImpl& getIndexBuffer() = 0;
protected:
	GraphicsContext* context;
};

class DLL_PUBLIC VertexArray{
public:

	VertexArray(VertexArrayImpl* impl): impl(impl){}
	~VertexArray(){}

	inline void set(VertexBuffer& vb,IndexBuffer& ib,VertexBufferLayout& layout){impl->set(vb,ib,layout);}

	inline void bind(){impl->bind();}
	inline void unbind(){impl->unbind();}

	inline VertexArrayImpl* getImpl() {return &impl;}
private:
	ImplPointer<VertexArrayImpl> impl;
};

}

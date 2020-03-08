/*
 * Model.h
 *
 *  Created on: 30.04.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include "api/VertexBuffer.h"
#include "api/IndexBuffer.h"
#include "../math/mat.h"

#include <vector>

namespace Zoe {

class DLL_PUBLIC Model2 {
public:
	Model2();
	virtual ~Model2();

	void flushBuffers(VertexBuffer& vb, IndexBuffer& ib);
	mat3x3 getModelViewMatrix();

	unsigned int pushVertex(float x,float y,float r,float g,float b);
	void pushIndices(unsigned int i0,unsigned int i1,unsigned int i2);

	void pushTriangle(float x0,float x1,float x2,float y0,float y1,float y2,float r0,float r1,float r2,float g0,float g1,float g2,float b0,float b1,float b2);

	void setPosition(float x,float y);
	void setRotation(float angle);
	void setScale(float width,float height);
private:
	unsigned int count,indexCount;
	std::vector<float> buffer;
	std::vector<unsigned int> indices;
	float x,y,angle,width,height;
};

} /* namespace Zoe */

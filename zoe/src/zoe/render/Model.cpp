/*
 * Model.cpp
 *
 *  Created on: 30.04.2019
 *      Author: florian
 */

#include "Model.h"

namespace Zoe {

Model2::Model2(): count(0),indexCount(0),x(0),y(0),angle(0),width(1),height(1) {

}

Model2::~Model2() {

}

void Model2::flushBuffers(VertexBuffer& vb, IndexBuffer& ib) {
	vb.setData(buffer.data(), this->count*sizeof(float));
	ib.setData(indices.data(), indexCount);
}

mat3x3 Model2::getModelViewMatrix() {
	return translate2D(x, y)*rotate2D(angle)*scale2D(width, height);
}

unsigned int Model2::pushVertex(float x, float y, float r, float g, float b) {
	buffer.push_back(x);
	buffer.push_back(y);
	buffer.push_back(r);
	buffer.push_back(g);
	buffer.push_back(b);
	count++;
	return count-1;
}

void Model2::pushIndices(unsigned int i0,unsigned int i1,unsigned int i2){
	indices.push_back(i0);
	indices.push_back(i1);
	indices.push_back(i2);
	indexCount++;
}


void Model2::pushTriangle(float x0,float x1,float x2,float y0,float y1,float y2,float r0,float r1,float r2,float g0,float g1,float g2,float b0,float b1,float b2){
	unsigned int i0 = pushVertex(x0,y0,r0,g0,b0);
	unsigned int i1 = pushVertex(x1,y1,r1,g1,b1);
	unsigned int i2 = pushVertex(x2,y2,r2,g2,b2);
	pushIndices(i0, i1, i2);
}

void Model2::setPosition(float x, float y) {
	this->x=x;
	this->y=y;
}

void Model2::setRotation(float angle){
	this->angle = angle;
}

void Model2::setScale(float width,float height){
	this->width = width;
	this->height = height;
}

}

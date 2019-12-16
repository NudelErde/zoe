/*
 * Material.h
 *
 *  Created on: 25.06.2019
 *      Author: florian
 */

#pragma once

#include "Core.h"
#include "math/mat.h"
#include "render/api/Shader.h"

namespace Zoe{

class DLL_PUBLIC Material2D{
public:

	void bind(mat3x3& projectionMatrix,mat3x3& modelViewMatrix);

private:
	Shader* shader;
	void(*uniformLoader)(Shader*);
	std::string projectionMatrixName;
	std::string modelViewMatrixName;
};

}

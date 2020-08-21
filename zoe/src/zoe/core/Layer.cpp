/*
 * Layer.cpp
 *
 *  Created on: 11.04.2019
 *      Author: florian
 */

#include "Layer.h"

#include <utility>

namespace Zoe {

Layer::Layer(std::string  name): name(std::move(name)) {}

Layer::~Layer() = default;

} /* namespace Zoe */

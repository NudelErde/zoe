//
// Created by Florian on 21.06.2020.
//

#include "GraphicsContext.h"

const int &Zoe::GraphicsContext::generateID() {
    static int id = 0;
    return ++id;
}

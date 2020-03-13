//
// Created by florian on 12.03.20.
//

#include <zoe.h>

using namespace Zoe;

int main(){
    vec3 s = {5,6,15};
    vec3 o = {8,8,10};
    vec3 r = {6,4,0};

    vec3 n = (o-s).crossProduct(r).crossProduct(r);
    info(n);
    n = n.normalize();
    float number = n*s;
    info(number);
}
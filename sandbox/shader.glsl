#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec4 v_color;
out vec2 v_pos;

uniform mat4 Projection;

void main(){
    v_pos = position.xy;
    vec4 pos = vec4(position, 0, 1);
    gl_Position = Projection  * pos;
}

#shader fragment
#version 130

in vec2 v_pos;

out vec4 color;

uniform float maxIterations;
uniform vec2 juliaConst;
uniform float mandelJuliaRatio;

//(a+bi)(c+di) = a*c + a*di + bi*c -b*d

float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

vec2 multiply(vec2 a, vec2 b){
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 pow(vec2 z, float n){
    vec2 zn = z;
    for(int i = 1; i < n; ++i){
        zn = multiply(z, zn);
    }
    return zn;
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){ // (a+bi)³ = 1331 a³ + 3a²bi - 3ab² - b³i
    vec2 z = 0.01*v_pos-vec2(1,0.27);
    vec2 c = juliaConst * (1 - mandelJuliaRatio) + mandelJuliaRatio * z;
    int iterations = 0;
    while (length(z) < 4 && iterations < maxIterations){
        z = pow(z, 2) + c;
        //z.x*z.x*z.x - 3*z.x*z.y*z.y, 3*z.x*z.x*z.y - z.y*z.y*z.y    z³
        //z.x*z.x-z.y*z.y, 2*z.x*z.y 			z²
        ++iterations;
    }
    if(iterations == maxIterations){
        color = vec4(0,0,0,1);
    }else{
        color = vec4(hsv2rgb(vec3(10 * iterations / maxIterations, 1, 1)), 1);
    }
}
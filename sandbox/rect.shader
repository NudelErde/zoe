#shader vertex
#version 130

layout(location=0) in vec2 position;

uniform mat4 Projection;

void main(){
    vec4 pos = vec4(position, 0, 1);
    gl_Position = Projection  * pos;
}

#shader fragment
#version 130

out vec4 color;
uniform vec3 u_color;

void main(){ // (a+bi)³ = 1331 a³ + 3a²bi - 3ab² - b³i
    color = vec4(u_color, 1);
}
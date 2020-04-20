#tag modelMatrix model
#tag viewMatrix view
#tag projectionMatrix projection
#shader vertex
#version 130

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texturePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * position;
}

#shader fragment
#version 130

out vec4 color;

void main(){
    color = vec4(1,0,0,1);
}

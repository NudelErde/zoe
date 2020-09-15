#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec4 v_color;

uniform mat4 ModelViewProjection;
uniform vec4 Color;

void main(){
    vec4 pos = vec4(position,0,1);
    gl_Position = ModelViewProjection * pos;
    v_color = Color;
}

#shader fragment
#version 130

in vec4 v_color;

out vec4 color;

void main(){
    color = v_color;
}
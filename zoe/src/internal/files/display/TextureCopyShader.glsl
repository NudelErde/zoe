#shader vertex
#version 130

layout(location=0) in vec2 position;
layout(location=1) in vec2 texPosition;

out vec2 v_texPos;

void main(){
    gl_Position = vec4(position,0,1);
    v_texPos = texPosition;
}

#shader fragment
#version 130

in vec2 v_texPos;

out vec4 color;

uniform sampler2D u_texture;

void main(){
    color = texture2D(u_texture, v_texPos);
}
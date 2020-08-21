#shader vertex
#version 130

layout(location=0) in vec2 position;
layout(location=1) in vec2 texPosition;

out vec2 v_texPosition;

uniform mat4 ModelViewProjection;

void main(){
    gl_Position = ModelViewProjection * vec4(position,0,1);
    v_texPosition = texPosition;
}

#shader fragment
#version 130

in vec2 v_texPosition;

out vec4 color;

uniform vec4 u_textColor;
uniform sampler2D tex;

void main(){
    color = u_textColor*texture2D(tex,v_texPosition).r;
}
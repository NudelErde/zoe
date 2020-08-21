#shader vertex
#version 130

layout(location=0) in vec2 position;

out vec2 v_pos;

uniform mat4 ModelViewProjection;

void main(){
    vec4 pos = vec4(position,0,1);
    gl_Position = ModelViewProjection * pos;
    v_pos = position;
}

#shader fragment
#version 130

in vec2 v_pos;

out vec4 color;

uniform sampler2D u_image;

void main(){
    color = texture2D(u_image, v_pos);
}
#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec4 v_color;
out vec2 v_pos;

uniform mat4 ModelViewProjection;
uniform vec4 Color;

void main(){
    v_pos = position.xy;
    vec4 pos = vec4(position,0,1);
    gl_Position = ModelViewProjection * pos;
    v_color = Color;
}

#shader fragment
#version 130

in vec4 v_color;
in vec2 v_pos;

out vec4 color;

float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main(){
    float length = length(v_pos);
    if(length>1){
        discard;
    }
    if(length>0.95){
        color = v_color * vec4(1,1,1,map(length,0.95,1,1,0));
    }
    else{
        color = v_color;
    }
}
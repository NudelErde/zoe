#tag modelMatrix model
#tag viewMatrix view
#tag projectionMatrix projection
#tag modelMatrix_it modelIT
#shader vertex
#version 130

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texturePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelIT;

out vec4 v_position;
out vec3 v_normal;

void main(){
    gl_Position = projection * view * model * position;
    v_position = model * position;
    v_normal = mat3(modelIT) * normal;
}

#shader fragment
#version 130

out vec4 color;

in vec4 v_position;
in vec3 v_normal;

uniform vec4 lightSource;
uniform vec3 lightColor;
uniform vec3 ambiLight;

void main(){
    vec3 lightDirection = normalize((lightSource - v_position).xyz);
    float coef = max(dot(v_normal, lightDirection), 0.0);
    vec3 light = coef * lightColor * (1-ambiLight) + ambiLight;
    vec3 preColor = vec3(1, 1, 1);
    color = vec4(preColor * light, 1);
}

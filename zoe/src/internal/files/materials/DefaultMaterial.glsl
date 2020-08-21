#tag modelMatrix model
#tag viewMatrix view
#tag projectionMatrix projection
#tag modelMatrix_it normalTranslationMatrix
#shader vertex
#version 130

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texturePosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalTranslationMatrix;

out vec3 v_normal;

void main() {
    gl_Position = projection * view * model * position;
    v_normal = vec3(normalTranslationMatrix * vec4(normal, 0));
}

#shader fragment
#version 130

uniform sampler2D u_texture;

in vec3 v_normal;
out vec4 color;

void main() {
    float diff = max(dot(normalize(v_normal), normalize(vec3(1,1,0))), 0.0);
    color = vec4(vec3(1,1,1) * (0.2 + diff*0.8), 1.0);
}
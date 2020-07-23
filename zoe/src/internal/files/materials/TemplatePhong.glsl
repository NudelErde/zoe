#tag modelMatrix model
#tag viewMatrix view
#tag projectionMatrix projection
#tag modelMatrix_it model_it
#shader vertex
#version 130

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texturePosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_normal;
out vec2 v_texturePosition;

void main() {
    gl_Position = projection * view * model * position;
    v_normal = normal;
    v_texturePosition = texturePosition;
}

#shader fragment
#version 130

uniform sampler2D u_texture;

uniform mat4 model_it;

uniform vec3 ambientLight;
uniform vec3 lightColor;

in vec3 v_normal;
in vec2 v_texturePosition;

uniform vec4 cameraPosition;
uniform vec4 lightPosition;

void main() {
    vec3 normalVector = normalize(mat3(model_it)*v_normal);
    vec3 color = texture2D(u_texture, v_texturePosition).xyz;

    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightPosition, normalVector);

    float diff = max(dot(normalVector, normalize(lightPosition - FragPos)), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    FragColor = vec4(color * (ambientLight + (diff*lightColor) + (spec*lightColor)), 1.0);
}
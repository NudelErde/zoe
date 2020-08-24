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
uniform mat4 model_it;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texturePosition;

void main() {
    v_position = (model * position).xyz;
    v_normal = normalize(mat3(model_it)*normal);
    v_texturePosition = texturePosition;
    gl_Position = projection * view * model * position;
}

    #shader fragment
    #version 130

uniform vec3 ambientReflectivity_mtl;
uniform vec3 diffuseReflectivity_mtl;
uniform vec3 specularReflectivity_mtl;
uniform float specularExponent_mtl;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float specularIntensity;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texturePosition;

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;

out vec4 outColor;

void main() {
    vec3 ambientResult = ambientReflectivity_mtl;

    vec3 lightDirection = normalize(lightPosition - v_position);
    float diff = max(dot(v_normal, lightDirection), 0.0);
    vec3 diffuseResult = diff * lightColor * diffuseReflectivity_mtl;

    vec3 viewDir = normalize(v_position - cameraPosition);
    vec3 reflectDir = reflect(lightDirection, v_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent_mtl);


    vec3 specularResult = spec * lightColor * specularReflectivity_mtl;

    //TODO: add texture stuff
    vec3 result = (ambientResult * vec3(1.0, 1.0, 1.0) * ambientIntensity)
    + (diffuseResult * vec3(1.0, 1.0, 1.0) * diffuseIntensity)
    + (specularResult * vec3(1.0, 1.0, 1.0) * specularIntensity);

    outColor = vec4(result, 1.0);
}
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;

uniform mat4 projectionMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

void main()
{
    fs_in.FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    fs_in.Normal = transpose(inverse(mat3(modelMatrix))) * aNormal;
    fs_in.TexCoords = aTextureCoord;
    fs_in.FragPosLightSpace = lightSpaceMatrix * vec4(fs_in.FragPos, 1.0);
	gl_Position  = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos,1.0);
}

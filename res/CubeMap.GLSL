#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 FragColor;

uniform samplerCube skybox;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec3 TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
    FragColor = texture(skybox, TexCoords);
}  
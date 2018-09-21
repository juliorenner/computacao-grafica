#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//out vec3 ourColor;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
//    ourColor = vec3(0.0f, 0.0f, 1.0f);
}

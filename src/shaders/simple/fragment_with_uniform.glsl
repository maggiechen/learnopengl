#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // set this with OpenGL code

void main()
{
    FragColor = ourColor;
}
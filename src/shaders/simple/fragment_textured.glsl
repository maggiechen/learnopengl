#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    // texture is GLSL's builtin function. Takes in a sampler and texture coords
    // it uses the texture settings we specify to OpenGL (mipmaps, sampling modes for min/maxification, etc)
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float interp;
void main()
{
    // texture is GLSL's builtin function. Takes in a sampler and texture coords
    // it uses the texture settings we specify to OpenGL (mipmaps, sampling modes for min/maxification, etc)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.s, TexCoord.t)), clamp(interp, 0.0, 1.0));
}
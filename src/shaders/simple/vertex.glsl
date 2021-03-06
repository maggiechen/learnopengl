#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;

uniform vec2 offset;

out vec3 ourColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos.xy + offset, aPos.z, 1.0); // see how we construct a vec4's using swizzling
    // ourColor = aColor; // set the output variable to a dark-red color
    ourColor = gl_Position.xyz;
}

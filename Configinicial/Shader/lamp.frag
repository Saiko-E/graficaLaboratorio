#version 330 core
out vec4 FragColor;
uniform float brightness = 1.0f;


void main()
{
     FragColor = vec4(vec3(brightness), 1.0f);
}

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int proceduralTextureMode;

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    // La geometría procedural no tiene UV: proyectar césped, troncos y copas.
    if (proceduralTextureMode == 1)
        TexCoords = (position.xz + vec2(0.5)) * vec2(12.0, 8.0);
    else if (proceduralTextureMode == 2)
        TexCoords = vec2(abs(normal.x) > 0.5 ? position.z : position.x,
                         position.y * 2.5) + vec2(0.5, 1.25);
    else if (proceduralTextureMode == 3)
        TexCoords = position.xy * 0.5 + vec2(0.5);
    else
        TexCoords = texCoords;
}

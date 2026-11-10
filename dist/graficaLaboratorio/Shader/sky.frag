#version 330 core
in vec2 screenPosition;
out vec4 color;
uniform mat4 inverseProjection;
uniform mat3 inverseViewRotation;

void main()
{
    vec4 cameraRay = inverseProjection * vec4(screenPosition, 1.0, 1.0);
    vec3 worldDirection = normalize(inverseViewRotation * cameraRay.xyz);
    float horizon = 1.0 - smoothstep(-0.08, 0.55, worldDirection.y);
    vec3 night = mix(vec3(0.008, 0.018, 0.055), vec3(0.07, 0.11, 0.19), horizon);

    vec3 cell = floor(worldDirection * 120.0);
    float seed = fract(sin(dot(cell, vec3(127.1, 311.7, 74.7))) * 43758.5453);
    float starDistance = length(worldDirection - normalize(cell + 0.5));
    float stars = step(0.982, seed) * (1.0 - smoothstep(0.0010, 0.0025, starDistance));
    stars *= smoothstep(0.01, 0.18, worldDirection.y);

    vec3 moonDirection = normalize(vec3(-0.45, 0.46, -0.77));
    float moonDistance = length(worldDirection - moonDirection);
    float moon = 1.0 - smoothstep(0.035, 0.038, moonDistance);
    float shadow = 1.0 - smoothstep(0.032, 0.036,
        length(worldDirection - normalize(moonDirection + vec3(0.020, 0.010, 0.0))));
    night += vec3(0.07, 0.09, 0.16) * exp(-moonDistance * moonDistance * 220.0);
    color = vec4(mix(night + stars * vec3(0.65, 0.75, 0.95),
        vec3(0.93, 0.94, 0.85), moon * (1.0 - shadow)), 1.0);
}

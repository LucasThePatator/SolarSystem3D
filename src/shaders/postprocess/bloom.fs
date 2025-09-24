#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float exposure;
out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec3 texelColor = texture(texture0, fragTexCoord).rgb;
    vec3 bloomColor = texture(texture1, fragTexCoord).rgb;

    texelColor += bloomColor;
    vec3 result = vec3(1.0) - exp(-texelColor * exposure);
    finalColor.rgb = pow(result, vec3(1 / 2.2));
    finalColor.a = 1.0;
}
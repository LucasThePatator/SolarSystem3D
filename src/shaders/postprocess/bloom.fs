#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D bloomTexture;
uniform sampler2D texture0;

out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec4 bloomColor = texture(bloomTexture, fragTexCoord);

    finalColor = texelColor + bloomColor;
}
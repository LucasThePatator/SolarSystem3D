#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec3 vWorldPos;
in vec3 vRayleighColor;
in vec3 vMieColor;
in float vAtmosphereDepth;

in vec4 vsDebug;

uniform vec3 viewPos;
out vec4 FragColor;

// Constantes
const float PI = 3.14159265359;
const float g = 0.76;// Paramètre de phase Mie (anisotropie)

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
    float power;
};

uniform Light lights[MAX_LIGHTS];
uniform float scale;

// Fonction de phase de Rayleigh
float phaseRayleigh(float cosTheta) {
    return (3.0 / (16.0 * PI)) * (1.0 + cosTheta * cosTheta);
}

// Fonction de phase de Mie (Henyey-Greenstein)
float phaseMie(float cosTheta, float g) {
    float g2 = g * g;
    float num = (1.0 - g2);
    float denom = (1.0 + g2 - 2.0 * g * cosTheta);
    return (1.0 / (4.0 * PI)) * num / pow(denom, 1.5);
}

void main() {
    vec3 viewDir = normalize(viewPos - vWorldPos);
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lights[0].position - vWorldPos);
    vec3 lightColor = lights[0].color.rgb;

    // Angle entre direction de vue et direction lumière
    float cosTheta = dot(viewDir, lightDirection);

    // Application des fonctions de phase
    float phaseR = phaseRayleigh(cosTheta);
    float phaseM = phaseMie(cosTheta, g);

    // Couleur finale du scattering
    vec3 scatteredLight = vRayleighColor * phaseR + vMieColor * phaseM;
    scatteredLight *= lightColor * lightColor;

    // Combinaison
    vec3 finalColor = scatteredLight;

    // Effet de limbe atmosphérique (halo)
    float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), 3.0);
    vec3 atmosphereGlow = scatteredLight * fresnel * 2.0;

    //finalColor += atmosphereGlow * vAtmosphereDepth;

    // at 0 : alpha = 0
    // at 1 : alpha = 1

    FragColor = vec4(finalColor, 1);

}

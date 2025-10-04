#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec4 vertexColor;

// Input uniform values
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;

uniform vec3 viewPos;
uniform float planetRadius;
uniform float atmosphereRadius;
uniform float scale;

// Constantes de scattering
const vec3 rayleighCoefficient = vec3(5.8e-6, 13.5e-6, 33.1e-6);
const float mieCoefficient = 21e-6;
const float rayleighScaleHeight = 8000.0;
const float mieScaleHeight = 1200.0;
const int numSamples = 16;
const int numLightSamples = 32;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vWorldPos;
out vec3 vRayleighColor;
out vec3 vMieColor;
out float vAtmosphereDepth;

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

out vec4 vsDebug;

float densityRayleigh(float height) {
    return exp(-height / rayleighScaleHeight);
}

float densityMie(float height) {
    return exp(-height / mieScaleHeight);
}

vec3 vertexNormalExtrusion(vec3 vertex, vec3 normal)
{
    return vertex + (atmosphereRadius - planetRadius) * normal;
}

// Calcul de l'intersection rayon-sphère
bool raySphereIntersect(vec3 origin, vec3 direction, float radius, out float t0, out float t1) {
    vec3 oc = origin;
    float b = 2.0 * dot(direction, oc);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0 * c;

    t0 = 0;
    t1 = 0;

    if (discriminant < 0.0) {
        return false;
    }

    float sqrtDisc = sqrt(discriminant);
    t0 = (-b - sqrtDisc) / 2.0;
    t1 = (-b + sqrtDisc) / 2.0;

    return true;
}

vec3 computeScattering(vec3 rayOrigin, vec3 rayDir, float rayLength, vec3 lightDirection) {
    vec3 rayleigh = vec3(0.0);
    vec3 mie = vec3(0.0);

    float segmentLength = rayLength / float(numSamples);

    for (int i = 0; i < numSamples; i++) {
        float t = (float(i) + 0.5) * segmentLength;
        vec3 samplePos = rayOrigin + rayDir * t;
        float height = length(samplePos) - planetRadius;

        if (height < 0.0) continue;

        // Densité au point d'échantillonnage
        float hr = densityRayleigh(height / scale) * segmentLength / scale;
        float hm = densityMie(height / scale) * segmentLength / scale;

        // Calcul de l'optical depth vers la lumière
        float t0Light, t1Light;
        raySphereIntersect(samplePos, lightDirection, atmosphereRadius, t0Light, t1Light);

        float opticalDepthLightR = 0.0;
        float opticalDepthLightM = 0.0;

        float segmentLengthLight = t1Light / float(numLightSamples);

        for (int j = 0; j < numLightSamples; j++) {
            float tLight = (float(j) + 0.5) * segmentLengthLight;
            //vec3 lightSamplePos = samplePos + lights[0].position * tLight;
            vec3 lightSamplePos = samplePos + lightDirection * tLight;

            float heightLight = length(lightSamplePos) - planetRadius;

            if (heightLight < 0.0) {
                opticalDepthLightR = 1e20;
                opticalDepthLightM = 1e20;
                break;
            }

            opticalDepthLightR += densityRayleigh(heightLight  / scale) * segmentLengthLight / scale;
            opticalDepthLightM += densityMie(heightLight / scale) * segmentLengthLight / scale;
        }


        if (opticalDepthLightR < 1e20) {
            vec3 tau = rayleighCoefficient * (hr + opticalDepthLightR) +
            mieCoefficient * 1.1 * (hm + opticalDepthLightM);
            vec3 attenuation = exp(-tau);

            rayleigh += attenuation * hr;
            mie += attenuation * hm;
        }
    }

    return (rayleigh * rayleighCoefficient + mie * mieCoefficient * 1.1);
}

void main() {
    vec3 vertexNewPosition = vertexNormalExtrusion(vertexPosition, vertexNormal);
    vec4 worldPos = matModel * vec4(vertexNewPosition, 1.0);
    vWorldPos = worldPos.xyz;
    vPosition = vertexNewPosition;
    vNormal = mat3(transpose(inverse(matModel))) * vertexNormal;

    gl_Position = matProjection * matView * worldPos;

    // Centre de la planète/atmosphère dans l'espace monde
    vec3 planetCenter = (matModel * vec4(0.0, 0.0, 0.0, 1.0)).xyz;

    // Calcul du scattering dans le repère local (centré sur la planète)
    vec3 rayOrigin = viewPos - planetCenter;// Position caméra relative au centre
    vec3 rayDir = normalize(worldPos.xyz - viewPos);

    float t0 = 0, t1 = 0, t0Planet=0, t1Planet=0;
    bool hitAtmosphere = raySphereIntersect(rayOrigin, rayDir, atmosphereRadius, t0, t1);
    bool hitPlanet = raySphereIntersect(rayOrigin, rayDir, planetRadius, t0Planet, t1Planet);

    vec3 lightDirection = normalize(lights[0].position - vWorldPos);

    if (hitAtmosphere && t1 > 0.0) {
        // Calculer le point d'entrée et de sortie dans l'atmosphère
        vec3 atmosphereEntry = rayOrigin + rayDir * t0;
        float rayLength = 0;
        if (!hitPlanet)
        {
            rayLength = t1 - t0;
        } else
        {
            rayLength = t0Planet - t0;
        }

        vec3 scattering = computeScattering(atmosphereEntry, rayDir, rayLength, lightDirection);
        vsDebug = vec4(scattering, 1);
        // Séparer Rayleigh et Mie
        vRayleighColor = scattering * 0.8;
        vMieColor = scattering * 0.2;
        vAtmosphereDepth = rayLength / (atmosphereRadius - planetRadius);
    } else {
        vRayleighColor = vec3(0.0);
        vMieColor = vec3(0.0);
        vAtmosphereDepth = 0.0;
    }
}

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool useVignette;
uniform bool useFilmGrain;
uniform bool useNegative;
uniform bool useSepia;
uniform bool useFisheye;
uniform bool useColorFilter;
uniform bool useGrayscale;
uniform float vignetteIntensity;
uniform float filmGrainIntensity;
uniform float fisheyeStrength;
uniform vec3 colorFilterColor;

float vignette(vec2 uv, float intensity) {
    vec2 dist = uv - vec2(0.5, 0.5);
    return 1.0 - dot(dist, dist) * intensity;
}

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 applySepia(vec3 color) {
    float r = color.r;
    float g = color.g;
    float b = color.b;
    color.r = min(1.0, (r * 0.393) + (g * 0.769) + (b * 0.189));
    color.g = min(1.0, (r * 0.349) + (g * 0.686) + (b * 0.168));
    color.b = min(1.0, (r * 0.272) + (g * 0.534) + (b * 0.131));
    return color;
}

vec2 fisheye(vec2 uv, float strength) {
    vec2 centered = uv - vec2(0.5, 0.5);
    float dist = length(centered);


    float distortion = 1.0 - strength * dist * dist;
    vec2 distorted = centered * distortion;

    return distorted + vec2(0.5, 0.5);
}

vec3 applyGrayscale(vec3 color) {
    float gray = dot(color, vec3(0.299, 0.587, 0.114));
    return vec3(gray);
}

void main()
{
    vec2 uv = TexCoords;

    if (useFisheye) {
        uv = fisheye(TexCoords, fisheyeStrength);
    }

    uv = clamp(uv, vec2(0.0, 0.0), vec2(1.0, 1.0));

    vec3 color = texture(screenTexture, uv).rgb;

    if (useVignette) {
        color *= vignette(uv, vignetteIntensity);
    }

    if (useFilmGrain) {
        float noise = random(uv * 100.0) * filmGrainIntensity;
        color += vec3(noise) * 0.1;
    }

    if (useNegative) {
        color = vec3(1.0) - color;
    }

    if (useSepia) {
        color = applySepia(color);
    }

    if (useColorFilter) {
        color *= colorFilterColor;
    }

    if (useGrayscale) {
        color = applyGrayscale(color);
    }

    FragColor = vec4(color, 1.0);
}

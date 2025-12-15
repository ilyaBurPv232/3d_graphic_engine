#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform int shininess;
uniform int lightType;
uniform float cutOff;
uniform float outerCutOff;
uniform bool lightSphereEnabled;
uniform float lightContrast;
uniform float lightSaturation;
uniform float lightSharpness;
uniform float lightFalloff;
uniform float gamma;
uniform float time;
uniform float letterScale;
uniform vec3 letterColorB;
uniform vec3 letterColorP;
uniform float letterIntensity;

uniform bool debugTime = false;

vec3 applyContrast(vec3 color, float contrast) {
    return ((color - 0.5) * max(contrast, 0.0)) + 0.5;
}

vec3 applySaturation(vec3 color, float saturation) {
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(vec3(luminance), color, saturation);
}

vec3 applyGamma(vec3 color, float gammaValue) {
    return pow(color, vec3(1.0 / gammaValue));
}

bool pointInRightSemicircleRing(float x, float y, float center_x, float center_y,
                               float inner_radius, float outer_radius) {
    if (x < center_x) return false;
    float dx = x - center_x;
    float dy = y - center_y;
    float dist_sq = dx * dx + dy * dy;
    return dist_sq >= inner_radius * inner_radius &&
           dist_sq <= outer_radius * outer_radius;
}


vec2 applyWaveEffect(vec2 coord, float animTime) {

    float centerX = 7.5;
    float centerY = 7.5;

    float dx = coord.x - centerX;
    float dy = coord.y - centerY;
    float r = sqrt(dx * dx + dy * dy);

    float e = sin((r - animTime/3.0)*12.0) / (r + 2.0) / 6.0;

    coord.x += (centerX - coord.x) * e;
    coord.y += (centerY - coord.y) * e;

    return coord;
}

float renderLetterB(vec2 coord, float animTime) {
    vec2 nx_ny = coord;
    float thickness = 1.5;
    float round_radius = 3.0;

    bool is_inside_letter = false;

    if (nx_ny.x >= 1.5 && nx_ny.x <= 1.5 + thickness &&
        nx_ny.y >= 1.5 && nx_ny.y <= 13.5) {
        is_inside_letter = true;
    }
    else if (nx_ny.y >= 1.5 && nx_ny.y <= 1.5 + thickness &&
             nx_ny.x >= 1.5 && nx_ny.x <= 4.5 + round_radius) {
        is_inside_letter = true;
    }
    else if (nx_ny.y >= 13.5 - thickness && nx_ny.y <= 13.5 &&
             nx_ny.x >= 1.5 && nx_ny.x <= 6.75 + round_radius) {
        is_inside_letter = true;
    }
    else if (nx_ny.y >= 6.75 - thickness/2.0 && nx_ny.y <= 6.75 + thickness/2.0 &&
             nx_ny.x >= 1.5 && nx_ny.x <= 7.5) {
        is_inside_letter = true;
    }
    else if (nx_ny.x >= 4.5 && nx_ny.x <= 13.5 &&
             nx_ny.y >= 1.5 && nx_ny.y <= 7.5) {
        vec2 center = vec2(7.5, 4.5);

        float inner_radius = round_radius - thickness;
        if (pointInRightSemicircleRing(nx_ny.x, nx_ny.y, center.x, center.y,
                                      inner_radius, round_radius)) {
            is_inside_letter = true;
        }
    }
    return is_inside_letter ? 1.0 : 0.0;
}

float renderLetterP(vec2 coord, float animTime) {
    vec2 nx_ny = coord;
    float thickness = 1.5;
    float round_radius = 3.0;

    bool is_inside_letter = false;

    if (nx_ny.x >= 1.5 && nx_ny.x <= 1.5 + thickness &&
        nx_ny.y >= 1.5 && nx_ny.y <= 13.5) {
        is_inside_letter = true;
    }
    else if (nx_ny.y >= 13.5 - thickness && nx_ny.y <= 13.5 &&
             nx_ny.x >= 1.5 && nx_ny.x <= 4.5 + round_radius) {
        is_inside_letter = true;
    }
    else if (nx_ny.y >= 9.0 - thickness && nx_ny.y <= 9.0 &&
             nx_ny.x >= 1.5 && nx_ny.x <= 4.5 + round_radius) {
        is_inside_letter = true;
    }
    else if (nx_ny.x >= 4.5 && nx_ny.x <= 13.5 &&
             nx_ny.y >= 7.5 && nx_ny.y <= 13.5) {
        vec2 center = vec2(7.5, 10.5);

        float inner_radius = round_radius - thickness;
        if (pointInRightSemicircleRing(nx_ny.x, nx_ny.y, center.x, center.y,
                                      inner_radius, round_radius)) {
            is_inside_letter = true;
        }
    }
    return is_inside_letter ? 1.0 : 0.0;
}

void main()
{
    if (!lightSphereEnabled) {
        vec3 ambient = ambientStrength * lightColor;
        vec3 result = ambient * objectColor;
        vec3 finalColor = texture(ourTexture, TexCoord).rgb * result;
        finalColor = applyContrast(finalColor, lightContrast);
        finalColor = applySaturation(finalColor, lightSaturation);
        finalColor = applyGamma(finalColor, gamma);
        FragColor = vec4(finalColor, 1.0);
        return;
    }

    float animTime = time * 2.0;

    vec2 scaledCoord = TexCoord * letterScale * 15.0;
    vec2 cell = floor(scaledCoord / 15.0);
    vec2 localCoord = mod(scaledCoord, 15.0);

    // Применяем волновой эффект к координатам
    localCoord = applyWaveEffect(localCoord, animTime);

    // Также можно добавить легкое дрожание (опционально)
    localCoord.x += sin(animTime * 0.5 + cell.y * 0.5) * 0.1;
    localCoord.y += cos(animTime * 0.5 + cell.x * 0.5) * 0.1;

    float letterValue = 0.0;
    vec3 letterColor = vec3(0.0);

    if (mod(cell.x + cell.y, 2.0) < 1.0) {
        letterValue = renderLetterB(localCoord, animTime);
        float colorPulse = 0.5 + 0.5 * sin(animTime * 1.5);
        letterColor = letterColorB * colorPulse;
    } else {
        letterValue = renderLetterP(localCoord, animTime);
        float colorPulse = 0.5 + 0.5 * sin(animTime * 1.5 + 3.14);
        letterColor = letterColorP * colorPulse;
    }

    if (debugTime) {
        float timeNormalized = fract(animTime * 0.1);
        FragColor = vec4(0.0, 0.0, timeNormalized, 1.0);
        return;
    }

    vec3 texColor = texture(ourTexture, TexCoord).rgb;
    vec3 mixedColor = mix(texColor, letterColor, letterValue * letterIntensity);

    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir;
    float diff;
    float spec;

    if (lightType == 0) { // Точечный свет
        lightDir = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        attenuation = pow(attenuation, lightFalloff);
        diff = max(dot(norm, lightDir), 0.0) * attenuation;
        reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * lightSharpness) * attenuation;
    }
    else if (lightType == 1) { // Направленный свет
        lightDir = normalize(-lightDirection);
        diff = max(dot(norm, lightDir), 0.0);
        reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * lightSharpness);
    }
    else if (lightType == 2) { // Проекторный свет
        lightDir = normalize(lightPos - FragPos);
        float theta = dot(lightDir, normalize(-lightDirection));
        float epsilon = cutOff - outerCutOff;
        float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
        intensity = pow(intensity, lightFalloff * 2.0);
        if (theta > outerCutOff) {
            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
            diff = max(dot(norm, lightDir), 0.0) * intensity * attenuation;
            reflectDir = reflect(-lightDir, norm);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * lightSharpness) * intensity * attenuation;
        } else {
            diff = 0.0;
            spec = 0.0;
        }
    }

    vec3 diffuse = diff * lightColor;
    vec3 specular = specularStrength * spec * lightColor;
    vec3 result = (ambient + diffuse + specular) * mixedColor;
    vec3 finalColor = result;

    finalColor = applyContrast(finalColor, lightContrast);
    finalColor = applySaturation(finalColor, lightSaturation);
    finalColor = applyGamma(finalColor, gamma);
    finalColor = clamp(finalColor, 0.0, 1.0);

    FragColor = vec4(finalColor, 1.0);
}

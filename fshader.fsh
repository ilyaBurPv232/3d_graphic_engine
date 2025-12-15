#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightPos;        // Позиция света (из сферы)
uniform vec3 lightDirection;  // Направление света (из сферы)
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform int shininess;
uniform int lightType;        // 0=точечный, 1=направленный, 2=проекторный
uniform float cutOff;         // Угол раскрыва проектора
uniform float outerCutOff;    // Внешний угол проектора
uniform bool lightSphereEnabled; // Включена ли сфера как источник света

// Новые параметры для настройки качества света
uniform float lightContrast = 1.0;    // Контрастность освещения (1.0 - норма)
uniform float lightSaturation = 1.0;  // Насыщенность освещения
uniform float lightSharpness = 1.0;   // Резкость бликов
uniform float lightFalloff = 1.0;     // Спад освещения
uniform float gamma = 2.2;            // Гамма-коррекция

// Функция для коррекции контраста
vec3 applyContrast(vec3 color, float contrast) {
    return ((color - 0.5) * max(contrast, 0.0)) + 0.5;
}

// Функция для коррекции насыщенности
vec3 applySaturation(vec3 color, float saturation) {
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(vec3(luminance), color, saturation);
}

// Функция гамма-коррекции
vec3 applyGamma(vec3 color, float gammaValue) {
    return pow(color, vec3(1.0 / gammaValue));
}

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir;
    float diff;
    float spec;

    // Если сфера как источник света выключена, используем только ambient
    if (!lightSphereEnabled) {
        vec3 result = ambient * objectColor;
        vec3 finalColor = texture(ourTexture, TexCoord).rgb * result;

        // Применяем пост-обработку
        finalColor = applyContrast(finalColor, lightContrast);
        finalColor = applySaturation(finalColor, lightSaturation);
        finalColor = applyGamma(finalColor, gamma);

        FragColor = vec4(finalColor, 1.0);
        return;
    }

    // Выбор типа освещения
    if (lightType == 0) { // Точечный свет
        lightDir = normalize(lightPos - FragPos);
        // Применяем спад освещения
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

        // Усиливаем спад для проектора
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

    vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 finalColor = texture(ourTexture, TexCoord).rgb * result;

    // Применяем пост-обработку
    finalColor = applyContrast(finalColor, lightContrast);
    finalColor = applySaturation(finalColor, lightSaturation);
    finalColor = applyGamma(finalColor, gamma);

    // Гарантируем, что цвет не выходит за пределы
    finalColor = clamp(finalColor, 0.0, 1.0);

    FragColor = vec4(finalColor, 1.0);
}

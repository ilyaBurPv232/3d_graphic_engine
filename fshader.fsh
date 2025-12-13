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
        FragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);
        return;
    }

    // Выбор типа освещения
    if (lightType == 0) { // Точечный свет
        lightDir = normalize(lightPos - FragPos);
        diff = max(dot(norm, lightDir), 0.0);
        reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    }
    else if (lightType == 1) { // Направленный свет
        lightDir = normalize(-lightDirection); // Направление от сферы
        diff = max(dot(norm, lightDir), 0.0);
        reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    }
    else if (lightType == 2) { // Проекторный свет
        lightDir = normalize(lightPos - FragPos);
        float theta = dot(lightDir, normalize(-lightDirection));
        float epsilon = cutOff - outerCutOff;
        float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

        if (theta > outerCutOff) {
            diff = max(dot(norm, lightDir), 0.0) * intensity;
            reflectDir = reflect(-lightDir, norm);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * intensity;
        } else {
            diff = 0.0;
            spec = 0.0;
        }
    }

    vec3 diffuse = diff * lightColor;
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);
}

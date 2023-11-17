#include "glm/glm.hpp"
#include "uniform.h"
#include "fragment.h"
#include <cstdlib>  
#include "color.h"
#include "FastNoiseLite.h"
#include "framebuffer.h"
#include "vertex.h"
#include <cstdlib> 
#pragma once


enum Shaders{
    sol,
    mercurio,
    venus,
    tierra,
    marte,
    luna,
    nave
};

struct Planeta{
    Uniform uniform;
    std::vector<Vertex>* vertex;
    Shaders shader;
};

Vertex vertexShader(const Vertex& vertex, const Uniform& uniform) {
    glm::vec4 transformedVertex = uniform.viewport * uniform.projection * uniform.view * uniform.model * glm::vec4(vertex.position, 1.0f);
    glm::vec3 vertexRedux;
    vertexRedux.x = transformedVertex.x / transformedVertex.w;
    vertexRedux.y = transformedVertex.y / transformedVertex.w;
    vertexRedux.z = transformedVertex.z / transformedVertex.w;
    Color fragmentColor(255, 0, 0, 255);
    glm::vec3 normal = glm::normalize(glm::mat3(uniform.model) * vertex.normal);
    Fragment fragment;
    fragment.position = glm::ivec2(transformedVertex.x, transformedVertex.y);
    fragment.color = fragmentColor;
    return Vertex {vertexRedux, normal, vertex.tex, vertex.position};
}

Color shaderStar(Fragment& fragment) {
     Color color;

    // Define el color base para todo el planeta
    glm::vec3 baseColor = glm::vec3(0.95f, 0.76f, 0.21f); // #F2C335

    // Colores de las bacterias
    glm::vec3 secondColor = glm::vec3(0.95f, 0.45f, 0.02f); // #F27405
    glm::vec3 thirdColor = glm::vec3(0.85f, 0.24f, 0.02f); // #D93D04
    glm::vec3 fourthColor = glm::vec3(0.55f, 0.01f, 0.01f); // #8C0303

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    // Ajusta la escala del ruido para controlar la distribución de las bacterias
    float noiseScale = 7.10f;
    float noiseValue = noiseGenerator.GetNoise(uv.x * noiseScale, uv.y * noiseScale * noiseScale);

    // Define un umbral para separar las áreas con bacterias y sin bacterias
    float bacteriaThreshold = 0.3f;

    if (noiseValue > bacteriaThreshold) {
        // Asigna uno de los colores de las bacterias de manera aleatoria
        glm::vec3 bacteriaColors[] = { baseColor, secondColor, thirdColor, fourthColor };
        int colorIndex = int(noiseValue * 900.0f) % 4;
        glm::vec3 tmpColor = bacteriaColors[colorIndex];

        // Aplica el color uniforme a la intensidad del color de las bacterias
        tmpColor *= baseColor;

        color = Color(tmpColor.x * 255, tmpColor.y * 255, tmpColor.z * 255);
    }
    else {
        // Si no hay bacteria, asigna un color de fondo
        color = Color(baseColor.x * 255, baseColor.y * 255, baseColor.z * 255);
    }

    // Aplica la intensidad del fragmento al color calculado
    fragment.color = color * fragment.z * fragment.intensity;

    return fragment.color;
}

Color shaderPlanet1(Fragment& fragment) {
     // Define los colores base para el planeta "Cartílago Hialino"
    glm::vec3 baseColor = glm::vec3(0.686f, 0.443f, 0.851f); // #AF71D9 (Color predominante)
    glm::vec3 color1 = glm::vec3(0.768f, 0.796f, 0.949f);     // #C4CBF2 (Color secundario)
    glm::vec3 color2 = glm::vec3(0.018f, 0.615f, 0.850f);     // #049DD9 (Color secundario)
    glm::vec3 color3 = glm::vec3(0.018f, 0.698f, 0.850f);     // #04B2D9 (Color secundario)
    glm::vec3 color4 = glm::vec3(0.023f, 0.859f, 0.949f);     // #05DBF2 (Color secundario)

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    // Simula un patrón en el planeta "Cartílago Hialino" basado en coordenadas uv
    float pattern = sin(uv.x * 50.0f) * cos(uv.y * 50.0f);

    // Ajusta la escala y la apariencia del patrón
    float cellSize = 1.105f; // Tamaño de las células
    pattern = glm::fract(pattern / cellSize); // Divide el patrón en celdas más pequeñas

    // Ajusta la apariencia del planeta basada en el patrón
    Color finalColor;
    if (pattern < 0.1f) {
        finalColor = Color(baseColor.x * 255, baseColor.y * 255, baseColor.z * 255);
    } else if (pattern < 0.3f) {
        finalColor = Color(color1.x * 255, color1.y * 255, color1.z * 255);
    } else if (pattern < 0.5f) {
        finalColor = Color(color2.x * 255, color2.y * 255, color2.z * 255);
    } else if (pattern < 0.7f) {
        finalColor = Color(color3.x * 255, color3.y * 255, color3.z * 255);
    } else {
        finalColor = Color(color4.x * 255, color4.y * 255, color4.z * 255);
    }

    // Aplica la intensidad del fragmento al color calculado
    fragment.color = finalColor * fragment.z * fragment.intensity;

    return fragment.color;
}

Color shaderPlanet2(Fragment& fragment) {
     // Define los cuatro colores para las regiones del cerebro
    glm::vec3 colorRegion1 = glm::vec3(0.65f, 0.46f, 0.40f); // #A67665
    glm::vec3 colorRegion2 = glm::vec3(0.85f, 0.65f, 0.59f); // #D9A796
    glm::vec3 colorRegion3 = glm::vec3(0.25f, 0.05f, 0.0f);  // #400D01
    glm::vec3 colorRegion4 = glm::vec3(0.45f, 0.21f, 0.16f); // #733729

    // Simula el aspecto del cerebro utilizando ruido y ondas
    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin); // Cambiado el tipo de ruido a Perlin

    // Ajusta la escala del ruido para controlar la distribución de las regiones
    float noiseScale = 10000.0f;
    float noiseValue = noiseGenerator.GetNoise(uv.x * noiseScale, uv.y * noiseScale);

    // Define la frecuencia y amplitud de las ondas
    float waveFrequency = 100.0f;
    float waveAmplitude = 0.5f;

    // Calcula un valor de onda utilizando la función seno en ambos ejes
    float waveValue = (sin(uv.x * waveFrequency) * sin(uv.y * waveFrequency) * waveAmplitude);

    // Combina el valor de ruido con el valor de onda para agregar textura
    float finalValue = noiseValue + waveValue;

    // Define umbrales para las regiones del cerebro
    float threshold1 = 0.2f;
    float threshold2 = 0.7f;
    float threshold3 = 0.1f;
    float threshold4 = 0.1f;

    // Asigna colores a las regiones según el valor combinado de ruido y onda
    Color finalColor;
    if (finalValue < threshold1) {
        finalColor = Color(colorRegion1.x * 255, colorRegion1.y * 255, colorRegion1.z * 255);
    } else if (finalValue < threshold2) {
        finalColor = Color(colorRegion2.x * 255, colorRegion2.y * 255, colorRegion2.z * 255);
    } else if (finalValue < threshold3) {
        finalColor = Color(colorRegion3.x * 255, colorRegion3.y * 255, colorRegion3.z * 255);
    } else {
        finalColor = Color(colorRegion4.x * 255, colorRegion4.y * 255, colorRegion4.z * 255);
    }

    // Aplica el color al fragmento procesado
    fragment.color = finalColor;

    return fragment.color;
}

Color shaderPlanet3(Fragment& fragment) {
    Color color;

    // Colores base
    glm::vec3 secondColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 mainColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    float offsetX = 8000.0f;
    float offsetY = 1000.0f;
    float scale = 30000.0f;

    // Genera el valor de ruido
    float noiseValue = noiseGenerator.GetNoise((uv.x + offsetX) * scale, (uv.y + offsetY) * scale);
    noiseValue = (noiseValue + 1.0f) * 0.9f;

    // Interpola entre el color base y el color secundario basado en el valor de ruido
    secondColor = glm::mix(mainColor, secondColor, noiseValue);

    if (noiseValue > 0.99f) {
        // Calcula el valor sinusoide para crear líneas
        float sinValue = glm::cos(uv.y * 100.01f) * 0.1f;
        sinValue = glm::smoothstep(100.8f, 10.0f, sinValue);

        // Combina el color base con las líneas sinusoide
        secondColor = secondColor + glm::vec3(sinValue);
    }

    // Ajusta el color base y secundario
    mainColor = glm::mix(mainColor, mainColor, noiseValue);
    secondColor = glm::mix(mainColor, secondColor, noiseValue);

    color = Color(secondColor.x * 255, secondColor.y * 255, secondColor.z * 255);

    // Aplica la intensidad y la profundidad del fragmento
    fragment.color = color * fragment.z * fragment.intensity;

    return fragment.color;
}

Color shaderPlanet4(Fragment& fragment) {
    Color color;

    // Define los colores base para el planeta Omicron
    glm::vec3 baseColor = glm::vec3(0.2f, 0.3f, 0.8f); // Azul
    glm::vec3 landColor = glm::vec3(0.5f, 0.7f, 0.3f); // Verde
    glm::vec3 cloudColor = glm::vec3(1.0f, 1.0f, 1.0f); // Blanco

    glm::vec2 uv = glm::vec2(fragment.original.x, fragment.original.y);

    // Simulación de terreno
    float elevation = sin(uv.x * 10.0f) * cos(uv.y * 10.0f); /* Calcular la elevación basada en coordenadas uv */;

    // Ajusta la apariencia del planeta Omicron basado en la elevación
    if (elevation < 0.1f) {
        // Color del agua
        color = Color(baseColor.x * 255, baseColor.y * 255, baseColor.z * 255);
    } else if (elevation < 0.5f) {
        // Color de la mercurio
        color = Color(landColor.x * 255, landColor.y * 255, landColor.z * 255);
    } else {
        // Color de las nubes
        color = Color(cloudColor.x * 255, cloudColor.y * 255, cloudColor.z * 255); 
    }

    // Aplica la intensidad y la profundidad del fragmento
    fragment.color = color * fragment.z * fragment.intensity;

    return fragment.color;
}


Color shaderNave(Fragment& fragment) {
    // Base color for the star (cyan)
    Color baseColorCyan(230, 56, 252);

    // Coefficient of blending between the base color and noise
    float mixFactor = 0.1; // Ajusta según tus preferencias

    // Distance from the center for the gradient
    float gradientFactor = 4.0 - length(fragment.original);

    // Intensity to adjust brightness
    float intensity = 1; // Ajusta según tus preferencias

    // Blend between the base color and noise
    Color starColor = baseColorCyan * (1.0f - mixFactor) * gradientFactor * intensity;

    // Get UV coordinates of the fragment
    float uvX = fragment.original.x;
    float uvY = fragment.original.y;

    // Parameters for noise
    float noiseScale = 02.2; // Ajusta según tus preferencias

    // Generate noise for the star texture
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    // Combine the two levels of noise
    float noiseValue = (noiseGenerator.GetNoise((uvX + 500.0f) * 500.0f, (uvY + 3000.0f) * 500.0f) +
                        noiseGenerator.GetNoise((uvX + 300.0f) * 500.0f, (uvY + 5000.0f) * 500.0f) * 0.2f) * 0.05f;

    // Blend between the base color and noise based on the noise value
    Color finalColor = starColor + (Color(1.5f, 1.0f, 1.0f) * noiseValue); // Ensure '1.0f' and correct data types

    // Apply intensity and the depth of the fragment
    fragment.color = finalColor * fragment.z * intensity;

    return fragment.color;
}


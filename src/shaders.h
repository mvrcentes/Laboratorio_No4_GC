#pragma once
#include <glm/glm.hpp>
#include "uniforms.h"
#include "fragment.h"
#include "color.h"
#include <random>
#include "print.h"
#include "FastNoiseLite.h"




int selectedPlanet = 1;

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.position,
        // vertex.normal, // non transformed normal
    };
}


// Planeta 1: Gaseoso
Fragment fragmentShader(const Fragment& fragment) {

if (selectedPlanet == 1) {
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
        color = Color(baseColor.x, baseColor.y, baseColor.z);
    } else if (elevation < 0.5f) {
        // Color de la tierra
        color = Color(landColor.x, landColor.y, landColor.z);
    } else {
        // Color de las nubes
        color = Color(cloudColor.x, cloudColor.y, cloudColor.z); 
    }

    Fragment processedFragment = fragment;
    processedFragment.color = color * fragment.intensity;

    return processedFragment;
}
}

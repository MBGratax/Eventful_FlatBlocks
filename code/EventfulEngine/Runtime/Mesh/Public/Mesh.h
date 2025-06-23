#pragma once

#include "CoreTypes.h"

namespace EventfulEngine {

    struct MeshDesc {
        EFString name;
        /** Packed XYZ vertex positions (x,y,z per vertex). */
        std::vector<float> positions;
        /** Packed XYZ vertex normals (x,y,z per vertex). */
        std::vector<float> normals;
        /** Packed XYZ vertex tangents (x,y,z per vertex). */
        std::vector<float> tangents;
        /** Packed UV coordinates (u,v per vertex). */
        std::vector<float> uvs;
        /** Triangle indices referencing the vertex arrays. */
        std::vector<uint32> indices;

        [[nodiscard]] uint32 vertexCount() const { return static_cast<uint32_t>(positions.size() / 3); }
        [[nodiscard]] uint32 indexCount() const { return static_cast<uint32_t>(indices.size()); }
    };
} // EventfulEngine

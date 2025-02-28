/*******************************************************************************
 * Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
 * Please visit https://www.irit.fr/tplay/.
 *
 * All rights reserved.
 * This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Initial Contributors:
 *   - Nicolas Mellado (IRIT)
 *   - Gauthier Bouyjou (IRIT)
 *******************************************************************************/

#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "core/Base.hpp"

namespace hub {
namespace data {

///
/// \brief The Vertex class
/// represents textured point in cartezian space with 3D normal
///
struct Vertex {
    /// \brief px
    float px;
    /// \brief py
    float py;
    /// \brief pz
    float pz;
    /// \brief nx
    float nx;
    /// \brief ny
    float ny;
    /// \brief nz
    float nz;
    /// \brief tx
    float tx;
    /// \brief ty
    float ty;

    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.px, self.py, self.pz, self.nx, self.ny, self.nz, self.tx, self.ty );
    }

    ///
    /// \brief toString
    /// \return
    ///
    std::string toString() const;

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Vertex& other ) const {
        return !std::memcmp( this, &other, sizeof( Vertex ) );
    }
};
static_assert( sizeof( float ) == 4 );
static_assert( sizeof( Vertex ) == 4 * 8 );

///
/// \brief The Shape class
/// represents single manifold object
///
struct Shape {
    ///
    /// \brief vertices
    ///
    std::vector<Vertex> vertices;

    ///
    /// \brief hasNormal
    ///
    bool hasNormal;

    ///
    /// \brief indices
    ///
    std::vector<unsigned int> indices;

    ///
    /// \brief name
    ///
    std::string name;

    ///
    /// \brief material
    ///
    int material;

    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.vertices, self.hasNormal, self.indices, self.name, self.material );
    }

    ///
    /// \brief toString
    /// \return
    ///
    auto toString() const {
        std::string str;
        str += std::to_string( vertices.size() ) + " " + std::to_string( hasNormal ) + " " +
               std::to_string( indices.size() ) + " " + name + " " + std::to_string( material );
        return str;
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Shape& other ) const {
        return vertices == other.vertices && hasNormal == other.hasNormal &&
               indices == other.indices && name == other.name && material == other.material;
    }
};
static_assert( sizeof( unsigned int ) == 4 );

///
/// \brief The Material class
/// represents render information
///
struct Material {
    ///
    /// \brief Material id name
    ///
    std::string name;

    ///
    /// \brief Ambient color
    ///
    float Ka[3]; /* Ambient */

    ///
    /// \brief Diffuse color
    ///
    float Kd[3]; /* Diffuse */
    ///
    /// \brief Specular color
    ///
    float Ks[3]; /* Specular */
    ///
    /// \brief Emission color
    ///
    float Ke[3]; /* Emission */
    ///
    /// \brief Transmission color
    ///
    float Kt[3]; /* Transmittance */
    ///
    /// \brief Shininess factor
    ///
    float Ns; /* Shininess */
    ///
    /// \brief Index of refraction
    ///
    float Ni; /* Index of refraction */
    ///
    /// \brief Transmission filter color
    ///
    float Tf[3]; /* Transmission filter */
    ///
    /// \brief Alpha factor
    ///
    float d; /* Disolve (alpha) */
    ///
    /// \brief Illumination model
    ///
    int illum; /* Illumination model */

    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.name,
                 self.Ka,
                 self.Kd,
                 self.Ks,
                 self.Ke,
                 self.Kt,
                 self.Ns,
                 self.Ni,
                 self.Tf,
                 self.d,
                 self.illum );
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Material& other ) const {
        return name == other.name && !std::memcmp( &Ka, &other.Ka, sizeof( float ) * 21 ) &&
               illum == other.illum;
    }
};

class MeshImpl;

///
/// \brief The Mesh class
/// represent a set of Shape with different material
///
class SRC_API Mesh
{
  public:
    ///
    /// \brief name
    /// \return
    ///
    static constexpr auto name() { return "Mesh"; };

    ///
    /// \brief Mesh
    ///
    Mesh() = default;

    ///
    /// \brief Construct mesh with file
    /// \param filePath [in] file is going to import to mesh
    /// (extension supported: .gltf)
    /// \warning Only gltf file is importable
    ///
    explicit Mesh( const std::string& filePath );

    ///
    /// \brief Construct mesh with list of files
    /// \param filePaths [in] list of files is going to import to mesh
    /// (extension supported: .gltf)
    /// \warning Only gltf file is importable
    ///
    explicit Mesh( std::initializer_list<std::string> filePaths );

    ///
    /// \brief to_string
    /// \return
    ///
    std::string toString() const {

        std::string str = "";
        str += "'" + m_name + "'";
        str += ", draw:" + std::to_string( m_nDraw );
        str += ", vx:" + std::to_string( m_nVertice );
        str += ", tri:" + std::to_string( m_nTriangle );

        return str;
    }

    ///
    /// \brief Getter function
    /// \return Shapes of mesh
    ///
    const std::vector<Shape>& getShapes() const { return m_shapes; }

    ///
    /// \brief Getter function
    /// \return Materials of mesh
    ///
    const std::vector<Material>& getMaterials() const { return m_materials; }

    ///
    /// \brief Print status information of mesh
    ///
    void printStats() const;

    ///
    /// \brief Print information of mesh
    ///
    void printInfo() const;

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Mesh& other ) const {
        return m_name == other.m_name && m_shapes == other.m_shapes &&
               m_materials == other.m_materials && m_nVertice == other.m_nVertice &&
               m_nTriangle == other.m_nTriangle && m_nDraw == other.m_nDraw &&
               m_nMesh == other.m_nMesh && m_mesh_triangles == other.m_mesh_triangles &&
               m_mesh_vertices == other.m_mesh_vertices &&
               m_total_triangles == other.m_total_triangles &&
               m_total_instances == other.m_total_instances && m_total_draws == other.m_total_draws;
    }

#if CPP_VERSION >= 20
    static constexpr auto serialize( const auto& archive, auto& self ) { return archive(); }
#else
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_name,
                 self.m_shapes,
                 self.m_materials,
                 self.m_nVertice,
                 self.m_nTriangle,
                 self.m_nDraw,
                 self.m_nMesh,
                 self.m_mesh_triangles,
                 self.m_mesh_vertices,
                 self.m_total_triangles,
                 self.m_total_instances,
                 self.m_total_draws );
    }
#endif

    ///
    /// \brief getName
    /// \return
    ///
    const std::string& getName() const { return m_name; }

  private:
    std::string m_name = "";
    std::vector<Shape> m_shapes;
    std::vector<Material> m_materials;
    uint64_t m_nVertice        = 0;
    uint64_t m_nTriangle       = 0;
    uint64_t m_nDraw           = 0;
    uint64_t m_nMesh           = 0;
    uint64_t m_mesh_triangles  = 0;
    uint64_t m_mesh_vertices   = 0;
    uint64_t m_total_triangles = 0;
    uint64_t m_total_instances = 0;
    uint64_t m_total_draws     = 0;

    std::shared_ptr<MeshImpl> m_pimpl;
};

} // namespace data
} // namespace hub

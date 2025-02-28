

#include <cassert>

#include <data/Mesh.hpp>

#define HUB_CPP_SOURCE
#include "native_Mesh.hpp"

namespace hub {
namespace native {

int mesh_getNShape( const data::Mesh* mesh ) {
    return mesh->getShapes().size();
}

const data::Shape* mesh_getShape( const data::Mesh* mesh, int iShape ) {
    return &mesh->getShapes().at( iShape );
}

void mesh_toString( const data::Mesh* mesh, char* output, int* strLen ) {
    const auto& str = mesh->getName();
    *strLen         = str.size();
    memcpy( output, str.data(), *strLen + 1 );
}

void shape_getVertices( const data::Shape* shape, float* vertices, int* size ) {
    const auto& lVertices = shape->vertices;
    const auto& lSize     = shape->vertices.size() * sizeof( data::Vertex );
    *size                 = lSize;
    memcpy( vertices, lVertices.data(), lSize );
}

static_assert( sizeof( int ) == sizeof( unsigned int ) );
void shape_getIndices( const data::Shape* shape, int* indices, int* size ) {
    const auto& lIndices = shape->indices;
    const auto& lSize    = shape->indices.size() * sizeof( int );
    *size                = lSize;
    memcpy( indices, lIndices.data(), lSize );
}

void shape_getName( const data::Shape* shape, char* output, int* strLen ) {
    // const auto& str = shape->toString();
    const auto& str = shape->name;
    *strLen         = str.size();
    memcpy( output, str.data(), *strLen + 1 );
}

int shape_getNVertice( const data::Shape* shape ) {
    return shape->vertices.size();
}

int shape_getNIndice( const data::Shape* shape ) {
    return shape->indices.size();
}

void mesh_getColor( const data::Mesh* mesh, int iShape, float* colors ) {
    const auto& shape   = mesh->getShapes().at( iShape );
    const auto& iMat    = shape.material;
    const auto& diffuse = mesh->getMaterials().at( iMat ).Kd;
    colors[0]           = diffuse[0];
    colors[1]           = diffuse[1];
    colors[2]           = diffuse[2];
}

} // namespace native
} // namespace hub

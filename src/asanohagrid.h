#pragma once

#include <cstdint>
#include <iterator>

#include "geom.h"

template<typename coord>
class AsanohaGrid {
public:
        using coord_t = coord;
        using point_t = point<coord>;
        using xform_t = xform<coord>;
        using edge_t = std::pair<point_t, point_t>;

        enum TileType {
                // triangles in CCW order starting from the +x-axis
                TRIANGLE_1 = 0,
                TRIANGLE_2 = 1,
                TRIANGLE_3 = 2,
                TRIANGLE_4 = 3,
                TRIANGLE_5 = 4,
                TRIANGLE_6 = 5,
                INVALID = -1,
        };

        enum TileShape {
                TRIANGLE_SHAPE = 0
        };

public:
        inline static GridType grid_type = ASANOHA;

        inline static size_t num_tile_types = 6; 
        inline static size_t num_tile_shapes = 1;

        inline static TileType getTileType( const point_t& p ) {
                const size_t idx = (((p.y_%5)+5)%5)*5 + (((p.x_%5)+5)%5);
                // 5x5 magic lookup table
                static const TileType types[] = {
                        INVALID, INVALID, INVALID, INVALID, INVALID,
                        INVALID, INVALID, TRIANGLE_1, INVALID, INVALID,
                        INVALID, TRIANGLE_3, TRIANGLE_2, INVALID, INVALID,
                        INVALID, INVALID, INVALID, TRIANGLE_6, TRIANGLE_4,
                        INVALID, INVALID, INVALID, TRIANGLE_5, INVALID
                };
                return types[ idx ];
        }

        inline static TileShape getTileShape( const point_t& p ) {
                return TRIANGLE_SHAPE;
        }

        inline static point_t getOrigin( const point_t& p ) {
                return origins[(size_t)getTileType(p)];
        }

        static size_t numNeighbours( const point_t& p ) {
                return 21;
        }

        static const point<int8_t> *getNeighbourVectors( const point_t& p ) {
                return all_neighbours[getTileType(p)];
        }

        static size_t numEdgeNeighbours( const point_t& p ) {
                return 3;
        }

        static const point<int8_t> *getEdgeNeighbourVectors( const point_t& p ) {
                return edge_neighbours[getTileType(p)];
        }

        static bool translatable( const point_t& p, const point_t& q ) {
                TileType t1 = getTileType(p), t2 = getTileType(q);
                return t1 == t2 && t1 != INVALID && t2 != INVALID;
        }

        static const size_t num_orientations;
        static const xform<int8_t> orientations[12];

        static const point<int8_t> all_neighbours[6][21];
        static const point<int8_t> edge_neighbours[6][3];
        static const point<int8_t> origins[6];

        static const point<int8_t> vertices[6][3];

        static size_t numVertices(const point_t& p) {
                return 3;
        }

        static point_t getVertexCentre(const point_t& p) {
                point_t pTrans = p - getOrigin(p);
                pTrans = point_t { (coord_t)(pTrans.x_ * 6.0/5.0), (coord_t)(pTrans.y_ * 6.0/5.0)};
                return pTrans;
        }

        static const point<int8_t> *getVertexVectors(const point_t& p) {
                return vertices[getTileType(p)];
        }

        static point<double> vertexToGrid( const point_t& pt ) {
                return { pt.x_ * 5.0 / 6.0, pt.y_ *  5.0 / 6.0 };
        }

        static point<double> gridToPage( const point<double>& pt ) {
                const double sqrt3 = 1.73205080756887729353;
                return { pt.x_ + 0.5*pt.y_, 0.5 * sqrt3 * pt.y_ };
        }

        static const point_t translationV1;
        static const point_t translationV2;
};

template<typename coord>
const point<int8_t> AsanohaGrid<coord>::all_neighbours[6][21] = {
        {
                { -1, 1 },
                { -3, 2 },
                { -4, 2 },
                { -5, 1 },
                { -5, 0 },
                { -4, -2 },
                { -3, -3 },
                { -1, -4 },
                { 0, -4 },
                { 1, -3 },
                { 1, -2 },
                { 0, 1 },
                { 1, 2 },
                { 2, 2 },
                { 4, 1 },
                { 5, 0 },
                { 6, -2 },
                { 6, -3 },
                { 5, -4 },
                { 4, -4 },
                { 2, -3 }
        },
        {
                { -1, 0 },
                { -3, 1 },
                { -4, 2 },
                { -5, 4 },
                { -5, 5 },
                { -4, 6 },
                { -3, 6 },
                { -1, 5 },
                { 0, 4 },
                { 1, 2 },
                { 1, 1 },
                { 0, -1 },
                { 1, -3 },
                { 2, -4 },
                { 4, -5 },
                { 5, -5 },
                { 6, -4 },
                { 6, -3 },
                { 5, -1 },
                { 4, 0 },
                { 2, 1 }
        },
        {
                { 1, 0 },
                { 2, 1 },
                { 2, 2 },
                { 1, 4 },
                { 0, 5 },
                { -2, 6 },
                { -3, 6 },
                { -4, 5 },
                { -4, 4 },
                { -3, 2 },
                { -2, 1 },
                { 1, -1 },
                { 2, -3 },
                { 2, -4 },
                { 1, -5 },
                { 0, -5 },
                { -2, -4 },
                { -3, -3 },
                { -4, -1 },
                { -4, 0 },
                { -3, 1 }
        },
        {
                { -1, 0 },
                { -2, -1 },
                { -2, -2 },
                { -1, -4 },
                { 0, -5 },
                { 2, -6 },
                { 3, -6 },
                { 4, -5 },
                { 4, -4 },
                { 3, -2 },
                { 2, -1 },
                { -1, 1 },
                { -2, 3 },
                { -2, 4 },
                { -1, 5 },
                { 0, 5 },
                { 2, 4 },
                { 3, 3 },
                { 4, 1 },
                { 4, 0 },
                { 3, -1 }
        },
        {
                { 0, -1 },
                { -1, -2 },
                { -2, -2 },
                { -4, -1 },
                { -5, 0 },
                { -6, 2 },
                { -6, 3 },
                { -5, 4 },
                { -4, 4 },
                { -2, 3 },
                { -1, 2 },
                { 1, -1 },
                { 3, -2 },
                { 4, -2 },
                { 5, -1 },
                { 5, 0 },
                { 4, 2 },
                { 3, 3 },
                { 1, 4 },
                { 0, 4 },
                { -1, 3 }
        },
        {
                { 1, 0 },
                { 3, -1 },
                { 4, -2 },
                { 5, -4 },
                { 5, -5 },
                { 4, -6 },
                { 3, -6 },
                { 1, -5 },
                { 0, -4 },
                { -1, -2 },
                { -1, -1 },
                { 0, 1 },
                { -1, 3 },
                { -1, 4 },
                { -2, 4 },
                { -4, 5 },
                { -5, 5 },
                { -6, 4 },
                { -6, 3 },
                { -5, 1 },
                { -4, 0 }
        },
};

template<typename coord>
const point<int8_t> AsanohaGrid<coord>::edge_neighbours[6][3] = {
        {
                { 0, 1 },
                { -1, 1 },
                { 1, -2 }
        },
        {
                { 1, 1 },
                { -1, 0 },
                { 0, -1 }
        },
        {
                { 1, -1 },
                { 1, 0 },
                { -2, 1 }
        },
        {
                { -1, 1 },
                { -1, 0 },
                { 2, -1 }
        },
        {
                { 1, -1 },
                { -1, 2 },
                { 0, -1 }
        },
        {
                { 1, 0 },
                { 0, 1 },
                { -1, -1 }
        }
};


template<typename coord>
const point<int8_t> AsanohaGrid<coord>::origins[6] = {
        { 2, 1 },
        { 2, 2 },
        { 1, 2 },
        { -1, 3 },
        { -2, 4 },
        { -2, 3 }
};

template<typename coord>
const size_t AsanohaGrid<coord>::num_orientations = 12;

template<typename coord>
const xform<int8_t> AsanohaGrid<coord>::orientations[12] = {
        { 1, 0, 0,     0, 1, 0 },
        { 0, -1, 0,    1, 1, 0 },
        { -1, -1, 0,   1, 0, 0 },
        { -1, 0, 0,    0, -1, 0 },
        { 0, 1, 0,     -1, -1, 0 },
        { 1, 1, 0,     -1, 0, 0 },

        { 0, 1, 0,     1, 0, 0 },
        { -1, 0, 0,    1, 1, 0 },
        { -1, -1, 0,   0, 1, 0 },
        { 0, -1, 0,    -1, 0, 0 },
        { 1, 0, 0,     -1, -1, 0 },
        { 1, 1, 0,     0, -1, 0 }
};

template<typename coord>
const point<int8_t> AsanohaGrid<coord>::vertices[6][3] = {
        { // {2,1}
                { 0, 0 }, { 6, 0 }, { 2, 2 }
        },
        { // {2,2}
                { 6, 0 }, { 0, 6 }, { 2, 2 }
        },
        { // {1,2}
                { 0, 0 }, { 2, 2 }, { 0, 6 }
        },
        { // {-1,3}
                { 0, 0 }, { 0, 6 }, { -2, 4 }
        },
        { // {-2,4}
                { 0, 6 }, { -6, 6 }, { -2, 4 }
        },
        { // {-2,3}
                { 0, 0 }, { -2, 4 }, { -6, 6 }
        }
};

template<typename coord>
const point<coord> AsanohaGrid<coord>::translationV1 {5, 0};

template<typename coord>
const point<coord> AsanohaGrid<coord>::translationV2 {0, 5};

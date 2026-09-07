#pragma once

#include <cstdint>
#include <iterator>

#include "geom.h"

template<typename coord>
class FloretGrid {
public:
        using coord_t = coord;
        using point_t = point<coord>;
        using xform_t = xform<coord>;
        using edge_t = std::pair<point_t, point_t>;

        enum TileType {
                // Pentagons in CCW order starting from the +x-axis
                PENTAGON_NE = 0,
                PENTAGON_N = 1,
                PENTAGON_NW = 2,
                PENTAGON_SW = 3,
                PENTAGON_S = 4,
                PENTAGON_SE = 5,
                INVALID = -1,
        };

        enum TileShape {
                PENTAGON_SHAPE = 0
        };

public:
        inline static GridType grid_type = FLORET;

        inline static size_t num_tile_types = 6; 
        inline static size_t num_tile_shapes = 1;

        inline static TileType getTileType( const point_t& p ) {
                coord_t mx = ((p.x_ % 21) + 21) % 21;
                coord_t my = ((p.y_ % 21) + 21) % 21;
                // Use a 21x21 magic lookup table, but write only the first row and use mod
                // arithmetic instead writing the entire table.
                static const TileType types[] = {
                        INVALID, INVALID, INVALID, PENTAGON_SW, INVALID, INVALID, PENTAGON_SE,
                        INVALID, INVALID, PENTAGON_S, INVALID, INVALID, PENTAGON_N, INVALID,
                        INVALID, PENTAGON_NW, INVALID, INVALID, PENTAGON_NE, INVALID, INVALID
                };
                coord_t ind = (mx + 17 * my) % 21;
                return types[ ind ];
        }

        inline static TileShape getTileShape( const point_t& p ) {
                return PENTAGON_SHAPE;
        }

        inline static point_t getOrigin( const point_t& p ) {
                return origins[(size_t)getTileType(p)];
        }

        static size_t numNeighbours( const point_t& p ) {
                return 8;
        }

        static const point<int8_t> *getNeighbourVectors( const point_t& p ) {
                return all_neighbours[getTileType(p)];
        }

        static size_t numEdgeNeighbours( const point_t& p ) {
                return 5;
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

        static const point<int8_t> all_neighbours[6][8];
        static const point<int8_t> edge_neighbours[6][5];
        static const point<int8_t> origins[6];

        static const point<int8_t> vertices[6][5];

        static size_t numVertices(const point_t& p) {
                return 5;
        }

        static point_t getVertexCentre(const point_t& p) {
                return p - getOrigin(p);
        }

        static const point<int8_t> *getVertexVectors(const point_t& p) {
                return vertices[getTileType(p)];
        }

        static point<double> vertexToGrid( const point_t& pt ) {
                return { (double)pt.x_, (double)pt.y_ };
        }

        static point<double> gridToPage( const point<double>& pt ) {
                const double sqrt3 = 1.73205080756887729353;
                return { pt.x_ + 0.5*pt.y_, 0.5 * sqrt3 * pt.y_ };
        }

        static const point_t translationV1;
        static const point_t translationV2;
};

template<typename coord>
const point<int8_t> FloretGrid<coord>::all_neighbours[6][8] = {
        {
                { 1, 1 },
                { -1, 2 },
                { -2, 1 },
                { -3, 0 },
                { -2, -2 },
                { 0, -3 },
                { 1, -2 },
                { 2, -1 }
        },
        {
                { 2, -1 },
                { 1, 1 },
                { -1, 2 },
                { -2, 1 },
                { -1, -1 },
                { 0, -3 },
                { 2, -4 },
                { 3, -3 }
        },
        {
                { 1, 1 },
                { -1, 2 },
                { -2, 1 },
                { -1, -1 },
                { 1, -2 },
                { 3, -3 },
                { 4, -2 },
                { 3, 0 }
        },
        {
                { -1, 2 },
                { -2, 1 },
                { -1, -1 },
                { 1, -2 },
                { 2, -1 },
                { 3, 0 },
                { 2, 2 },
                { 0, 3 }
        },
        {
                { 1, 1 },
                { 2, -1 },
                { 1, -2 },
                { -1, -1 },
                { -2, 1 },
                { -3, 3 },
                { -2, 4 },
                { 0, 3 }
        },
        {
                { -1, -1 },
                { 1, -2 },
                { 2, -1 },
                { 1, 1 },
                { -1, 2 },
                { -3, 3 },
                { -4, 2 },
                { -3, 0 }
        }
};

template<typename coord>
const point<int8_t> FloretGrid<coord>::edge_neighbours[6][5] = {
        {
                { 1, 1 },
                { -1, 2 },
                { -2, 1 },
                { 1, -2 },
                { 2, -1 }
        },
        {
                { 2, -1 },
                { 1, 1 },
                { -1, 2 },
                { -2, 1 },
                { -1, -1 }
        },
        {
                { 1, 1 },
                { -1, 2 },
                { -2, 1 },
                { -1, -1 },
                { 1, -2 }
        },
        {
                { -1, 2 },
                { -2, 1 },
                { -1, -1 },
                { 1, -2 },
                { 2, -1 }
        },
        {
                { 1, 1 },
                { 2, -1 },
                { 1, -2 },
                { -1, -1 },
                { -2, 1 }
        },
        {
                { -1, -1 },
                { 1, -2 },
                { 2, -1 },
                { 1, 1 },
                { -1, 2 }
        }
};


template<typename coord>
const point<int8_t> FloretGrid<coord>::origins[6] = {
        { 1, 1 },
        { -1, 2 },
        { -2, 1 },
        { -1, -1 },
        { 1, -2 },
        { 2, -1 }
};

template<typename coord>
const size_t FloretGrid<coord>::num_orientations = 12;

template<typename coord>
const xform<int8_t> FloretGrid<coord>::orientations[12] = {
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
const point<int8_t> FloretGrid<coord>::vertices[6][5] = {
        { // {1,1}
                { 0, 0 }, { 2, 0 }, { 2, 1 }, { 1, 2 }, { 0, 2 }
        },
        { // {-1,2}
                { 0, 0 }, { 0, 2 }, { -1, 3 }, { -2, 3 }, { -2, 2 }
        },
        { // {-2,1}
                { 0, 0 }, { -2, 2 }, { -3, 2 }, { -3, 1 }, { -2, 0 }
        },
        { // {-1,-1}
                { 0, 0 }, { -2, 0 }, { -2, -1 }, { -1, -2 }, { 0, -2 }
        },
        { // {1,-2}
                { 0, 0 }, { 0, -2 }, { 1, -3 }, { 2, -3 }, { 2, -2 }
        },
        { // {2,-1}
                { 0, 0 }, { 2, -2 }, { 3, -2 }, { 3, -1 }, { 2, 0 }
        }
};

template<typename coord>
const point<coord> FloretGrid<coord>::translationV1 {4, 1};

template<typename coord>
const point<coord> FloretGrid<coord>::translationV2 {-1, 5};

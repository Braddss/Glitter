#include "marching_cubes.hpp"
#include <math.h>
#include "PerlinNoise.hpp"


//http://paulbourke.net/geometry/polygonise/
//klacansky.com

static int8_t const marching_cubes_edge2indices[12][2] = {
        { 0, 1,},
        { 2, 1,},
        { 3, 2,},
        { 3, 0,},
        { 4, 5,},
        { 6, 5,},
        { 7, 6,},
        { 7, 4,},
        { 4, 0,},
        { 5, 1,},
        { 6, 2,},
        { 7, 3,},
};
static int const edgeTable[256] = {
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };

static int8_t const marching_cubes_edges[256][16] = {
        { -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  8,  3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  1,  9,  0, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  1,  9,  8,  3,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  2, 10,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  8,  3,  1,  2, 10, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  9,  2, 10,  9,  0,  2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  3,  2, 10,  3, 10,  8,  8, 10,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  2,  3, 11, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 11,  0,  8, 11,  2,  0, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  1,  9,  0,  2,  3, 11, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  2,  1,  9,  2,  9, 11, 11,  9,  8, -1,  0,  0,  0,  0,  0,  0,},
        {  3, 10,  1,  3, 11, 10, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  1,  0,  8,  1,  8, 10, 10,  8, 11, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  3, 11,  0, 11,  9,  9, 11, 10, -1,  0,  0,  0,  0,  0,  0,},
        { 11, 10,  9, 11,  9,  8, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  7,  8, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  3,  0,  4,  7,  3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  7,  8,  9,  0,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  9,  4,  7,  9,  7,  1,  1,  7,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  4,  7,  8,  1,  2, 10, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  3,  0,  4,  7,  3,  2, 10,  1, -1,  0,  0,  0,  0,  0,  0,},
        {  2,  9,  0,  2, 10,  9,  4,  7,  8, -1,  0,  0,  0,  0,  0,  0,},
        {  3,  2,  7,  7,  9,  4,  7,  2,  9,  9,  2, 10, -1,  0,  0,  0,},
        {  8,  4,  7,  3, 11,  2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  7, 11,  2,  7,  2,  4,  4,  2,  0, -1,  0,  0,  0,  0,  0,  0,},
        {  2,  3, 11,  1,  9,  0,  8,  4,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  2,  1,  9,  2,  9,  4,  2,  4, 11, 11,  4,  7, -1,  0,  0,  0,},
        { 10,  3, 11, 10,  1,  3,  8,  4,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  4,  7,  0,  0, 10,  1,  7, 10,  0,  7, 11, 10, -1,  0,  0,  0,},
        {  8,  4,  7,  0,  3, 11,  0, 11,  9,  9, 11, 10, -1,  0,  0,  0,},
        {  7,  9,  4,  7, 11,  9,  9, 11, 10, -1,  0,  0,  0,  0,  0,  0,},
        {  4,  9,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  3,  0,  4,  9,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  5,  4,  0,  1,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  8,  3,  4,  3,  5,  5,  3,  1, -1,  0,  0,  0,  0,  0,  0,},
        {  1,  2, 10,  9,  5,  4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  9,  5,  8,  3,  0,  1,  2, 10, -1,  0,  0,  0,  0,  0,  0,},
        { 10,  5,  4, 10,  4,  2,  2,  4,  0, -1,  0,  0,  0,  0,  0,  0,},
        {  4,  8,  3,  4,  3,  2,  4,  2,  5,  5,  2, 10, -1,  0,  0,  0,},
        {  2,  3, 11,  5,  4,  9, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 11,  0,  8, 11,  2,  0,  9,  5,  4, -1,  0,  0,  0,  0,  0,  0,},
        {  5,  0,  1,  5,  4,  0,  3, 11,  2, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  2,  8,  8,  5,  4,  2,  5,  8,  2,  1,  5, -1,  0,  0,  0,},
        {  3, 10,  1,  3, 11, 10,  5,  4,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  9,  5,  4,  1,  0,  8,  1,  8, 10, 10,  8, 11, -1,  0,  0,  0,},
        { 10,  5, 11, 11,  0,  3, 11,  5,  0,  0,  5,  4, -1,  0,  0,  0,},
        {  4, 10,  5,  4,  8, 10, 10,  8, 11, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  9,  5,  7,  8,  9, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  9,  5,  0,  5,  3,  3,  5,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  8,  0,  1,  8,  1,  7,  7,  1,  5, -1,  0,  0,  0,  0,  0,  0,},
        {  3,  1,  5,  3,  5,  7, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  7,  9,  5,  7,  8,  9,  1,  2, 10, -1,  0,  0,  0,  0,  0,  0,},
        {  1,  2, 10,  0,  9,  5,  0,  5,  3,  3,  5,  7, -1,  0,  0,  0,},
        {  7,  8,  5,  5,  2, 10,  8,  2,  5,  8,  0,  2, -1,  0,  0,  0,},
        { 10,  3,  2, 10,  5,  3,  3,  5,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  9,  7,  8,  9,  5,  7, 11,  2,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  9,  2,  2,  7, 11,  2,  9,  7,  7,  9,  5, -1,  0,  0,  0,},
        {  3, 11,  2,  8,  0,  1,  8,  1,  7,  7,  1,  5, -1,  0,  0,  0,},
        {  2,  7, 11,  2,  1,  7,  7,  1,  5, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  1,  3, 11, 10,  1,  7,  8,  9,  7,  9,  5, -1,  0,  0,  0,},
        { 11, 10,  1, 11,  1,  7,  7,  1,  0,  7,  0,  9,  7,  9,  5, -1,},
        {  5,  7,  8,  5,  8, 10, 10,  8,  0, 10,  0,  3, 10,  3, 11, -1,},
        { 11, 10,  5, 11,  5,  7, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 10,  6,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  8,  3, 10,  6,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  9,  0,  1,  5, 10,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  1,  9,  8,  3,  1, 10,  6,  5, -1,  0,  0,  0,  0,  0,  0,},
        {  6,  1,  2,  6,  5,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  6,  1,  2,  6,  5,  1,  0,  8,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  5,  9,  0,  5,  0,  6,  6,  0,  2, -1,  0,  0,  0,  0,  0,  0,},
        {  6,  5,  2,  2,  8,  3,  5,  8,  2,  5,  9,  8, -1,  0,  0,  0,},
        {  2,  3, 11, 10,  6,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0, 11,  2,  0,  8, 11,  6,  5, 10, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  1,  9,  3, 11,  2, 10,  6,  5, -1,  0,  0,  0,  0,  0,  0,},
        { 10,  6,  5,  2,  1,  9,  2,  9, 11, 11,  9,  8, -1,  0,  0,  0,},
        { 11,  6,  5, 11,  5,  3,  3,  5,  1, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  6,  8,  8,  1,  0,  8,  6,  1,  1,  6,  5, -1,  0,  0,  0,},
        {  0,  3, 11,  0, 11,  6,  0,  6,  9,  9,  6,  5, -1,  0,  0,  0,},
        {  5, 11,  6,  5,  9, 11, 11,  9,  8, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  8,  4,  6,  5, 10, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  3,  4,  7,  3,  0,  4,  5, 10,  6, -1,  0,  0,  0,  0,  0,  0,},
        {  6,  5, 10,  7,  8,  4,  9,  0,  1, -1,  0,  0,  0,  0,  0,  0,},
        {  5, 10,  6,  9,  4,  7,  9,  7,  1,  1,  7,  3, -1,  0,  0,  0,},
        {  1,  6,  5,  1,  2,  6,  7,  8,  4, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  0,  4,  7,  3,  0,  6,  5,  1,  6,  1,  2, -1,  0,  0,  0,},
        {  4,  7,  8,  5,  9,  0,  5,  0,  6,  6,  0,  2, -1,  0,  0,  0,},
        {  2,  6,  5,  2,  5,  3,  3,  5,  9,  3,  9,  4,  3,  4,  7, -1,},
        {  4,  7,  8,  5, 10,  6, 11,  2,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  6,  5, 10,  7, 11,  2,  7,  2,  4,  4,  2,  0, -1,  0,  0,  0,},
        {  4,  7,  8,  9,  0,  1,  6,  5, 10,  3, 11,  2, -1,  0,  0,  0,},
        {  6,  5, 10, 11,  4,  7, 11,  2,  4,  4,  2,  9,  9,  2,  1, -1,},
        {  7,  8,  4, 11,  6,  5, 11,  5,  3,  3,  5,  1, -1,  0,  0,  0,},
        {  0,  4,  7,  0,  7,  1,  1,  7, 11,  1, 11,  6,  1,  6,  5, -1,},
        {  4,  7,  8,  9,  6,  5,  9,  0,  6,  6,  0, 11, 11,  0,  3, -1,},
        {  7, 11,  4, 11,  9,  4, 11,  5,  9, 11,  6,  5, -1,  0,  0,  0,},
        { 10,  4,  9, 10,  6,  4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 10,  4,  9, 10,  6,  4,  8,  3,  0, -1,  0,  0,  0,  0,  0,  0,},
        {  1, 10,  6,  1,  6,  0,  0,  6,  4, -1,  0,  0,  0,  0,  0,  0,},
        {  4,  8,  6,  6,  1, 10,  6,  8,  1,  1,  8,  3, -1,  0,  0,  0,},
        {  9,  1,  2,  9,  2,  4,  4,  2,  6, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  8,  3,  9,  1,  2,  9,  2,  4,  4,  2,  6, -1,  0,  0,  0,},
        {  0,  2,  6,  0,  6,  4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  3,  4,  8,  3,  2,  4,  4,  2,  6, -1,  0,  0,  0,  0,  0,  0,},
        {  4, 10,  6,  4,  9, 10,  2,  3, 11, -1,  0,  0,  0,  0,  0,  0,},
        {  8,  2,  0,  8, 11,  2,  4,  9, 10,  4, 10,  6, -1,  0,  0,  0,},
        {  2,  3, 11,  1, 10,  6,  1,  6,  0,  0,  6,  4, -1,  0,  0,  0,},
        {  8, 11,  2,  8,  2,  4,  4,  2,  1,  4,  1, 10,  4, 10,  6, -1,},
        {  3, 11,  1,  1,  4,  9, 11,  4,  1, 11,  6,  4, -1,  0,  0,  0,},
        {  6,  4,  9,  6,  9, 11, 11,  9,  1, 11,  1,  0, 11,  0,  8, -1,},
        { 11,  0,  3, 11,  6,  0,  0,  6,  4, -1,  0,  0,  0,  0,  0,  0,},
        {  8, 11,  6,  8,  6,  4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  6,  7,  8,  6,  8, 10, 10,  8,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  3,  0,  7,  7, 10,  6,  0, 10,  7,  0,  9, 10, -1,  0,  0,  0,},
        {  1, 10,  6,  1,  6,  7,  1,  7,  0,  0,  7,  8, -1,  0,  0,  0,},
        {  6,  1, 10,  6,  7,  1,  1,  7,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  9,  1,  8,  8,  6,  7,  8,  1,  6,  6,  1,  2, -1,  0,  0,  0,},
        {  7,  3,  0,  7,  0,  6,  6,  0,  9,  6,  9,  1,  6,  1,  2, -1,},
        {  8,  6,  7,  8,  0,  6,  6,  0,  2, -1,  0,  0,  0,  0,  0,  0,},
        {  2,  6,  7,  2,  7,  3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 11,  2,  3,  6,  7,  8,  6,  8, 10, 10,  8,  9, -1,  0,  0,  0,},
        {  9, 10,  6,  9,  6,  0,  0,  6,  7,  0,  7, 11,  0, 11,  2, -1,},
        {  3, 11,  2,  0,  7,  8,  0,  1,  7,  7,  1,  6,  6,  1, 10, -1,},
        {  6,  7, 10,  7,  1, 10,  7,  2,  1,  7, 11,  2, -1,  0,  0,  0,},
        {  1,  3, 11,  1, 11,  9,  9, 11,  6,  9,  6,  7,  9,  7,  8, -1,},
        {  6,  7, 11,  9,  1,  0, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  0,  7,  0,  6,  7,  0, 11,  6,  0,  3, 11, -1,  0,  0,  0,},
        {  6,  7, 11, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  6, 11,  7, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  3,  0,  8, 11,  7,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  6, 11,  7,  9,  0,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  1,  8,  3,  1,  9,  8,  7,  6, 11, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  7,  6,  2, 10,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  1,  2, 10,  0,  8,  3, 11,  7,  6, -1,  0,  0,  0,  0,  0,  0,},
        {  9,  2, 10,  9,  0,  2, 11,  7,  6, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  7,  6,  3,  2, 10,  3, 10,  8,  8, 10,  9, -1,  0,  0,  0,},
        {  2,  7,  6,  2,  3,  7, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  7,  6,  8,  6,  0,  0,  6,  2, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  2,  3,  7,  6,  2,  1,  9,  0, -1,  0,  0,  0,  0,  0,  0,},
        {  8,  7,  9,  9,  2,  1,  9,  7,  2,  2,  7,  6, -1,  0,  0,  0,},
        {  6, 10,  1,  6,  1,  7,  7,  1,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  6, 10,  1,  6,  1,  0,  6,  0,  7,  7,  0,  8, -1,  0,  0,  0,},
        {  7,  6,  3,  3,  9,  0,  6,  9,  3,  6, 10,  9, -1,  0,  0,  0,},
        {  6,  8,  7,  6, 10,  8,  8, 10,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  8,  6, 11,  8,  4,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 11,  3,  0, 11,  0,  6,  6,  0,  4, -1,  0,  0,  0,  0,  0,  0,},
        {  6,  8,  4,  6, 11,  8,  0,  1,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  1,  9,  3,  3,  6, 11,  9,  6,  3,  9,  4,  6, -1,  0,  0,  0,},
        {  8,  6, 11,  8,  4,  6, 10,  1,  2, -1,  0,  0,  0,  0,  0,  0,},
        {  2, 10,  1, 11,  3,  0, 11,  0,  6,  6,  0,  4, -1,  0,  0,  0,},
        { 11,  4,  6, 11,  8,  4,  2, 10,  9,  2,  9,  0, -1,  0,  0,  0,},
        {  4,  6, 11,  4, 11,  9,  9, 11,  3,  9,  3,  2,  9,  2, 10, -1,},
        {  3,  8,  4,  3,  4,  2,  2,  4,  6, -1,  0,  0,  0,  0,  0,  0,},
        {  2,  0,  4,  2,  4,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  1,  9,  3,  8,  4,  3,  4,  2,  2,  4,  6, -1,  0,  0,  0,},
        {  9,  2,  1,  9,  4,  2,  2,  4,  6, -1,  0,  0,  0,  0,  0,  0,},
        {  6, 10,  4,  4,  3,  8,  4, 10,  3,  3, 10,  1, -1,  0,  0,  0,},
        {  1,  6, 10,  1,  0,  6,  6,  0,  4, -1,  0,  0,  0,  0,  0,  0,},
        { 10,  9,  0, 10,  0,  6,  6,  0,  3,  6,  3,  8,  6,  8,  4, -1,},
        { 10,  9,  4, 10,  4,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  6, 11,  7,  5,  4,  9, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  8,  3,  9,  5,  4,  7,  6, 11, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  5,  4,  0,  1,  5,  6, 11,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  6, 11,  4,  8,  3,  4,  3,  5,  5,  3,  1, -1,  0,  0,  0,},
        {  2, 10,  1, 11,  7,  6,  5,  4,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  8,  3,  1,  2, 10,  4,  9,  5, 11,  7,  6, -1,  0,  0,  0,},
        {  6, 11,  7, 10,  5,  4, 10,  4,  2,  2,  4,  0, -1,  0,  0,  0,},
        {  6, 11,  7,  5,  2, 10,  5,  4,  2,  2,  4,  3,  3,  4,  8, -1,},
        {  2,  7,  6,  2,  3,  7,  4,  9,  5, -1,  0,  0,  0,  0,  0,  0,},
        {  4,  9,  5,  8,  7,  6,  8,  6,  0,  0,  6,  2, -1,  0,  0,  0,},
        {  3,  6,  2,  3,  7,  6,  0,  1,  5,  0,  5,  4, -1,  0,  0,  0,},
        {  1,  5,  4,  1,  4,  2,  2,  4,  8,  2,  8,  7,  2,  7,  6, -1,},
        {  5,  4,  9,  6, 10,  1,  6,  1,  7,  7,  1,  3, -1,  0,  0,  0,},
        {  4,  9,  5,  7,  0,  8,  7,  6,  0,  0,  6,  1,  1,  6, 10, -1,},
        {  3,  7,  6,  3,  6,  0,  0,  6, 10,  0, 10,  5,  0,  5,  4, -1,},
        {  4,  8,  5,  8, 10,  5,  8,  6, 10,  8,  7,  6, -1,  0,  0,  0,},
        {  5,  6, 11,  5, 11,  9,  9, 11,  8, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  9,  5,  0,  5,  6,  0,  6,  3,  3,  6, 11, -1,  0,  0,  0,},
        {  8,  0, 11, 11,  5,  6, 11,  0,  5,  5,  0,  1, -1,  0,  0,  0,},
        { 11,  5,  6, 11,  3,  5,  5,  3,  1, -1,  0,  0,  0,  0,  0,  0,},
        { 10,  1,  2,  5,  6, 11,  5, 11,  9,  9, 11,  8, -1,  0,  0,  0,},
        {  2, 10,  1,  3,  6, 11,  3,  0,  6,  6,  0,  5,  5,  0,  9, -1,},
        {  0,  2, 10,  0, 10,  8,  8, 10,  5,  8,  5,  6,  8,  6, 11, -1,},
        { 11,  3,  6,  3,  5,  6,  3, 10,  5,  3,  2, 10, -1,  0,  0,  0,},
        {  2,  3,  6,  6,  9,  5,  3,  9,  6,  3,  8,  9, -1,  0,  0,  0,},
        {  5,  0,  9,  5,  6,  0,  0,  6,  2, -1,  0,  0,  0,  0,  0,  0,},
        {  6,  2,  3,  6,  3,  5,  5,  3,  8,  5,  8,  0,  5,  0,  1, -1,},
        {  6,  2,  1,  6,  1,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  9,  5,  8,  5,  3,  3,  5,  6,  3,  6, 10,  3, 10,  1, -1,},
        {  1,  0, 10,  0,  6, 10,  0,  5,  6,  0,  9,  5, -1,  0,  0,  0,},
        {  0,  3,  8, 10,  5,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 10,  5,  6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { 11,  5, 10, 11,  7,  5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  5, 11,  7,  5, 10, 11,  3,  0,  8, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  5, 10, 11,  7,  5,  9,  0,  1, -1,  0,  0,  0,  0,  0,  0,},
        {  9,  3,  1,  9,  8,  3,  5, 10, 11,  5, 11,  7, -1,  0,  0,  0,},
        {  2, 11,  7,  2,  7,  1,  1,  7,  5, -1,  0,  0,  0,  0,  0,  0,},
        {  3,  0,  8,  2, 11,  7,  2,  7,  1,  1,  7,  5, -1,  0,  0,  0,},
        {  2, 11,  0,  0,  5,  9,  0, 11,  5,  5, 11,  7, -1,  0,  0,  0,},
        {  9,  8,  3,  9,  3,  5,  5,  3,  2,  5,  2, 11,  5, 11,  7, -1,},
        { 10,  2,  3, 10,  3,  5,  5,  3,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  5, 10,  7,  7,  0,  8, 10,  0,  7, 10,  2,  0, -1,  0,  0,  0,},
        {  1,  9,  0, 10,  2,  3, 10,  3,  5,  5,  3,  7, -1,  0,  0,  0,},
        {  7,  5, 10,  7, 10,  8,  8, 10,  2,  8,  2,  1,  8,  1,  9, -1,},
        {  7,  5,  1,  7,  1,  3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  1,  0,  8,  7,  1,  1,  7,  5, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  5,  9,  0,  3,  5,  5,  3,  7, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  5,  9,  7,  9,  8, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  5, 10,  4, 10,  8,  8, 10, 11, -1,  0,  0,  0,  0,  0,  0,},
        { 11,  3, 10, 10,  4,  5, 10,  3,  4,  4,  3,  0, -1,  0,  0,  0,},
        {  9,  0,  1,  4,  5, 10,  4, 10,  8,  8, 10, 11, -1,  0,  0,  0,},
        {  3,  1,  9,  3,  9, 11, 11,  9,  4, 11,  4,  5, 11,  5, 10, -1,},
        {  8,  4, 11, 11,  1,  2,  4,  1, 11,  4,  5,  1, -1,  0,  0,  0,},
        {  5,  1,  2,  5,  2,  4,  4,  2, 11,  4, 11,  3,  4,  3,  0, -1,},
        { 11,  8,  4, 11,  4,  2,  2,  4,  5,  2,  5,  9,  2,  9,  0, -1,},
        {  2, 11,  3,  5,  9,  4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  5, 10,  4, 10,  2,  4,  2,  8,  8,  2,  3, -1,  0,  0,  0,},
        { 10,  4,  5, 10,  2,  4,  4,  2,  0, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  1,  9,  8,  2,  3,  8,  4,  2,  2,  4, 10, 10,  4,  5, -1,},
        { 10,  2,  5,  2,  4,  5,  2,  9,  4,  2,  1,  9, -1,  0,  0,  0,},
        {  4,  3,  8,  4,  5,  3,  3,  5,  1, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  4,  5,  0,  5,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  3,  9,  3,  5,  9,  3,  4,  5,  3,  8,  4, -1,  0,  0,  0,},
        {  4,  5,  9, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  7,  4,  9,  7,  9, 11, 11,  9, 10, -1,  0,  0,  0,  0,  0,  0,},
        {  8,  3,  0,  7,  4,  9,  7,  9, 11, 11,  9, 10, -1,  0,  0,  0,},
        {  0,  1,  4,  4, 11,  7,  1, 11,  4,  1, 10, 11, -1,  0,  0,  0,},
        { 10, 11,  7, 10,  7,  1,  1,  7,  4,  1,  4,  8,  1,  8,  3, -1,},
        {  2, 11,  7,  2,  7,  4,  2,  4,  1,  1,  4,  9, -1,  0,  0,  0,},
        {  0,  8,  3,  1,  4,  9,  1,  2,  4,  4,  2,  7,  7,  2, 11, -1,},
        {  7,  2, 11,  7,  4,  2,  2,  4,  0, -1,  0,  0,  0,  0,  0,  0,},
        {  7,  4, 11,  4,  2, 11,  4,  3,  2,  4,  8,  3, -1,  0,  0,  0,},
        {  7,  4,  3,  3, 10,  2,  3,  4, 10, 10,  4,  9, -1,  0,  0,  0,},
        {  2,  0,  8,  2,  8, 10, 10,  8,  7, 10,  7,  4, 10,  4,  9, -1,},
        {  4,  0,  1,  4,  1,  7,  7,  1, 10,  7, 10,  2,  7,  2,  3, -1,},
        {  4,  8,  7,  1, 10,  2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  9,  7,  4,  9,  1,  7,  7,  1,  3, -1,  0,  0,  0,  0,  0,  0,},
        {  8,  7,  0,  7,  1,  0,  7,  9,  1,  7,  4,  9, -1,  0,  0,  0,},
        {  4,  0,  3,  4,  3,  7, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  4,  8,  7, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  9, 10,  8, 10, 11, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0, 11,  3,  0,  9, 11, 11,  9, 10, -1,  0,  0,  0,  0,  0,  0,},
        {  1,  8,  0,  1, 10,  8,  8, 10, 11, -1,  0,  0,  0,  0,  0,  0,},
        {  3,  1, 10,  3, 10, 11, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  2,  9,  1,  2, 11,  9,  9, 11,  8, -1,  0,  0,  0,  0,  0,  0,},
        {  0,  9,  3,  9, 11,  3,  9,  2, 11,  9,  1,  2, -1,  0,  0,  0,},
        { 11,  8,  0, 11,  0,  2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  2, 11,  3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  3, 10,  2,  3,  8, 10, 10,  8,  9, -1,  0,  0,  0,  0,  0,  0,},
        {  9, 10,  2,  9,  2,  0, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  3,  8,  2,  8, 10,  2,  8,  1, 10,  8,  0,  1, -1,  0,  0,  0,},
        {  2,  1, 10, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  8,  9,  1,  8,  1,  3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  1,  0,  9, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        {  0,  3,  8, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
        { -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
};

double frequence_slider = 2;
double amplitude_slider = 4;
double offset_slider = 0;


MarchingCubes::MarchingCubes(const char* fileName, glm::u16vec3 bounds) :bounds(bounds)
{
    pointCloudObject = PointObject();
    surface = TriObject();
    spacing = 0.1f;
    surfaceLevel = 0.8f;
    pointValuesFromRaw(fileName);
    pointCloudFromPoints();
    surfaceFromPoints();


}

MarchingCubes::MarchingCubes()
{
    bounds = vec3(100, 50, 100);
    //bounds = vec3(2, 2, 2);
    pointCloudObject = PointObject();
    surface = TriObject();
    spacing = 0.1f;
    surfaceLevel = 0.5f;
    pointValuesFromDensityFunction();
    //pointsValues = std::vector<uint8>{ 0, 20, 255,200, 255, 255,100,150};
    pointCloudFromPoints();
    surfaceFromPoints();


}



void MarchingCubes::pointValuesFromDensityFunction()
{
    unsigned int width = bounds.x, height = bounds.z;

    unsigned int seed = 237;
    PerlinNoise pn(seed);

    std::vector<uint8> noiseImg;

    unsigned int kk = 0;
    for (unsigned int i = 0; i < height; ++i) {     // y
        for (unsigned int j = 0; j < width; ++j) {  // x
            double x = (double)j / ((double)width);
            double y = (double)i / ((double)height);

            // Typical Perlin noise
            double n = pn.noise(10 * x, 10 * y, 0.8);

            // Wood like structure
            //n = 20 * pn.noise(x, y, 0.8);
            //n = n - floor(n);

            // Map the values to the [0, 255] interval, for simplicity we use 
            // tones of grey
            noiseImg.push_back(floor(255 * n));
        }
    }




    std::vector<uint8> pointCloud;
    for (int64 z = 0;  z < bounds.z;  z++)
    {
        for (int64 y = 0; y < bounds.y; y++)
        {
            for (int64 x = 0; x < bounds.x; x++)
            {
                float xF = static_cast<float>(x) / bounds.x;
                float yF = static_cast<float>(y) / bounds.y;
                float zF = static_cast<float>(z) / bounds.z;



                float density = -yF;
                double frequency= 0;
                double amplitudes = 2;
                double offset = 1;
                

                offset = 0+offset_slider;
                amplitudes = 0.2* amplitude_slider;
                frequency = 0.005* frequence_slider;
                density += static_cast<float>(pn.noise(frequency * x+ offset, frequency * z+ offset, 0.2+ yF * 4)) / amplitudes;
                offset +=10;
                amplitudes = 0.5* amplitude_slider;
                frequency = 0.001* frequence_slider;
                
                density += static_cast<float>(pn.noise(frequency * x + offset, frequency * z + offset, 5 + yF*4)) / amplitudes;
                offset += 20;
                amplitudes = 5* amplitude_slider;
                frequency = 0.01* frequence_slider;
                density += static_cast<float>(pn.noise(frequency * x + offset, frequency * z + offset, 8 + yF * 4)) / amplitudes;
                offset += 30;
                amplitudes = 50* amplitude_slider;
                frequency = 0.1* frequence_slider;
                density += static_cast<float>(pn.noise(frequency * x + offset, frequency * z + offset, 0.1 + yF * 4)) / amplitudes;
               
                if (density < 0) density *= -1;
                density = density - static_cast<int>(density);
                

                pointCloud.push_back(static_cast<uint8>(density *255));
            }
        }
    }

    pointsValues = pointCloud;
    vector<uint8>().swap(pointCloud);
}

float MarchingCubes::densityFunction(int x, int y, int z)
{
    float xF = static_cast<float>(x) / bounds.x;
    float yF = static_cast<float>(y) / bounds.y;
    float zF = static_cast<float>(z) / bounds.z;



    float density = -yF+0.5f;
    return density;
}

void MarchingCubes::pointValuesFromRaw(const char* fileName) //Todo -->pointCloud from Raw and then verticesFrom zeroLevel
{
	std::string path = PROJECT_SOURCE_DIR "/Glitter/DataSets/";
	std::string valueData;
	std::ifstream rawFile;
	rawFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


	try
	{
		rawFile.open(path + fileName,std::ios::in| std::ios::binary);

		std::stringstream rawStream;
		rawStream << rawFile.rdbuf();

		rawFile.close();
		valueData = rawStream.str();
		
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::RAWFILE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	long min = 100;
	long max = 0;
	std::vector<uint8> pointValues;

	for (int i = 0; i < valueData.length(); i++)
	{
		long val = static_cast<long>(valueData[i]);
        uint8 valTest =static_cast<uint8>(val + (pow(2, 8 * sizeof(uint8))/2));
		pointValues.push_back(valTest);
		if (valTest < min)min = valTest;
		if (valTest > max)max = valTest;
	};

	pointsValues = pointValues;
	std::cout << min << " " << max <<std::endl;
}



void MarchingCubes::pointCloudFromPoints()
{
	vector<float> pointCloud;

	for (int64_t z = 0; z < bounds.z; z++)
	{
		for (int64_t y = 0; y < bounds.y; y++)
		{
			for (int64_t x = 0; x < bounds.x; x++)
			{
				pointCloud.push_back(static_cast<float>(x) * spacing);
				pointCloud.push_back(static_cast<float>(y) * spacing);
				pointCloud.push_back(static_cast<float>(z) * spacing);
				int size = pow(2, 8 * sizeof(pointsValues[0]))-1;

				float val = static_cast<float>(pointsValues[x + y * bounds.x + z * bounds.x * bounds.y]) / size;

				pointCloud.push_back(val);
				float alpha = val < surfaceLevel ? 0 : 1;
                //pointCloud.push_back(alpha);
                pointCloud.push_back(1.f);
			}
		}
	}

    pointCloudObject.updateData(pointCloud,5);

    vector<float>().swap(pointCloud);

}

//http://paulbourke.net/geometry/polygonise/
//klacansky.com
void MarchingCubes::surfaceFromPoints()
{

    std::vector<float> surfaceVertices;
    float grid[8]; //todo for loops parralelisieren
    for (int z = 0; z < bounds.z -1; z++)
    {
        for (int y = 0; y < bounds.y - 1; y++)
        {
            for (int x = 0; x < bounds.x - 1; x++)
            {
             /*   grid[0] = gridpoints[(x)+(y)*bounds.x + (z)*bounds.x * bounds.y];
                grid[1] = gridpoints[(x + 1) + (y)*bounds.x + (z)*bounds.x * bounds.y];
                grid[2] = gridpoints[(x + 1) + (y)*bounds.x + (z + 1) * bounds.x * bounds.y];
                grid[3] = gridpoints[(x)+(y)*bounds.x + (z + 1) * bounds.x * bounds.y];
                grid[4] = gridpoints[(x)+(y + 1) * bounds.x + (z)*bounds.x * bounds.y];
                grid[5] = gridpoints[(x + 1) + (y + 1) * bounds.x + (z)*bounds.x * bounds.y];
                grid[6] = gridpoints[(x + 1) + (y + 1) * bounds.x + (z + 1) * bounds.x * bounds.y];
                grid[7] = gridpoints[(x)+(y + 1) * bounds.x + (z + 1) * bounds.x * bounds.y];*/
                for (int i = 0; i < 8; i++)
                {
                    grid[i] = static_cast<float>(pointsValues[this->vertexIndexToArrIndex(i, x, y, z)])/255;
                }
               

                uint8 cubeindex = 0;
                if (grid[0] <= surfaceLevel) cubeindex |= 1;
                if (grid[1] <= surfaceLevel) cubeindex |= 2;
                if (grid[2] <= surfaceLevel) cubeindex |= 4;
                if (grid[3] <= surfaceLevel) cubeindex |= 8;
                if (grid[4] <= surfaceLevel) cubeindex |= 16;
                if (grid[5] <= surfaceLevel) cubeindex |= 32;
                if (grid[6] <= surfaceLevel) cubeindex |= 64;
                if (grid[7] <= surfaceLevel) cubeindex |= 128;

                //int8_t edges[16] = marching_cubes_edges[cubeindex];

                for each (int8_t val in marching_cubes_edges[cubeindex])
                {
                    if (val == -1) break;
                    int64 indexOne = vertexIndexToArrIndex(marching_cubes_edge2indices[val][0], x, y, z) * 5;
                    int64 indexTwo = vertexIndexToArrIndex(marching_cubes_edge2indices[val][1], x, y, z) * 5;

                   
                    vec3 pointOne = vec3(pointCloudObject.vertices[indexOne], pointCloudObject.vertices[indexOne + 1], pointCloudObject.vertices[indexOne + 2]);
                    vec3 pointTwo = vec3(pointCloudObject.vertices[indexTwo], pointCloudObject.vertices[indexTwo + 1], pointCloudObject.vertices[indexTwo + 2]);

                    //int vertIndices[] = marching_cubes_edge2indices[val];
                    //pushback parrallel stuff issues
                    vec3 newVert = vertexInterpolation(pointOne, pointTwo, static_cast<float>(pointsValues[this->vertexIndexToArrIndex(marching_cubes_edge2indices[val][0], x, y, z)]) / 255, static_cast<float>(pointsValues[this->vertexIndexToArrIndex(marching_cubes_edge2indices[val][1], x, y, z)]) / 255);
                    surfaceVertices.push_back(newVert.x);
                    surfaceVertices.push_back(newVert.y);
                    surfaceVertices.push_back(newVert.z);
                }
            }
        }
    }


    std::vector<float> surfaceVerticesWithNormals;

    

    for (int64 vIt = 0; vIt < surfaceVertices.size()/9; vIt++)
    {
        vec3 normal;
        vec3 triVerts[3];
        for (int64 in = 0; in < 3; in++)
        {
            triVerts[in] = vec3(surfaceVertices[in*3+vIt*9], surfaceVertices[in * 3 + vIt * 9 +1], surfaceVertices[in * 3 + vIt * 9 +2]);
            
        }
       

        normal = glm::cross(triVerts[2] - triVerts[0], triVerts[1] - triVerts[0]);

        for (int j = 0; j < 3; j++)
        {
           
            surfaceVerticesWithNormals.push_back(triVerts[j].x);
            surfaceVerticesWithNormals.push_back(triVerts[j].y);
            surfaceVerticesWithNormals.push_back(triVerts[j].z);

            surfaceVerticesWithNormals.push_back(normal.x);
            surfaceVerticesWithNormals.push_back(normal.y);
            surfaceVerticesWithNormals.push_back(normal.z);

        }
        //todo check if normal points in right direction
    }

    //todo set vertices to object and push to graphics card
    surface.updateData(surfaceVerticesWithNormals,5);


    vector<float>().swap(surfaceVertices);
    vector<float>().swap(surfaceVerticesWithNormals);
}


int64 MarchingCubes::vertexIndexToArrIndex(int vertIndex, int64 x, int64 y, int64 z)
{
    int64 arrIndex =-1;

    switch (vertIndex)
    {
    case(0):
        arrIndex = (x)+(y)*bounds.x + (z)*bounds.x * bounds.y;
        break;
    case(1):
        arrIndex = (x + 1) + (y)*bounds.x + (z)*bounds.x * bounds.y;
        break;
    case(2):
        arrIndex = (x + 1) + (y)*bounds.x + (z + 1) * bounds.x * bounds.y;
        break;
    case(3):
        arrIndex = (x)+(y)*bounds.x + (z + 1) * bounds.x * bounds.y;
        break;
    case(4):
        arrIndex = (x)+(y + 1) * bounds.x + (z)*bounds.x * bounds.y;
        break;
    case(5):
        arrIndex = (x + 1) + (y + 1) * bounds.x + (z)*bounds.x * bounds.y;
        break;
    case(6):
        arrIndex = (x + 1) + (y + 1) * bounds.x + (z + 1) * bounds.x * bounds.y;
        break;
    case(7):
        arrIndex = (x)+(y + 1) * bounds.x + (z + 1) * bounds.x * bounds.y;
        break;

    }
    return arrIndex;
    
}


vec3 MarchingCubes::vertexInterpolation(vec3 one, vec3 two, float valOne, float valTwo)
{
    vec3 p;

    /*if (glm::abs(surfaceLevel - valTwo) < 0.00001)
        return(one);
    if (glm::abs(surfaceLevel - valTwo) < 0.00001)
       return(two);
       */
     if (glm::abs(valOne - valTwo) < 0.00001)
        return(one);

    float interPolVal = (surfaceLevel - valOne) / (valTwo - valOne);
    p.x = one.x + interPolVal * (two.x - one.x);
    p.y = one.y + interPolVal * (two.y - one.y);
    p.z = one.z + interPolVal * (two.z - one.z);


    vec3 ret = one + 0.5f * (two - one);
   
    return p; //todo

}


void MarchingCubes::drawElements()
{
	//pointCloudObject.draw();
    
    surface.draw();
}



void MarchingCubes::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		pointCloudObject.scaleObj(pointCloudObject.getScale() * 1.01f);
        surface.scaleObj(surface.getScale() * 1.01f);
		//pointCloudFromPoints();
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{

		pointCloudObject.scaleObj(pointCloudObject.getScale() * 0.99f);
        surface.scaleObj(surface.getScale() * 0.99f);
		//pointCloudFromPoints();
	}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		surfaceLevel += 0.01f;
		if (surfaceLevel < 0)surfaceLevel = 0;
		if (surfaceLevel > 1)surfaceLevel = 1;

        cout << surfaceLevel << endl;
		//pointCloudFromPoints();
        surfaceFromPoints();
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		surfaceLevel -= 0.01f;
		if (surfaceLevel < 0)surfaceLevel = 0;
		if (surfaceLevel > 1)surfaceLevel = 1;

        cout << surfaceLevel << endl;
		//pointCloudFromPoints();
        surfaceFromPoints();
	}

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        vec3 dir(0, 0, -1);
        dir *= 0.1f;
        pointCloudObject.translateObj(pointCloudObject.getPosition() +dir);
        surface.translateObj(surface.getPosition() + dir);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        vec3 dir(0, 0, 1);
        dir *= 0.1f;
        pointCloudObject.translateObj(pointCloudObject.getPosition() + dir);
        surface.translateObj(surface.getPosition() + dir);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        vec3 dir(1, 0, 0);
        dir *= 0.1f;
        pointCloudObject.translateObj(pointCloudObject.getPosition() + dir);
        surface.translateObj(surface.getPosition() + dir);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        vec3 dir(-1, 0, 0);
        dir *= 0.1f;
        pointCloudObject.translateObj(pointCloudObject.getPosition() + dir);
        surface.translateObj(surface.getPosition() + dir);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        vec3 dir(0, -1, 0);
        dir *= 0.1f;
        pointCloudObject.translateObj(pointCloudObject.getPosition() + dir);
        surface.translateObj(surface.getPosition() + dir);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        vec3 dir(0, 1, 0);
        dir *= 0.1f;
        pointCloudObject.translateObj(pointCloudObject.getPosition() + dir);
        surface.translateObj(surface.getPosition() + dir);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        pointCloudObject.rotateObj(vec3(1, 0, 0), pointCloudObject.getRotation().w + 1);
        surface.rotateObj(vec3(1, 0, 0), surface.getRotation().w + 1);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        pointCloudObject.rotateObj(vec3(0, 1, 0), pointCloudObject.getRotation().w + 1);
        surface.rotateObj(vec3(0, 1, 0), surface.getRotation().w + 1);
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        pointCloudObject.rotateObj(vec3(0, 0, 1), pointCloudObject.getRotation().w + 1);
        surface.rotateObj(vec3(0, 0, 1), surface.getRotation().w + 1);
    }


   
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        frequence_slider *= 1.05;
        pointValuesFromDensityFunction();
        surfaceFromPoints();
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        frequence_slider *= 0.95;
        pointValuesFromDensityFunction();
        surfaceFromPoints();
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        amplitude_slider *= 1.05;
        pointValuesFromDensityFunction();
        surfaceFromPoints();
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        amplitude_slider *= 0.95;
        pointValuesFromDensityFunction();
        surfaceFromPoints();
    }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    {
        offset_slider += 0.01;
        pointValuesFromDensityFunction();
        surfaceFromPoints();
    }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
    {
        offset_slider -= 0.01;
        pointValuesFromDensityFunction();
        surfaceFromPoints();
    }
		
}
 
//template class MarchingCubes<uint8_t>;
//template class MarchingCubes<uint16_t>;
//template class MarchingCubes<uint32_t>


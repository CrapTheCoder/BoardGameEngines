#include <bits/stdc++.h>

using namespace std;

const int BENT_SIZE = 93;
const int MAX_PLY = 100, MAX_MERGES = 700;
const int MAX_CHECKPOINTS = 100;
const int TIME_FREQUENCY = 8;
const int ALL_SIDES = 7;
const int EVAL_FACTOR = 10000;

const int WIN = 30000;
const int LOSS = -30000;

enum {
    WHITE_PIECE = 1,
    BLACK_PIECE = -1
};

typedef enum {
    BLUE_PIECE = 'B',
    RED_PIECE = 'R',
    EMPTY_PIECE = '-'
} Color;

const int neighbors[BENT_SIZE][7] = {
        {1,  24, 23, -1, -1, -1, -1},
        {0,  2,  25, 24, -1, -1, -1},
        {1,  3,  26, 25, -1, -1, -1},
        {4,  2,  26, 27, -1, -1, -1},
        {3,  27, 5,  28, -1, -1, -1},
        {4,  6,  29, 28, -1, -1, -1},
        {5,  7,  30, 29, -1, -1, -1},
        {8,  6,  30, 31, -1, -1, -1},
        {7,  31, 9,  -1, -1, -1, -1},
        {10, 8,  32, 31, -1, -1, -1},
        {9,  11, 32, 33, -1, -1, -1},
        {10, 12, 33, 34, -1, -1, -1},
        {34, 35, 11, 13, -1, -1, -1},
        {14, 12, 36, 35, -1, -1, -1},
        {13, 15, 36, 37, -1, -1, -1},
        {14, 16, 37, 38, -1, -1, -1},
        {17, 38, 15, -1, -1, -1, -1},
        {16, 18, 39, 38, -1, -1, -1},
        {19, 17, 39, 40, -1, -1, -1},
        {18, 20, 40, 41, -1, -1, -1},
        {21, 42, 41, 19, -1, -1, -1},
        {20, 22, 43, 42, -1, -1, -1},
        {23, 21, 43, 44, -1, -1, -1},
        {22, 0,  44, 24, -1, -1, -1},
        {45, 0,  44, 25, 1,  23, -1},
        {2,  45, 1,  26, 24, 46, -1},
        {47, 2,  25, 3,  27, 46, -1},
        {47, 4,  48, 26, 3,  28, -1},
        {49, 4,  48, 29, 27, 5,  -1},
        {6,  49, 5,  30, 28, 50, -1},
        {51, 6,  29, 7,  31, 50, -1},
        {51, 8,  32, 30, 7,  9,  -1},
        {10, 51, 52, 31, 9,  33, -1},
        {53, 10, 52, 34, 11, 32, -1},
        {12, 53, 33, 54, 11, 35, -1},
        {55, 12, 36, 54, 34, 13, -1},
        {14, 55, 56, 35, 13, 37, -1},
        {57, 14, 56, 38, 15, 36, -1},
        {57, 16, 37, 39, 17, 15, -1},
        {18, 57, 40, 17, 38, 58, -1},
        {59, 18, 41, 58, 19, 39, -1},
        {59, 20, 40, 60, 42, 19, -1},
        {61, 20, 60, 43, 21, 41, -1},
        {22, 61, 44, 21, 42, 62, -1},
        {45, 22, 24, 62, 23, 43, -1},
        {24, 63, 46, 62, 25, 44, -1},
        {45, 47, 25, 63, 64, 26, -1},
        {27, 64, 26, 48, 46, 65, -1},
        {49, 47, 65, 27, 28, 66, -1},
        {28, 67, 48, 50, 66, 29, -1},
        {49, 51, 29, 67, 68, 30, -1},
        {31, 68, 30, 32, 50, 52, -1},
        {51, 53, 68, 32, 69, 33, -1},
        {69, 34, 33, 54, 52, 70, -1},
        {55, 53, 70, 34, 35, 71, -1},
        {35, 72, 54, 36, 71, 56, -1},
        {55, 57, 72, 36, 73, 37, -1},
        {73, 38, 58, 37, 39, 56, -1},
        {57, 59, 40, 74, 73, 39, -1},
        {41, 74, 75, 58, 60, 40, -1},
        {61, 59, 42, 76, 41, 75, -1},
        {77, 42, 62, 76, 43, 60, -1},
        {61, 45, 44, 63, 77, 43, -1},
        {78, 45, 46, 62, 64, 77, -1},
        {47, 78, 65, 46, 63, 79, -1},
        {47, 80, 64, 48, 79, 66, -1},
        {80, 49, 65, 81, 67, 48, -1},
        {49, 82, 50, 66, 81, 68, -1},
        {51, 82, 52, 50, 67, 69, -1},
        {53, 82, 70, 52, 68, 83, -1},
        {53, 84, 69, 54, 83, 71, -1},
        {84, 55, 70, 85, 72, 54, -1},
        {55, 86, 56, 71, 85, 73, -1},
        {57, 86, 58, 56, 74, 72, -1},
        {86, 59, 75, 58, 87, 73, -1},
        {59, 88, 60, 74, 87, 76, -1},
        {88, 61, 60, 77, 89, 75, -1},
        {61, 78, 76, 62, 63, 89, -1},
        {63, 64, 77, 79, 89, -1, -1},
        {80, 78, 90, 65, 64, 89, -1},
        {79, 81, 66, 91, 65, 90, -1},
        {82, 80, 66, 91, 67, 83, -1},
        {81, 67, 68, 69, 83, -1, -1},
        {84, 82, 91, 70, 81, 69, -1},
        {83, 85, 91, 71, 92, 70, -1},
        {84, 86, 71, 92, 87, 72, -1},
        {74, 87, 85, 72, 73, -1, -1},
        {88, 86, 92, 75, 74, 85, -1},
        {89, 87, 76, 92, 75, 90, -1},
        {88, 78, 76, 90, 77, 79, -1},
        {80, 88, 79, 91, 89, 92, -1},
        {84, 80, 90, 81, 83, 92, -1},
        {88, 84, 90, 87, 85, 91, -1}};

const int VIRTUAL_ENEMY[BENT_SIZE][7][2] = {{{1,  23}, {-1, -1}},
                                            {{0,  25}, {2,  24}, {-1, -1}},
                                            {{1,  26}, {3,  25}, {-1, -1}},
                                            {{2,  27}, {4,  26}, {-1, -1}},
                                            {{3,  28}, {5,  27}, {-1, -1}},
                                            {{4,  29}, {6,  28}, {-1, -1}},
                                            {{5,  30}, {7,  29}, {-1, -1}},
                                            {{6,  31}, {8,  30}, {-1, -1}},
                                            {{7,  9},  {-1, -1}},
                                            {{8,  32}, {10, 31}, {-1, -1}},
                                            {{9,  33}, {11, 32}, {-1, -1}},
                                            {{10, 34}, {12, 33}, {-1, -1}},
                                            {{11, 35}, {13, 34}, {-1, -1}},
                                            {{12, 36}, {14, 35}, {-1, -1}},
                                            {{13, 37}, {15, 36}, {-1, -1}},
                                            {{14, 38}, {16, 37}, {-1, -1}},
                                            {{15, 17}, {-1, -1}},
                                            {{16, 39}, {18, 38}, {-1, -1}},
                                            {{17, 40}, {19, 39}, {-1, -1}},
                                            {{18, 41}, {20, 40}, {-1, -1}},
                                            {{19, 42}, {21, 41}, {-1, -1}},
                                            {{20, 43}, {22, 42}, {-1, -1}},
                                            {{21, 44}, {23, 43}, {-1, -1}},
                                            {{0,  44}, {22, 24}, {-1, -1}},
                                            {{0,  25}, {0,  44}, {1,  23}, {1,  45}, {23, 45}, {25, 44}, {-1, -1}},
                                            {{1,  26}, {1,  45}, {2,  24}, {2,  46}, {24, 46}, {26, 45}, {-1, -1}},
                                            {{2,  27}, {2,  46}, {3,  25}, {3,  47}, {25, 47}, {27, 46}, {-1, -1}},
                                            {{3,  28}, {3,  47}, {4,  26}, {4,  48}, {26, 48}, {28, 47}, {-1, -1}},
                                            {{4,  29}, {4,  48}, {5,  27}, {5,  49}, {27, 49}, {29, 48}, {-1, -1}},
                                            {{5,  30}, {5,  49}, {6,  28}, {6,  50}, {28, 50}, {30, 49}, {-1, -1}},
                                            {{6,  31}, {6,  50}, {7,  29}, {7,  51}, {29, 51}, {31, 50}, {-1, -1}},
                                            {{7,  9},  {7,  51}, {8,  30}, {8,  32}, {9,  51}, {30, 32}, {-1, -1}},
                                            {{9,  33}, {9,  51}, {10, 31}, {10, 52}, {31, 52}, {33, 51}, {-1, -1}},
                                            {{10, 34}, {10, 52}, {11, 32}, {11, 53}, {32, 53}, {34, 52}, {-1, -1}},
                                            {{11, 35}, {11, 53}, {12, 33}, {12, 54}, {33, 54}, {35, 53}, {-1, -1}},
                                            {{12, 36}, {12, 54}, {13, 34}, {13, 55}, {34, 55}, {36, 54}, {-1, -1}},
                                            {{13, 37}, {13, 55}, {14, 35}, {14, 56}, {35, 56}, {37, 55}, {-1, -1}},
                                            {{14, 38}, {14, 56}, {15, 36}, {15, 57}, {36, 57}, {38, 56}, {-1, -1}},
                                            {{15, 17}, {15, 57}, {16, 37}, {16, 39}, {17, 57}, {37, 39}, {-1, -1}},
                                            {{17, 40}, {17, 57}, {18, 38}, {18, 58}, {38, 58}, {40, 57}, {-1, -1}},
                                            {{18, 41}, {18, 58}, {19, 39}, {19, 59}, {39, 59}, {41, 58}, {-1, -1}},
                                            {{19, 42}, {19, 59}, {20, 40}, {20, 60}, {40, 60}, {42, 59}, {-1, -1}},
                                            {{20, 43}, {20, 60}, {21, 41}, {21, 61}, {41, 61}, {43, 60}, {-1, -1}},
                                            {{21, 44}, {21, 61}, {22, 42}, {22, 62}, {42, 62}, {44, 61}, {-1, -1}},
                                            {{22, 24}, {22, 62}, {23, 43}, {23, 45}, {24, 62}, {43, 45}, {-1, -1}},
                                            {{24, 46}, {24, 62}, {25, 44}, {25, 63}, {44, 63}, {46, 62}, {-1, -1}},
                                            {{25, 47}, {25, 63}, {26, 45}, {26, 64}, {45, 64}, {47, 63}, {-1, -1}},
                                            {{26, 48}, {26, 64}, {27, 46}, {27, 65}, {46, 65}, {48, 64}, {-1, -1}},
                                            {{27, 49}, {27, 65}, {28, 47}, {28, 66}, {47, 66}, {49, 65}, {-1, -1}},
                                            {{28, 50}, {28, 66}, {29, 48}, {29, 67}, {48, 67}, {50, 66}, {-1, -1}},
                                            {{29, 51}, {29, 67}, {30, 49}, {30, 68}, {49, 68}, {51, 67}, {-1, -1}},
                                            {{30, 32}, {30, 68}, {31, 50}, {31, 52}, {32, 68}, {50, 52}, {-1, -1}},
                                            {{32, 53}, {32, 68}, {33, 51}, {33, 69}, {51, 69}, {53, 68}, {-1, -1}},
                                            {{33, 54}, {33, 69}, {34, 52}, {34, 70}, {52, 70}, {54, 69}, {-1, -1}},
                                            {{34, 55}, {34, 70}, {35, 53}, {35, 71}, {53, 71}, {55, 70}, {-1, -1}},
                                            {{35, 56}, {35, 71}, {36, 54}, {36, 72}, {54, 72}, {56, 71}, {-1, -1}},
                                            {{36, 57}, {36, 72}, {37, 55}, {37, 73}, {55, 73}, {57, 72}, {-1, -1}},
                                            {{37, 39}, {37, 73}, {38, 56}, {38, 58}, {39, 73}, {56, 58}, {-1, -1}},
                                            {{39, 59}, {39, 73}, {40, 57}, {40, 74}, {57, 74}, {59, 73}, {-1, -1}},
                                            {{40, 60}, {40, 74}, {41, 58}, {41, 75}, {58, 75}, {60, 74}, {-1, -1}},
                                            {{41, 61}, {41, 75}, {42, 59}, {42, 76}, {59, 76}, {61, 75}, {-1, -1}},
                                            {{42, 62}, {42, 76}, {43, 60}, {43, 77}, {60, 77}, {62, 76}, {-1, -1}},
                                            {{43, 45}, {43, 77}, {44, 61}, {44, 63}, {45, 77}, {61, 63}, {-1, -1}},
                                            {{45, 64}, {45, 77}, {46, 62}, {46, 78}, {62, 78}, {64, 77}, {-1, -1}},
                                            {{46, 65}, {46, 78}, {47, 63}, {47, 79}, {63, 79}, {65, 78}, {-1, -1}},
                                            {{47, 66}, {47, 79}, {48, 64}, {48, 80}, {64, 80}, {66, 79}, {-1, -1}},
                                            {{48, 67}, {48, 80}, {49, 65}, {49, 81}, {65, 81}, {67, 80}, {-1, -1}},
                                            {{49, 68}, {49, 81}, {50, 66}, {50, 82}, {66, 82}, {68, 81}, {-1, -1}},
                                            {{50, 52}, {50, 82}, {51, 67}, {51, 69}, {52, 82}, {67, 69}, {-1, -1}},
                                            {{52, 70}, {52, 82}, {53, 68}, {53, 83}, {68, 83}, {70, 82}, {-1, -1}},
                                            {{53, 71}, {53, 83}, {54, 69}, {54, 84}, {69, 84}, {71, 83}, {-1, -1}},
                                            {{54, 72}, {54, 84}, {55, 70}, {55, 85}, {70, 85}, {72, 84}, {-1, -1}},
                                            {{55, 73}, {55, 85}, {56, 71}, {56, 86}, {71, 86}, {73, 85}, {-1, -1}},
                                            {{56, 58}, {56, 86}, {57, 72}, {57, 74}, {58, 86}, {72, 74}, {-1, -1}},
                                            {{58, 75}, {58, 86}, {59, 73}, {59, 87}, {73, 87}, {75, 86}, {-1, -1}},
                                            {{59, 76}, {59, 87}, {60, 74}, {60, 88}, {74, 88}, {76, 87}, {-1, -1}},
                                            {{60, 77}, {60, 88}, {61, 75}, {61, 89}, {75, 89}, {77, 88}, {-1, -1}},
                                            {{61, 63}, {61, 89}, {62, 76}, {62, 78}, {63, 89}, {76, 78}, {-1, -1}},
                                            {{63, 79}, {63, 89}, {64, 77}, {64, 89}, {77, 79}, {-1, -1}},
                                            {{64, 80}, {64, 89}, {65, 78}, {65, 90}, {78, 90}, {80, 89}, {-1, -1}},
                                            {{65, 81}, {65, 90}, {66, 79}, {66, 91}, {79, 91}, {81, 90}, {-1, -1}},
                                            {{66, 82}, {66, 91}, {67, 80}, {67, 83}, {80, 83}, {82, 91}, {-1, -1}},
                                            {{67, 69}, {67, 83}, {68, 81}, {68, 83}, {69, 81}, {-1, -1}},
                                            {{69, 81}, {69, 84}, {70, 82}, {70, 91}, {81, 84}, {82, 91}, {-1, -1}},
                                            {{70, 85}, {70, 91}, {71, 83}, {71, 92}, {83, 92}, {85, 91}, {-1, -1}},
                                            {{71, 86}, {71, 92}, {72, 84}, {72, 87}, {84, 87}, {86, 92}, {-1, -1}},
                                            {{72, 74}, {72, 87}, {73, 85}, {73, 87}, {74, 85}, {-1, -1}},
                                            {{74, 85}, {74, 88}, {75, 86}, {75, 92}, {85, 88}, {86, 92}, {-1, -1}},
                                            {{75, 89}, {75, 92}, {76, 87}, {76, 90}, {87, 90}, {89, 92}, {-1, -1}},
                                            {{76, 78}, {76, 90}, {77, 79}, {77, 88}, {78, 90}, {79, 88}, {-1, -1}},
                                            {{79, 88}, {79, 91}, {80, 89}, {80, 92}, {88, 91}, {89, 92}, {-1, -1}},
                                            {{80, 83}, {80, 92}, {81, 84}, {81, 90}, {83, 92}, {84, 90}, {-1, -1}},
                                            {{84, 87}, {84, 90}, {85, 88}, {85, 91}, {87, 90}, {88, 91}, {-1, -1}}};

const int VIRTUAL_FRIEND[BENT_SIZE][7] = {{24, -1},
                                          {24, 25, -1},
                                          {25, 26, -1},
                                          {26, 27, -1},
                                          {27, 28, -1},
                                          {28, 29, -1},
                                          {29, 30, -1},
                                          {30, 31, -1},
                                          {31, -1},
                                          {31, 32, -1},
                                          {32, 33, -1},
                                          {33, 34, -1},
                                          {34, 35, -1},
                                          {35, 36, -1},
                                          {36, 37, -1},
                                          {37, 38, -1},
                                          {38, -1},
                                          {38, 39, -1},
                                          {39, 40, -1},
                                          {40, 41, -1},
                                          {41, 42, -1},
                                          {42, 43, -1},
                                          {43, 44, -1},
                                          {24, 44, -1},
                                          {1,  23, 0,  25, 44, 45, -1},
                                          {2,  24, 1,  26, 45, 46, -1},
                                          {3,  25, 2,  27, 46, 47, -1},
                                          {4,  26, 3,  28, 47, 48, -1},
                                          {5,  27, 4,  29, 48, 49, -1},
                                          {6,  28, 5,  30, 49, 50, -1},
                                          {7,  29, 6,  31, 50, 51, -1},
                                          {8,  30, 7,  9,  32, 51, -1},
                                          {10, 31, 9,  33, 51, 52, -1},
                                          {11, 32, 10, 34, 52, 53, -1},
                                          {12, 33, 11, 35, 53, 54, -1},
                                          {13, 34, 12, 36, 54, 55, -1},
                                          {14, 35, 13, 37, 55, 56, -1},
                                          {15, 36, 14, 38, 56, 57, -1},
                                          {16, 37, 15, 17, 39, 57, -1},
                                          {18, 38, 17, 40, 57, 58, -1},
                                          {19, 39, 18, 41, 58, 59, -1},
                                          {20, 40, 19, 42, 59, 60, -1},
                                          {21, 41, 20, 43, 60, 61, -1},
                                          {22, 42, 21, 44, 61, 62, -1},
                                          {23, 43, 22, 24, 45, 62, -1},
                                          {25, 44, 24, 46, 62, 63, -1},
                                          {26, 45, 25, 47, 63, 64, -1},
                                          {27, 46, 26, 48, 64, 65, -1},
                                          {28, 47, 27, 49, 65, 66, -1},
                                          {29, 48, 28, 50, 66, 67, -1},
                                          {30, 49, 29, 51, 67, 68, -1},
                                          {31, 50, 30, 32, 52, 68, -1},
                                          {33, 51, 32, 53, 68, 69, -1},
                                          {34, 52, 33, 54, 69, 70, -1},
                                          {35, 53, 34, 55, 70, 71, -1},
                                          {36, 54, 35, 56, 71, 72, -1},
                                          {37, 55, 36, 57, 72, 73, -1},
                                          {38, 56, 37, 39, 58, 73, -1},
                                          {40, 57, 39, 59, 73, 74, -1},
                                          {41, 58, 40, 60, 74, 75, -1},
                                          {42, 59, 41, 61, 75, 76, -1},
                                          {43, 60, 42, 62, 76, 77, -1},
                                          {44, 61, 43, 45, 63, 77, -1},
                                          {46, 62, 45, 64, 77, 78, -1},
                                          {47, 63, 46, 65, 78, 79, -1},
                                          {48, 64, 47, 66, 79, 80, -1},
                                          {49, 65, 48, 67, 80, 81, -1},
                                          {50, 66, 49, 68, 81, 82, -1},
                                          {51, 67, 50, 52, 69, 82, -1},
                                          {53, 68, 52, 70, 82, 83, -1},
                                          {54, 69, 53, 71, 83, 84, -1},
                                          {55, 70, 54, 72, 84, 85, -1},
                                          {56, 71, 55, 73, 85, 86, -1},
                                          {57, 72, 56, 58, 74, 86, -1},
                                          {59, 73, 58, 75, 86, 87, -1},
                                          {60, 74, 59, 76, 87, 88, -1},
                                          {61, 75, 60, 77, 88, 89, -1},
                                          {62, 76, 61, 63, 78, 89, -1},
                                          {64, 77, 63, 79, 89, -1},
                                          {65, 78, 64, 80, 89, 90, -1},
                                          {66, 79, 65, 81, 90, 91, -1},
                                          {67, 80, 66, 82, 91, 83, -1},
                                          {68, 81, 67, 69, 83, -1},
                                          {82, 70, 69, 84, 91, 81, -1},
                                          {71, 83, 70, 85, 91, 92, -1},
                                          {72, 84, 71, 86, 92, 87, -1},
                                          {73, 85, 72, 74, 87, -1},
                                          {86, 75, 74, 88, 92, 85, -1},
                                          {76, 87, 75, 89, 92, 90, -1},
                                          {77, 88, 78, 76, 79, 90, -1},
                                          {89, 80, 79, 91, 92, 88, -1},
                                          {81, 90, 83, 80, 84, 92, -1},
                                          {85, 91, 87, 84, 88, 90, -1}};

const int ADJACENT_FRIEND[BENT_SIZE][2] = {{23, 1},
                                           {0,  2},
                                           {1,  3},
                                           {2,  4},
                                           {3,  5},
                                           {4,  6},
                                           {5,  7},
                                           {6,  8},
                                           {7,  9},
                                           {8,  10},
                                           {9,  11},
                                           {10, 12},
                                           {11, 13},
                                           {12, 14},
                                           {13, 15},
                                           {14, 16},
                                           {15, 17},
                                           {16, 18},
                                           {17, 19},
                                           {18, 20},
                                           {19, 21},
                                           {20, 22},
                                           {21, 23},
                                           {22, 0}};

const int ADJACENT_ENEMY[BENT_SIZE][2] = {{24, 24},
                                          {24, 25},
                                          {25, 26},
                                          {26, 27},
                                          {27, 28},
                                          {28, 29},
                                          {29, 30},
                                          {30, 31},
                                          {31, 31},
                                          {31, 32},
                                          {32, 33},
                                          {33, 34},
                                          {34, 35},
                                          {35, 36},
                                          {36, 37},
                                          {37, 38},
                                          {38, 38},
                                          {38, 39},
                                          {39, 40},
                                          {40, 41},
                                          {41, 42},
                                          {42, 43},
                                          {43, 44},
                                          {44, 24}};

const int EDGES[BENT_SIZE] = {5, 1, 1, 1, 1, 1, 1, 1,
                              3, 2, 2, 2, 2, 2, 2, 2,
                              6, 4, 4, 4, 4, 4, 4, 4};

typedef enum {
    BLUE_WIN = 0,
    RED_WIN = 1,
    IN_PROGRESS = 2
} Result;

class DSU {
    struct Merge {
        int u{}, u_value{}, u_sides{};
        int v{}, v_value{};
        int winner_index = -1;
    };

    Merge merges[MAX_MERGES];

    int parent[BENT_SIZE]{}, sides[BENT_SIZE]{};
    int snapshots[MAX_CHECKPOINTS]{};
    int merge_count = 0, snapshot_count = 0, snapshot_size = 0;

    int find_root(int w) {
        while (parent[w] >= 0)
            w = parent[w];

        return w;
    }

public:
    DSU() {
        memset(parent, -1, sizeof(parent));
        for (int i = 0; i <= 23; i++)
            sides[i] |= EDGES[i];
    }

    bool merge(int u, int v) {
        u = find_root(u), v = find_root(v);

        if (u == v)
            return false;

        if (-parent[u] < -parent[v])
            swap(u, v);

        merges[merge_count] = {
                u, parent[u], sides[u],
                v, parent[v]
        };

        parent[u] += parent[v];
        sides[u] |= sides[v];
        parent[v] = u;

        merge_count++, snapshot_size++;
        merges[merge_count].winner_index = (sides[u] == ALL_SIDES) ? u : merges[merge_count - 1].winner_index;

        return true;
    }

    void rollback_snapshot() {
        int steps = snapshots[--snapshot_count];
        while (steps--) {
            --merge_count;

            parent[merges[merge_count].u] = merges[merge_count].u_value;
            sides[merges[merge_count].u] = merges[merge_count].u_sides;
            parent[merges[merge_count].v] = merges[merge_count].v_value;
        }
    }

    inline void update_snapshot() {
        snapshots[snapshot_count++] = snapshot_size;
        snapshot_size = 0;
    }

    inline int get_win_index() {
        return merges[merge_count].winner_index;
    }
};

__uint128_t g_lehmer64_state = 5135432 * 0xda942042e4dd58b5;

inline unsigned int lehmer32() {
    return (g_lehmer64_state *= 0xda942042e4dd58b5) >> 96;
}

inline unsigned int random_bounded(unsigned long long range) {
    return (range * lehmer32()) >> 32;
}

inline void shuffle(int array[], unsigned int size) {
    if (size == 0)
        return;

    for (unsigned int i = size - 1; i > 0; i--)
        swap(array[i], array[random_bounded(i)]);
}

const int TT_SIZE = 1 << 20;

struct Entry {
    unsigned long long zobrist = ULLONG_MAX;
    int current_eval;
    pair<int, int> move_list[BENT_SIZE];
};

Entry entries[TT_SIZE]{};

class TranspositionTable {
    static inline unsigned int get_hash(const unsigned long long &zobrist) {
        return zobrist & (TT_SIZE - 1);
    }

public:
    static void insert(const unsigned long long &zobrist,
                       const int &current_eval,
                       const pair<int, int> move_list[BENT_SIZE]) {

        unsigned int hash = get_hash(zobrist);
        entries[hash] = {zobrist, current_eval};
        copy(move_list, move_list + BENT_SIZE, entries[hash].move_list);
    }

    static bool find(const unsigned long long &zobrist,
                     int &current_eval,
                     pair<int, int> move_list[BENT_SIZE]) {

        unsigned int hash = get_hash(zobrist);
        if (entries[hash].zobrist == zobrist) {
            current_eval = entries[hash].current_eval;
            copy(entries[hash].move_list, entries[hash].move_list + BENT_SIZE, move_list);
            return true;
        }

        return false;
    }
};

class Y {
    unsigned long long zobrist_values[BENT_SIZE][2]{};
    mt19937_64 rng;

    DSU dsu;

    bitset<BENT_SIZE> white_pieces, black_pieces;

    int moves[MAX_PLY]{};
    int move_count = 0;

    int player_piece = WHITE_PIECE;

    void move_color(const int &index, const int &move_piece) {
        auto &player = (move_piece == WHITE_PIECE) ? white_pieces : black_pieces;
        player.set(index);

        moves[move_count++] = index;
        zobrist ^= zobrist_values[index][move_piece == WHITE_PIECE];

        for (int i = 0; neighbors[index][i] != -1; i++)
            if (player[neighbors[index][i]])
                dsu.merge(index, neighbors[index][i]);

        dsu.update_snapshot();
    }

public:
    unsigned long long zobrist = 0;

    Y() : rng(chrono::steady_clock::now().time_since_epoch().count()) {
        for (auto &zobrist_value: zobrist_values)
            zobrist_value[0] = rng(), zobrist_value[1] = rng();
    }

    void move(const int &index) {
        auto &player = (player_piece == WHITE_PIECE) ? white_pieces : black_pieces;
        player.set(index);

        moves[move_count++] = index;
        zobrist ^= zobrist_values[index][player_piece == WHITE_PIECE];

        for (int i = 0; neighbors[index][i] != -1; i++)
            if (player[neighbors[index][i]])
                dsu.merge(index, neighbors[index][i]);

        dsu.update_snapshot();
        player_piece *= -1;
    }

    void unmove() {
        player_piece *= -1;
        dsu.rollback_snapshot();

        const int index = moves[--move_count];
        zobrist ^= zobrist_values[index][player_piece == WHITE_PIECE];

        auto &player = (player_piece == 1) ? white_pieces : black_pieces;
        player.reset(index);
    }

    Result get_result() {
        const int index = dsu.get_win_index();

        if (index == -1) return IN_PROGRESS;
        if (white_pieces[index]) return BLUE_WIN;
        if (black_pieces[index]) return RED_WIN;

        assert(false);
    }

    [[nodiscard]] Color get_turn() const {
        return (player_piece == WHITE_PIECE) ? BLUE_PIECE : RED_PIECE;
    }

    int get_moves(const int &playout_limit, pair<int, int> move_list[BENT_SIZE]) {
        int wins[BENT_SIZE]{}, counts[BENT_SIZE]{};
        int total_wins = 0;

        for (int playout = 0; playout < playout_limit; playout++) {
            auto player_1 = white_pieces, player_2 = black_pieces;

            int empty_cells[93], empty_count = 0;
            for (int index = 0; index < BENT_SIZE; index++)
                if (!(player_1[index] || player_2[index]))
                    empty_cells[empty_count++] = index;

            if (player_piece == BLACK_PIECE)
                swap(player_1, player_2);

            shuffle(empty_cells, empty_count);
            for (int i = 0; i < empty_count; i++) {
                const int &index = empty_cells[i];

                if (player_1[index] || player_2[index])
                    continue;

                player_1.set(index);

                vector<int> threats;

                for (int x = 0; VIRTUAL_FRIEND[index][x] != -1; x++) {
                    if (player_2[VIRTUAL_ENEMY[index][x][0]] && player_2[VIRTUAL_ENEMY[index][x][1]] &&
                        !player_1[VIRTUAL_FRIEND[index][x]] && !player_2[VIRTUAL_FRIEND[index][x]]) {

                        threats.push_back(VIRTUAL_FRIEND[index][x]);
                    }
                }

                if (!threats.empty()) {
                    player_2[threats[random_bounded(threats.size())]] = true;
                    continue;
                }

                if (0 <= index && index <= 23) {
                    for (int x = 0; x <= 1; x++) {
                        if (player_2[ADJACENT_ENEMY[index][x]] &&
                            !player_1[ADJACENT_FRIEND[index][x]] && !player_2[ADJACENT_FRIEND[index][x]]) {

                            threats.push_back(ADJACENT_FRIEND[index][x]);
                        }
                    }

                    if (!threats.empty()) {
                        player_2[threats[random_bounded(threats.size())]] = true;
                        continue;
                    }
                }

                swap(player_1, player_2);
            }

            const auto &player = (player_piece == WHITE_PIECE) ? player_2 : player_1;

            bool visited[BENT_SIZE]{};
            auto dfs = [&](int u, auto &&dfs) -> int {
                int side = EDGES[u];
                for (const int &v: neighbors[u]) {
                    if (v == -1)
                        break;

                    if (!visited[v] && player[v]) {
                        visited[v] = true;
                        side |= dfs(v, dfs);
                    }
                }

                return side;
            };

            bool has_won = false;
            for (int i = 0; i <= 8; i++) {
                if (!visited[i] && player[i]) {
                    visited[i] = true;
                    if (dfs(i, dfs) == 7) {
                        has_won = true;
                        total_wins++;
                        break;
                    }
                }
            }

            for (int i = 0; i < empty_count; i++) {
                int index = empty_cells[i];
                if (player[index]) {
                    counts[index]++;
                    if (has_won)
                        wins[index]++;
                }
            }
        }

        for (int position = 0; position < BENT_SIZE; position++) {
            if (counts[position] != 0)
                move_list[position] = {wins[position] * EVAL_FACTOR / counts[position], position};
            else
                move_list[position] = {-EVAL_FACTOR, position};
        }

        sort(move_list, move_list + BENT_SIZE, greater<>());
        return move_list[0].first - (EVAL_FACTOR >> 1);
    }

    string get_board_state() {
        string board_state;
        for (int i = 0; i < BENT_SIZE; i++) {
            if (white_pieces[i])
                board_state += (char) (BLUE_PIECE);
            else if (black_pieces[i])
                board_state += (char) (RED_PIECE);
            else
                board_state += (char) (EMPTY_PIECE);
        }

        return board_state;
    }

    void convert_board_state(const string &board_state, const int &move_piece = 0) {
        vector<int> blues, reds;

        for (int i = 0; i < 93; i++) {
            if (board_state[i] == BLUE_PIECE)
                move_color(i, WHITE_PIECE);
            else if (board_state[i] == RED_PIECE)
                move_color(i, BLACK_PIECE);
        }

        player_piece = move_piece;
        if (move_piece == 0)
            player_piece = (ranges::count(board_state, BLUE_PIECE) <= ranges::count(board_state, RED_PIECE)) ? 1 : -1;
    }
};

class Engine {
    Y game;

    const int time_limit;
    int iterations = 0;

    bool timeout = false;
    chrono::time_point<chrono::high_resolution_clock> start{};

    int pv_move = -1;

    long duration() {
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }

    int negamax(int alpha, int beta, int depth, int ply) {
        if (game.get_result() != IN_PROGRESS)
            return LOSS + 100 * ply;

        if (timeout || (++iterations % TIME_FREQUENCY == 0 && duration() >= time_limit)) {
            timeout = true;
            return 0;
        }

        pair<int, int> moves[93];
        int current_eval;

        if (!TranspositionTable::find(game.zobrist, current_eval, moves)) {
            current_eval = game.get_moves(4096, moves);
            TranspositionTable::insert(game.zobrist, current_eval, moves);
        }

        if (depth == 0)
            return current_eval;

        int move_count = 5;
        for (const auto [_, move]: moves) {
            if (!move_count--)
                break;

            game.move(move);
            int eval = -negamax(-beta, -alpha, depth - 1, ply + 1);
            game.unmove();

            if (eval > alpha) {
                alpha = eval;

                if (ply == 0)
                    pv_move = move;

                if (eval >= beta)
                    return beta;
            }
        }

        return alpha;
    }

    pair<int, int> iterative_deepening() {
        pv_move = -1;

        timeout = false;
        iterations = 0;

        start = chrono::high_resolution_clock::now();

        int best_move = -1, best_eval = -1;
        int alpha = LOSS, beta = WIN;

        for (int depth = 1; depth <= 15;) {
            int current_eval = negamax(alpha, beta, depth, 0);

            if (timeout)
                break;

            best_eval = current_eval;
            best_move = pv_move;

            depth++;
        }

        return {best_move, best_eval};
    }

public:
    explicit Engine(const double &time_limit) : time_limit((int) (time_limit * 850)) {}

    [[maybe_unused]] explicit Engine(const double &limit, const string &position) : time_limit((int) (limit * 850)) {
        game.convert_board_state(position);
    }

    pair<int, int> get_move() {
        if (game.get_result() != IN_PROGRESS)
            return {-1, false};

        auto [best_move, eval] = iterative_deepening();

        game.move(best_move);
        return {best_move, game.get_result()};
    }

    bool should_swap() {
        auto [best_move, eval] = iterative_deepening();
        return eval < 0;
    }

    void make_move(const int &index) {
        game.move(index);
    }

    Result get_result() {
        return game.get_result();
    }

    Color get_turn() {
        return game.get_turn();
    }

    string get_board() {
        return game.get_board_state();
    }
};

void start_engine() {
    float time_limit;

    cout << "\nEnter time per move in seconds: ";
    cin >> time_limit;

    cout << "\nCommands:\n";
    cout << "* get_move (g) - get an engine move\n";
    cout << "* play index (p) - make a move at index\n";
    cout << "* should_swap (s) - check if the current player should swap\n\n";

    Engine engine(time_limit);

    while (true) {
        cout << "Turn: " << (engine.get_turn() == BLUE_PIECE ? "Blue" : "Red") << endl;
        cout << "Board state: " << engine.get_board() << endl;

        string command;
        cin >> command;

        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command == "get_move" || command == "g" || command == "get") {
            auto [move, won] = engine.get_move();
            cout << "Engine plays: " << move + 1 << endl;

        } else if (command == "play" || command == "p") {
            int index;
            cin >> index;
            engine.make_move(index - 1);

        } else if (command == "should_swap" || command == "s" || command == "swap") {
            cout << (engine.should_swap() ? "Yes" : "No") << endl;
        }

        if (engine.get_result() != IN_PROGRESS) {
            cout << (engine.get_result() == BLUE_WIN ? "Blue" : "Red") << " wins\n";
            break;
        }
    }
}

int main() {
    start_engine();
}

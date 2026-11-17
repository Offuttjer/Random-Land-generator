#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>
#include <cstdint>



double frand();
void dropDirtball(int** grid, int width, int height, int radius, int strength);
int boundCheck(int width, int height, int x, int y);
int findMax(int** grid, int width, int height);
void normalize255(int** grid, int width, int height);
char** polish(int** grid, int width, int height, int waterLine);
int printToFile(int** grid, int width, int height, const char* fileName, bool toConsole);
int printToFile(char** grid, int width, int height, const char* fileName, bool toConsole);
bool inCircle(int xpoint, int ypoint, int xcenter, int ycenter, int radius);
double distance(int xpoint, int ypoint, int xcenter, int ycenter);
void makeBMP(char** grid, int width, int height, const char * fileName);

//followed the tutorial in: https://medium.com/@muiz6/c-how-to-write-a-bitmap-image-from-scratch-f6449bdfa548
#pragma pack(push, 1)
struct BmpHeader {
    char bitMapSignatureBytes [2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile = 54 + 786432;
    uint32_t reservedByte  = 0;
    uint32_t pixelDataOffset = 54;
};

struct BMPInfo
{
    uint32_t sizeOfThisHeader = 40;
    int32_t width = 52; //in pixels 
    int32_t height = 512;
    uint16_t numberOfColorPlanes = 1;
    uint16_t colorDepth = 24;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize =0; //ignored
    int32_t horizontalResolution = 3780; //pixels per meter
    int32_t verticalResolution = 3780;
    uint32_t colorTableEntries =0;
    uint32_t importantColors =0;
};

struct pixel
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
#pragma pack(pop)

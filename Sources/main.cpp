#include <iostream>
#include <ctime>
#include <thread>
#include <assert.h>

#include <windows.h>

#include "test.hpp"
#include "mathHelper.hpp"
#include "algo.hpp"

#define Black       0x0
#define Blue        0x1
#define Green       0x2
#define Aqua        0x3
#define Red         0x4
#define Purple      0x5
#define Yellow      0x6
#define White       0x7
#define Gray        0x8
#define LightBlue   0x9
#define LightGreen  0xa
#define LightAqua   0xb
#define LightRed    0xc
#define LightPurple 0xd
#define LightYellow 0xe
#define BrightWhite 0xf

static int ColorPalete[] =
{
    Black,
    Gray,
    Purple,
    LightPurple,
    Blue,
    LightBlue,
    Aqua,
    LightAqua,
    Green,
    LightGreen,
    Yellow,
    LightYellow,
    Red,
    LightRed,
    White,
    BrightWhite,
};

Vec3D rotate(const Vec3D &in, float rotation)
{
    Vec4D pos = Vec4D(in.x, in.y, in.z, 1);
    Mat4D rot;
    pos = rot.CreateTransformMatrix(Vec3D(4*rotation, 3*rotation, 2*rotation), Vec3D(48, 48, 0), Vec3D(10, 10, 10)) * pos;
    return Vec3D(pos.x, pos.y, pos.z);
}

struct Triangle
{
    Vec3D vertices[3];
    unsigned char color;
};

Triangle tris[] = {
    {Vec3D(-1.740639, 0.263649, -0.447956), Vec3D(0.064408, -0.592643, 0.447956), Vec3D(0.000000, 0.000000, 0.895913)    ,1u },
    {Vec3D(-0.303772, 0.664045, -0.000000), Vec3D(1.570167, -0.593793, 0.000000), Vec3D(-0.456986, -0.394036, -0.300000) ,2u },
    {Vec3D(-0.760758, 0.270010, 0.300000), Vec3D(0.456986, 0.394035, -0.300000), Vec3D(-0.514815, 0.212329, -0.300000)   ,3u },
    {Vec3D(-0.303772, 0.664045, -0.000000), Vec3D(-0.456986, -0.394036, -0.300000), Vec3D(-0.971801, -0.181706, 0.000000),4u },
    {Vec3D(-0.303772, 0.664045, -0.000000), Vec3D(-0.456986, -0.394035, 0.300000), Vec3D(1.570167, -0.593793, 0.000000)  ,5u },
    {Vec3D(3.168733, -0.300000, -0.000000), Vec3D(-5.092798, 1.098304, 0.000000), Vec3D(-4.168733, 0.000000, 1.000000)   ,6u },
    {Vec3D(-1.000000, -0.300000, -1.000000), Vec3D(0.000000, 0.000000, 2.000000), Vec3D(-0.924065, 1.098304, 1.000000)   ,7u },
    {Vec3D(-1.000000, -0.300000, 1.000000), Vec3D(0.000000, 0.000000, -2.000000), Vec3D(4.168733, 0.000000, -1.000000)   ,8u },
    {Vec3D(-0.743818, -0.300000, -0.772652), Vec3D(-0.246623, 0.000000, -0.378231), Vec3D(-0.493364, 1.488035, -0.719115),9u },
    {Vec3D(-0.743818, -0.300000, 0.772652), Vec3D(-0.493364, 1.488036, 0.719115), Vec3D(-0.246623, 0.000000, 0.378231)   ,10u },
    {Vec3D(-1.346018, -0.439340, 1.086163), Vec3D(0.355577, 0.139340, 0.064720), Vec3D(0.108836, 1.627376, 0.405604)     ,11u },
    {Vec3D(-1.346019, -0.439340, -1.086163), Vec3D(0.108837, 1.627375, -0.405604), Vec3D(0.355578, 0.139340, -0.064720)  ,12u },
    {Vec3D(-1.237182, 1.188035, -1.491767), Vec3D(-0.108837, -1.627375, 0.405604), Vec3D(0.493364, -1.488035, 0.719115)  ,13u },
    {Vec3D(-0.990441, -0.300000, -1.150883), Vec3D(0.246623, 0.000000, 0.378231), Vec3D(-0.355578, -0.139340, 0.064720)  ,14u },
    {Vec3D(-0.990441, -0.300000, 1.150883), Vec3D(-0.355577, -0.139340, -0.064720), Vec3D(0.246623, 0.000000, -0.378231) ,15u },
    {Vec3D(-1.237182, 1.188036, 1.491767), Vec3D(0.493364, -1.488036, -0.719115), Vec3D(-0.108836, -1.627376, -0.405604) ,1u },
    {Vec3D(-0.582366, -0.288396, -0.530632), Vec3D(-0.166579, 0.139211, 0.000000), Vec3D(-0.000001, -0.527187, -2.703453),2u },
    {Vec3D(-0.748945, -0.149185, -0.530632), Vec3D(1.271236, -0.085085, 0.000000), Vec3D(0.166578, -0.666398, -2.703453) ,3u },
    {Vec3D(-0.582366, -0.288396, -0.530632), Vec3D(-0.000001, -0.527187, -2.703453), Vec3D(1.104657, 0.054126, 0.000000) ,4u },
    {Vec3D(0.522291, -0.234269, 0.530632), Vec3D(-1.271236, 0.085084, 0.000000), Vec3D(-1.104657, -0.581312, 2.703453)   ,5u },
    {Vec3D(-0.582366, -0.288396, 0.530632), Vec3D(1.104657, 0.054127, 0.000000), Vec3D(0.000000, -0.527185, 2.703453)    ,6u },
    {Vec3D(-0.748945, -0.149185, 0.530632), Vec3D(0.166579, -0.139211, 0.000000), Vec3D(0.166579, -0.666396, 2.703453)   ,7u },
    {Vec3D(3.168733, -0.300000, -0.000000), Vec3D(-4.168733, 0.000000, -1.000000), Vec3D(-5.092798, 1.098304, 0.000000)  ,8u },
    {Vec3D(-1.676231, -0.328994, 0.000000), Vec3D(0.360777, 0.514515, 0.279947), Vec3D(-0.064408, 0.592643, 0.447957)    ,9u },
    {Vec3D(-1.676231, -0.328994, 0.000000), Vec3D(0.596570, 0.156257, 0.000000), Vec3D(0.360777, 0.514515, 0.279947)     ,10u },
    {Vec3D(-1.315454, 0.185521, -0.279947), Vec3D(-0.360777, -0.514515, 0.279947), Vec3D(-0.425185, 0.078128, -0.168009) ,11u },
    {Vec3D(-1.315454, 0.185521, -0.279947), Vec3D(0.235793, -0.358258, 0.279947), Vec3D(-0.360777, -0.514515, 0.279947)  ,12u },
    {Vec3D(-1.315454, 0.185521, 0.279947), Vec3D(-0.425185, 0.078128, -0.727903), Vec3D(-0.425185, 0.078128, 0.168010)   ,13u },
    {Vec3D(-1.315454, 0.185521, 0.279947), Vec3D(0.000000, 0.000000, -0.559894), Vec3D(-0.425185, 0.078128, -0.727903)   ,14u }
};

const Vec3D t[6][3] = {

    {
        {-7, -4, 0},
        {0, -17, 0},
        {7, -4, 0},
    },
    {
        {0, 8, 0},
        {7, -4, 0},
        {14, 8, 0},
    },
    {
        {-14, 8, 0},
        {-7, -4, 0},
        {0, 8, 0},
    },

    {
        {7, -4, 0},
        {0, -17, 0},
        {-7, -4, 0},
    },
    {
        {14, 8, 0},
        {7, -4, 0},
        {0, 8, 0},
    },
    {
        {0, 8, 0},
        {-7, -4, 0},
        {-14, 8, 0},
    }
};

int main(int argc, char *argv[])
{
    for (int i = 0; i < sizeof(tris) / sizeof(tris[0]); i++)
    {
        tris[i].vertices[1] = tris[i].vertices[1] + tris[i].vertices[0];
        tris[i].vertices[2] = tris[i].vertices[2] + tris[i].vertices[0];
    }
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    const Int2D size(96, 96);
    unsigned char *data = new unsigned char[size.x * size.y];
    float* depthB = new float[size.x * size.y];
    // Draw "Le Force 3"

    size_t frames = 0;
    int lastColor = -1;

    while (true)
    {
        for (int i = 0; i < size.x * size.y; i++)
        {
            data[i] = 0;
            depthB[i] = 1e20;
        }
        for (int i = 0; i < sizeof(tris)/sizeof(tris[0]); i++)
        {
            Vec3D points[3];
            for (int k = 0; k < 3; k++) points[k] = rotate(tris[i].vertices[k], frames);
            size_t min = points[2].y*size.x+points[2].x;
            size_t max = points[2].y*size.x+points[2].x;
            Vec3D last = points[2];
            for (int j = 0; j < 3; j++)
            {
                size_t tmp = points[j].y*size.x+points[j].x;
                if (tmp < min) min = tmp;
                if (tmp > max) max = tmp;
                //math::drawLine(data, size, points[j], last);
                last = points[j];
            }
            for (size_t index = min-size.x-1; index < max+size.x+1; index++)
            {
                Vec3D pos = Vec3D(index%size.x+0.5f,index/size.x+0.5f, 0);
                bool out = false;
                last = points[2];
                float delta = math::edgeFunction(points[0],points[1],points[2]);
                Vec3D coords;
                float depth = 0.0f;
                for (int k = 0; k < 3; k++)
                {
                    float edge = math::edgeFunction(pos,points[k],last);
                    coords[k] = edge / delta;
                    depth += coords[k] * points[k].z;
                    if (edge < 0)
                    {
                        out = true;
                        break;
                    }
                    last = points[k];
                }
                if (!out && depth < depthB[index])
                {
                    data[index] = tris[i].color;
                    depthB[index] = depth;
                }
            }
        }
        std::cout.seekp(0, std::ios_base::_Seekbeg);
        SetConsoleCursorPosition(hConsole, {0,0});
        for (int j = 0; j < size.y; j++)
        {
            for (int i = 0; i < size.x; i++)
            {
                int color = (data[j * size.x + i]);
                if (color < 0) color = 0;
                if (color > 15) color = 15;
                int c = 15 + ColorPalete[color] * 16;
                if (c != lastColor)
                {
                    SetConsoleTextAttribute(hConsole, c);
                    lastColor = c;
                }
                std::cout.write(" ", 2);
            }
            std::cout.write("\n",1);
        }
        std::cout.flush();
        timespec tmp = {0};
        tmp.tv_nsec = (long)(1000000000 / 30);
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // nanosleep(&tmp, NULL);
        frames++;
    }

    delete[] data;
    delete[] depthB;

    return 0;
}

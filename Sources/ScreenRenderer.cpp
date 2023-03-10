#include "ScreenRenderer.hpp"

#include <chrono>

using namespace Maths;

struct Triangle
{
    Vec3 vertices[3];
    unsigned int color;
};

Triangle tris[] = {
    {Vec3(-1.740639f,  0.263649f, -0.447956f), Vec3( 0.064408f, -0.592643f,  0.447956f), Vec3( 0.000000f,  0.000000f,  0.895913f), 0xff0000 },
    {Vec3(-0.303772f,  0.664045f, -0.000000f), Vec3( 1.570167f, -0.593793f,  0.000000f), Vec3(-0.456986f, -0.394036f, -0.300000f), 0x00ff00 },
    {Vec3(-0.760758f,  0.270010f,  0.300000f), Vec3( 0.456986f,  0.394035f, -0.300000f), Vec3(-0.514815f,  0.212329f, -0.300000f), 0x0000ff },
    {Vec3(-0.303772f,  0.664045f, -0.000000f), Vec3(-0.456986f, -0.394036f, -0.300000f), Vec3(-0.971801f, -0.181706f,  0.000000f), 0x880000 },
    {Vec3(-0.303772f,  0.664045f, -0.000000f), Vec3(-0.456986f, -0.394035f,  0.300000f), Vec3( 1.570167f, -0.593793f,  0.000000f), 0x008800 },
    {Vec3( 3.168733f, -0.300000f, -0.000000f), Vec3(-5.092798f,  1.098304f,  0.000000f), Vec3(-4.168733f,  0.000000f,  1.000000f), 0x000088 },
    {Vec3(-1.000000f, -0.300000f, -1.000000f), Vec3( 0.000000f,  0.000000f,  2.000000f), Vec3(-0.924065f,  1.098304f,  1.000000f), 0xff8888 },
    {Vec3(-1.000000f, -0.300000f,  1.000000f), Vec3( 0.000000f,  0.000000f, -2.000000f), Vec3( 4.168733f,  0.000000f, -1.000000f), 0x88ff88 },
    {Vec3(-0.743818f, -0.300000f, -0.772652f), Vec3(-0.246623f,  0.000000f, -0.378231f), Vec3(-0.493364f,  1.488035f, -0.719115f), 0x8888ff },
    {Vec3(-0.743818f, -0.300000f,  0.772652f), Vec3(-0.493364f,  1.488036f,  0.719115f), Vec3(-0.246623f,  0.000000f,  0.378231f), 0xff00ff },
    {Vec3(-1.346018f, -0.439340f,  1.086163f), Vec3( 0.355577f,  0.139340f,  0.064720f), Vec3( 0.108836f,  1.627376f,  0.405604f), 0x8800ff },
    {Vec3(-1.346019f, -0.439340f, -1.086163f), Vec3( 0.108837f,  1.627375f, -0.405604f), Vec3( 0.355578f,  0.139340f, -0.064720f), 0xff00ff },
    {Vec3(-1.237182f,  1.188035f, -1.491767f), Vec3(-0.108837f, -1.627375f,  0.405604f), Vec3( 0.493364f, -1.488035f,  0.719115f), 0xffffff },
    {Vec3(-0.990441f, -0.300000f, -1.150883f), Vec3( 0.246623f,  0.000000f,  0.378231f), Vec3(-0.355578f, -0.139340f,  0.064720f), 0x88ff00 },
    {Vec3(-0.990441f, -0.300000f,  1.150883f), Vec3(-0.355577f, -0.139340f, -0.064720f), Vec3( 0.246623f,  0.000000f, -0.378231f), 0xff8800 },
    {Vec3(-1.237182f,  1.188036f,  1.491767f), Vec3( 0.493364f, -1.488036f, -0.719115f), Vec3(-0.108836f, -1.627376f, -0.405604f), 0x00ff88 },
    {Vec3(-0.582366f, -0.288396f, -0.530632f), Vec3(-0.166579f,  0.139211f,  0.000000f), Vec3(-0.000001f, -0.527187f, -2.703453f), 0xff0088 },
    {Vec3(-0.748945f, -0.149185f, -0.530632f), Vec3( 1.271236f, -0.085085f,  0.000000f), Vec3( 0.166578f, -0.666398f, -2.703453f), 0x0088ff },
    {Vec3(-0.582366f, -0.288396f, -0.530632f), Vec3(-0.000001f, -0.527187f, -2.703453f), Vec3( 1.104657f,  0.054126f,  0.000000f), 0x8800ff },
    {Vec3( 0.522291f, -0.234269f,  0.530632f), Vec3(-1.271236f,  0.085084f,  0.000000f), Vec3(-1.104657f, -0.581312f,  2.703453f), 0x8f8f8f },
    {Vec3(-0.582366f, -0.288396f,  0.530632f), Vec3( 1.104657f,  0.054127f,  0.000000f), Vec3( 0.000000f, -0.527185f,  2.703453f), 0x008f8f },
    {Vec3(-0.748945f, -0.149185f,  0.530632f), Vec3( 0.166579f, -0.139211f,  0.000000f), Vec3( 0.166579f, -0.666396f,  2.703453f), 0x8f008f },
    {Vec3( 3.168733f, -0.300000f, -0.000000f), Vec3(-4.168733f,  0.000000f, -1.000000f), Vec3(-5.092798f,  1.098304f,  0.000000f), 0x8f8f00 },
    {Vec3(-1.676231f, -0.328994f,  0.000000f), Vec3( 0.360777f,  0.514515f,  0.279947f), Vec3(-0.064408f,  0.592643f,  0.447957f), 0x00008f },
    {Vec3(-1.676231f, -0.328994f,  0.000000f), Vec3( 0.596570f,  0.156257f,  0.000000f), Vec3( 0.360777f,  0.514515f,  0.279947f), 0x008f00 },
    {Vec3(-1.315454f,  0.185521f, -0.279947f), Vec3(-0.360777f, -0.514515f,  0.279947f), Vec3(-0.425185f,  0.078128f, -0.168009f), 0x8f0000 },
    {Vec3(-1.315454f,  0.185521f, -0.279947f), Vec3( 0.235793f, -0.358258f,  0.279947f), Vec3(-0.360777f, -0.514515f,  0.279947f), 0x8f8888 },
    {Vec3(-1.315454f,  0.185521f,  0.279947f), Vec3(-0.425185f,  0.078128f, -0.727903f), Vec3(-0.425185f,  0.078128f,  0.168010f), 0x8f888f },
    {Vec3(-1.315454f,  0.185521f,  0.279947f), Vec3( 0.000000f,  0.000000f, -0.559894f), Vec3(-0.425185f,  0.078128f, -0.727903f), 0x888f8f }
};

ScreenRenderer::ScreenRenderer(IVec2 resIn) : res(resIn)
{
    pixelCount = res.x * res.y;
    imageData = new unsigned int[pixelCount];
    depthB = new float[pixelCount];
    for (int i = 0; i < sizeof(tris) / sizeof(tris[0]); i++)
    {
        tris[i].vertices[1] = tris[i].vertices[1] + tris[i].vertices[0];
        tris[i].vertices[2] = tris[i].vertices[2] + tris[i].vertices[0];
    }
}

void ScreenRenderer::DrawScreen()
{
    // clear color buffer and depth buffer
    for (uint32_t i = 0; i < pixelCount; i++)
    {
        imageData[i] = 0;
        depthB[i] = 1e20f;
    }

	int64_t tm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	float gTime = tm / 1000.0f;
    Mat4 mvp = Mat4::CreatePerspectiveProjectionMatrix(0.1f, 100.0f, 60.0f, res.x * 1.0f / res.y) * Mat4::CreateTransformMatrix(Vec3(0, 0, 8), Vec3(gTime * 40.0f, gTime * 20.0f, gTime * 30.0f));
    // screen render
    for (int t = 0; t < sizeof(tris) / sizeof(tris[0]); ++t)
    {
        Vec3 points[3];
        for (int k = 0; k < 3; k++)
        {
            points[k] = ((mvp * tris[t].vertices[k]).Homogenize().GetVector() + 1.0f) * Vec3(res.x / 2.0f, res.y / 2.0f, 0.5f);
            points[k].z = 1.0f / points[k].z;
        }

        s32 minY = (s32)Util::MinF(Util::MinF(points[0].y, points[1].y), points[2].y);
        s32 maxY = (s32)Util::MaxF(Util::MaxF(points[0].y, points[1].y), points[2].y);
        s32 minX = (s32)Util::MinF(Util::MinF(points[0].x, points[1].x), points[2].x);
        s32 maxX = (s32)Util::MaxF(Util::MaxF(points[0].x, points[1].x), points[2].x);

        float area = Util::EdgeFunction(Vec2(points[0].x, points[0].y), Vec2(points[1].x, points[1].y), Vec2(points[2].x, points[2].y));

        Vec3 A;
        Vec3 B;
        Vec3 row;
        Vec2 pos = Vec2(minX + 0.5f, minY + 0.5f);
        for (int i = 0; i < 3; i++)
        {
            A[i] = (points[(i + 1) % 3].y - points[(i + 2) % 3].y);
            B[i] = (points[(i + 2) % 3].x - points[(i + 1) % 3].x);
            row[i] = Util::EdgeFunction(pos, Vec2(points[(i + 1) % 3].x, points[(i + 1) % 3].y), Vec2(points[(i + 2) % 3].x, points[(i + 2) % 3].y));
        }
        for (s32 y = minY; y <= maxY; y++)
        {
            //assert(y < res.y && y >= 0);
            if (y >= res.y) continue;
            bool inside = false;
            for (s32 x = minX; x <= maxX; x++)
            {
                //assert(x < res.x && x >= 0);
                if (x >= res.x) continue;
                s64 pIndex = (s64)y * res.x + x;
                if (pIndex < 0 || pIndex >= pixelCount) continue;
                Vec3 w = row - B * (f32)(y - minY) - A * (f32)(x - minX);
                //assert(pIndex >= 0 && pIndex < pixelCount);
                if (w[0] < 0 || w[1] < 0 || w[2] < 0)
                {
                    if (inside) break;
                    continue;
                }
                inside = true;
                w = w / area;
                float depth = 0;
                for (int k = 0; k < 3; k++)
                {
                    depth += w[k] * points[k].z;
                }
                if (depth == 0) depth = 0.0001f;
                depth = -1 / depth;

                if (depth >= depthB[pIndex]) continue;

                depthB[pIndex] = depth;
                imageData[pIndex] = tris[t].color;
            }
        }
    }
}

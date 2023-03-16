#include "ScreenRenderer.hpp"

#include <chrono>

using namespace Maths;

struct Triangle
{
    Vec3 vertices[3];
    Vec3 normal;
    Vec3 color;
};

const Triangle tris[] = {
    {Vec3(-1.740639f, 0.263649f,-0.447956f),Vec3(-1.676231f,-0.328994f,-0.000000f),Vec3(-1.740639f, 0.263649f, 0.447957f),Vec3(-0.993900f,-0.110400f,-0.000000f),Vec3( 1.000000f, 0.032634f, 0.000000f) },
    {Vec3(-0.303772f, 0.664045f, 0.000000f),Vec3( 1.266395f, 0.070252f, 0.000000f),Vec3(-0.760758f, 0.270009f,-0.300000f),Vec3( 0.173000f, 0.456100f,-0.872900f),Vec3( 0.047274f, 0.296477f, 0.800000f) },
    {Vec3(-0.760758f, 0.270010f, 0.300000f),Vec3(-0.303772f, 0.664045f, 0.000000f),Vec3(-1.275573f, 0.482339f, 0.000000f),Vec3(-0.126100f, 0.693500f, 0.709300f),Vec3( 0.047274f, 0.296477f, 0.800000f) },
    {Vec3(-0.303772f, 0.664045f, 0.000000f),Vec3(-0.760758f, 0.270009f,-0.300000f),Vec3(-1.275573f, 0.482339f, 0.000000f),Vec3(-0.126100f, 0.693500f,-0.709300f),Vec3( 0.047274f, 0.296477f, 0.800000f) },
    {Vec3(-0.303772f, 0.664045f, 0.000000f),Vec3(-0.760758f, 0.270010f, 0.300000f),Vec3( 1.266395f, 0.070252f, 0.000000f),Vec3( 0.173000f, 0.456100f, 0.872900f),Vec3( 0.047274f, 0.296477f, 0.800000f) },
    {Vec3( 3.168733f,-0.300000f,-0.000000f),Vec3(-1.924065f, 0.798304f, 0.000000f),Vec3(-1.000000f,-0.300000f, 1.000000f),Vec3( 0.157600f, 0.732800f, 0.661900f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.000000f,-0.300000f,-1.000000f),Vec3(-1.000000f,-0.300000f, 1.000000f),Vec3(-1.924065f, 0.798304f, 0.000000f),Vec3(-0.763700f,-0.645600f,-0.000000f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.000000f,-0.300000f, 1.000000f),Vec3(-1.000000f,-0.300000f,-1.000000f),Vec3( 3.168733f,-0.300000f,-0.000000f),Vec3(-0.000000f,-1.000000f,-0.000000f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.743818f,-0.300000f,-0.772652f),Vec3(-0.990441f,-0.300000f,-1.150883f),Vec3(-1.237182f, 1.188035f,-1.491767f),Vec3( 0.838000f, 0.015600f,-0.545500f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.743818f,-0.300000f, 0.772652f),Vec3(-1.237182f, 1.188036f, 1.491767f),Vec3(-0.990441f,-0.300000f, 1.150883f),Vec3( 0.838000f, 0.015700f, 0.545500f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.346018f,-0.439340f, 1.086163f),Vec3(-0.990441f,-0.300000f, 1.150883f),Vec3(-1.237182f, 1.188036f, 1.491767f),Vec3(-0.086500f,-0.236100f, 0.967900f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.346019f,-0.439340f,-1.086163f),Vec3(-1.237182f, 1.188035f,-1.491767f),Vec3(-0.990441f,-0.300000f,-1.150883f),Vec3(-0.086500f,-0.236100f,-0.967900f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.237182f, 1.188035f,-1.491767f),Vec3(-1.346019f,-0.439340f,-1.086163f),Vec3(-0.743818f,-0.300000f,-0.772652f),Vec3(-0.489500f, 0.244800f, 0.836900f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.990441f,-0.300000f,-1.150883f),Vec3(-0.743818f,-0.300000f,-0.772652f),Vec3(-1.346019f,-0.439340f,-1.086163f),Vec3( 0.323100f,-0.922100f,-0.212800f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.990441f,-0.300000f, 1.150883f),Vec3(-1.346018f,-0.439340f, 1.086163f),Vec3(-0.743818f,-0.300000f, 0.772652f),Vec3( 0.323100f,-0.922100f, 0.212800f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.237182f, 1.188036f, 1.491767f),Vec3(-0.743818f,-0.300000f, 0.772652f),Vec3(-1.346018f,-0.439340f, 1.086163f),Vec3(-0.489500f, 0.244800f,-0.836900f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.582366f,-0.288396f,-0.530632f),Vec3(-0.748945f,-0.149185f,-0.530632f),Vec3(-0.582367f,-0.815582f,-3.234085f),Vec3(-0.637600f,-0.755700f, 0.149600f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.748945f,-0.149185f,-0.530632f),Vec3( 0.522291f,-0.234270f,-0.530632f),Vec3(-0.582367f,-0.815582f,-3.234085f),Vec3( 0.063200f, 0.969600f,-0.236500f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.582366f,-0.288396f,-0.530632f),Vec3(-0.582367f,-0.815582f,-3.234085f),Vec3( 0.522291f,-0.234270f,-0.530632f),Vec3( 0.047100f,-0.981000f, 0.188300f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3( 0.522291f,-0.234269f, 0.530632f),Vec3(-0.748945f,-0.149185f, 0.530632f),Vec3(-0.582366f,-0.815582f, 3.234085f),Vec3( 0.063100f, 0.969600f, 0.236500f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.582366f,-0.288396f, 0.530632f),Vec3( 0.522291f,-0.234269f, 0.530632f),Vec3(-0.582366f,-0.815582f, 3.234085f),Vec3( 0.047100f,-0.981000f,-0.188300f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-0.748945f,-0.149185f, 0.530632f),Vec3(-0.582366f,-0.288396f, 0.530632f),Vec3(-0.582366f,-0.815582f, 3.234085f),Vec3(-0.637600f,-0.755700f,-0.149600f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3( 3.168733f,-0.300000f,-0.000000f),Vec3(-1.000000f,-0.300000f,-1.000000f),Vec3(-1.924065f, 0.798304f, 0.000000f),Vec3( 0.157600f, 0.732800f,-0.661900f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.676231f,-0.328994f,-0.000000f),Vec3(-1.315454f, 0.185521f, 0.279947f),Vec3(-1.740639f, 0.263649f, 0.447957f),Vec3( 0.205500f,-0.577000f, 0.790500f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.676231f,-0.328994f,-0.000000f),Vec3(-1.079661f,-0.172737f,-0.000000f),Vec3(-1.315454f, 0.185521f, 0.279947f),Vec3( 0.141200f,-0.549100f, 0.823700f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.315454f, 0.185521f,-0.279947f),Vec3(-1.676231f,-0.328994f,-0.000000f),Vec3(-1.740639f, 0.263649f,-0.447956f),Vec3( 0.205500f,-0.577100f,-0.790400f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.315454f, 0.185521f,-0.279947f),Vec3(-1.079661f,-0.172737f,-0.000000f),Vec3(-1.676231f,-0.328994f,-0.000000f),Vec3( 0.141200f,-0.549100f,-0.823700f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.315454f, 0.185521f, 0.279947f),Vec3(-1.740639f, 0.263649f,-0.447956f),Vec3(-1.740639f, 0.263649f, 0.447957f),Vec3( 0.180700f, 0.983500f,-0.000000f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
    {Vec3(-1.315454f, 0.185521f, 0.279947f),Vec3(-1.315454f, 0.185521f,-0.279947f),Vec3(-1.740639f, 0.263649f,-0.447956f),Vec3( 0.180700f, 0.983500f,-0.000000f),Vec3( 0.494990f, 0.618285f, 0.800000f) },
};

const Vec3 lightDir = Vec3(1, 1, -0.5f).UnitVector();
const Vec3 lightCol = Vec3(0.5f, 0.5f, 0.5f).UnitVector();
const float smoothness = 1024.0f;

ScreenRenderer::ScreenRenderer(IVec2 resIn) : res(resIn)
{
    pixelCount = res.x * res.y;
    imageData = new unsigned int[pixelCount];
    depthB = new float[pixelCount];
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
    Mat4 m = Mat4::CreateTransformMatrix(Vec3(0, 0, 8), Vec3(gTime * 40.0f, gTime * 20.0f, gTime * 30.0f));
    Mat4 mvp = Mat4::CreatePerspectiveProjectionMatrix(0.1f, 100.0f, 60.0f, res.x * 1.0f / res.y) * m;
    // screen render
    for (int t = 0; t < sizeof(tris) / sizeof(tris[0]); ++t)
    {
        Vec3 points[3];
        Vec3 spos[3];
        Vec3 normal;
        for (int k = 0; k < 3; k++)
        {
            points[k] = ((mvp * tris[t].vertices[k]).Homogenize().GetVector() + 1.0f) * Vec3(res.x / 2.0f, res.y / 2.0f, 0.5f);
            spos[k] = (m * Vec4(tris[t].vertices[k], 1.0f)).GetVector() / points[k].z;
            points[k].z = 1.0f / points[k].z;
        }
        normal = (m * Vec4(tris[t].normal, 0.0f)).GetVector();

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
                Vec3 worldPos;
                for (int k = 0; k < 3; k++)
                {
                    depth += w[k] * points[k].z;
                    worldPos += spos[k] * w[k];
                }
                if (depth == 0) depth = 0.0001f;
                depth = -1 / depth;
                if (depth >= depthB[pIndex]) continue;

                depthB[pIndex] = depth;

                worldPos *= depth;

                float deltaA = (lightDir.DotProduct(normal));
                if (deltaA < 0) deltaA = 0;
                Vec3 halfV = (lightDir - worldPos.UnitVector()).UnitVector();
                float deltaB = powf(Util::MaxF(normal.DotProduct(halfV), 0.0), smoothness);
                Vec3 color = tris[t].color + lightCol * deltaA + deltaB;
                Color4 col = Color4(color);
                unsigned int c = (col.r << 16) + (col.g << 8) + col.b;
                imageData[pIndex] = c;
            }
        }
    }
}

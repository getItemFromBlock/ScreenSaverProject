#pragma once

#include "Maths/Maths.hpp"

class ScreenRenderer
{
public:
	ScreenRenderer() : ScreenRenderer(Maths::IVec2(64, 64)) {};

	ScreenRenderer(Maths::IVec2 resIn);

	~ScreenRenderer() = default;

	void DrawScreen();
	const unsigned int* GetData() const { return imageData; }
	const unsigned int* GetLineData(unsigned int line) const { return imageData + res.x*1llu*line; }

private:
	Maths::IVec2 res;
	u32 pixelCount;
	u32* imageData = nullptr;
	f32* depthB = nullptr;
};

#pragma once
#include "Graphics.h"

class Numbers
{
public:
	void Draw(int x, int y, int num, Color c, Graphics& gfx) const;
private:
	void drawnum0(int x, int y, Color c, Graphics& gfx) const;
	void drawnum1(int x, int y, Color c, Graphics& gfx) const;
	void drawnum2(int x, int y, Color c, Graphics& gfx) const;
	void drawnum3(int x, int y, Color c, Graphics& gfx) const;
	void drawnum4(int x, int y, Color c, Graphics& gfx) const;
	void drawnum5(int x, int y, Color c, Graphics& gfx) const;
	void drawnum6(int x, int y, Color c, Graphics& gfx) const;
	void drawnum7(int x, int y, Color c, Graphics& gfx) const;
	void drawnum8(int x, int y, Color c, Graphics& gfx) const;
	void drawnum9(int x, int y, Color c, Graphics& gfx) const;
	void draw_minus(int x, int y, Color c, Graphics& gfx) const;
};


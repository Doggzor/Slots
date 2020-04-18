#pragma once
#include "Graphics.h"
#include "rng.h"
class Slots
{
public:
	Slots(Graphics& gfx);
	void Draw();
	void Roll0();
	void Roll1();
	void Roll2();
	void DrawUI();
	int slot1 = 0;
	int slot2 = 0;
	int slot3 = 0;
private:
	static constexpr int width = 450;
	static constexpr int chance0 = 29;
	static constexpr int chance1 = 29;
	static constexpr int chance2 = 22;
	static constexpr int chance3 = 14;
	static constexpr int chance4 = 6;
	int rollnum0 = 0;
	int rollnum1 = 0;
	int rollnum2 = 0;
	void Draw0(int x, int y, int dim);
	void Draw1(int x, int y, int dim);
	void Draw2(int x, int y, int dim);
	void Draw3(int x, int y, int dim);
	void Draw4(int x, int y, int dim);
	Graphics& gfx;
};


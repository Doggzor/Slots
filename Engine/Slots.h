#pragma once
#include "Graphics.h"
#include "rng.h"
#include "Numbers.h"
class Slots
{
public:
	Slots(Graphics& gfx);
	void Draw();
	void Roll0();
	void Roll1();
	void Roll2();
	void DrawUI();
	void Update();
	void UpdateCredit();
	int credits = 100;
private:
	static constexpr int width = 450;
	static constexpr int chance0 = 35;
	static constexpr int chance1 = 27;
	static constexpr int chance2 = 20;
	static constexpr int chance3 = 13;
	static constexpr int chance4 = 5;
	int rollnum0 = 0;
	int rollnum1 = 0;
	int rollnum2 = 0;
	int slot1 = 0;
	int slot2 = 0;
	int slot3 = 0;
	static constexpr int win00x = 2;
	static constexpr int win000 = 5;
	static constexpr int win11x = 4;
	static constexpr int win111 = 12;
	static constexpr int win22x = 10;
	static constexpr int win222 = 30;
	static constexpr int win33x = 25;
	static constexpr int win333 = 75;
	static constexpr int win44x = 60;
	static constexpr int win444 = 200;
	void Draw0(int x, int y, int dim);
	void Draw1(int x, int y, int dim);
	void Draw2(int x, int y, int dim);
	void Draw3(int x, int y, int dim);
	void Draw4(int x, int y, int dim);
	Graphics& gfx;
	Numbers creditcount;
};


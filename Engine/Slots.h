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
	void UpdateFrame();
	int credits = 1000;
	int creditsDisplayed = 1000;
	int rollCost = 1;
private:
	Color cc = Colors::White;
	int rgb = 255;
	bool plus_rgb = false;
	static constexpr int width = 450;
	static constexpr int chance0 = 37;
	static constexpr int chance1 = 23;
	static constexpr int chance2 = 19;
	static constexpr int chance3 = 14;
	static constexpr int chance4 = 7;
	int rollnum0 = 0;
	int rollnum1 = 0;
	int rollnum2 = 0;
	int slot1 = 0;
	int slot2 = 0;
	int slot3 = 0;
	static constexpr int win0xx = 1;
	static constexpr int win00x = 2;
	static constexpr int win000 = 3;
	static constexpr int win11x = 4;
	static constexpr int win111 = 10;
	static constexpr int win222 = 15;
	static constexpr int win333 = 40;
	static constexpr int win444 = 200;
	void Draw0s(int x, int y);
	void Draw1s(int x, int y);
	void Draw2s(int x, int y);
	void Draw3s(int x, int y);
	void Draw4s(int x, int y);
	void DrawEQsign(int x, int y);
	void DrawCredits(int x, int y);
	void DrawBet(int x, int y);
	void DrawUpDown(int x, int y);
	void DrawSpace(int x, int y, Color c);
	void Draw0(int x, int y);
	void Draw1(int x, int y);
	void Draw2(int x, int y);
	void Draw3(int x, int y);
	void Draw4(int x, int y);
	Graphics& gfx;
	Numbers creditcount;
};


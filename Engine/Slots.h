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
	bool isRolling = false;
private:
	Color cc = Colors::White;
	Color sc0 = Colors::Gray;
	Color sc1 = Colors::Gray;
	Color sc2 = Colors::Gray;
	int rgb = 255;
	bool plus_rgb = false;
	static constexpr int width = 450;
	static constexpr int chance0 = 33; //Cherry chance
	static constexpr int chance1 = 20; //Lemon Chance
	static constexpr int chance2 = 17; //Apple chance
	static constexpr int chance3 = 13; //Watermelon chance
	static constexpr int chance4 = 10; //Pineapple chance
	static constexpr int chance5 = 7; //Diamond chance
	int rollnum0 = 0;
	int rollnum1 = 0;
	int rollnum2 = 0;
	int slot1 = 0;
	int slot2 = 0;
	int slot3 = 0;
	static constexpr int win0xx = 1; //Single cherry win multiplier
	static constexpr int win00x = 2; //Double cherry win multiplier
	static constexpr int win000 = 3; //Triple cherry win multiplier
	static constexpr int win11x = 4; //Double lemon win multiplier
	static constexpr int win111 = 12; //Triple lemon win multiplier
	static constexpr int win222 = 20; //Triple apple win multiplier
	static constexpr int win333 = 50; //Triple watermelon win multiplier
	static constexpr int win444 = 120; //Triple pineapple win multiplier
	static constexpr int win555 = 300; //Triple diamond win multiplier
	void Draw0s(int x, int y);
	void Draw1s(int x, int y);
	void Draw2s(int x, int y);
	void Draw3s(int x, int y);
	void Draw4s(int x, int y);
	void Draw5s(int x, int y);
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
	void Draw5(int x, int y);
	Graphics& gfx;
	Numbers creditcount;
};


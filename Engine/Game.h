/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Numbers.h"
#include "FrameTimer.h"
#include "Slots.h"
#include "Sound.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
    void DrawSoundON(int x, int y);
    void DrawSoundOFF(int x, int y);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
    FrameTimer ft;
    Slots slots;
    Sound click;
    Sound roll;
    Sound payout;
    Sound bigpayout;
    Sound rollstop;
    float vol = 1.0f;
    bool blipPlayed0 = false;
    bool blipPlayed1 = false;
    bool isRollInhibited = false;
    bool isUpInhibited = false;
    bool isDownInhibited = false;
    float timer = 0.0f;
    float betadjTimer0 = 0.0f;
    float betadjTimer1 = 0.5f;
    static constexpr float rolltime = 3.0f;
    static constexpr int soundbX = 750;
    static constexpr int soundbY = 20;
    bool isSoundON = true;
    bool isMouseInhibited = false;
    //Test comment
	/********************************/
};
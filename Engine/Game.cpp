/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
    slots(gfx),
    click(L"click.wav"),
    roll(L"roll.wav", 0.001f, 0.13f),
    payout(L"payout.wav"),
    bigpayout(L"bigpayout.wav"),
    rollstop(L"rollstop.wav")
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
    const float dt = ft.Mark();
    timer += dt;
    if (wnd.kbd.KeyIsPressed(VK_SPACE) && !isRollInhibited && !slots.isRolling && slots.credits >= slots.rollCost)
    {
        slots.isRolling = true;
        slots.credits -= slots.rollCost;
        timer = 0.0f;
        roll.Play(1.5f, 0.2f * vol);
    }
    isRollInhibited = wnd.kbd.KeyIsPressed(VK_SPACE);

    slots.UpdateFrame();

    if (timer < rolltime * 0.5f && slots.isRolling)
        slots.Roll0();
    if (timer >= rolltime * 0.5f && slots.isRolling && !blipPlayed0)
    {
        rollstop.Play(1.2f, 0.6f * vol);
        blipPlayed0 = true;
    }
    if (timer < rolltime * 0.75f && slots.isRolling)
        slots.Roll1();
    if (timer >= rolltime * 0.75f && slots.isRolling && !blipPlayed1)
    {
        rollstop.Play(1.2f, 0.6f * vol);
        blipPlayed1 = true;
    }
    if (timer < rolltime && slots.isRolling)
        slots.Roll2();
    if (timer >= rolltime && slots.isRolling)
    {
        rollstop.Play(1.2f, 0.6f * vol);
        timer = 0.0f;
        slots.Update();
        slots.UpdateCredit();
        if (slots.creditsDisplayed < slots.credits)
        {
            if (slots.credits - slots.creditsDisplayed >= 50 * slots.rollCost)
                bigpayout.Play(1.0f, 1.0f * vol);
            else
                payout.Play(1.0f, 1.0f * vol);
        }
        slots.isRolling = false;
        blipPlayed0 = false;
        blipPlayed1 = false;
        roll.StopAll();
    }

    //Commands for adjusting BET amount
    if (!slots.isRolling)
    {
        if (wnd.kbd.KeyIsPressed(VK_UP) && !isDownInhibited && slots.rollCost < 100)
        {
            if (!isUpInhibited)
            {
                ++slots.rollCost;
                click.Play(1.5f, 0.2f * vol);
            }
            else if (wnd.kbd.KeyIsPressed(VK_UP) && isUpInhibited && slots.rollCost < 100)
            {
                betadjTimer0 += dt;
                if (betadjTimer0 > betadjTimer1)
                {
                    betadjTimer0 = 0.0f;
                    betadjTimer1 = std::max(betadjTimer1 -= betadjTimer1 * 0.15f, 0.04f);
                    ++slots.rollCost;
                    click.Play(1.5f, 0.2f * vol);
                }
            }
        }
        isUpInhibited = wnd.kbd.KeyIsPressed(VK_UP);
        if (wnd.kbd.KeyIsPressed(VK_DOWN) && !isUpInhibited && slots.rollCost > 1)
        {
            if (!isDownInhibited)
            {
                --slots.rollCost;
                click.Play(1.5f, 0.2f * vol);
            }
            else if (wnd.kbd.KeyIsPressed(VK_DOWN) && isDownInhibited && slots.rollCost > 1)
            {
                betadjTimer0 += dt;
                if (betadjTimer0 > betadjTimer1)
                {
                    betadjTimer0 = 0.0f;
                    betadjTimer1 = std::max(betadjTimer1 -= betadjTimer1 * 0.15f, 0.04f);
                    --slots.rollCost;
                    click.Play(1.5f, 0.2f * vol);
                }
            }
        }
    }
    isDownInhibited = wnd.kbd.KeyIsPressed(VK_DOWN);
    if ((!isUpInhibited && !isDownInhibited) || (wnd.kbd.KeyIsPressed(VK_DOWN) && isUpInhibited) || (wnd.kbd.KeyIsPressed(VK_UP) && isDownInhibited))
    {
        betadjTimer0 = 0.0f;
        betadjTimer1 = 0.5f;
    }

    //Turning ingame sounds ON or OFF
    if (wnd.mouse.LeftIsPressed() && !isMouseInhibited)
    {
        const int lmX = wnd.mouse.GetPosX();
        const int lmY = wnd.mouse.GetPosY();
        if (lmX > soundbX && lmX < soundbX + 35 && lmY > soundbY && lmY < soundbY + 30)
        {
            if (isSoundON)
                isSoundON = false;
            else if (!isSoundON)
                isSoundON = true;
        }
    }
    isMouseInhibited = wnd.mouse.LeftIsPressed();
    if (!isSoundON)
    {
        click.StopAll();
        roll.StopAll();
        rollstop.StopAll();
        payout.StopAll();
        bigpayout.StopAll();
        vol = 0.0f;
    }
    else
        vol = 1.0f;
}

void Game::ComposeFrame()
{
    slots.Draw();
    slots.DrawUI();
    if (isSoundON)
        DrawSoundON(soundbX, soundbY);
    else
        DrawSoundOFF(soundbX, soundbY);
}

void Game::DrawSoundON(int x, int y)
{
    gfx.PutPixel(17 + x, 2 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 2 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 2 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 2 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 2 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 3 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 3 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 3 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 3 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 3 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 3 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 4 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 5 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 6 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 7 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(5 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(6 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(7 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(8 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(9 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(10 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(11 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(23 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 8 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(5 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(6 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(7 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(8 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(9 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(10 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(11 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(23 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 9 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(5 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(6 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(7 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(8 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(9 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(10 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(11 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 10 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 11 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 12 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 13 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 14 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 15 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 16 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(32 + x, 17 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 18 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(5 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(6 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(7 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(8 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(9 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(10 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(11 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(23 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(31 + x, 19 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(5 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(6 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(7 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(8 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(9 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(10 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(11 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(23 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(25 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 20 + y, 255, 255, 255);
    gfx.PutPixel(2 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(3 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(4 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(5 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(6 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(7 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(8 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(9 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(10 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(11 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(24 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(30 + x, 21 + y, 255, 255, 255);
    gfx.PutPixel(12 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(29 + x, 22 + y, 255, 255, 255);
    gfx.PutPixel(13 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(26 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 23 + y, 255, 255, 255);
    gfx.PutPixel(14 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(27 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(28 + x, 24 + y, 255, 255, 255);
    gfx.PutPixel(15 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 25 + y, 255, 255, 255);
    gfx.PutPixel(16 + x, 26 + y, 255, 255, 255);
    gfx.PutPixel(17 + x, 26 + y, 255, 255, 255);
    gfx.PutPixel(18 + x, 26 + y, 255, 255, 255);
    gfx.PutPixel(19 + x, 26 + y, 255, 255, 255);
    gfx.PutPixel(20 + x, 26 + y, 255, 255, 255);
    gfx.PutPixel(21 + x, 26 + y, 255, 255, 255);

}

void Game::DrawSoundOFF(int x, int y)
{
    gfx.PutPixel(17 + x, 2 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 2 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 2 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 2 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 2 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 3 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 3 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 3 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 3 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 3 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 3 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 4 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 5 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 6 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 7 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(5 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(6 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(7 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(8 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(9 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(10 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(11 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 8 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(5 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(6 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(7 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(8 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(9 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(10 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(11 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(23 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(33 + x, 9 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(5 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(6 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(7 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(8 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(9 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(10 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(11 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(23 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(33 + x, 10 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(23 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(33 + x, 11 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(28 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 12 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(28 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 13 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(28 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 14 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(28 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 15 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(28 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 16 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(23 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(27 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(29 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(33 + x, 17 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(23 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(26 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(30 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(33 + x, 18 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(5 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(6 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(7 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(8 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(9 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(10 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(11 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(23 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(24 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(25 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(31 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(32 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(33 + x, 19 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(5 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(6 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(7 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(8 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(9 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(10 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(11 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 20 + y, 195, 195, 195);
    gfx.PutPixel(2 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(3 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(4 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(5 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(6 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(7 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(8 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(9 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(10 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(11 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 21 + y, 195, 195, 195);
    gfx.PutPixel(12 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 22 + y, 195, 195, 195);
    gfx.PutPixel(13 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 23 + y, 195, 195, 195);
    gfx.PutPixel(14 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 24 + y, 195, 195, 195);
    gfx.PutPixel(15 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 25 + y, 195, 195, 195);
    gfx.PutPixel(16 + x, 26 + y, 195, 195, 195);
    gfx.PutPixel(17 + x, 26 + y, 195, 195, 195);
    gfx.PutPixel(18 + x, 26 + y, 195, 195, 195);
    gfx.PutPixel(19 + x, 26 + y, 195, 195, 195);
    gfx.PutPixel(20 + x, 26 + y, 195, 195, 195);
    gfx.PutPixel(21 + x, 26 + y, 195, 195, 195);

}

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

    if (!slots.isRolling)
    {
        if (wnd.kbd.KeyIsPressed(VK_UP) && slots.rollCost < 100)
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
        if (wnd.kbd.KeyIsPressed(VK_DOWN) && slots.rollCost > 1)
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
    if (!isUpInhibited && !isDownInhibited)
    {
        betadjTimer0 = 0.0f;
        betadjTimer1 = 0.5f;
    }
    if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
    {
        click.StopAll();
        roll.StopAll();
        rollstop.StopAll();
        payout.StopAll();
        bigpayout.StopAll();
        vol = 0.0f;
    }
}

void Game::ComposeFrame()
{
    slots.Draw();
    slots.DrawUI();
}

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
    slots(gfx)
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
    if (wnd.kbd.KeyIsPressed(VK_RETURN) && !isRollInhibited && !isRolling)
    {
        isRolling = true;
        timer = 0.0f;
    }
    isRollInhibited = wnd.kbd.KeyIsPressed(VK_RETURN);

    if (timer < rolltime * 0.5f && isRolling)
        slots.Roll0();
    if (timer < rolltime * 0.75f && isRolling)
        slots.Roll1();
    if (timer < rolltime && isRolling)
        slots.Roll2();
    if (timer >= rolltime)
    {
        timer = 0.0f;
        isRolling = false;
    }
}

void Game::ComposeFrame()
{
    slots.Draw();
}

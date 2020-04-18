#include "Slots.h"

Slots::Slots(Graphics& gfx)
	:
	gfx ( gfx )
{
}

void Slots::Draw()
{
	const int thck = 3;
	const int dim = width / 3;
	const int x = (gfx.ScreenWidth - width) / 2;
	const int y = (gfx.ScreenHeight - dim) / 2;
	gfx.DrawRectEmpty(x, y, dim, dim, -thck, Colors::Gray);
	gfx.DrawRectEmpty(x + dim + thck, y, dim, dim, -3, Colors::Gray);
	gfx.DrawRectEmpty(x + (dim + thck) * 2, y, dim, dim, -3, Colors::Gray);

	const int chance00 = chance0;
	const int chance01 = chance00 + chance1;
	const int chance02 = chance01 + chance2;
	const int chance03 = chance02 + chance3;
	
	if (rollnum0 > 0 && rollnum0 <= chance00)
		Draw0(x, y, dim);
	else if (rollnum0 > chance00 && rollnum0 <= chance01)
		Draw1(x, y, dim);
	else if (rollnum0 > chance01 && rollnum0 <= chance02)
		Draw2(x, y, dim);
	else if (rollnum0 > chance02 && rollnum0 <= chance03)
		Draw3(x, y, dim);
	else if (rollnum0 > chance03 && rollnum0 <= 100)
		Draw4(x, y, dim);

	if (rollnum1 > 0 && rollnum1 <= chance00)
		Draw0(x + dim + thck, y, dim);
	else if (rollnum1 > chance00 && rollnum1 <= chance01)
		Draw1(x + dim + thck, y, dim);
	else if (rollnum1 > chance01 && rollnum1 <= chance02)
		Draw2(x + dim + thck, y, dim);
	else if (rollnum1 > chance02 && rollnum1 <= chance03)
		Draw3(x + dim + thck, y, dim);
	else if (rollnum1 > chance03 && rollnum1 <= 100)
		Draw4(x + dim + thck, y, dim);

	if (rollnum2 > 0 && rollnum2 <= chance00)
		Draw0(x + (dim + thck) * 2, y, dim);
	else if (rollnum2 > chance00 && rollnum2 <= chance01)
		Draw1(x + (dim + thck) * 2, y, dim);
	else if (rollnum2 > chance01 && rollnum2 <= chance02)
		Draw2(x + (dim + thck) * 2, y, dim);
	else if (rollnum2 > chance02 && rollnum2 <= chance03)
		Draw3(x + (dim + thck) * 2, y, dim);
	else if (rollnum2 > chance03 && rollnum2 <= 100)
		Draw4(x + (dim + thck) * 2, y, dim);
}

void Slots::Roll0()
{
	rollnum0 = rng::rdm_int(1, 100);
}

void Slots::Roll1()
{
	rollnum1 = rng::rdm_int(1, 100);
}

void Slots::Roll2()
{
	rollnum2 = rng::rdm_int(1, 100);
}

void Slots::Draw0(int x, int y, int dim)
{
	gfx.DrawRectDim(x, y, dim, dim, Colors::Red);
}

void Slots::Draw1(int x, int y, int dim)
{
	gfx.DrawRectDim(x, y, dim, dim, Colors::Orange);
}

void Slots::Draw2(int x, int y, int dim)
{
	gfx.DrawRectDim(x, y, dim, dim, Colors::Green);
}

void Slots::Draw3(int x, int y, int dim)
{
	gfx.DrawRectDim(x, y, dim, dim, Colors::Yellow);
}

void Slots::Draw4(int x, int y, int dim)
{
	gfx.DrawRectDim(x, y, dim, dim, Colors::Cyan);
}

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
#include <assert.h>
#include <algorithm>
#include "Star.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ct(gfx),
	cam(ct),
	camCtrl(wnd.mouse, cam)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> xDist(-worldWidth / 2.0f, worldWidth / 2.0f);
	std::uniform_real_distribution<float> yDist(-worldHeight / 2.0f, worldHeight / 2.0f);

	std::normal_distribution<float> radDist(meanStarRadius, devStarRadius);
	std::normal_distribution<float> ratDist(meanInnerRatio, devInnerRatio);
	std::normal_distribution<float> flareDist(meanFlares, devFlares);
	std::normal_distribution<float> colorFreqDist(meanColorFreq, devColorFreq);
	std::uniform_real_distribution<float> phaseDist(0.0f, 2.0f * 3.14159f);
	std::normal_distribution<float> radiusAmplitudeDist(meanRadiusAmplitude, devRadiusAmplitude);
	std::normal_distribution<float> radiusFreqDist(meanRadiusFreq, devRadiusFreq);

	const Color colors[] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Cyan, Colors::Yellow, Colors::Magenta };
	std::uniform_int_distribution<size_t> colorSampler(0, std::end(colors) - std::begin(colors));

	while (stars.size() < nStars)
	{
		const auto rad = std::clamp(radDist(rng), minStarRadius, maxStarRadius);
		const Vec2 pos = { xDist(rng), yDist(rng) };
		const float radiusAmplitude = std::clamp(radiusAmplitudeDist(rng), minRadiusAmplitude, maxRadiusAmplitude);
		const float maxRad = rad * (1.0f + radiusAmplitude);
		if (std::any_of(stars.begin(), stars.end(), [&](const FancyEntity& star)
			{ return (star.GetPos() - pos).GetLength() < maxRad + star.GetMaxRadius(); }
		))
		{
			continue;
		};

		const auto rat = std::clamp(ratDist(rng), minInnerRatio, maxInnerRatio);
		const int flares = std::clamp((int)flareDist(rng), minFlares, maxFlares);
		const Color c = colors[colorSampler(rng)];
		const float colorFreq = std::clamp(colorFreqDist(rng), minColorFreq, maxColorFreq);
		const float colorPhase = phaseDist(rng);
		const float radiusFreq = std::clamp(radiusFreqDist(rng), minRadiusFreq, maxRadiusFreq);
		const float radiusPhase = phaseDist(rng);

		stars.emplace_back(pos, rad, rat, flares, c, colorFreq, colorPhase, radiusAmplitude, radiusFreq, radiusPhase);
	}

}

void Game::Go()
{
	gfx.BeginFrame();
	ProcessInput();
	const float elapsedTime = ft.Mark();
	float time = elapsedTime;
	while (time > 0.0f) {
		const float dt = std::min(precision, time);
		UpdateModel(dt);
		time -= dt;
	}
	numberOfFrames++;
	timeSecond += elapsedTime;
	if (timeSecond >= 1.0f) {
		timeSecond -= 1.0f;
		FPS = numberOfFrames;
		numberOfFrames = 0;
	}

	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
////////////// KEYBOARD ///////////////
	// Keys
	while (!wnd.kbd.KeyIsEmpty())
	{
		const Keyboard::Event keyPressed = wnd.kbd.ReadKey();
		if (keyPressed.IsValid() && keyPressed.IsPress())
		{
			
		}
	}
	// Characters
	while (!wnd.kbd.CharIsEmpty())
	{
		const char character = wnd.kbd.ReadChar();
	}
///////////////////////////////////////

//////////////// MOUSE ////////////////
	//while (!wnd.mouse.IsEmpty())
	//{
	//	const auto e = wnd.mouse.Read();
	//	// buttons
	//	// editor
	//}
	camCtrl.Update();
///////////////////////////////////////
}

void Game::UpdateModel(float dt)
{
	const RectI boxRect = fontBase.GetRectForText(message, Vei2(posBox));
	posBox += dir * dt * 200.0f;
	if (boxRect.left < walls.left)
	{
		dir.x = -dir.x;
		posBox.x = float(walls.left);
	}
	else if (boxRect.right > walls.right)
	{
		dir.x = -dir.x;
		posBox.x = float(walls.right - boxRect.GetWidth());
	}
	if (boxRect.top < walls.top)
	{
		dir.y = -dir.y;
		posBox.y = float(walls.top);
	}
	else if (boxRect.bottom > walls.bottom)
	{
		dir.y = -dir.y;
		posBox.y = float(walls.bottom - boxRect.GetHeight());
	}

	for (auto& star : stars)
	{
		star.Update(dt);
	}
}

void Game::ComposeFrame()
{
	const RectI boxRect = fontBase.GetRectForText(message, Vei2(posBox)).GetExpanded(5);
	gfx.DrawRect(boxRect, Colors::Blue);
	fontBase.DrawText(message, Vei2(posBox), Colors::White, gfx);
	// Draw FPS
	const std::string fpsText = "FPS: " + std::to_string(FPS);
	fontXs.DrawText(fpsText, Vei2{ 10, 10 }, Colors::White, gfx);

	const auto vp = cam.GetViewportRect();
	for (const auto& star : stars)
	{
		if (star.GetBoundingRect().IsOverlappingWith(vp))
		{
			cam.Draw(star.GetDrawable());
		}
	}
}
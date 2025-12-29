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

#include <vector>
#include <random>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Font.h"
#include "Button.h"
#include "TextBox.h"
#include "MyMessageBox.h"
#include "CoordinateTransformer.h"
#include "Camera.h"
#include "MouseCameraController.h"

#include "Entity.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();

private:
	void ProcessInput();
	void UpdateModel(float dt);
	void ComposeFrame();
	/********************************/
	/*  User Functions              */

	/********************************/

private:
	MainWindow& wnd;
	Graphics gfx;

	////////// FONTS /////////
	const Font fontXs = Font("Files/Fonts/font8x14.bmp");
	const Font fontSm = Font("Files/Fonts/font12x21.bmp");
	const Font fontBase = Font("Files/Fonts/font16x28.bmp");
	const Font fontLg = Font("Files/Fonts/font18x32.bmp");
	const Font fontXl = Font("Files/Fonts/font20x35.bmp");
	const Font font2Xl = Font("Files/Fonts/font24x42.bmp");
	const Font font3Xl = Font("Files/Fonts/font32x56.bmp");
	///////////////////////////

	std::mt19937 rng = std::mt19937(std::random_device{}());
	const float precision = 0.0025f;
	FrameTimer ft;
	MyMessageBox myMessageBox = MyMessageBox(&fontLg);

	CoordinateTransformer ct;
	Camera cam;
	MouseCameraController camCtrl;

	/////////// FPS ///////////
	int FPS = 0;
	float timeFrame = 0.0f;
	float timeSecond = 0.0f;
	int numberOfFrames = 0;
	///////////////////////////

	/********************************/
	/*  User Variables  */
	const std::string message = "  Chili  \n DirectX \nWojtkowski";
	const RectI walls = Graphics::GetScreenRect();
	Vec2 posBox = Vec2(Graphics::GetScreenCenter());
	Vec2 dir = Vec2{ 1.0, 1.0f };

	std::vector<FancyEntity> stars;

	static constexpr float worldWidth = 10000.0f;
	static constexpr float worldHeight = 6000.0f;
	static constexpr int nStars = 1100;

	static constexpr float meanStarRadius = 160.0f;
	static constexpr float devStarRadius = 90.0f;
	static constexpr float minStarRadius = 40.0f;
	static constexpr float maxStarRadius = 300.0f;

	static constexpr float meanInnerRatio = 0.4f;
	static constexpr float devInnerRatio = 0.25f;
	static constexpr float minInnerRatio = 0.15f;
	static constexpr float maxInnerRatio = 0.8f;

	static constexpr float meanFlares = 6.5f;
	static constexpr float devFlares = 2.0f;
	static constexpr int minFlares = 3;
	static constexpr int maxFlares = 10;

	static constexpr float meanColorFreq = 1.6f;
	static constexpr float devColorFreq = 1.0f;
	static constexpr float minColorFreq = 0.6f;
	static constexpr float maxColorFreq = 4.0f;

	static constexpr float meanRadiusAmplitude = 0.5f;
	static constexpr float devRadiusAmplitude = 0.3f;
	static constexpr float minRadiusAmplitude = 0.1f;
	static constexpr float maxRadiusAmplitude = 0.9f;

	static constexpr float meanRadiusFreq = 1.8f;
	static constexpr float devRadiusFreq = 1.0f;
	static constexpr float minRadiusFreq = 0.6f;
	static constexpr float maxRadiusFreq = 4.0f;
	/********************************/
};

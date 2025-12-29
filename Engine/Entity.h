#pragma once

#include <vector>
#include <cmath>
#include "Vec2.h"
#include "Drawable.h"
#include "Star.h"
#include "Rect.h"

class Entity
{
public:
	Entity( std::vector<Vec2> model,const Vec2& pos = { 0.0f,0.0f },Color c = Colors::Yellow )
		:
		c( c ),
		pos( pos ),
		model( std::move( model ) )
	{}
	const Vec2& GetPos() const
	{
		return pos;
	}
	void SetPos( const Vec2& newPos )
	{
		pos = newPos;
	}
	void TranslateBy( const Vec2& offset )
	{
		pos += offset;
	}
	void SetScale( float s )
	{
		scale = s;
	}
	float GetScale() const
	{
		return scale;
	}
	Drawable GetDrawable() const
	{
		Drawable d( model,c );
		d.Scale( scale );
		d.Translate( pos );
		return d;
	}
	void SetColor(Color c_in)
	{
		c = c_in;
	}

private:
	Color c;
	float scale = 1.0f;
	Vec2 pos = {0.0f,0.0f};
	std::vector<Vec2> model;
};

class FancyEntity : public Entity
{
public:
	FancyEntity(Vec2 pos, float radius, float innerRation, int nFlares, Color c, float colorFreq, float colorPhase,
		float radiusFactorAmplitude, float radiusFactorFreq, float radiusFactorPhase)
		:
		Entity(Star::Make(radius, radius * innerRation, nFlares), pos, c),
		radius(radius),
		colorFreqFactor(colorFreq * 2.0f * 3.14159f),
		colorPhase(colorPhase),
		baseColor(c),
		radiusFactorAmplitude(radiusFactorAmplitude),
		radiusFactorFreq(radiusFactorFreq * 2.0f * 3.14159f),
		radiusFactorPhase(radiusFactorPhase)
	{}
	void Update(float dt)
	{
		time += dt;
		UpdateColor();
		UpdateScale();
	}
	float GetRadius() const
	{
		return radius * GetScale();
	}
	float GetMaxRadius() const
	{
		return radius * (1.0f + radiusFactorAmplitude);
	}
	RectF GetBoundingRect() const
	{
		return RectF::FromCenter(GetPos(), GetRadius(), GetRadius());
	}

private:
	void UpdateColor()
	{
		Color c;
		const int offset = int(127.0f * std::sin(colorFreqFactor * time + colorPhase)) + 128;
		c.SetR(std::min(baseColor.GetR() + offset, 255));
		c.SetG(std::min(baseColor.GetG() + offset, 255));
		c.SetB(std::min(baseColor.GetB() + offset, 255));
		SetColor(c);
	}
	void UpdateScale()
	{
		const float factor = radiusFactorAmplitude * std::sin(radiusFactorFreq * time + radiusFactorPhase);
		SetScale(1.0f + factor);
	}

private:
	float radius;
	Color baseColor;
	float colorFreqFactor;
	float colorPhase;
	float time = 0.0f;
	float radiusFactorAmplitude;
	float radiusFactorFreq;
	float radiusFactorPhase;
};
#include "PreCompile.h"
#include "EngineMath.h"

const FVector2D FVector2D::ZERO  = {  0,  0 };
const FVector2D FVector2D::LEFT  = { -1,  0 };
const FVector2D FVector2D::RIGHT = {  1,  0 };
const FVector2D FVector2D::UP    = {  0, -1 };
const FVector2D FVector2D::DOWN  = {  0,  1 };

FVector2D FVector2D::Lerp(FVector2D& _Start, FVector2D& _End, float _ElapsedTime, float _Duration)
{
	float Alpha = _ElapsedTime / _Duration;
	Alpha = Clamp(Alpha, 0.0f, 1.0f);

	FVector2D Result = ((1.0f - Alpha) * _Start) + (Alpha * _End);
	return Result;

}

FVector2D FVector2D::Lerp(FVector2D& _Start, FVector2D& _End, float _Alpha)
{
	FVector2D Result = ((1.0f - _Alpha) * _Start) + (_Alpha * _End);
	return Result;

}

float FVector2D::Clamp(float _Value, float _Min, float _Max)
{
	if (_Value < _Min)
	{
		return _Min;
	}
	else if (_Value > _Max)
	{
		return _Max;
	}
	else
	{
		return _Value;
	}
}


const FIntPoint FIntPoint::LEFT  = {-1,  0 };
const FIntPoint FIntPoint::RIGHT = { 1,  0 };
const FIntPoint FIntPoint::UP    = { 0, -1 };
const FIntPoint FIntPoint::DOWN  = { 0,  1 };
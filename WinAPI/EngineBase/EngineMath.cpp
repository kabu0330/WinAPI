#include "PreCompile.h"
#include "EngineMath.h"

const FVector2D FVector2D::ZERO  = {  0,  0 };
const FVector2D FVector2D::LEFT  = { -1,  0 };
const FVector2D FVector2D::RIGHT = {  1,  0 };
const FVector2D FVector2D::UP    = {  0, -1 };
const FVector2D FVector2D::DOWN  = {  0,  1 };


FVector2D FVector2D::Abs(FVector2D& _Vector)
{
	if (0 > _Vector.X)
	{
		_Vector.X *= -1;
	}	
	else if (0 > _Vector.Y)
	{
		_Vector.Y *= -1;
	}
	return _Vector;
}

int FVector2D::Abs(int& _Value)
{
	if (0 > _Value)
	{
		_Value *= -1;
	}
	
	return _Value;
}

float FVector2D::Abs(float& _Value)
{
	if (0 > _Value)
	{
		_Value *= -1;
	}

	return _Value;
}

float FVector2D::DotProduct(const FVector2D& _Vec1, const FVector2D& _Vec2)
{
	return _Vec1.X * _Vec2.X + _Vec1.Y * _Vec2.Y;
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

std::function<bool(const FTransform&, const FTransform&)> FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		// 데이터 영역이 초기화 될때 초기화하는 일을 자동으로 수행할수 있다.
		// 데이터 영역이 만들어질때 이 작업은 자동으로 실행된다.
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;
	}
};

// 데이터 영역
CollisionFunctionInit Inst = CollisionFunctionInit();

bool FTransform::Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right)
{
	return FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)](_Left, _Right);
}

bool FTransform::RectToRect(const FTransform& _Left, const FTransform& _Right)
{
	if (_Left.CenterLeft() > _Right.CenterRight())
	{
		return false;
	}

	if (_Left.CenterRight() < _Right.CenterLeft())
	{
		return false;
	}

	if (_Left.CenterTop() > _Right.CenterBottom())
	{
		return false;
	}

	if (_Left.CenterBottom() < _Right.CenterTop())
	{
		return false;
	}
	return true;
}

bool FTransform::CirCleToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FVector2D Len = _Left.Location - _Right.Location;

	// 트랜스폼을 원으로 봤을때 반지름은 x의 절반크기를 반지름으로 보겠습니다.

	// 두원의 반지름의 합이 벡터의 길이보다 크다면 
	if (Len.Length() < _Left.Scale.hX() + _Right.Scale.hX())
	{
		return true;
	}

	return false;
}


const FIntPoint FIntPoint::LEFT  = {-1,  0 };
const FIntPoint FIntPoint::RIGHT = { 1,  0 };
const FIntPoint FIntPoint::UP    = { 0, -1 };
const FIntPoint FIntPoint::DOWN  = { 0,  1 };

const UColor UColor::WHITE = { 255, 255, 255, 0 };
const UColor UColor::BLACK = { 0, 0, 0, 0 };

FIntPoint FVector2D::ConvertToPoint() const
{
	return { iX(), iY() };
}
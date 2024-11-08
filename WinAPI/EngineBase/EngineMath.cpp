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
		// ������ ������ �ʱ�ȭ �ɶ� �ʱ�ȭ�ϴ� ���� �ڵ����� �����Ҽ� �ִ�.
		// ������ ������ ��������� �� �۾��� �ڵ����� ����ȴ�.
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;
	}
};

// ������ ����
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

	// Ʈ�������� ������ ������ �������� x�� ����ũ�⸦ ���������� ���ڽ��ϴ�.

	// �ο��� �������� ���� ������ ���̺��� ũ�ٸ� 
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
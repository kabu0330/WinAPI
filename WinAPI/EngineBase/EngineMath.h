#pragma once
#include <cmath>
#include <numbers>
#include <algorithm>

class UEngineMath
{
public:
	static float Sqrt(float _Value)
	{
		return ::sqrtf(_Value);
	}

	template <typename DataType>
	DataType ClampMax(DataType value, DataType maxValue)
	{
		return (value > maxValue) ? maxValue : value;
	}

	template <typename DataType>
	DataType ClampMin(DataType value, DataType minValue)
	{
		return (value < minValue) ? minValue : value;
	}

	template <typename DataType>
	static DataType Clamp(DataType value, DataType minValue, DataType maxValue)
	{
		if (value < minValue)
			return minValue;
		else if (value > maxValue)
			return maxValue;
		else
			return value;
	}

	template <typename DataType>
	static DataType Min(DataType _A, DataType _B)
	{
		return (_A < _B) ? _A : _B;
	}

	template <typename VectorType>
	static VectorType AngleToRadian(const float& _Angle)
	{
		float Radian = _Angle * (std::numbers::pi / 180.0f);
		float DirVectorX = std::cos(Radian);
		float DirVectorY = std::sin(Radian);

		return VectorType(DirVectorX, DirVectorY);
	}

	template <typename VectorType>
	static VectorType AngleToRadian(const VectorType& _DirUintVector, const float& _Angle)
	{
		float Radian = static_cast<float>(_Angle * (std::numbers::pi / 180.0f));
		float RotatedX = _DirUintVector.X * std::cos(Radian) - _DirUintVector.Y * std::sin(Radian);
		float RotatedY = _DirUintVector.X * std::sin(Radian) + _DirUintVector.Y * std::cos(Radian);

		return VectorType(RotatedX, RotatedY);
	}

};

class FVector2D
{
public:
	float X = 0.0f;
	float Y = 0.0f;

	static const FVector2D ZERO;
	static const FVector2D LEFT;
	static const FVector2D RIGHT;
	static const FVector2D UP;
	static const FVector2D DOWN;

	// Lerp : ���������� : �������� ���� ������ �߰� ��(����)�� ã�� ���
	// _Start : ���� ��ġ, _End : ������, _ElapsedTime : DeltaTime�� ������ų ����, _Duration : �̵��� �ð�
	static FVector2D Lerp(const FVector2D& _Start, const FVector2D& _End, float _ElapsedTime, float _Duration)
	{
		float Alpha = _ElapsedTime / _Duration;
		Alpha = Clamp(Alpha, 0.0f, 1.0f);
		return Lerp(_Start, _End, Alpha);
	}

	// _Start : ���� ��ġ, _End : ������, _Alpha : �� �� ������ ���� ����(0 ~ 1)
	static FVector2D Lerp(const FVector2D& _Start, const FVector2D& _End, float _Alpha)
	{
		FVector2D Result = ((1.0f - _Alpha) * _Start) + (_Alpha * _End);
		return Result;
	}

	// ����
	static FVector2D Abs(FVector2D& _Vector);
	static int       Abs(int&   _Value);
	static float     Abs(float& _Value);

	// vector�� ����
	static float DotProduct(const FVector2D& _Vec1, const FVector2D& _Vec2);

	// _Value ����  _Min ~ _Max ���հ����θ� ����
	static float Clamp(float _Value, float _Min = 0.0f, float _Max = 1.0f);

	FVector2D& GetNormal()
	{
		this->Normalize();
		return *this;
	}

	FVector2D() {}

	FVector2D(float _X, float _Y) : X(_X), Y(_Y) {}

	FVector2D(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)) {}

	FVector2D(long _X, long _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)) {}

	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}

	bool IsZero() const
	{
		return X == 0.0f || Y == 0.0f;
	}

	FVector2D Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}

	bool EqualToInt(FVector2D _Other) const
	{
		return iX() == _Other.iX() && iY() == _Other.iY();
	}

	float Length() const
	{
		return UEngineMath::Sqrt(X * X + Y * Y);
	}


	class FIntPoint ConvertToPoint() const;

	void Normalize()
	{
		float Len = Length();
		if (0.0f < Len && false == isnan(Len))
		{
			X = X / Len;
			Y = Y / Len;
		}
		return;
	}

	FVector2D& Normalize(FVector2D _Value)
	{
		_Value.Normalize();
		return _Value;
	}

	float Dot(const FVector2D& other) const
	{
		return X * other.X + Y * other.Y;
	}

	std::string ToString()
	{
		std::string Stream;

		Stream += "X : [";
		Stream += std::to_string(X);
		Stream += "] Y : [";
		Stream += std::to_string(Y);
		Stream += "]";
		return Stream;
	}

	FVector2D operator+(const FVector2D& _Other) const
	{
		FVector2D Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FVector2D operator-(const FVector2D& _Other) const
	{
		FVector2D Result;
		Result.X = X - _Other.X;
		Result.Y = Y - _Other.Y;
		return Result;
	}

	FVector2D operator*(const FVector2D& _Other) const
	{
		FVector2D Result;
		Result.X = X * _Other.X;
		Result.Y = Y * _Other.Y;
		return Result;
	}

	FVector2D operator*(const float& _Value) const
	{
		FVector2D Result;
		Result.X = X * _Value;
		Result.Y = Y * _Value;
		return Result;
	}

	FVector2D operator/(const int& _Value) const
	{
		FVector2D Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	FVector2D operator/(const float& _Value) const
	{
		FVector2D Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	FVector2D operator/(const FVector2D& _Other) const
	{
		FVector2D Result;
		Result.X = X / _Other.X;
		Result.Y = Y / _Other.Y;
		return Result;
	}

	bool operator==(const FVector2D& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	FVector2D& operator+=(const FVector2D& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}

	FVector2D& operator-=(const FVector2D& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		return *this;
	}

	FVector2D& operator*= (const FVector2D& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		return *this;
	}

	FVector2D& operator*= (const float& _Value)
	{
		X *= _Value;
		Y *= _Value;
		return *this;
	}

	FVector2D& operator/= (const float& _Value)
	{
		X /= _Value;
		Y /= _Value;
		return *this;
	}

	FVector2D operator-() const
	{
		FVector2D Result;
		Result.X = -X;
		Result.Y = -Y;
		return Result;
	}

	friend FVector2D operator*(const float& _Value, const FVector2D& _Other)
	{
		FVector2D Result;
		Result.X = _Other.X * _Value;
		Result.Y = _Other.Y * _Value;
		return Result;
	}

};

enum class ECollisionType
{
	Point,
	Rect,
	CirCle,
	Max
};

class FTransform
{
private:
	friend class CollisionFunctionInit;

	static std::function<bool(const FTransform&, const FTransform&)> AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

public:
	static bool Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right);

	static bool PointToCirCle(const FTransform& _Left, const FTransform& _Right);
	static bool PointToRect(const FTransform& _Left, const FTransform& _Right);

	static bool RectToRect(const FTransform& _Left, const FTransform& _Right);
	static bool RectToCirCle(const FTransform& _Left, const FTransform& _Right);

	static bool CirCleToCirCle(const FTransform& _Left, const FTransform& _Right);
	static bool CirCleToRect(const FTransform& _Left, const FTransform& _Right);



	FVector2D Scale;
	FVector2D Location;

	FVector2D CenterLeftTop() const
	{
		return Location - Scale.Half();
	}

	FVector2D CenterLeftBottom() const
	{	
		return FVector2D(Location.X - Scale.hX(), Location.Y + Scale.hY());
	}

	FVector2D CenterRightTop() const
	{
		return	FVector2D(Location.X + Scale.hX(), Location.Y - Scale.hY());
	}

	FVector2D CenterRightBottom() const
	{
		return Location + Scale.Half();
	}

	float CenterLeft() const
	{
		return Location.X - Scale.hX();
	}

	float CenterRight() const
	{
		return Location.X + Scale.hX();
	}

	float CenterTop() const
	{
		return Location.Y - Scale.hY();
	}

	float CenterBottom() const
	{
		return Location.Y + Scale.hY();
	}
};

class FIntPoint
{
public:
	int X = 0;
	int Y = 0;

	static const FIntPoint LEFT;
	static const FIntPoint RIGHT;
	static const FIntPoint UP;
	static const FIntPoint DOWN;

	FIntPoint()
	{

	}

	FIntPoint(int _X, int _Y) : X(_X), Y(_Y)
	{

	}

	FIntPoint operator+(const FIntPoint& _Other) const
	{
		FIntPoint Result; 
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FIntPoint operator/(const int& _Value) const
	{
		FIntPoint Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	bool operator==(const FIntPoint& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	FIntPoint& operator+=(const FIntPoint& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}
};

class UColor
{
public:
	static const UColor WHITE;
	static const UColor BLACK;
	union
	{
		int Color;
		struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};
	};

	UColor(unsigned long _Value)
		:Color(_Value)
	{

	}

	bool operator==(const UColor& _Other) const
	{
		return R == _Other.R && G == _Other.G && B == _Other.B;
	}


	UColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		: R(_R), G(_G), B(_B), A(_A)
	{

	}
};
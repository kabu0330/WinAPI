#pragma once

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

	FVector2D()
	{

	}

	FVector2D(float _X, float _Y) : X(_X), Y(_Y)
	{

	}


	FVector2D(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y))
	{

	}

	FVector2D(long _X, long _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y))
	{

	}

	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	bool IsZero() const
	{
		return X == 0.0f || Y == 0.0f;
	}

	FVector2D Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}

	FVector2D operator+(FVector2D _Other) const
	{
		FVector2D Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FVector2D operator-(FVector2D _Other) const
	{
		FVector2D Result;
		Result.X = X - _Other.X;
		Result.Y = Y - _Other.Y;
		return Result;
	}

	FVector2D operator*(float _Value) const
	{
		FVector2D Result;
		Result.X = X * _Value;
		Result.Y = Y * _Value;
		return Result;
	}

	FVector2D operator/(int _Value) const
	{
		FVector2D Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	FVector2D operator*(FVector2D _Other) const
	{
		FVector2D Result;
		Result.X = X * _Other.X;
		Result.Y = Y * _Other.Y;
		return Result;
	}

	FVector2D operator/(FVector2D _Other) const
	{
		FVector2D Result;
		Result.X = X / _Other.X;
		Result.Y = Y / _Other.Y;
		return Result;
	}

	bool operator==(FVector2D _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	bool EqualToInt(FVector2D _Other) const
	{
		return iX() == _Other.iX() && iY() == _Other.iY();
	}

	FVector2D& operator+=(FVector2D _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}

	float Length() const
	{
		return sqrtf(X * X + Y * Y);
	}

	void Normalize()
	{
		float Len = Length();
		if (0.0f < Len && false == isnan(Len))
		{
			X = X / Len;
			X = Y / Len;
		}
		return;
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

	friend FVector2D operator*(float _Value, FVector2D& _Other)
	{
		FVector2D Result;
		Result.X = _Other.X * _Value;
		Result.Y = _Other.Y * _Value;
		return Result;
	}

	float Clamp(float _Value, float _Min, float _Max)
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

	// Lerp : 선형보간법 : 시작점과 끝점 사이의 중간 값(비율)을 찾는 방법
	// _Start : 현재 위치, _End : 목적지, _ElapsedTime : DeltaTime을 누적시킬 변수, _Duration : 이동할 시간
	FVector2D Lerp(FVector2D& _Start, FVector2D& _End, float _ElapsedTime, float _Duration)
	{
		float Alpha = _ElapsedTime / _Duration;
		      Alpha = Clamp(Alpha, 0.0f, 1.0f);

		FVector2D Result = ((1.0f - Alpha) * _Start) + (Alpha * _End);
		return Result;

	}

	FVector2D Lerp(FVector2D& _Start, FVector2D& _End, float _Alpha)
	{
		FVector2D Result = ((1.0f - _Alpha) * _Start) + (_Alpha * _End);
		return Result;

	}

};

class FTransform
{
public:
	FVector2D Scale;
	FVector2D Location;

	FVector2D CenterLeftTop() const
	{
		return Location - Scale.Half();
	}

	FVector2D CenterRightBottom() const
	{
		return Location + Scale.Half();
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

	FIntPoint operator+(FIntPoint _Other) const
	{
		FIntPoint Result; 
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FIntPoint operator/(int _Value) const
	{
		FIntPoint Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	bool operator==(FIntPoint _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	FIntPoint& operator+=(FIntPoint _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}
};

class EngineMath
{
};

class UColor
{
public:
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

	UColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		: R(_R), G(_G), B(_B), A(_A)
	{

	}
};
#pragma once
#include <EngineBase/EngineMath.h>

// 설명 : 전역 변수
class Global
{
public:
	static FVector2D WindowSize ;
	static FVector2D WindowHalfScale;
	static FVector2D PlayerHeadOffset;

	template<typename EnumType>
	static FVector2D SwitchEnumToDir(EnumType _Dir)
	{
		int Direction = static_cast<int>(_Dir);
		if (1 == Direction)
		{
			return FVector2D::LEFT;
		}
		if (2 == Direction)
		{
			return FVector2D::RIGHT;
		}
		if (3 == Direction)
		{
			return FVector2D::UP;
		}
		if (4 == Direction)
		{
			return FVector2D::DOWN;
		}
		return FVector2D::ZERO;
	}
};


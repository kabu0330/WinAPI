#pragma once

// 설명 :
enum class ERenderOrder
{
	BOLDERLINE = -2000,
	BACKGROUND = -1000,
	CONTROLS = -990,
	DOOR = -800,
	PLAYER = 0,
	TEAR = 1000,
	UI = 9999,
};

enum class ECollisionGroup
{
	WARP,
	PLAYER_BODY,
	PLAYER_ATTACK,
	MONSTER_BODY,
	MONSTER_FLY,
	MONSTER_ATTACK,
	MONSETR_DETECTINRANGE,
	OBJECT_WALL,
	OBJECT_ATTACK, // 오브젝트를 지나갈 수 있지만 닿으면 체력 감소
	OBJECT_BOMP, // 폭탄
	
};


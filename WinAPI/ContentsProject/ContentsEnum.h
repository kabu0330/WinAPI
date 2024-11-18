#pragma once

// 설명 :
enum class ERenderOrder
{
	Background = -1000,
	Controls = -990,
	MonsterDeathDebris = -900,
	Door = -800,
	Object_Back = -500,
	Player = 0,
	PlayerHead = 1,
	Monster = 100,
	Object_Front = 200,
	MonsterEffect = 500,
	Tear = 1000,
	UI = 9999,
	Fade = 20000,
	Loading = 30000,
	Menu = 40000
};

enum class ECollisionGroup
{
	None = -100,
	Warp = 0,
	Player_Warp = 99,
	Player_Body = 100,
	Player_Attack = 101,
	Monster_Body = 2000,
	Monster_BodyNonCollision = 2100,
	Monster_FlyingBody = 2200,
	Monster_FlyingBodyNonCollision = 2300,
	Monster_Attack = 2400,
	Monster_Barrier = 2550,
	Monster_DetectInRange = 2999,
	Object_Wall = 3000,
	Object = 3500,
	Object_Attack = 3900, // 오브젝트를 지나갈 수 있지만 닿으면 체력 감소
	Item_Bomb = 4000, // 폭탄
	
};


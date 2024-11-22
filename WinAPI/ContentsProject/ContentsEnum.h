#pragma once

// ¼³¸í :
enum class ERenderOrder
{
	Background = -1000,
	Decal = -999,
	Controls = -990,
	MonsterDeathDebris = -900,
	Door = -800,
	Object_Back = -500,
	Item_Back = -101,
	Item = -100,	
	Item_Front = -50,
	Player = 0,
	PlayerHead = 1,
	Object_Front = 99,
	Monster = 100,
	MonsterEffect = 500,
	Tear = 1000,
	ItemEffect = 2000, 
	UI_Back = 9998,
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
	Room_Wall = 3000,
	Room_TearBoundary = 3100,
	Object = 4000, 
	Item = 5000,
	Item_UniversalHit = 5500, // ÆøÅº
	Item_Impact = 5600, // ÆøÅº
	
};


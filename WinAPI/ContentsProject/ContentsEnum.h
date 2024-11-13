#pragma once

// 설명 :
enum class ERenderOrder
{
	Background = -1000,
	Controls = -990,
	Door = -800,
	Player = 0,
	PlayerHead = 1,
	Tear = 1000,
	UI = 9999,
	Fade = 20000,
	Loading = 30000,
	Menu = 40000
};

enum class ECollisionGroup
{
	Warp,
	Player_Warp,
	Player_Body,
	Player_Attack,
	Monster_Body,
	Monster_Fly,
	Monster_Attack,
	Monster_DetectInRange,
	Object_Wall,
	Object_Attack, // 오브젝트를 지나갈 수 있지만 닿으면 체력 감소
	Object_Bomb, // 폭탄
	
};


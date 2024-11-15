#pragma once

// ���� :
enum class ERenderOrder
{
	Background = -1000,
	Controls = -990,
	Door = -800,
	Player = 0,
	PlayerHead = 1,
	Monsetr = 100,
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
	Monster_BodyNonCollision,
	Monster_FlyingBody,
	Monster_FlyingBodyNonCollision,
	Monster_Attack,
	Monster_Barrier,
	Monster_DetectInRange,
	Object_Wall,
	Object_Attack, // ������Ʈ�� ������ �� ������ ������ ü�� ����
	Object_Bomb, // ��ź
	
};


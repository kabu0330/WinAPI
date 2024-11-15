#pragma once

// ���� :
enum class ERenderOrder
{
	Background = -1000,
	Controls = -990,
	Door = -800,
	Player = 0,
	PlayerHead = 1,
	Monster = 100,
	MonsterEffect = 101,
	Tear = 1000,
	UI = 9999,
	Fade = 20000,
	Loading = 30000,
	Menu = 40000
};

enum class ECollisionGroup
{
	Warp = 0,
	Player_Warp = 99,
	Player_Body = 100,
	Player_Attack = 101,
	Monster_Body = 200,
	Monster_BodyNonCollision = 201,
	Monster_FlyingBody = 210,
	Monster_FlyingBodyNonCollision = 211,
	Monster_Attack = 250,
	Monster_Barrier = 260,
	Monster_DetectInRange = 299,
	Object_Wall = 300,
	Object_Attack = 301, // ������Ʈ�� ������ �� ������ ������ ü�� ����
	Item_Bomb = 400, // ��ź
	
};


#pragma once

// ���� :
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
	PLAYER_WARPGATE,
	PLAYER_BODY,
	PLAYER_ATTACK,
	MONSTER_BODY,
	MONSTER_ATTACK,
	OBJECT_WALL,
	OBJECT_ATTACK, // ������Ʈ�� ������ �� ������ ������ ü�� ����
	OBJECT_BOMP, // ��ź
	
};


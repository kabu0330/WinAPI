#include "PreCompile.h"
#include "Room.h"
#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineAPICore.h>

ARoom::ARoom()
{
	//FVector2D WindowSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();

	//SetActorScale(WindowSize.Half());
	//SetActorLocation(WindowSize.Half());
}

ARoom::~ARoom()
{
	// map���� insert�� �ڷ���� ��� delete�ϱ� ������ ���� �Ҹ��ڸ� ȣ������ �ʿ䰡 ����.
}

bool ARoom::IsLinking(ARoom* _Room)
{
	std::map<RoomDir, ARoom*>::iterator StartIter = Rooms.begin();
	std::map<RoomDir, ARoom*>::iterator EndIter = Rooms.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (_Room == StartIter->second)
		{
			return true;
		}
	}
	return false;
}

bool ARoom::InterLinkRoom(ARoom* _Room, RoomDir _Dir)
{
	this->LinkRoom(_Room, _Dir);

	// _Room�� this�� �� ��ġ�� ���� ���ݴ밡 �Ǿ�� �Ѵ�.
	if (RoomDir::LEFT == _Dir)
	{
		_Dir = RoomDir::RIGHT;
	}
	else if (RoomDir::RIGHT == _Dir)
	{
		_Dir = RoomDir::LEFT;
	}
	else if (RoomDir::UP == _Dir)
	{
		_Dir = RoomDir::DOWN;
	}
	else if (RoomDir::DOWN == _Dir)
	{
		_Dir = RoomDir::UP;
	}

	_Room->LinkRoom(this, _Dir);

	return true;
}

ARoom* ARoom::LinkRoom(ARoom* _Room, RoomDir _Dir)
{
	if (this == _Room)
	{
		MSGASSERT("�ڱ� �ڽ��� ������ �� �����ϴ�.");
		return nullptr;
	}

	if (true == IsLinking(_Room))
	{
		MSGASSERT("�̹� ����� ���Դϴ�.");
		return nullptr;
	}

	switch (_Dir)
	{
	case RoomDir::NONE:
		break;
	case RoomDir::LEFT:
	{
		Rooms.insert({ RoomDir::LEFT, _Room });
		return Rooms.find(RoomDir::LEFT)->second;
		break;
	}
	case RoomDir::RIGHT:
	{
		Rooms.insert({ RoomDir::RIGHT, _Room });
		return Rooms.find(RoomDir::RIGHT)->second;
		break;
	}
	case RoomDir::UP:
	{
		Rooms.insert({ RoomDir::UP, _Room });
		return Rooms.find(RoomDir::UP)->second;
		break;
	}
	case RoomDir::DOWN:
	{
		Rooms.insert({ RoomDir::DOWN, _Room });
		return Rooms.find(RoomDir::DOWN)->second;
		break;
	}
	case RoomDir::MAX:
		break;
	default:
		break;
	}

    return nullptr;
}






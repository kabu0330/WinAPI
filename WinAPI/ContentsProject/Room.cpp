#include "PreCompile.h"
#include "Room.h"

ARoom::ARoom()
{
}

ARoom::~ARoom()
{
}

ARoom* ARoom::LinkRoom(ARoom* _Room, RoomDir _Dir)
{
	switch (_Dir)
	{
	case RoomDir::NONE:
		break;
	case RoomDir::LEFT:
		{
			Room[0] = _Room;
			_Room->LinkRoom(*Room, RoomDir::RIGHT);
			break;
		}
	case RoomDir::RIGHT:
		{
			Room[1] = _Room;
			_Room->LinkRoom(*Room, RoomDir::LEFT);
			break;
		}
	case RoomDir::UP:
		{
			Room[2] = _Room;
			_Room->LinkRoom(*Room, RoomDir::DOWN);
			break;
		}
	case RoomDir::DOWN:
		{
			Room[3] = _Room;
			_Room->LinkRoom(*Room, RoomDir::UP);
			break;
		}
	case RoomDir::MAX:
		break;
	default:
		break;
	}


    return nullptr;
}




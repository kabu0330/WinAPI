#pragma once
#include <Enginebase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>
#include <vector>
#include <functional>

enum class KeyEvent
{
	DOWN,
	PRESS,
	FREE,
	UP
};

// ���� : Ű����&���콺 �Է��� üũ�Ͽ� Ȱ��
class UEngineInput
{
public:
	// constrcuter destructer
	~UEngineInput();

	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;

	static UEngineInput& GetInst()
	{
		// ���� �̱��� / ���� static
		static UEngineInput Inst = UEngineInput();
		return Inst;
	}
	void KeyCheck(float _DeltaTime);
	void EventCheck(float _DeltaTime);
	void BindAction(int _KeyIndex, KeyEvent _EventType, std::function<void(float)> _Function);

	bool IsDoubleClick(int _KeyIndex, float _Time)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		return Keys[_KeyIndex].IsDown && Keys[_KeyIndex].FreeTime < _Time;
	}

	bool IsDown(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("���� ��ϵ��� ���� Ű�Դϴ�.");
			return false;
		}
		
		return Keys[_KeyIndex].IsDown;
	}

	bool IsPress(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("���� ��ϵ��� ���� Ű�Դϴ�.");
			return false;
		}

		return Keys[_KeyIndex].IsPress;
	}

	float IsPressTime(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("���� ��ϵ��� ���� Ű�Դϴ�.");
			return false;
		}

		return Keys[_KeyIndex].PressTime;
	}

	bool IsUp(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("���� ��ϵ��� ���� Ű�Դϴ�.");
			return false;
		}

		return Keys[_KeyIndex].IsUp;
	}

	bool IsFree(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("���� ��ϵ��� ���� Ű�Դϴ�.");
			return false;
		}

		return Keys[_KeyIndex].IsFree;
	}

protected:

private:
	class UEngineKey
	{
	public:
		int   Key       = -1;
		bool  IsDown    = false;
		bool  IsPress   = false;
		bool  IsUp      = false;
		bool  IsFree    = true;
		float PressTime = 0.0f;
		float FreeTime = 0.0f;

		std::vector<std::function<void(float)>> DownEvents;
		std::vector<std::function<void(float)>> PressEvents;
		std::vector<std::function<void(float)>> UpEvents;
		std::vector<std::function<void(float)>> FreeEvents;

		UEngineKey()
		{
		}
		
		UEngineKey(int _Key)
			: Key(_Key)
		{
		}

		void KeyCheck(float _DeltaTime);
		void EventCheck(float _DeltaTime);
	};

	std::map<int, UEngineKey> Keys;

	UEngineInput();
};


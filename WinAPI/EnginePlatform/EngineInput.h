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

// 설명 : 키보드&마우스 입력을 체크하여 활용
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
		// 값형 싱글톤 / 지역 static
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
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return Keys[_KeyIndex].IsDown && Keys[_KeyIndex].FreeTime < _Time;
	}

	bool IsDown(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키입니다.");
			return false;
		}
		
		return Keys[_KeyIndex].IsDown;
	}

	bool IsPress(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키입니다.");
			return false;
		}

		return Keys[_KeyIndex].IsPress;
	}

	float IsPressTime(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키입니다.");
			return false;
		}

		return Keys[_KeyIndex].PressTime;
	}

	bool IsUp(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키입니다.");
			return false;
		}

		return Keys[_KeyIndex].IsUp;
	}

	bool IsFree(int _KeyIndex)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키입니다.");
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


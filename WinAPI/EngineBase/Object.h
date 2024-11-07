#pragma once
#include <string>

// 설명 : 이름이 필요한 모든 클래스는 UObject를 상속받아야 한다.
class UObject
{
public:
	// constrcuter destructer
	UObject();

	// 많은 곳에서 쓰일 것이기 때문에 혹여나 자식 클래스의 소멸자가 호출되지 않는 위험성을 막기 위해
	// 가상함수로 소멸자를 선언한다.
	virtual ~UObject();

	// delete Function
	UObject(const UObject& _Other) = delete;
	UObject(UObject&& _Other) noexcept = delete;
	UObject& operator=(const UObject& _Other) = delete;
	UObject& operator=(UObject&& _Other) noexcept = delete;

	std::string GetName() const
	{
		return Name;
	}

	std::string_view GetNameView() const
	{
		return Name.c_str();
	}

	virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}


	// Tick이 꺼지거나, 액터가 죽었거나
	virtual bool IsActive()
	{
		return IsActiveValue && false == IsDestroyValue;
	}

	virtual bool IsDestroy()
	{
		return IsDestroyValue;
	}

	// _Time 시간 후에 죽어라.
	void Destroy(float _Time = 0.0f)
	{
		DeathTime = _Time;

		if (0.0f < _Time)
		{
			IsDeathTimeCheck = true;
			return;
		}

		IsDestroyValue = true;
	}

	// 
	virtual void ReleaseCheck(float _DeltaTime)
	{
		if (false == IsDeathTimeCheck)
		{
			return;
		}

		CurDeathTime += _DeltaTime;

		if (DeathTime <= CurDeathTime)
		{
			IsDestroyValue = true;
		}
	}

	// 모든 기능 정지.
	// IsActive가 false가 된 객체는 더 이상 Tick이 돌지 않기 때문에 외부에서 다른 객체가 풀어줘야 한다.
	void SetActive(bool _IsActive)
	{
		IsActiveValue = _IsActive;
	}

	void SetActiveSwitch()
	{
		IsActiveValue = !IsActiveValue;
	}

	// Debug
	bool IsDebug()
	{
		return IsDebugValue;
	}

	void DebugOn()
	{
		IsDebugValue = true;
	}

	void DebugOff()
	{
		IsDebugValue = false;
	}

	void DebugSwitch()
	{
		IsDebugValue = !IsDebugValue;
	}


protected:

private:
	bool IsDestroyValue = false; // 죽었냐
	bool IsActiveValue = true; // Tick을 끌 거냐

	bool IsDeathTimeCheck = false; // 완전히 Tick이 꺼질 때까지 걸리는 시간
	float DeathTime = 0.0f;
	float CurDeathTime = 0.0f;

	std::string Name;

	bool IsDebugValue = false;
};


#pragma once
#include <string>

// ���� : �̸��� �ʿ��� ��� Ŭ������ UObject�� ��ӹ޾ƾ� �Ѵ�.
class UObject
{
public:
	// constrcuter destructer
	UObject();

	// ���� ������ ���� ���̱� ������ Ȥ���� �ڽ� Ŭ������ �Ҹ��ڰ� ȣ����� �ʴ� ���輺�� ���� ����
	// �����Լ��� �Ҹ��ڸ� �����Ѵ�.
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


	// Tick�� �����ų�, ���Ͱ� �׾��ų�
	virtual bool IsActive()
	{
		return IsActiveValue && false == IsDestroyValue;
	}

	virtual bool IsDestroy()
	{
		return IsDestroyValue;
	}

	// _Time �ð� �Ŀ� �׾��.
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

	// ��� ��� ����.
	// IsActive�� false�� �� ��ü�� �� �̻� Tick�� ���� �ʱ� ������ �ܺο��� �ٸ� ��ü�� Ǯ����� �Ѵ�.
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
	bool IsDestroyValue = false; // �׾���
	bool IsActiveValue = true; // Tick�� �� �ų�

	bool IsDeathTimeCheck = false; // ������ Tick�� ���� ������ �ɸ��� �ð�
	float DeathTime = 0.0f;
	float CurDeathTime = 0.0f;

	std::string Name;

	bool IsDebugValue = false;
};


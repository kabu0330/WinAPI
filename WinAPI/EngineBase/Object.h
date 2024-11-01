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

protected:

private:
	std::string Name;
};


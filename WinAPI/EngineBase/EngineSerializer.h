#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

// 설명 :
class UEngineSerializer
{
public:
	UEngineSerializer();
	~UEngineSerializer();

	// 데이터의 크기
	void Write(void* _Data, unsigned int _Size);

	void operator<<(int& _Data)
	{
		Write(&_Data, sizeof(int));
	}

	void operator<<(bool& _Data)
	{
		Write(&_Data, sizeof(bool));
	}

	void operator<<(FVector2D& _Data)
	{
		Write(&_Data, sizeof(FVector2D));
	}

	void operator<<(FIntPoint& _Data)
	{
		Write(&_Data, sizeof(FIntPoint));
	}

	void operator<<(std::string& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		operator<<(Size);
		Write(&_Data[0], static_cast<int>(_Data.size()));
	}

	void operator<<(class ISerializObject& _Data);

	template<typename DataType>
	void operator<<(std::vector<DataType>& _vector)
	{
		int Size = static_cast<int>(_vector.size());
		operator<<(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator<<(_vector[i]);
		}
	}

	void Read(void* _Data, unsigned int _Size);

	void operator>>(int& _Data)
	{
		Read(&_Data, sizeof(int));
	}

	void operator>>(bool& _Data)
	{
		Read(&_Data, sizeof(bool));
	}

	void operator>>(FVector2D& _Data)
	{
		Read(&_Data, sizeof(FVector2D));
	}

	void operator>>(FIntPoint& _Data)
	{
		Read(&_Data, sizeof(FIntPoint));
	}

	void operator>>(std::string& _Data)
	{
		int Size;
		operator>>(Size);
		// 사이즈를 읽어낸다.
		_Data.resize(Size);

		Read(&_Data[0], static_cast<int>(_Data.size()));
	}

	void operator>>(class ISerializObject& _Data);

	template<typename DataType>
	void operator>>(std::vector<DataType>& _vector)
	{
		int Size = 0;
		operator>>(Size);
		_vector.resize(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator>>(_vector[i]);
		}
	}

	void* GetDataPtr()
	{
		return &Data[0];
	}

	int GetWriteOffset()
	{
		return WriteOffset;
	}

	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

protected:

private:
	int WriteOffset = 0;
	int ReadOffset = 0;

	std::vector<char> Data;

	// delete Function
	UEngineSerializer(const UEngineSerializer& _Other) = delete;
	UEngineSerializer(UEngineSerializer&& _Other) noexcept = delete;
	UEngineSerializer& operator=(const UEngineSerializer& _Other) = delete;
	UEngineSerializer& operator=(UEngineSerializer&& _Other) noexcept = delete;
};

class ISerializObject
{
public:
	// 데이터를 직렬화(압축)
	virtual void Serialize(UEngineSerializer& _Ser) = 0;
	// 데이터를 복구(할때)
	virtual void DeSerialize(UEngineSerializer& _Ser) = 0;
};

#pragma once
#include <Windows.h>
#include <iostream>
#include <io.h>
#include "EnginePath.h"

const int MAXPATH = 256;

// 설명 : 파일 읽기/쓰기
class UEngineFile : public UEnginePath
{
public:
	UEngineFile();
	UEngineFile(const std::string& _Path);
	UEngineFile(std::string_view _Path);
	UEngineFile(std::filesystem::path _Path);
	~UEngineFile();

	// 데이터 저장용 파일 만드는 함수 
	void Write(class UEngineSerializer& _Ser);
	void Read(class UEngineSerializer& _Ser);

	void Write(const void* _Ptr, size_t _Size);
	void Read(void* _Ptr, size_t _Size);

	void FileOpen(const char* _Mode);
	void Close();

	int GetFileSize();

private:
	FILE* File = nullptr;
};


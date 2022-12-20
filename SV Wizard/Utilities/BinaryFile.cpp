#include "BinaryFile.h"

BinaryWriter::BinaryWriter()
	:fileHandle(NULL), size(0)
{
}

BinaryWriter::~BinaryWriter()
{
}

void BinaryWriter::Open(tstring filePath, UINT openOption)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, NULL, 
		openOption, FILE_ATTRIBUTE_NORMAL, NULL);

	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryWriter::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

void BinaryWriter::Bool(bool data)
{
	WriteFile(fileHandle, &data, sizeof(bool), &size, NULL);
}

void BinaryWriter::Word(WORD data)
{
	WriteFile(fileHandle, &data, sizeof(WORD), &size, NULL);
}

void BinaryWriter::Int(int data)
{
	WriteFile(fileHandle, &data, sizeof(int), &size, NULL);
}

void BinaryWriter::UInt(UINT data)
{
	WriteFile(fileHandle, &data, sizeof(UINT), &size, NULL);
}

void BinaryWriter::Float(float data)
{
	WriteFile(fileHandle, &data, sizeof(float), &size, NULL);
}

void BinaryWriter::Double(double data)
{
	WriteFile(fileHandle, &data, sizeof(double), &size, NULL);
}

void BinaryWriter::String(const tstring& data)
{
	UInt((UINT)data.size());

	const TCHAR* str = data.c_str();
	WriteFile(fileHandle, str, (DWORD)data.size(), &size, NULL);
}

void BinaryWriter::Byte(void* data, UINT dataSize)
{
	WriteFile(fileHandle, data, dataSize, &size, NULL);
}

//////////////////////////////////////////////////////////////////////////

BinaryReader::BinaryReader()
	: fileHandle(NULL), size(0), fileSize(0)
{

}

BinaryReader::~BinaryReader()
{

}

void BinaryReader::Open(wstring filePath)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile
	(
		filePath.c_str()
		, GENERIC_READ
		, FILE_SHARE_READ
		, NULL
		, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL
		, NULL
	);
	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);

	fileSize = GetFileSize(fileHandle, NULL);
}

void BinaryReader::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

bool BinaryReader::Bool()
{
	bool temp = false;
	assert(ReadFile(fileHandle, &temp, sizeof(bool), &size, NULL));

	return temp;
}

WORD BinaryReader::Word()
{
	WORD temp = 0;
	assert(ReadFile(fileHandle, &temp, sizeof(WORD), &size, NULL));

	return temp;
}

int BinaryReader::Int()
{
	int temp = 0;
	assert(ReadFile(fileHandle, &temp, sizeof(int), &size, NULL));

	return temp;
}

UINT BinaryReader::UInt()
{
	UINT temp = 0;
	assert(ReadFile(fileHandle, &temp, sizeof(UINT), &size, NULL));

	return temp;
}

float BinaryReader::Float()
{
	float temp = 0.0f;
	assert(ReadFile(fileHandle, &temp, sizeof(float), &size, NULL));

	return temp;
}

double BinaryReader::Double()
{
	double temp = 0.0f;
	assert(ReadFile(fileHandle, &temp, sizeof(double), &size, NULL));

	return temp;
}

tstring BinaryReader::String()
{
	UINT size = Int();

	TCHAR* temp = new TCHAR[size + 1];
	assert(ReadFile(fileHandle, temp, sizeof(TCHAR) * size, &this->size, NULL));
	temp[size] = '\0';

	return temp;
}

void BinaryReader::Byte(void** data, UINT dataSize)
{
	assert(ReadFile(fileHandle, *data, dataSize, &size, NULL));
}

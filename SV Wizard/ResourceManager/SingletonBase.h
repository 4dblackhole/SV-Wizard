#pragma once

template <typename T>
class SingletonBase
{
public:
	static T& GetInstance()
	{ 
		static T s;
		return s;
	}

protected:
	SingletonBase() { }
	SingletonBase(const T& ref) { }
	SingletonBase& operator=(const T& ref) { }
	~SingletonBase() {}

};
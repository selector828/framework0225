#pragma once

namespace utils
{
	template<class T>
	void SafeDelete(T *& pointer)
	{
		if (pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}
	template<class T>
	void SafeDeleteArray(T *& pointer)
	{
		if (pointer)
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}
	template<class T, class ... Args>
	void SafeNew(T *& pointer, Args ... args)
	{
		if (pointer == nullptr)
			pointer = new T(args ...);
	}
	template<class  T, class K, class ... Args>
	void SafeNew(K *& pointer, Args ... args)
	{
		if (pointer == nullptr)
			pointer = new T(args ...);
	}
	template<class T>
	void SafeRelease(T *& pointer)
	{
		if (pointer)
		{
			pointer->Release();
			pointer = nullptr;
		}
	}
}
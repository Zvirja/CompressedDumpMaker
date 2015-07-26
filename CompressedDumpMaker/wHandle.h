#pragma once
#include <windows.h>

class wHandle
{
public:
	wHandle(HANDLE h) : _handle(h)
	{
	};

	wHandle(wHandle&) = delete;

	wHandle(wHandle&& other) :wHandle(other._handle)
	{
		other.preventReclaim();
	}

	wHandle& operator=(wHandle&) = delete;

	wHandle& operator=(wHandle&& other)
	{
		this->_handle = other._handle;
		other.preventReclaim();
		return *this;
	}

	wHandle& operator=(HANDLE handle)
	{
		this->_handle = handle;
		return *this;
	}

	~wHandle()
	{
		if (this->isValid())
		{
			CloseHandle(this->_handle);
			this->_handle = nullptr;
		}
	}

	HANDLE get() const
	{
		return this->_handle;
	};

	bool isValid() const
	{
		return this->_handle != nullptr && this->_handle != INVALID_HANDLE_VALUE;
	}

	void preventReclaim()
	{
		this->_handle = nullptr;
	}

private:
	HANDLE _handle;
};
#pragma once

template<class _Model>
class IComponent
{
	friend _Model;

public:
	IComponent(_Model * parent) : parent_(parent) {}
	virtual ~IComponent(void) {}

private:
	_Model * parent_ = nullptr;

public:
	_Model * const Parent(void)
	{
		return this->parent_;
	}
	template<class _Parent>
	_Parent * const Parent(void)
	{
		return static_cast<_Parent*>(this->parent_);
	}

private:
	virtual void Update(void) = 0;
	virtual void Pause(void) = 0;
	virtual void Always(void) = 0;
};
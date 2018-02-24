#pragma once

#pragma warning(disable: 4346)
#pragma warning(disable: 4005)

#include <component.h>

#include <d3dx11.h>
#include <string>
#include <typeinfo>
#include <unordered_map>

template<class T> class IScene;

class ID3D11Model
{
	friend class D3D11Renderer;
	friend IScene<ID3D11Model>;

public:
	ID3D11Model(IScene<ID3D11Model> * parent, std::string vertex_buffer, std::string shader) : parent_(parent), vertex_buffer_(vertex_buffer), shader_(shader) {}
	virtual ~ID3D11Model(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			delete component;
			itr = this->component_list_.erase(itr);
		}
	}

private:
	virtual void UpdateConstantBuffer(D3D11_MAPPED_SUBRESOURCE & data) = 0;

private:
	std::string vertex_buffer_;
	std::string shader_;

private:
	IScene<ID3D11Model> * parent_;

public:
	IScene<ID3D11Model> * const Parent(void)
	{
		return this->parent_;
	}

private:
	std::unordered_map<std::string, IComponent<ID3D11Model>*> component_list_;

public:
	template<class _Component, class ... Args>
	_Component * const Add(Args ... args)
	{
		auto component = new _Component(this, args ...);
		auto name = typeid(_Component).name();
		this->component_list_[name] = component;
		return component;
	}
	template<class _Component, class ... Args>
	_Component * const Get(Args ... args)
	{
		auto name = typeid(_Component).name();
		return static_cast<_Component*>(this->component_list_[name]);
	}

private:
	void Update(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			if (component)
			{
				component->Update();
				++itr;
			}
			else
			{
				delete component;
				itr = this->component_list_.erase(itr);
			}
		}
	}
	void Pause(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			if (component)
			{
				component->Pause();
				++itr;
			}
			else
			{
				delete component;
				itr = this->component_list_.erase(itr);
			}
		}
	}
	void Always(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			if (component)
			{
				component->Always();
				++itr;
			}
			else
			{
				delete component;
				itr = this->component_list_.erase(itr);
			}
		}
	}
	virtual void Adjust(void) {}

public:
	bool destroy_ = false;

public:
	void Destroy(void)
	{
		this->destroy_ = true;
	}
};

template<class _VertexBuffer, class _Shader>
class D3D11Model : public ID3D11Model
{
public:
	typename _Shader::CB constant_buffer_;

public:
	D3D11Model(IScene<ID3D11Model> * parent) : ID3D11Model(parent, typeid(_VertexBuffer).name(), typeid(_Shader).name()) {}
	void UpdateConstantBuffer(D3D11_MAPPED_SUBRESOURCE & data) override
	{
		memcpy_s(data.pData, data.RowPitch, (void*)(&this->constant_buffer_), sizeof(this->constant_buffer_));
	}
};
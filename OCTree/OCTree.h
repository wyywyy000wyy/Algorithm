#pragma once
#include <iostream>
struct Vector3
{
	float x=0;
	float y=0;
	float z=0;

	Vector3()
	{

	}

	Vector3(float v) :x(v), y(v), z(v)
	{

	}

	Vector3(float x, float y, float z):x(x),y(y),z(z)
	{

	}

	Vector3 operator+(const Vector3& other)
	{
		return Vector3(x+other.x, y+other.y, z+other.z);
	}

	Vector3 operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	template<typename T>
	Vector3 operator*(const T& other)
	{
		return Vector3(x*other, y*other, z*other);
	}

	template<typename T>
	Vector3 operator/(const T& other)
	{
		return Vector3(x / other, y */ other, z / other);
	}

	Vector3& operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template<typename T>
	Vector3& operator*=(const T& other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	template<typename T>
	Vector3& operator/=(const T& other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

};

//typedef const Vector3& constVector3Ref;

template <typename T>
struct OCTreeNodeData
{
	OCTreeNodeData(T data,const Vector3& pos):data(data),dataPos(pos){}

	T data;
	Vector3 dataPos;
};

template <typename T>
struct OCTreeNode
{
	typedef OCTreeNode<T> Node;
	typedef OCTreeNodeData<T> NodeData;


	OCTreeNode(OCTreeNode* parent,const Vector3& center, float extent):parent(parent),center(center),extent(extent){
		memset(children, 0, sizeof(Node*) * 8);
	}

	Node* Find(const Vector3& pos)
	{
		int index = pos.x <= center.x ? 0 : 1;
		index += pos.y <= center.y ? 0 : 2;
		index += pos.z <= center.z ? 0 : 4;

		if (children[index] == nullptr)
		{
			return this;
		}

		return children[index]->Find(pos);
	}

	Node* Add(NodeData* data, const Vector3& pos)
	{
		if (this->isLeaf)
		{
			this->isLeaf = false;
			this->Add(this->data, this->data->dataPos);
			this->data = nullptr;
		}

		int index = pos.x <= center.x ? 0 : 1;
		index += pos.y <= center.y ? 0 : 2;
		index += pos.z <= center.z ? 0 : 4;

		if (children[index] == nullptr)
		{
			float childExtent = extent / 2;
			float childOffset = childExtent / 2;
			Node* node = new Node(this, 
				Vector3(
					pos.x <= center.x ? (center.x - childOffset) : (center.x + childOffset),
					pos.y <= center.y ? (center.y - childOffset) : (center.y + childOffset),
					pos.z <= center.z ? (center.z - childOffset) : (center.z + childOffset)
				), 
				childExtent);
			node->data = data;
			children[index] = node;
			return node;
		}

		return children[index]->Add(data, pos);
	}

	void Remove(Node* node)
	{

		int index = node->center.x <= center.x ? 0 : 1;
		index += node->center.y <= center.y ? 0 : 2;
		index += node->center.z <= center.z ? 0 : 4;

		children[index] = nullptr;
		delete node;
		shrink();
	}

	void shrink()
	{
		if (parent == nullptr)
		{
			return;
		}

		int index = -1;
		for (int i = 0; i < 8; ++i)
		{
			Node* node = children[i];
			if (node != nullptr)
			{
				if (!node->isLeaf)
				{
					return;
				}
				if (index != -1)
				{
					return;
				}
				index = i;
			}
		}

		if (index != -1)
		{
			Node* node = children[index];
			children[index] = nullptr;

			data = node->data;
			isLeaf = true;
			delete node;
		}

		parent->shrink();
	}

	Node* children[8];
	Node* parent = nullptr;

	bool isLeaf = true;
	NodeData* data = nullptr;

	Vector3 center;
	float extent;
};

template <typename T>
class OCTree
{
	typedef OCTreeNode<T> Node;
	typedef OCTreeNodeData<T> NodeData;

public:

	OCTree(const Vector3& center, float extent) : root(new OCTreeNode<T>(nullptr, center, extent)){
		root->isLeaf = false;
	}

	Node* Add(T data, const Vector3&& pos)
	{
		NodeData* nodeData = new NodeData(data, pos);
		Node* p = root->Find(pos);
		return p->Add(nodeData, pos);
	}

	void Remove(Node* node)
	{
		node->parent->Remove(node);
	}

	Node* Find(const Vector3&& pos)
	{
		return root->Find(std::forward(pos));
	}
protected:
	Node* root;
};
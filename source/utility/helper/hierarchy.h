#pragma once

namespace util
{
	template<typename Type>
	class Hierarchy
	{
	public:
		Hierarchy();
		~Hierarchy();

		void SetOwner(Type* object);
		Type* GetOwner() const;

		bool ParentHasOwner() const;
		bool IsParentedBy(const Hierarchy& node) const;
		void SetParent(Hierarchy& node);//Makes the given node the parent. (Adds to the beginning of the children)
		void SetParentLast(Hierarchy& node);//Makes the given node the parent. (Adds to the end of the children)
		void MakeSiblingAfter(Hierarchy& node);//Makes the given node a sibling after the passed in node.
		void RemoveParent();
		void Destroy();

		Type* GetParent() const;
		Type* GetChild() const;
		Type* GetSibling() const;
		Type* GetPrevSibling() const; //Returns NULL if no parent, or if it is the first child.
		Type* Next() const; //Goes through all nodes of the hierarchy.
		Type* NextLeaf() const; //Goes through all leaf nodes of the hierarchy.

	private:
		Hierarchy* m_parent;
		Hierarchy* m_sibling;
		Hierarchy* m_child;
		Type* m_owner;

		Hierarchy<Type>* GetPrevSiblingNode() const; //Returns NULL if no parent, or if it is the first child.
	};
}

#include "hierarchy.inl"

namespace util
{
	template<typename Type>
	Hierarchy<Type>::Hierarchy()
		: m_parent(nullptr)
		, m_sibling(nullptr)
		, m_child(nullptr)
		, m_owner(nullptr)
	{}

	template<typename Type>
	Hierarchy<Type>::~Hierarchy()
	{
		Destroy();
	}

	template<typename Type>
	Type* Hierarchy<Type>::GetOwner() const
	{
		return m_owner;
	}

	template<typename Type>
	void Hierarchy<Type>::SetOwner(Type* object)
	{
		m_owner = object;
	}

	template<typename Type>
	bool Hierarchy<Type>::ParentHasOwner() const
	{
		return m_parent->m_owner != nullptr;
	}

	template<typename Type>
	bool Hierarchy<Type>::IsParentedBy(const Hierarchy& node) const
	{
		if (m_parent == &node)
		{
			return true;
		}
		else if (m_parent)
		{
			return m_parent->IsParentedBy(node);
		}
		return false;
	}

	template<typename Type>
	void Hierarchy<Type>::SetParent(Hierarchy& node)
	{
		RemoveParent();

		m_parent = &node;
		m_sibling = node.m_child;
		node.m_child = this;
	}

	template<typename Type>
	void Hierarchy<Type>::SetParentLast(Hierarchy& node)
	{
		RemoveParent();

		m_parent = &node;
		m_sibling = nullptr;
		Hierarchy<Type>* child = node.m_child;
		if (!child)
		{
			node.m_child = this;
		}
		else
		{
			while (child)
			{
				Hierarchy<Type>* nextChild = child->m_sibling;
				if (!nextChild)
				{
					child->m_sibling = this;
					break;
				}
				child = nextChild;
			}
		}
	}

	template<typename Type>
	void Hierarchy<Type>::MakeSiblingAfter(Hierarchy &node)
	{
		RemoveParent();

		m_parent = node.m_parent;
		m_sibling = node.m_sibling;
		node.m_sibling = this;
	}

	template<typename Type>
	void Hierarchy<Type>::RemoveParent()
	{
		Hierarchy<Type>* prev;

		if (m_parent)
		{
			prev = GetPrevSiblingNode();
			if (prev)
			{
				prev->m_sibling = m_sibling;
			}
			else
			{
				m_parent->m_child = m_sibling;
			}
		}

		m_parent = nullptr;
		m_sibling = nullptr;
	}

	template<typename Type>
	void Hierarchy<Type>::Destroy()
	{
		Hierarchy<Type>* parentNode;
		Hierarchy<Type>* node;

		parentNode = m_parent;
		RemoveParent();

		if (parentNode)
		{
			while (m_child)
			{
				node = m_child;
				node->RemoveParent();
				node->SetParent(*parentNode);
			}
		}
		else
		{
			while (m_child)
			{
				m_child->RemoveParent();
			}
		}
	}

	template<typename Type>
	Type* Hierarchy<Type>::GetParent() const
	{
		if (m_parent)
		{
			return m_parent->m_owner;
		}
		return nullptr;
	}

	template<typename Type>
	Type* Hierarchy<Type>::GetChild() const
	{
		if (m_child)
		{
			return m_child->m_owner;
		}
		return nullptr;
	}

	template<typename Type>
	Type* Hierarchy<Type>::GetSibling() const
	{
		if (m_sibling)
		{
			return m_sibling->m_owner;
		}
		return nullptr;
	}

	template<typename Type>
	Hierarchy<Type>* Hierarchy<Type>::GetPrevSiblingNode() const
	{
		if (!m_parent || (m_parent->m_child == this))
		{
			return nullptr;
		}

		Hierarchy<Type>* prev;
		Hierarchy<Type>* node;

		node = m_parent->m_child;
		prev = nullptr;
		while ((node != this) && (node != nullptr))
		{
			prev = node;
			node = node->m_sibling;
		}

		//assert(node == this, "Hierarchy", "could not find sibling node in parent's list of children");

		return prev;
	}

	template<typename Type>
	Type* Hierarchy<Type>::GetPrevSibling() const
	{
		Hierarchy<Type>* prior;

		prior = GetPrevSiblingNode();
		if (prior)
		{
			return prior->m_owner;
		}

		return nullptr;
	}

	template<typename Type>
	Type* Hierarchy<Type>::Next() const
	{
		const Hierarchy<Type>* node;

		if (m_child)
		{
			return m_child->m_owner;
		}
		else
		{
			node = this;
			while (node && node->m_sibling == nullptr)
			{
				node = node->m_parent;
			}
			if (node)
			{
				return node->m_sibling->m_owner;
			}
			else
			{
				return nullptr;
			}
		}
	}

	template<typename Type>
	Type* Hierarchy<Type>::NextLeaf() const
	{
		const Hierarchy<Type>* node;

		if (m_child)
		{
			node = m_child;
			while (node->m_child)
			{
				node = node->m_child;
			}
			return node->m_owner;
		}
		else
		{
			node = this;
			while (node && node->m_sibling == nullptr)
			{
				node = node->m_parent;
			}
			if (node)
			{
				node = node->m_sibling;
				while (node->m_child)
				{
					node = node->m_child;
				}
				return node->m_owner;
			}
			else
			{
				return nullptr;
			}
		}
	}
}
#pragma once
#include "subscriber.h"

namespace util
{
	template<typename TYPE>
	void Publisher<TYPE>::Publish(TYPE arg)
	{
		assert(!m_publishing);
		m_publishing = true;

		if (!m_pending.empty())
		{
			for (size_t i = 0; i < m_pending.size(); ++i)
			{
				m_subscriptions.push_back(m_pending[i]);
			}
			m_pending.clear();
			m_subscriptions.sort();
		}

		m_iter = m_subscriptions.begin();
		while (m_iter != m_subscriptions.end())
		{
			(*m_iter).m_callback(arg);
			if (!m_erasure)
				++m_iter;
		}
		m_publishing = false;
	}

	template<typename TYPE>
	void Publisher<TYPE>::Subscribe(Subscriber* subscriber, Callback callback, const int& priority)
	{
		assert(subscriber);
		assert(callback != null);

		SubStruct subscription(subscriber, callback, priority);
		m_pending.push_back(subscription);

		subscriber->AddPublisher(this);
	}

	template<typename TYPE>
	VIRTUAL void Publisher<TYPE>::Unsubscribe(Subscriber* subscriber)
	{
		for (size_t i = 0; i < m_pending.size(); ++i)
		{
			if (m_pending[i].m_subscriber == subscriber)
			{
				subscriber->RemPublisher(this);
				m_pending.erase(m_pending.begin() + i);
				--i;
			}
		}

		SubIter local_iter = m_subscriptions.begin();
		while (local_iter != m_subscriptions.end())
		{
			if ((*local_iter).m_subscriber == subscriber)
			{
				//check for publishing and current iterator
				if (m_publishing && m_iter == local_iter)
				{
					m_erasure = true;
				}
				(*local_iter).m_subscriber->RemPublisher(this);
				local_iter = m_subscriptions.erase(local_iter);
				if (m_erasure)
				{
					m_iter = local_iter;
				}
			}
			else
			{
				++local_iter;
			}
		}
	}

	template<typename TYPE>
	VIRTUAL void Publisher<TYPE>::UnsubscribeAll()
	{
		for (size_t i = 0; i < m_pending.size(); ++i)
		{
			m_pending[i].m_subscriber->RemPublisher(this);
			m_pending.erase(m_pending.begin() + i);
			--i;
		}

		SubIter local_iter = m_subscriptions.begin();
		while (local_iter != m_subscriptions.end())
		{
			//check for publishing and current iterator
			if (m_publishing && m_iter == local_iter)
			{
				m_erasure = true;
			}
			(*local_iter).m_subscriber->RemPublisher(this);
			local_iter = m_subscriptions.erase(local_iter);
			if (m_erasure)
			{
				m_iter = local_iter;
			}
		}
	}
}
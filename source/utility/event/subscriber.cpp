#include "subscriber.h"
#include "publisher.h"

namespace util
{
	Subscriber::~Subscriber()
	{
		UnsubscribeAll();
	}

	void Subscriber::UnsubscribeAll()
	{
		while (!m_pubs.empty())
		{
			IPublisher* pub = m_pubs.back();
			m_pubs.pop_back();
			pub->Unsubscribe(this);
		}
	}

	void Subscriber::AddPublisher(IPublisher* pPub)
	{
		m_pubs.push_back(pPub);
	}

	void Subscriber::RemPublisher(IPublisher* pPub)
	{
		m_pubs.remove(pPub);
	}
}
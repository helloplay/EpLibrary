/*! 
BaseOutputter for the EpLibrary
Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "epBaseOutputter.h"

using namespace epl;

DECLARE_THREAD_SAFE_CLASS(System);

BaseOutputter::OutputNode::OutputNode()
{}
BaseOutputter::OutputNode::~OutputNode()
{}


BaseOutputter::BaseOutputter(LockPolicy lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_nodeListLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_nodeListLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_nodeListLock=EP_NEW NoLock();
		break;
	default:
		m_nodeListLock=NULL;
		break;
	}
}
BaseOutputter::BaseOutputter(const BaseOutputter& b)
{
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case LOCK_POLICY_CRITICALSECTION:
		m_nodeListLock=EP_NEW CriticalSectionEx();
		break;
	case LOCK_POLICY_MUTEX:
		m_nodeListLock=EP_NEW Mutex();
		break;
	case LOCK_POLICY_NONE:
		m_nodeListLock=EP_NEW NoLock();
		break;
	default:
		m_nodeListLock=NULL;
		break;
	}
}
BaseOutputter::~BaseOutputter()
{
	Clear();
	if(m_nodeListLock)
		EP_DELETE m_nodeListLock;
}


void BaseOutputter::Clear()
{
	LockObj lock(m_nodeListLock);
	std::vector<OutputNode*>::iterator iter;
	for(iter=m_list.begin();iter!=m_list.end();iter++)
	{
		EP_DELETE (*iter);
	}
	m_list.clear();
}

void BaseOutputter::Print()
{
	LockObj lock(m_nodeListLock);
	std::vector<OutputNode*>::iterator iter;
	for(iter=m_list.begin();iter!=m_list.end();iter++)
	{
		(*iter)->Print();
	}
}

void BaseOutputter::WriteToFile(EpFile* const file)
{
	if(file)
	{
		LockObj lock(m_nodeListLock);
		std::vector<OutputNode*>::iterator iter;
		for(iter=m_list.begin();iter!=m_list.end();iter++)
		{
			(*iter)->Write(file);
		}
	}
}
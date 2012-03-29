/*! 
@file epTinyObject.h
@author Andrei Alexandrescu
  recreated by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date April 16, 2011
@brief A Tiny Object Header Interface
@version 2.0

@section LICENSE

The Loki Library
Copyright (c) 2001 by Andrei Alexandrescu
This code accompanies the book:
Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
    Patterns Applied". Copyright (c) 2001. Addison-Wesley.
Permission to use, copy, modify, distribute and sell this software for any 
    purpose is hereby granted without fee, provided that the above copyright 
    notice appear in all copies and that both that copyright notice and this 
    permission notice appear in supporting documentation.
The author or Addison-Wesley Longman make no representations about the 
    suitability of this software for any purpose. It is provided "as is" 

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

@section DESCRIPTION

An Interface for Tiny Object.

*/
#ifndef __EP_TINY_OBJECT_H__
#define __EP_TINY_OBJECT_H__
#include "epLib.h"
#include "epSingletonHolder.h"
#include "epThreadSafeClass.h"


namespace epl {
	typedef int CacheType;
	enum CACHE_ENUM{
		CACHE_TYPE_FRAGMENT = 0x01,
		CACHE_TYPE_STATIC   = 0x02,
		CACHE_TYPE_POOL     = 0x04,
		CACHE_TYPE_COMPRESS = 0x08
	};

#ifndef DEFAULT_FRAGMENT_SIZE
#define DEFAULT_FRAGMENT_SIZE 4096
#endif

#ifndef MAX_TINY_OBJECT_SIZE
#define MAX_TINY_OBJECT_SIZE 64
#endif

#ifndef UCHAR_MAX
#define UCHAR_MAX     0xff      // maximum unsigned char value 
#endif 

	class EP_FOUNDATION StaticAllocator
	{
	public:
		class EP_FOUNDATION Fragment
		{
			friend StaticAllocator;

			void Init(unsigned int blockSize, unsigned char blocks);
			void *Allocate(unsigned int blockSize);
			void Deallocate(void *p, unsigned int blockSize);
			void Reset(unsigned int blockSize, unsigned char blocks);
			void Release();
			unsigned char *m_Data;
			unsigned char firstAvailableBlock;
			unsigned char numBlocksAvailable;
		};

		StaticAllocator(unsigned int blockSize = 0);
		StaticAllocator(const StaticAllocator& rhs);
		StaticAllocator& operator=(const StaticAllocator& rhs);

		void Swap(StaticAllocator& rhs);

		void* Allocate();
		void Deallocate(void *p, CacheType type);
		void Release();
		void Compress(CacheType type);
		void Delete();

		unsigned int GetBlockSize() const
		{
			return m_blockSize;
		}
		bool operator<(unsigned int rhs) const
		{ 
			return GetBlockSize() < rhs; 
		}

		unsigned int Size()
		{
			return m_fragments.size();
		}
		bool IsEmpty()
		{
			return m_fragments.empty();
		}
	private:
		void DoDeallocate(void* p, CacheType type);
		Fragment* VicinityFind(void* p);
		unsigned int m_blockSize;
		unsigned char m_numBlocks;
		typedef std::vector<Fragment> Fragments;
		Fragments m_fragments;
		Fragment* m_allocFragment;
		Fragment* m_deallocFragment;
	};


	class EP_FOUNDATION TinyObjAllocator
	{
	public:
		TinyObjAllocator(
			unsigned int fragmentSize, 
			unsigned int maxObjectSize);
		~TinyObjAllocator();

		void* Allocate(unsigned int numBytes);
		void Deallocate(void* p, unsigned int size, CacheType type);
		void Compress(CacheType type);


	private:
		TinyObjAllocator(const TinyObjAllocator&);
		TinyObjAllocator& operator=(const TinyObjAllocator&);

		typedef std::vector<StaticAllocator> Pool;
		Pool m_pool;
		StaticAllocator* m_lastAlloc;
		StaticAllocator* m_lastDealloc;
		unsigned int m_fragmentSize;
		unsigned int m_maxObjectSize;
	};

	template
		<
		CacheType cacheType = CACHE_TYPE_STATIC|CACHE_TYPE_POOL,
		unsigned int fragmentSize = DEFAULT_FRAGMENT_SIZE,
		unsigned int maxTinyObjectSize = MAX_TINY_OBJECT_SIZE
		>
	class EP_FOUNDATION TinyObject
	{

		struct EP_FOUNDATION MyTinyObjAllocator : public TinyObjAllocator
		{
			MyTinyObjAllocator() : TinyObjAllocator(fragmentSize, maxTinyObjectSize)
			{}
		};
		typedef ThreadSafeClass< MyTinyObjAllocator > MyThreadSafeClass;
	public:
		static void* operator new(unsigned int size)
		{
			typename MyThreadSafeClass::InstantLock lock;
#if (MAX_TINY_OBJECT_SIZE != 0) && (DEFAULT_FRAGMENT_SIZE != 0)
			return SingletonHolder<MyTinyObjAllocator>::Instance().Allocate(size);
#else
			return ::operator new(size);
#endif
		}
		static void operator delete(void* p, unsigned int size)
		{
			typename MyThreadSafeClass::InstantLock lock;
#if (MAX_TINY_OBJECT_SIZE != 0) && (DEFAULT_FRAGMENT_SIZE != 0)
			SingletonHolder<MyTinyObjAllocator>::Instance().Deallocate(p, size, cacheType);
#else
			::operator delete(p, size);
#endif
		}

		static void Compress()
		{
			typename MyThreadSafeClass::InstantLock lock;
#if (MAX_TINY_OBJECT_SIZE != 0) && (DEFAULT_FRAGMENT_SIZE != 0)
			SingletonHolder<MyTinyObjAllocator>::Instance().Compress(cacheType);
#else
#endif
		}
		virtual ~TinyObject() {}
	};
}

#endif //__EP_TINY_OBJECT_H__
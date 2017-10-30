#ifndef SEQMAP__H__
#define SEQMAP__H__
/*
	注释添加以及修改于 2014-4-3 

	封装一个模板类map和一个线程安全的模板类map
*/
#include <map>
#include <assert.h>
#include "mutex_wrapper.h"

class CriticalSectionWrapper;
namespace NetworkUtil
{
	template <class T,class Map = std::map<int,T> >
	class SeqMap
	{
	public:
		typedef SeqMap<T,Map> _MyT;
		typedef std::map<int,T> _MapT;
	public:
		virtual ~SeqMap(){}
		void put(int id,T t) {m_clientmap.insert(std::make_pair(id,t));}
		void cover(int id,T t) {m_clientmap[id] = t;}
		T* get(int id)
		{
			typename _MapT::iterator it;
			it = m_clientmap.find(id);
			if(it == m_clientmap.end())
				return NULL;
			return &(it->second);
		}
		void del (int id) {m_clientmap.erase(id);}
		unsigned int size() {return m_clientmap.size();}
		void clear() { m_clientmap.clear();}
		virtual _MapT* getMap() {return &m_clientmap;}
	private:
		_MapT m_clientmap;
	};

	template <class T,class Map = std::map<int,T> >
	class SeqMap_ThreadSafe : public SeqMap<T,Map>
	{
		typedef SeqMap<T,Map> SeqMapTM;
	public:
		SeqMap_ThreadSafe()
		:mCS(NULL)
        {
            mCS = CriticalSectionWrapper::CreateCriticalSection();
            assert(mCS!=NULL);
        }
		virtual ~SeqMap_ThreadSafe(){if(mCS)delete mCS;}

		void lock(){
			mCS->Enter();
		}

		void unlock(){
			mCS->Leave();
		}

		void put(int id,T t)
		{
            //加入临界区的保护，使之具有线程安全。
            CriticalSectionScoped g(mCS);
            SeqMap<T,Map>::put(id,t);
        }
		void cover(int id,T t)
		{
            CriticalSectionScoped g(mCS);
            SeqMap<T,Map>::cover(id,t);
        }
		T* get(int id)
		{
            CriticalSectionScoped g(mCS);
            return SeqMap<T,Map>::get(id);
        }
		void del(int id)
		{
            CriticalSectionScoped g(mCS);
            SeqMap<T,Map>::del(id);
        }
		unsigned int size()
		{
            CriticalSectionScoped g(mCS);
            return SeqMap<T,Map>::size();
        }
		void clear()
		{
            CriticalSectionScoped g(mCS);
            SeqMap<T,Map>::clear();
        }
	private:
		CriticalSectionWrapper* mCS;
	};

	template<class KeyT,class ValueT>
	class KVMap
	{
	private:
		typedef typename std::map<KeyT,ValueT> MapT;
		typedef typename std::map<KeyT,ValueT>::itearator Itearator;
		MapT value;
		CriticalSectionWrapper* _csw;

		bool _safe;
	public:
		KVMap(bool threadSafe);
		~KVMap();

		bool isThreadSafe(){return _safe;}
		void put(KeyT id,ValueT t) {CriticalSectionScoped g(_csw);value.insert(std::make_pair(id,t));}
		void cover(KeyT id,ValueT t) {CriticalSectionScoped g(_csw);value[id] = t;}
		ValueT* get(KeyT id)
		{
			CriticalSectionScoped g(_csw);
			Itearator it = value.find(id);
			if(it == value.end())
				return NULL;
			return &(it->second);
		}
		void del (KeyT id) {CriticalSectionScoped g(_csw);value.erase(id);}
		unsigned int size() {CriticalSectionScoped g(_csw);return value.size();}
		void clear() { CriticalSectionScoped g(_csw);value.clear();}
		MapT* getMap() {CriticalSectionScoped g(_csw);return &value;}
	};
}

#endif//SEQMAP__H__

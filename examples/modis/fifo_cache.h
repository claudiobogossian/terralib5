#ifndef __fifo_cache__
#define __fifo_cache__

// STL
#include <cassert>
#include <map>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

template<class T> class fifo_cache: public boost::noncopyable
{
  public:

    fifo_cache(std::size_t max_size);

    ~fifo_cache();

    const T* data(boost::uint64_t id) const;

    bool is_full() const;

    T* pop();

    void push(boost::uint64_t id, T* data);

  private:

    std::map<boost::uint64_t, T*> m_fifos;
    std::vector<boost::uint64_t> m_fifo;
    std::size_t m_size;
    std::size_t m_max_size;
    std::size_t m_fifo_idx;
};

template<class T> inline fifo_cache<T>::fifo_cache(std::size_t max_size)
  : m_size(0),
    m_max_size(max_size),
    m_fifo_idx(0)
{
  m_fifo.resize(max_size, 0);
}

template<class T> inline fifo_cache<T>::~fifo_cache()
{
// TODO: apagar
}

template<class T> inline const T* fifo_cache<T>::data(boost::uint64_t id) const
{
  std::map<boost::uint64_t, T*>::const_iterator it = m_fifos.find(id);

  return (it != m_fifos.end()) ? it->second : 0;
}

template<class T> inline bool fifo_cache<T>::is_full() const
{
  return m_size == m_max_size;
}

template<class T> inline T* fifo_cache<T>::pop()
{
  if(m_size == 0)
    throw std::runtime_error("fifo cache is empty!");

  assert(m_fifos.find(m_fifo[m_fifo_idx]) != m_fifos.end());

  T* d = m_fifos[m_fifo[m_fifo_idx] ];

  --m_size;

  return d;
}

template<class T> inline void fifo_cache<T>::push(boost::uint64_t id, T* data)
{
  if(is_full())
    throw std::runtime_error("fifo cache is full!");

  std::map<boost::uint64_t, T*>::const_iterator it = m_fifos.find(id);

  if(it != m_fifos.end())
    throw std::runtime_error("fifo cache already has an entry for this fifo!");

  m_fifos[id] = data;

  ++m_size;

  m_fifo[m_fifo_idx] = id;

  m_fifo_idx = (m_fifo_idx + 1) % m_max_size;
}

#endif // __fifo_cache__


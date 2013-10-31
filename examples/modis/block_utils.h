#ifndef __block_utils__
#define __block_utils__

// STL
#include <vector>

typedef void (*extract_pixel_data_fnct_t)(const std::vector<unsigned char*>&,
                                          unsigned int,
                                          unsigned char*);

void set_extract_pixel_data_strategy(extract_pixel_data_fnct_t* f, int data_type);

template<class T> void extract_pixel_data(const std::vector<unsigned char*>& blocks,
                                          unsigned int offset,
                                          unsigned char* data)
{
  const std::size_t nblocks = blocks.size();

  for(std::size_t i = 0; i != nblocks; ++i)
  {
    memcpy(data, blocks[i] + offset, sizeof(T));

    data += sizeof(T);
  }
}

#endif // __block_utils__


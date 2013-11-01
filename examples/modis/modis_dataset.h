#ifndef __modis_dataset__
#define __modis_dataset__

// STL
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace rst
  {
    class Grid;
  }
}

class modis_dataset: public boost::noncopyable
{
  public:

    modis_dataset(const std::map <std::string, std::map <std::string, std::string > >& tile_idx,
                  const std::string subdataset_prefix,
                  const std::string subdataset_suffix,
                  std::size_t max_pixel_cache_size = 2,
                  std::size_t max_block_cache_size = 1,
                  std::size_t max_raster_cache_size = 1);

    ~modis_dataset();

    void query(const double& longitude, const double& latitude,
               const unsigned char** values,
               const std::vector<boost::gregorian::date>** times) const;

    int data_type() const;

    const te::rst::Grid* grid() const;

  private:

    class impl;

    impl* m_pimpl;
};

typedef boost::shared_ptr<modis_dataset> modis_dataset_ptr;

#endif // __modis_dataset__


// MODIS
#include "block_utils.h"
#include "fifo_cache.h"
#include "modis_dataset.h"

// TerraLib
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/srs.h>

// STL
#include <cassert>
#include <set>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/timer/timer.hpp>

class modis_dataset::impl
{
  public:

    impl(const std::map <std::string, std::map <std::string, std::string > >& tile_idx,
         const std::string subdataset_prefix,
         const std::string subdataset_suffix,
         std::size_t max_pixel_cache_size,
         std::size_t max_block_cache_size,
         std::size_t max_raster_cache_size);

    ~impl();

    void query(const double& longitude, const double& latitude,
               const unsigned char** values,
               const std::vector<boost::gregorian::date>** times) const;

    int data_type() const;

    const te::rst::Grid* grid() const;

  private:

    unsigned char* check_in_block_cache(unsigned int col, unsigned int row) const;

    unsigned char* check_in_raster_cache(unsigned int col, unsigned int row) const;

    unsigned char* check_in_disk(unsigned int col, unsigned int row) const;

    unsigned char* sync_cache_from_blocks(const std::vector<unsigned char*>& blocks,
                                          unsigned int col,
                                          unsigned int row) const;

    unsigned char* sync_cache_from_rasters(const std::vector<te::rst::Raster*>& rasters,
                                           unsigned int col,
                                           unsigned int row) const;

    //unsigned char* alloc_pixel();

    std::vector<unsigned char*>* alloc_blocks() const;

    void extract_block_data(std::vector<unsigned char*>& blocks,
                            const std::vector<te::rst::Raster*>& rasters,
                            unsigned int block_x,
                            unsigned int block_y) const;

    void load_dataset_tile_info();

    void load_dataset_grid_info();

    void load_time_info();

  private:

    std::vector<boost::gregorian::date> m_times;
    std::string m_subdataset_prefix;
    std::string m_subdataset_suffix;
    extract_pixel_data_fnct_t m_extract_pixel_data;
    const std::map <std::string, std::map <std::string, std::string > >* m_tile_idx;
    te::rst::Grid* m_grid;
    te::srs::Converter* m_converter;
    fifo_cache<unsigned char>* m_pixel_cache;
    fifo_cache<std::vector<unsigned char*> >* m_block_cache;
    fifo_cache<std::vector<te::rst::Raster*> >* m_raster_cache;
    unsigned int m_blk_w;
    unsigned int m_blk_h;
    unsigned int m_blk_size;
    unsigned int m_nblks_x;
    unsigned int m_nblks_y;
    unsigned int m_nblks_per_tile;
    unsigned int m_modis_tile_w;
    unsigned int m_modis_tile_h;
    unsigned int m_ntiles_x;
    unsigned int m_ntiles_y;
    unsigned int m_modis_tile_h_offset;
    unsigned int m_modis_tile_v_offset;
    unsigned int m_tot_blocks_in_x;
    int m_value_data_type;
    int m_pixel_data_type_size;
};

modis_dataset::impl::impl(const std::map <std::string, std::map <std::string, std::string > >& tile_idx,
                          const std::string subdataset_prefix,
                          const std::string subdataset_suffix,
                          std::size_t max_pixel_cache_size,
                          std::size_t max_block_cache_size,
                          std::size_t max_raster_cache_size)
  : m_subdataset_prefix(subdataset_prefix),
    m_subdataset_suffix(subdataset_suffix),
    m_extract_pixel_data(0),
    m_tile_idx(&tile_idx),
    m_grid(0),
    m_converter(0),
    m_pixel_cache(0),
    m_block_cache(0),
    m_raster_cache(0),
    m_blk_w(0),
    m_blk_h(0),
    m_blk_size(0),
    m_nblks_x(0),
    m_nblks_y(0),
    m_nblks_per_tile(0),
    m_modis_tile_w(0),
    m_modis_tile_h(0),
    m_ntiles_x(0),
    m_ntiles_y(0),
    m_modis_tile_h_offset(0),
    m_modis_tile_v_offset(0),
    m_tot_blocks_in_x(0),
    m_value_data_type(0),
    m_pixel_data_type_size(0)
{
  load_time_info();

  load_dataset_tile_info();

  load_dataset_grid_info();

  set_extract_pixel_data_strategy(&m_extract_pixel_data, m_value_data_type);

  m_converter = new te::srs::Converter();

  //converter->setSourceSRID(4326);
  //converter->setTargetSRID(150000);

  m_pixel_cache = new fifo_cache<unsigned char>(max_pixel_cache_size);

  m_block_cache = new fifo_cache<std::vector<unsigned char*> >(max_block_cache_size);

  m_raster_cache = new fifo_cache<std::vector<te::rst::Raster*> >(max_raster_cache_size);

  m_tot_blocks_in_x = m_ntiles_x * m_nblks_per_tile;
}

modis_dataset::impl::~impl()
{
  delete m_grid;
  delete m_converter;
  delete m_pixel_cache;
  delete m_block_cache;
  delete m_raster_cache;
}

inline void modis_dataset::impl::query(const double& longitude, const double& latitude,
                                       const unsigned char** values,
                                       const std::vector<boost::gregorian::date>** times) const
{
  *values = 0;
  *times = 0;

  //if(longitude < -180.0 || longitude > 180.0 ||
  //   latitude < -90.0 || latitude > 90.0)
  //   throw std::runtime_error("point query is out of range!");

// tranform long/lat to sinu
  double x = longitude; //double x = 0.0;
  double y = latitude; //double y = 0.0;

  //m_converter->convert(longitude, latitude, x, y);

// check if tranformation is in range
  //if(x < m_grid->getExtent( || x > m_xmax ||
  //   y < m_ymin || y > m_ymax)
  //   throw std::runtime_error("point query after srs tranformation is out of range!");

// transform world in column and row indices
  double dcol = 0.0;
  double drow = 0.0;

  m_grid->geoToGrid(x, y, dcol, drow);

  unsigned int col = static_cast<unsigned int>(dcol);
  unsigned int row = static_cast<unsigned int>(drow);

// is data in the pixel cache?
  boost::uint64_t id = static_cast<boost::uint64_t>(row) *
                       static_cast<boost::uint64_t>(m_grid->getNumberOfColumns()) +
                       static_cast<boost::uint64_t>(col);

  const unsigned char* data = m_pixel_cache->data(id);

  if(data == 0)
  {
    data = check_in_block_cache(col, row);

    if(data == 0)
    {
      data = check_in_raster_cache(col, row);

      if(data == 0)
        data = check_in_disk(col, row);

      if(data == 0)
        throw std::logic_error("a coisa ta feia!");
    }
  }

  *values = data;
  *times = &m_times;

  return;
}

inline int modis_dataset::impl::data_type() const
{
  return m_value_data_type;
}

inline const te::rst::Grid* modis_dataset::impl::grid() const
{
  return m_grid;
}

inline unsigned char* modis_dataset::impl::check_in_block_cache(unsigned int col, unsigned int row) const
{
  boost::uint64_t v = row / m_modis_tile_h;
  boost::uint64_t h = col / m_modis_tile_w;

  boost::uint64_t tile_col = col % m_modis_tile_w;
  boost::uint64_t tile_row = row % m_modis_tile_h;

  boost::uint64_t tile_block_x = tile_col / m_blk_w;
  boost::uint64_t tile_block_y = tile_row / m_blk_h;

  boost::uint64_t id = (v * m_tot_blocks_in_x) +
                       (h * m_nblks_per_tile) +
                       (tile_block_y * m_nblks_x + tile_block_x);

  const std::vector<unsigned char*>* blocks = m_block_cache->data(id);

  return (blocks != 0) ? sync_cache_from_blocks(*blocks, col, row) : 0;
}

inline unsigned char* modis_dataset::impl::check_in_raster_cache(unsigned int col, unsigned int row) const
{
  unsigned int h = col / m_modis_tile_w;

  unsigned int v = row / m_modis_tile_h;

  boost::uint64_t id = static_cast<boost::uint64_t>(v) * m_ntiles_x + static_cast<boost::uint64_t>(h);

  const std::vector<te::rst::Raster*>* rasters = m_raster_cache->data(id);

  return (rasters != 0) ? sync_cache_from_rasters(*rasters, col, row) : 0;
}

inline unsigned char* modis_dataset::impl::check_in_disk(unsigned int col, unsigned int row) const
{
  const std::size_t nrasters = m_times.size();

  std::vector<te::rst::Raster*>* rasters = 0;

  if(m_raster_cache->is_full())
  {
    for(std::size_t i = 0; i != nrasters; ++i)
    {
      delete (*rasters)[i];
      (*rasters)[i] = 0;
    }
  }
  else
  {
    rasters = new std::vector<te::rst::Raster*>(nrasters);
  }

// loop through each raster
  unsigned int h = col / m_modis_tile_w + m_modis_tile_h_offset;

  unsigned int v = row / m_modis_tile_h + m_modis_tile_v_offset;

  std::string sh = boost::lexical_cast<std::string>(h);

  if(h < 10)
    sh = "0" + sh;

  std::string sv = boost::lexical_cast<std::string>(v);

  if(v < 10)
    sv = "0" + sv;

  std::string hv = "h" + sh + "v" + sv;

  const std::map<std::string, std::string>& raster_files = m_tile_idx->at(hv);

  std::map<std::string, std::string>::const_iterator it = raster_files.begin();

  std::size_t i = 0;

  boost::timer::cpu_timer timer;

  while(it != raster_files.end())
  {
    const std::string& raster_file = it->second;

    te::rst::Raster* raster = te::rst::RasterFactory::open("URI", m_subdataset_prefix + raster_file + m_subdataset_suffix);

    (*rasters)[i] = raster;

    ++it;
    ++i;
  }

  std::string ret_val = timer.format();
  std::cout << "tile dataset opened in: " << ret_val << std::endl;

  return (i != 0) ? sync_cache_from_rasters(*rasters, col, row) : 0;
}

inline unsigned char* modis_dataset::impl::sync_cache_from_blocks(const std::vector<unsigned char*>& blocks,
                                                                  unsigned int col,
                                                                  unsigned int row) const
{
  unsigned int tile_col = col % m_modis_tile_w;
  unsigned int tile_row = row % m_modis_tile_h;

  unsigned int offset = (tile_col % m_blk_w + ((tile_row % m_blk_h) * m_blk_w)) * m_pixel_data_type_size;

  unsigned char* data = 0;

  if(m_pixel_cache->is_full())
    data = m_pixel_cache->pop();
  else
    data = new unsigned char[m_times.size() * m_pixel_data_type_size];

  m_extract_pixel_data(blocks, offset, data);

  boost::uint64_t id = static_cast<boost::uint64_t>(row) *
                       static_cast<boost::uint64_t>(m_grid->getNumberOfColumns()) +
                       static_cast<boost::uint64_t>(col);

  m_pixel_cache->push(id, data);

  return data;
}

inline unsigned char* modis_dataset::impl::sync_cache_from_rasters(const std::vector<te::rst::Raster*>& rasters,
                                                                   unsigned int col,
                                                                   unsigned int row) const
{
  assert(rasters.size() == m_times.size());

  boost::uint64_t tile_col = col % m_modis_tile_w;
  boost::uint64_t tile_row = row % m_modis_tile_h;

  boost::uint64_t tile_block_x = tile_col / m_blk_w;
  boost::uint64_t tile_block_y = tile_row / m_blk_h;

  boost::uint64_t v = row / m_modis_tile_h;
  boost::uint64_t h = col / m_modis_tile_w;

  std::vector<unsigned char*>* blocks = 0;

  if(m_block_cache->is_full())
    blocks = m_block_cache->pop();
  else
    blocks = alloc_blocks();

  extract_block_data(*blocks, rasters, tile_block_x, tile_block_y);

  boost::uint64_t id = (v * m_tot_blocks_in_x) +
                       (h * m_nblks_per_tile) +
                       (tile_block_y * m_nblks_x + tile_block_x);

  m_block_cache->push(id, blocks);

  return sync_cache_from_blocks(*blocks, col, row);
}

inline std::vector<unsigned char*>* modis_dataset::impl::alloc_blocks() const
{
  const std::size_t nblocks = m_times.size();

  std::vector<unsigned char*>* blocks = new std::vector<unsigned char*>(nblocks);

  for(std::size_t i = 0; i != nblocks; ++i)
  {
    unsigned char* block_data = new unsigned char[m_blk_size];

    (*blocks)[i] = block_data;
  }

  return blocks;
}

inline void modis_dataset::impl::extract_block_data(std::vector<unsigned char*>& blocks,
                                                    const std::vector<te::rst::Raster*>& rasters,
                                                    unsigned int block_x,
                                                    unsigned int block_y) const
{
  assert(blocks.size() == rasters.size());
  assert(blocks.size() == m_times.size());

  const std::size_t nrasters = rasters.size();

  for(std::size_t i = 0; i != nrasters; ++i)
  {
    te::rst::Raster* raster = rasters[i];

    unsigned char* block_data = blocks[i];

    assert(raster->getNumberOfBands() == 1);

    raster->getBand(0)->read(block_x, block_y, block_data);
  }
}

inline void modis_dataset::impl::load_dataset_tile_info()
{
  if(m_tile_idx->empty() || m_tile_idx->begin()->second.empty())
    return;

  const std::string& sample_file = m_tile_idx->begin()->second.begin()->second;

  te::rst::RasterPtr raster(te::rst::RasterFactory::open("URI", m_subdataset_prefix + sample_file + m_subdataset_suffix));

  m_blk_w = raster->getBand(0)->getProperty()->m_blkw;
  m_blk_h = raster->getBand(0)->getProperty()->m_blkh;

  m_nblks_x = raster->getBand(0)->getProperty()->m_nblocksx;
  m_nblks_y = raster->getBand(0)->getProperty()->m_nblocksy;

  m_nblks_per_tile = m_nblks_x * m_nblks_y;

  m_value_data_type = raster->getBand(0)->getProperty()->getType();

  m_pixel_data_type_size = te::rst::GetPixelSize(m_value_data_type);

  m_blk_size = m_blk_w * m_blk_h * m_pixel_data_type_size;

  m_modis_tile_w = raster->getNumberOfColumns();
  m_modis_tile_h = raster->getNumberOfRows();
}

inline void modis_dataset::impl::load_dataset_grid_info()
{
  if(m_tile_idx->empty() || m_tile_idx->begin()->second.empty())
    return;

  unsigned int min_h = std::numeric_limits<unsigned int>::max();
  unsigned int min_v = std::numeric_limits<unsigned int>::max();
  unsigned int max_h = std::numeric_limits<unsigned int>::min();
  unsigned int max_v = std::numeric_limits<unsigned int>::min();

  te::gm::Envelope modis_extent;

// iterate over the first raster of each tile
  std::map <std::string, std::map <std::string, std::string > >::const_iterator it = m_tile_idx->begin();

  while(it != m_tile_idx->end())
  {
    const std::string& hv = it->first;

    const std::map <std::string, std::string >& rasters_of_a_tile = it->second;

    if(rasters_of_a_tile.empty())
    {
      ++it;
      continue;
    }
    else
    {
      unsigned int h = boost::lexical_cast<unsigned int>(hv.substr(1, 2));
      unsigned int v = boost::lexical_cast<unsigned int>(hv.substr(4, 2));

      if(h < min_h)
        min_h = h;

      if(v < min_v)
        min_v = v;

      if(h > max_h)
        max_h = h;

      if(v > max_v)
        max_v = v;

      const std::string& raster_file = rasters_of_a_tile.begin()->second;

      te::rst::RasterPtr raster(te::rst::RasterFactory::open("URI", m_subdataset_prefix + raster_file + m_subdataset_suffix));

      modis_extent.Union(*(raster->getGrid()->getExtent()));

      ++it;
    }
  }

  m_ntiles_x = max_h - min_h + 1;
  m_ntiles_y = max_v - min_v + 1;

  unsigned int ncols = (max_h - min_h + 1) * m_modis_tile_w;
  unsigned int nrows = (max_v - min_v + 1) * m_modis_tile_h;

  m_grid = new te::rst::Grid(ncols, nrows, new te::gm::Envelope(modis_extent));

  m_modis_tile_h_offset = min_h;
  m_modis_tile_v_offset = min_v;
}

void modis_dataset::impl::load_time_info()
{
  std::set<std::string> all_times;

  std::map <std::string, std::map <std::string, std::string > >::const_iterator hv_it = m_tile_idx->begin();

  while(hv_it != m_tile_idx->end())
  {
    std::map <std::string, std::string>::const_iterator it = hv_it->second.begin();

    while(it != hv_it->second.end())
    {
      //if(!all_times.find(it->first))
        all_times.insert(it->first);

      ++it;
    }

    ++hv_it;
  }

  std::set<std::string>::const_iterator time_it = all_times.begin();

  while(time_it != all_times.end())
  {
    m_times.push_back(boost::gregorian::from_string(*time_it));

    ++time_it;
  }
}

modis_dataset::modis_dataset(const std::map <std::string, std::map <std::string, std::string > >& tile_idx,
                             const std::string subdataset_prefix,
                             const std::string subdataset_suffix,
                             std::size_t max_pixel_cache_size,
                             std::size_t max_block_cache_size,
                             std::size_t max_raster_cache_size)
  : m_pimpl(0)
{
  m_pimpl = new impl(tile_idx, subdataset_prefix, subdataset_suffix,
                     max_pixel_cache_size, max_block_cache_size, max_raster_cache_size);
}

modis_dataset::~modis_dataset()
{
  delete m_pimpl;
}

void modis_dataset::query(const double& longitude, const double& latitude,
                          const unsigned char** values,
                          const std::vector<boost::gregorian::date>** times) const
{
  m_pimpl->query(longitude, latitude, values, times);
}

int modis_dataset::data_type() const
{
  return m_pimpl->data_type();
}

const te::rst::Grid* modis_dataset::grid() const
{
  return m_pimpl->grid();
}




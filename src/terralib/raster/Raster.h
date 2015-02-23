/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/raster/Raster.h

  \brief An abstract class for raster data strucutures.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTER_H
#define __TERRALIB_RASTER_INTERNAL_RASTER_H

// TerraLib
#include "../common/Enums.h"
#include "../datatype/AbstractData.h"
#include "../datatype/Enums.h"
#include "../geometry/Geometry.h"
#include "Config.h"
#include "Vectorizer.h"

// STL
#include <complex>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
// Forward declaration
  namespace gm { class Envelope; }

  namespace rst
  {
// Forward declaration
    class Band;
    class BandProperty;
    class Grid;

    /*!
      \class Raster

      \brief An abstract class for raster data strucutures.

      A raster data structure represents a geograhical area divided into rows and columns,
      which form a regular grid structure of cells. Each cell contains location co-ordinates and
      one or more attribute values.

      This interface should be implemented by drivers that provide the real access to raster data.

      \ingroup rst
    */
    class TERASTEREXPORT Raster: public te::dt::AbstractData
    {
      public:

        /*! \brief Default constructor. */
        Raster();

        /*!
          \brief Constructor to create a raster from parameters.

          \param grid   The grid definition. The Raster will take its ownership.
          \param p      Access Policy.
        */
        Raster(Grid* grid, te::common::AccessPolicy p = te::common::RAccess);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand side Raster.
        */
        Raster(const Raster& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Raster();

        /*!
          \brief Opens a raster.

          \param rinfo  The information needed by each driver to open the raster.
          \param p      Access Policy.
        */
        virtual void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess) = 0;

        /*!
          \brief Sets the raster name.

          \param name The new raster name.
        */
        void setName(const std::string name);

        /*! \brief Returns the raster name. */
        const std::string& getName() const;

        /*!
          \brief Sets the raster access policy.

          \param p The new raster access policy.
        */
        void setAccessPolicy(te::common::AccessPolicy p);

        /*! \brief Returns the raster access policy. */
        te::common::AccessPolicy getAccessPolicy() const;

        /*!
          \brief It returns additional information about the raster.

          The return of thi smethod is driver dependent.

          \return Additional information about the raster.
        */
        virtual std::map<std::string, std::string> getInfo() const = 0;

        /*! \brief It returns the raster grid. */
        Grid* getGrid();

        /*! \brief It returns the raster grid. */
        const Grid* getGrid() const;

        /*! \brief Returns the geographic extension of the raster data. */
        te::gm::Envelope* getExtent();

        /*! \brief Returns the geographic extension of the raster data. */
        const te::gm::Envelope* getExtent() const;

        /*!
          \brief Returns the geographic extension of the raster data, in a given SRS (distinct from its original one).

          \param srid        The target SRS id.
          \param roi         Pointer to a region of interest. If NULL the original extenstion will be considered.

          \return A pointer the raster extension in a given SRS or NULL if it fails.

          \note The caller will take the ownership of the returned pointer.
        */
        te::gm::Envelope* getExtent(int srid, te::gm::Envelope* roi = 0) const;

        /*! \brief Returns the raster spatial reference system identifier. */
        int getSRID() const;

        /*! \brief Returns the raster number of rows. */
        unsigned int getNumberOfRows() const;

        /*! \brief Returns the raster number of columns. */
        unsigned int getNumberOfColumns() const;

        /*! \brief Returns the raster horizontal (x-axis) resolution. */
        double getResolutionX() const;

        /*! \brief Returns the raster vertical (y-axis) resolution. */
        double getResolutionY() const;

        /*! \brief Returns the number of bands (dimension of cells attribute values) in the raster. */
        virtual std::size_t getNumberOfBands() const = 0;

        /*!
          \brief Returns the data type in a particular band (or dimension).

          \note The data types are listed in terralib/datatype/DataTypes.h

          \param i The desired band.

          \return The data type in a particular band (or dimension).
        */
        virtual int getBandDataType(std::size_t i) const = 0;

        /*!
          \brief Returns the raster i-th band.

          \param i The band index.

          \warning The caller is reponsible for providing a valid band index.
        */
        virtual const Band* getBand(std::size_t i) const = 0;

        /*!
          \brief Returns the raster i-th band.

          \param i The band index.

          \warning The caller is reponsible for providing a valid band index.
        */
        virtual Band* getBand(std::size_t i) = 0;

        /*!
          \brief Access band in i position.

          \param i The band index.

          \warning The caller is reponsible for providing a valid band index.

          \return A reference to the i-th band.
        */
        virtual const Band& operator[](std::size_t i) const = 0;

        /*!
          \brief Access band in i position.

          \param i The band index.

          \warning The caller is reponsible for providing a valid band index.

          \return A reference to the i-th band.
         */
        virtual Band& operator[](std::size_t i) = 0;

        /*!
          \brief Returns the attribute value of a band of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param value       To return the attribute value.
          \param b           A particular band of the cell attribute.

          \warning The caller is responsible for providing correct values for the range [c x r x b].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getValue(unsigned int c, unsigned int r, double& value, std::size_t b = 0) const;

        /*!
          \brief Sets the attribute value in a band of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param value       The attribute value to be assigned.
          \param b           A particular band of the cell attribute.

          \warning The caller is responsible for providing correct values for the range [c x r x b].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setValue(unsigned int c, unsigned int r, const double value, std::size_t b = 0);

        /*!
          \brief Returns the imaginary attribute value in a complex band of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param value       The attribute value to be assigned.
          \param b           A particular band of the cell attribute. Default value 0.

          \warning The caller is responsible for providing correct values for the range [c x r x b].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getIValue(unsigned int c, unsigned int r, double& value, std::size_t b = 0) const;

        /*!
          \brief Sets the imaginary attribute value in a complex band of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param value       The attribute value to be assigned.
          \param b           A particular band of the cell attribute. Default value 0.

          \warning The caller is responsible for providing correct values for the range [c x r x b].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setIValue(unsigned int c, unsigned int r, const double value, std::size_t b = 0);

        /*!
          \brief Returns the imaginary attribute value in a complex band of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param value       The complex attribute value to be assigned (real, imaginary).
          \param b           A particular band of the cell attribute. Default value 0.

          \warning The caller is responsible for providing correct values for the range [c x r x b].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getValue(unsigned int c, unsigned int r, std::complex<double>& value, std::size_t b = 0) const;

        /*!
          \brief Sets the imaginary attribute value in a complex band of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param value       The complex attribute value to be assigned (real, imaginary).
          \param b           A particular band of the cell attribute. Default value 0.

          \warning The caller is responsible for providing correct values for the range [c x r x b].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setValue(unsigned int c, unsigned int r, const std::complex<double>& value, std::size_t b = 0);

        /*!
          \brief Returns the imaginary attribute values in all complex bands of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param values      A vector of attribute values to be assigned (not complex values).

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getValues(unsigned int c, unsigned int r, std::vector<double>& values) const;

        /*!
          \brief Returns the imaginary attribute values in all complex bands of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param values      A vector of complex attribute values to be assigned (real, imaginary).

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getValues(unsigned int c, unsigned int r, std::vector<std::complex<double> >& values) const;

        /*!
          \brief Sets the imaginary attribute values in all complex bands of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param values      A vector of attribute values to be assigned (not complex values).

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setValues(unsigned int c, unsigned int r, const std::vector<double>& values);

        /*!
          \brief Sets the imaginary attribute values in all complex bands of a cell.

          \param c           The column location of the cell.
          \param r           The row location of the cell.
          \param values      A vector of complex attribute values to be assigned (real, imaginary).

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setValues(unsigned int c, unsigned int r, const std::vector<std::complex<double> >& values);

        int getTypeCode() const { return te::dt::RASTER_TYPE; }

        std::string toString(void) const;

        /*!
          \brief It calls a parameter function f to apply in all pixels from two rasters, e.g. pixel = f(lhs, rhs);

          \param (*f) a function with the signature complex<double>(*f)(complex<double>, complex<double>), const Raster& rhs)

          \param rhs The rhs raster to apply the function.

          \return The resultant raster.
        */
        Raster& callOperator(std::complex<double>(*f)(std::complex<double>, std::complex<double>), const Raster& rhs);

        /*!
          \brief It returns the raster sum (pixel by pixel).

          \param rhs The raster to be added, right-hand side.

          \note Both rasters must have the same properties, #bands, #columns,
                #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster sum.
        */
        virtual Raster& operator+=(Raster& rhs);

        /*!
          \brief It returns the sum of a constant value to all pixels in the raster.

          \param cvalue The constant value to be added.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster sum.
        */
        virtual Raster& operator+=(std::complex<double>& cvalue);

        /*!
          \brief It returns the raster subtraction (pixel by pixel).

          \param rhs The raster to be subtracted, right-hand side.

          \note Both rasters must have the same properties, #bands, #columns,
                #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster subtraction.
        */
        virtual Raster& operator-=(Raster& rhs);

        /*!
          \brief It returns the difference from all pixels in the raster to a constant value (pixels - constant).

          \param cvalue The constant value to be subtracted.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster difference.
        */
        virtual Raster& operator-=(std::complex<double>& cvalue);

        /*!
          \brief It returns the raster product (pixel by pixel).

          \param rhs The raster to be multiplied, right-hand side.

          \note Both rasters must have the same properties, #bands, #columns,
                #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster product.
        */
        virtual Raster& operator*=(Raster& rhs);

        /*!
          \brief It returns the product of a constant value to all pixels in the raster.

          \param cvalue The constant value to be multiplied.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster product.
        */
        virtual Raster& operator*=(std::complex<double>& cvalue);

        /*!
          \brief It returns the raster division (pixel by pixel).

          \param rhs The raster to be divided, right-hand side.

          \note Both rasters must have the same properties, #bands, #columns,
                #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster division.
        */
        virtual Raster& operator/=(Raster& rhs);

        /*!
          \brief It returns the division of all pixels in the raster by a constant value (pixels / constant).

          \param cvalue The constant value to be divided.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The raster division.
        */
        virtual Raster& operator/=(std::complex<double>& cvalue);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        virtual Raster& operator=(const Raster& rhs);

        /*!
          \brief Subsetting operation for trimming (cropping) the raster.

          \param env         The envelope (inside original extent) to crop the raster.
          \param rinfo       The parameters needed to build the output raster (see RasterFactory documentation).

          \return A pointer to the trimmed raster if success and a null pointer otherwise.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Raster* trim(const te::gm::Envelope* env, const std::map<std::string, std::string>& rinfo) const;

        /*!
          \brief Resample a subset of the raster, given a box.

          \param method      The method of interpolation. \sa te::rst::Interpolator
          \param drow        The starting row to make a subset of the image.
          \param dcolumn     The starting column to make a subset of the image.
          \param height      The height of the subset.
          \param width       The width of the subset.
          \param newheight   The resampled height of the new raster.
          \param newwidth    The resampled width of the new raster.
          \param rinfo       The parameters needed to build the output raster (see RasterFactory documentation).

          \return A pointer to the resampled raster if success and a null pointer otherwise.

          \note The caller will take the ownership of the returned pointer.

          \warning A scale of 0 is not allowed. Use 1, 2, 3, or -1, -2, 3...
        */
        virtual Raster* resample(int method, unsigned int drow, unsigned int dcolumn, unsigned int height, unsigned int width, unsigned int newheight, unsigned int newwidth, const std::map<std::string, std::string>& rinfo) const;

        /*!
          \brief Resample raster.

          \param method      The method of interpolation.
          \param scale       The scale of interpolation (use - to shrink or + to enlarge).
          \param rinfo       The parameters needed to build the output raster (see RasterFactory documentation).

          \return A pointer to the resampled raster if success and a null pointer otherwise.

          \note The caller will take the ownership of the returned pointer.

          \warning A scale of 0 is not allowed. Use 1, 2, 3, or -1, -2, 3...
        */
        virtual Raster* resample(int method, int scale, const std::map<std::string, std::string>& rinfo) const;

        /*!
          \brief Return the raster grid for a specific scale.

          \param scale The desired sacale of interpolation (use - to shrink or + to enlarge).

          \return The raster grid.

          \note The caller will take the ownership of the returned pointer.
        */
        Grid* getResampledGrid(int scale) const;

        /*!
          \brief Reprojects this raster to a distinct SRS.
                 This method reprojects this raster to a distinct SRS. The output resolution
                 is calculated in order to maintain the same number of pixels as in this raster.

          \param srid        The target SRS identifier.
          \param rinfo       The parameters needed to build the output raster (see RasterFactory documentation).
          \param m           The method of interpolation to apply (default 1 = nearest neighbor). \sa te::rst::Interpolator


          \return A pointer to the reprojected raster if success and a null pointer otherwise.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Raster* transform(int srid, const std::map<std::string, std::string>& rinfo, int m = 1) const;

        /*!
          \brief Reprojects a squared portion of this raster to a distinct SRS.
                 This method reprojects a squared portion of this raster to a distinct SRS.
                 The output resolution is calculated in order to maintain the same number
                 of pixels as in the desired portion of this raster.

          \param srid        The target SRS identifier.
          \param llx         Lower-left X-coordinate of the portion to be reprojected (in the original SRS).
          \param lly         Lower-left Y-coordinate of the portion to be reprojected (in the original SRS).
          \param urx         Upper-Right X-coordinate of the portion to be reprojected (in the original SRS).
          \param url         Upper-Right Y-coordinate of the portion to be reprojected (in the original SRS).
          \param rinfo       The parameters needed to build the output raster (see RasterFactory documentation).
          \param m           The method of interpolation to apply (default 1 = nearest neighbor). \sa te::rst::Interpolator

          \return A pointer to the reprojected raster if success and a null pointer otherwise.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Raster* transform(int srid, double llx, double lly, double urx, double ury, const std::map<std::string, std::string>& rinfo, int m = 1) const;

        /*!
          \brief Reprojects a squared portion of this raster to another SRS with a desired resolution.

          This method reprojects a squared portion of this raster to another SRS. The number of pixels
          in the output will be calculated according to the portion being reprojected and the asked resolution.

          \param srid        The SRS id of the target SRS.
          \param llx         Lower-left X-coordinate  of the portion to be reprojected (in the original SRS).
          \param lly         Lower-left Y-coordinate of the portion to be reprojected (in the original SRS).
          \param urx         Upper-Right X-coordinate of the portion to be reprojected (in the original SRS).
          \param url         Upper-Right Y-coordinate of the portion to be reprojected (in the original SRS).
          \param resx        The output x resolution (in units of the target SRS).
          \param resx        The output y resolution (in units of the target SRS).
          \param rinfo       The parameters needed to build the output raster (see RasterFactory documentation).
          \param m           The method of interpolation to apply (default 1 = nearest neighbor). \sa te::rst::Interpolator

          \return A pointer to the reprojected raster if success and a null pointer otherwise.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Raster* transform(int srid, double llx, double lly, double urx, double ury, double resx, double resy, const std::map<std::string, std::string>& rinfo, int m = 1) const;

        /*!
          \brief Vectorizes a given raster band, using GDALPolygonize function.

          \param g           A reference to a vector of geometries.
                             Will be filled with geometries found in band.
          \param b           The band index to vectorize.
          
          \note The caller of this method must take the ownership of the returned geometries and must delete them when necessary.
        */
        virtual void vectorize(std::vector<te::gm::Geometry*>& g, std::size_t b, unsigned int mp = 0);

        /*!
          \brief Rasterizes a given vector of geometries.

          \param g           A vector of geometries to be rasterized.
          \param vp          A vector of pixel values for each geometry.
                             Can have the same size of the vector of geometries, or be null.
          \param b           The band index to rasterize.
        */
        virtual void rasterize(std::vector<te::gm::Geometry*> g, std::vector<double> vp, std::size_t b = 0);
        
        /*!
          \brief Create a sub-sampled multi-resolution pyramid.

          \param levels The number of pyramid levels to generate.
          \param interpMethod The used interpolation method.
          
          \return true if OK, false if errors ocurred.
        */        
        virtual bool createMultiResolution( const unsigned int levels, const InterpolationMethod interpMethod ) = 0;
        
        /*!
          \brief Remove/Destroy a sub-sampled multi-resolution pyramid, if there is one.
          
          \return true if OK, false if errors ocurred.
        */        
        virtual bool removeMultiResolution() = 0;        
        
        /*!
          \brief Returns the current number of multi-resolution pyramid levels.

          \return Returns the current number of multi-resolution pyramid levels.
        */        
        virtual unsigned int getMultiResLevelsCount() const = 0;        
        
        /*!
          \brief Returns the required level of a multi-resolution pyramid or NULL if that level does not exists.
          
          \param level Level of a multi-resolution pyramid.

          \return Returns the required level of a multi-resolution pyramid or NULL if that level does not exists.
          
          \note The caller must take the ownership of the returned pointer.
        */        
        virtual Raster* getMultiResLevel( const unsigned int level ) const = 0;         

      protected:

        /*!
          \brief Scales a value according to a specific resampling scale.

          \param i           The scale to apply.
          \param v           The value to be scaled.

          \return The scaled value.
        */
        double applyScale(int i, const double& v) const;

      protected:

        std::string m_name;                      //!< The raster name.
        Grid* m_grid;                            //!< The spatial support for raster data.
        te::common::AccessPolicy m_policy;       //!< The access policy, can be te::common::{NoAccess, RAccess, RWAccess, WAccess}.
    };

    typedef boost::shared_ptr<Raster> RasterPtr;

  } // end namespace rst
}   // end namespace te

#endif //__TERRALIB_RASTER_INTERNAL_RASTER_H

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
  \file terralib/raster/Band.h

  \brief It gives access to values in one band (dimension) of a raster.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BAND_H
#define __TERRALIB_RASTER_INTERNAL_BAND_H

// TerraLib
#include "Config.h"
#include "Raster.h"
#include "RasterProperty.h"

// STL
#include <complex>
#include <string>
#include <vector>

namespace te
{
  namespace rst
  {
// Forward declaration
    class BandProperty;
    class Raster;

    /*!
      \class Band

      \brief A raster band description.

      The raster band describes how to interpret (and use) values that
      can be associated to each element of a raster, in one given dimension
      (e.g. a band of multi-hiperspectral remote sensing image).
      Each instance of this class refers to a particular dimension.
      This implementation defines a set of information commonly used to describe any
      raster.

      \ingroup rst

      \sa Raster, BandProperty
    */
    class TERASTEREXPORT Band
    {
      public:

        /*!
          \brief Constructor.

          \param p      The band property. The Band will take its ownership.
          \param idx    The band index.

          \warning The class takes the ownership of the passed pointer. Do not pass a null pointer.
        */
        Band(BandProperty* p, std::size_t idx);

        /*! \brief Virtual destructor. */
        virtual ~Band();

        /*! \brief Returns the associated raster. */
        virtual Raster* getRaster() const = 0;

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        virtual Band& operator=(const Band& rhs);

        /*!
          \brief Returns the cell attribute value.

          \param c      The column location of the cell.
          \param r      The row location of the cell.
          \param value  To return the attribute value.

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getValue(unsigned int c, unsigned int r, double& value) const = 0;

        /*!
          \brief Sets the cell attribute value.

          \param c      The column location of the cell.
          \param r      The row location of the cell.
          \param value  The attribute value to be assigned.

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setValue(unsigned int c, unsigned int r, const double value) = 0;

        /*!
          \brief Returns the imaginary attribute value in a complex band of a cell.

          \param c      The column location of the cell.
          \param r      The row location of the cell.
          \param value  The attribute value to be assigned.

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getIValue(unsigned int c, unsigned int r, double& value) const = 0;

        /*!
          \brief Sets the imaginary attribute value in a complex band of a cell.

          \param c      The column location of the cell.
          \param r      The row location of the cell.
          \param value  The attribute value to be assigned.

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setIValue(unsigned int c, unsigned int r, const double value) = 0;

        /*!
          \brief Returns the imaginary attribute value in a complex band of a cell.

          \param c      The column location of the cell.
          \param r      The row location of the cell.
          \param value  The complex attribute value to be assigned (real, imaginary).

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be read.
        */
        virtual void getValue(unsigned int c, unsigned int r, std::complex<double>& value) const;

        /*!
          \brief Sets the imaginary attribute value in a complex band of a cell.

          \param c      The column location of the cell.
          \param r      The row location of the cell.
          \param value  The complex attribute value to be assigned (real, imaginary).

          \warning The caller is responsible for providing correct values for the range [c x r].

          \exception Exception Subclasses may throw an exception if the data value can not be write.
        */
        virtual void setValue(unsigned int c, unsigned int r, const std::complex<double>& value);

        /*!
          \brief It reads a data block to the specified buffer.

          \param x      The block-id in x (or x-offset).
          \param y      The block-id in y (or y-offset).
          \param buffer The buffer to be used to read from the band.

          \note The upper-left corner is (0, 0).

          \warning The buffer must have been previously allocated and must have enough capacity.
        */
        virtual void read(int x, int y, void* buffer) const = 0;

        /*!
          \brief It reads and returns a data block.

          \param x The block-id in x (or x-offset).
          \param y The block-id in y (or y-offset).

          \note The upper-left corner is (0, 0).

          \return The specified data block.
        */
        virtual void* read(int x, int y) = 0;

        /*!
          \brief It writes a data block from the specified buffer.

          \param x      The block-id in x (or x-offset).
          \param y      The block-id in y (or y-offset).
          \param buffer The buffer to be used to write to the band.

          \note The upper-left corner is (0, 0).

          \warning The method will copy the same amount of bytes used by the internal band block representation.
        */
        virtual void write(int x, int y, void* buffer) = 0;

        /*!
          \brief It computes and returns the minimum occurring value in a window of the band.

          \param rs The starting row.
          \param cs The starting column.
          \param rf The final row.
          \param cf The final column.

          \note This method without parameters will compute for the entire image.

          \warning The caller is responsible for providing correct values for the range [rs, cs x rf, cf x b].

          \return The minimum occurring value in the window, real and imaginary (if present).
        */
        virtual std::complex<double> getMinValue(unsigned int rs = 0, unsigned int cs = 0, unsigned int rf = 0, unsigned int cf = 0) const;

        /*!
          \brief It computes and returns the maximum occurring value in a window of the band.

          \param rs The starting row.
          \param cs The starting column.
          \param rf The final row.
          \param cf The final column.

          \note This method without parameters will compute for the entire raster band.

          \warning The caller is responsible for providing correct values for the range [rs, cs x rf, cf].

          \return The maximum occurring value in the window, real and imaginary (if present).
        */
        virtual std::complex<double> getMaxValue(unsigned int rs = 0, unsigned int cs = 0, unsigned int rf = 0, unsigned int cf = 0) const;

        /*!
          \brief It computes and returns the standard deviation of the occurring values in a window of the band.

          \param rs The starting row.
          \param cs The starting column.
          \param rf The final row.
          \param cf The final column.

          \note This method without parameters will compute for the entire raster band.

          \warning The caller is responsible for providing correct values for the range [rs, cs x rf, cf].

          \return The standard deviation of the occurring values in the window, real and imaginary (if present).
        */
        virtual std::complex<double> getStdValue(unsigned int rs = 0, unsigned int cs = 0, unsigned int rf = 0, unsigned int cf = 0) const;

        /*!
          \brief It computes and returns the mean of the occurring values in a window of the band.

          \param rs The starting row.
          \param cs The starting column.
          \param rf The final row.
          \param cf The final column.

          \note This method without parameters will compute for the entire raster band.

          \warning The caller is responsible for providing correct values for the range [rs, cs x rf, cf].

          \return The mean of the occurring values in the window, real and imaginary (if present).
        */
        virtual std::complex<double> getMeanValue(unsigned int rs = 0, unsigned int cs = 0, unsigned int rf = 0, unsigned int cf = 0) const;

        /*!
          \brief It computes and returns the histogram occurring values (real part) in a window of the band.

          \param rs The starting row.
          \param cs The starting column.
          \param rf The final row.
          \param cf The final column.
          \param b  The number of bins (intervals from minimum pixel to maximum). When b = 0, the histogram will be divided according to all pixel values.

          \note This method without parameters will compute for the entire raster band.

          \warning The caller is responsible for providing correct values for the range [rs, cs x rf, cf].

          \return The histogram of the occurring values (real part) in the window.
        */
        virtual std::map<double, unsigned> getHistogramR(unsigned int rs = 0, unsigned int cs = 0, unsigned int rf = 0, unsigned int cf = 0, unsigned int b = 0) const;

        /*!
          \brief It computes and returns the histogram occurring values (imaginary part) in a window of the band.

          \param rs The starting row.
          \param cs The starting column.
          \param rf The final row.
          \param cf The final column.
          \param b  The number of bins (intervals from minimum pixel to maximum). When b = 0, the histogram will be divided according to all pixel values.

          \note This method without parameters will compute for the entire raster band.

          \warning The caller is responsible for providing correct values for the range [rs, cs x rf, cf].

          \return The histogram of the occurring values (imaginary part) in the window.
        */
        virtual std::map<double, unsigned> getHistogramI(unsigned int rs = 0, unsigned int cs = 0, unsigned int rf = 0, unsigned int cf = 0, unsigned int b = 0) const;

        /*! \brief It returns the scale values (real and imaginary) to be applied to the band. */
        std::complex<double> getScaleValue() const;

        /*!
          \brief Sets the scale values (real and imaginary) to be applied to the band.

          \param s The new scale.
        */
        void setScaleValue(const std::complex<double> s);

        /*! \brief It returns the offset values (real and imaginary) to be applied to the band. */
        std::complex<double> getOffsetValue() const;

        /*!
          \brief Sets the offset values (real and imaginary) to be applied to the band.

          \param o The new offset.
        */
        void setOffsetValue(const std::complex<double> o);

        /*! \brief Returns the band property. */
        BandProperty* getProperty();

        /*! \brief Returns the band property. */
        const BandProperty* getProperty() const;

        /*!
          \brief It calls a parameter function f to apply in all pixels from two bands, e.g. pixel = f(lhs, rhs);

          \param (*f) a function with the signature complex<double>(*f)(complex<double>, complex<double>)

          \param rhs The rhs band to apply the function.

          \return The resultant band.
        */
        Band& callOperator(std::complex<double>(*f)(std::complex<double>, std::complex<double>), Band& rhs);

        /*!
          \brief It calls a parameter function f to apply in all pixels from the band, e.g. pixel = f(lhs, rhs);

          \param (*f) a function with the signature complex<double>(*f)(complex<double>, complex<double>)

          \param cvalue The constant value that will work with the generic function.

          \return The resultant band.
        */
        Band& callOperator(std::complex<double>(*f)(std::complex<double>, std::complex<double>), std::complex<double>& cvalue);

        /*!
          \brief It returns the band sum (pixel by pixel).

          \param rhs The band to be added, right-hand side.

          \note Both bands must have the same properties, #columns, #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band sum.
        */
        virtual Band& operator+=(Band& rhs);

        /*!
          \brief It returns the sum of a constant value to all pixels in the band.

          \param cvalue The constant value to be added.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band sum.
        */
        virtual Band& operator+=(std::complex<double>& cvalue);

        /*!
          \brief It returns the band subtraction (pixel by pixel).

          \param rhs The band to be subtracted, right-hand side.

          \note Both bands must have the same properties, #columns, #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band subtraction.
        */
        virtual Band& operator-=(Band& rhs);

        /*!
          \brief It returns the difference from all pixels in the band to a constant value (pixel - constant).

          \param cvalue The constant value to be subtracted.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band difference.
        */
        virtual Band& operator-=(std::complex<double>& cvalue);

        /*!
          \brief It returns the band product (pixel by pixel).

          \param rhs The band to be multiplied, right-hand side.

          \note Both bands must have the same properties, #columns, #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band product.
        */
        virtual Band& operator*=(Band& rhs);

        /*!
          \brief It returns the product of a constant value to all pixels in the band.

          \param cvalue The constant value to be multiplied.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band product.
        */
        virtual Band& operator*=(std::complex<double>& cvalue);

        /*!
          \brief It returns the band division (pixel by pixel).

          \param rhs The band to be divided, right-hand side.

          \note Both bands must have the same properties, #columns, #rows, and data type.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band division.
        */
        virtual Band& operator/=(Band& rhs);

        /*!
          \brief It returns the division of all pixels in the band by a constant value (pixel / constant).

          \param cvalue The constant value to be divided.

          \note The caller is responsible to guarantee that resultant values
                will not exceed the range of the data type.

          \return The band division.
        */
        virtual Band& operator/=(std::complex<double>& cvalue);

        /*! \brief It returns the number of bytes ocuppied by a data block. */
        virtual int getBlockSize() const;

      protected:

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        Band(const Band& rhs);

      protected:

        BandProperty* m_property;      //!< The band information.
        std::size_t m_idx;             //!< The band index.
    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BAND_H

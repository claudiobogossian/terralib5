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
  \file terralib/geometry/GeometryFile.cpp
*/
#include "GeometryFile.h"

// TerraLib
#include "../../../geometry/Geometry.h"
#include "../../../geometry/WKBReader.h"

GeometryFile::GeometryFile() :
m_i(0)
{
  m_file.reset(0);
  m_posFile.reset(0);
}

GeometryFile::~GeometryFile()
{
  m_file->close();
  m_posFile->close();
}

void GeometryFile::openFile(const char* fileName) throw (te::common::Exception)
{
  m_file.reset(new std::ifstream(fileName, std::ios::in | std::ios::binary));
  m_posFile.reset(new std::ifstream((fileName+std::string(".pos")).c_str(), std::ios::in | std::ios::binary));

  if(!m_file->is_open() || !m_posFile->is_open())
    throw te::common::Exception("Fail to open file.");
}

te::gm::Geometry* GeometryFile::next()
{
  if((m_i > 0) && (m_i >= m_gids.size()) && !(m_gids.empty()))
    return 0;

  unsigned int gSize;
  unsigned int id = m_i;
  unsigned int gPos;

  if(!m_gids.empty())
    id = m_gids[m_i];

  m_i++;

  m_posFile->seekg(id*sizeof(unsigned int), std::ios::beg);
  m_posFile->read((char*)&gPos, sizeof(unsigned int));

  if(m_posFile->gcount() == 0)
    return 0;

  m_file->seekg(gPos, std::ios::beg);

  m_file->read((char*)&gSize, sizeof(unsigned int));

  if(m_file->gcount() == 0)
    return 0;

  char* wkb = new char[gSize];

  m_file->read(wkb, gSize);

  te::gm::Geometry* gm = 0;

  if(m_file->gcount() != 0)
    gm = te::gm::WKBReader::read(wkb);

  delete[] wkb;

  return gm;
}

void GeometryFile::writeGeometries(const char* fileName, const std::map<int, te::gm::Geometry*>& geoms)
{
  std::string gFileName = std::string(fileName) + ".geom";
  std::string posFileName = gFileName + ".pos";

  std::ofstream gFile(gFileName.c_str(), std::ios::out | std::ios::binary);
  std::ofstream gposFile(posFileName.c_str(), std::ios::out | std::ios::binary);

  if(!gFile.is_open() || !gposFile.is_open())
    return;

  unsigned int pos = 0;

  for(std::map<int, te::gm::Geometry*>::const_iterator it = geoms.begin(); it != geoms.end(); ++it)
  {
    // Getting the position on file
    unsigned int gBS;// = (unsigned int)gm->getWkbSize();

    size_t gBSi;

    char * wkb = it->second->asBinary(gBSi);

    gBS = (unsigned int)gBSi;

    // Index of the geometry in the file
    gposFile.write((char*)&pos, sizeof(unsigned int));

    // Write wkb size and wkb
    gFile.write((char*)&gBS, sizeof(unsigned int));
    gFile.write(wkb, gBS);

    delete[] wkb;

    // Updating position
    pos += sizeof(unsigned int)+gBS;
  }

  gFile.close();
  gposFile.close();
}

void GeometryFile::writeGeometries(const char* fileName, const std::vector<te::gm::Geometry*>& geoms)
{
  std::string gFileName = std::string(fileName) + ".geom";
  std::string posFileName = gFileName + ".pos";
  std::string idsFileName = gFileName + ".ids";

  std::ofstream gFile(gFileName.c_str(), std::ios::out | std::ios::binary);
  std::ofstream gposFile(posFileName.c_str(), std::ios::out | std::ios::binary);

  if(!gFile.is_open() || !gposFile.is_open())
    return;

  unsigned int pos = 0;
  int idCount = 0;

  for(std::vector<te::gm::Geometry*>::const_iterator it = geoms.begin(); it != geoms.end(); ++it)
  {
    // Getting the position on file
    unsigned int gBS;

    size_t gBSi;

    char * wkb = (*it)->asBinary(gBSi);

    gBS = (unsigned int)gBSi;

    // Index of the geometry in the file
    gposFile.write((char*)&pos, sizeof(unsigned int));

    // Write wkb size and wkb
    gFile.write((char*)&gBS, sizeof(unsigned int));
    gFile.write(wkb, gBS);

    delete[] wkb;

    // Updating position
    pos += sizeof(unsigned int)+gBS;
  }

  gFile.close();
  gposFile.close();
}

void GeometryFile::setGeometries(const std::vector<unsigned int>& gids)
{
  m_gids = gids;
  m_i = 0;
}

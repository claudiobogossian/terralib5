/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsByteArray.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsByteArray );

void TsByteArray::setUp()
{
}

void TsByteArray::tearDown()
{
}

void TsByteArray::tcByteArray()
{
//#ifdef TE_COMPILE_ALL
  te::dt::ByteArray* b  = new te::dt::ByteArray();
  CPPUNIT_ASSERT(b->getData() == 0);
  CPPUNIT_ASSERT(b->capacity() == 0);
  CPPUNIT_ASSERT(b->bytesUsed() == 0);

  te::dt::ByteArray b1;
  CPPUNIT_ASSERT(b1.getData() == 0);
  CPPUNIT_ASSERT(b1.capacity() == 0);
  CPPUNIT_ASSERT(b1.bytesUsed() == 0);

  delete b;

//#endif
}

void TsByteArray::tcByteArraySize()
{
//#ifdef TE_COMPILE_ALL
  te::dt::ByteArray* b = new te::dt::ByteArray(100);
  CPPUNIT_ASSERT(b->getData() != 0);
  CPPUNIT_ASSERT(b->capacity() == 100);
  CPPUNIT_ASSERT(b->bytesUsed() == 0);

  te::dt::ByteArray b1(100);
  CPPUNIT_ASSERT(b1.getData() != 0);
  CPPUNIT_ASSERT(b1.capacity() == 100);
  CPPUNIT_ASSERT(b1.bytesUsed() == 0);

  delete b;

//#endif
}

void TsByteArray::tcInit()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 bytes
  //char data[] = "12345678901234567890"; //20 bytes - on delete b - cai se declarar data assim.

  te::dt::ByteArray *b = new te::dt::ByteArray(data,20) ;
  CPPUNIT_ASSERT(b->capacity() == 20);
  CPPUNIT_ASSERT(b->bytesUsed() == 20);
  char* dataR = b->getData();
  size_t s = strlen(dataR);
  CPPUNIT_ASSERT(memcmp(data,dataR,s) == 0);
  CPPUNIT_ASSERT(strlen(data) == strlen(dataR));
  CPPUNIT_ASSERT(  data == dataR ); //init will take the ownership of the data
  CPPUNIT_ASSERT( *data == *dataR );
  CPPUNIT_ASSERT( strncmp(data,dataR,20) == 0);

  delete b;  //it will also delete char *data

//#endif
}
void TsByteArray::tcInit1()
{
//#ifdef TE_COMPILE_ALL
  char *data2;
  data2 = new char[33];
  strcpy(data2,"12345678901234567890123456789012"); //32 bytes
  te::dt::ByteArray *b2 = new te::dt::ByteArray(data2,100,32) ;
  CPPUNIT_ASSERT(b2->capacity() == 100);
  CPPUNIT_ASSERT(b2->bytesUsed() == 32);

  char* dataR2 = b2->getData();
  size_t s = strlen(dataR2);
  CPPUNIT_ASSERT(strlen(data2) == strlen(dataR2));
  CPPUNIT_ASSERT(memcmp(data2,dataR2,s) == 0);
  CPPUNIT_ASSERT(data2 == dataR2);

  delete b2;

//#endif
}

void TsByteArray::tcTake()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions

  te::dt::ByteArray *b = new te::dt::ByteArray(20) ;
  b->take(data,20);
  CPPUNIT_ASSERT(b->capacity() == 20);
  char* dataR = b->getData();
  size_t s = strlen(dataR);
  CPPUNIT_ASSERT(strlen(data) == strlen(dataR));
  CPPUNIT_ASSERT(data == dataR );
  CPPUNIT_ASSERT(strncmp(data,dataR,strlen(data)) == 0);
  CPPUNIT_ASSERT(memcmp(data,dataR,s) == 0);

  delete b;

//#endif
}

void TsByteArray::tcTake1()
{
//#ifdef TE_COMPILE_ALL

  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 bytes
  te::dt::ByteArray *b2 = new te::dt::ByteArray(100) ;
  CPPUNIT_ASSERT(b2->capacity() == 100);
  CPPUNIT_ASSERT(b2->bytesUsed() == 0);

  b2->take(data,100,32);
  CPPUNIT_ASSERT(b2->bytesUsed() == 32);
  CPPUNIT_ASSERT(b2->capacity() == 100);

  char* dataR2 = b2->getData();
  size_t s2 = strlen(dataR2);
  CPPUNIT_ASSERT(data == dataR2);
  CPPUNIT_ASSERT(strncmp(data,dataR2,strlen(data)) == 0);
  CPPUNIT_ASSERT(memcmp(data,dataR2,strlen(data)) == 0);

  delete b2;

//#endif
}

void TsByteArray::tcCopy()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions

  te::dt::ByteArray *b = new te::dt::ByteArray(15) ;
  b->copy(data,20);
  CPPUNIT_ASSERT(b->capacity() == 20);
  char* dataR = b->getData();
  CPPUNIT_ASSERT(data != dataR );  //as the capacity(15) < 20, a new pointer is allocated with 20 bytes 
  CPPUNIT_ASSERT(strncmp(data,dataR,strlen(data)) == 0);
  CPPUNIT_ASSERT(memcmp(data,dataR,strlen(data)) == 0);

  delete b;

//#endif
}

void TsByteArray::tcCopy1()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions
  te::dt::ByteArray *b = new te::dt::ByteArray(100) ;
  CPPUNIT_ASSERT(b->capacity() == 100);
  CPPUNIT_ASSERT(b->bytesUsed() == 0);

  b->copy(data,20);
  CPPUNIT_ASSERT(b->capacity() == 100);
  CPPUNIT_ASSERT(b->bytesUsed() == 20);

  char *data2;
  data2 = new char[33];
  strcpy(data2,"12345678901234567890123456789012"); //32 bytes

  b->copy(data2,32,20); //offset 20
  size_t su = b->bytesUsed();
  size_t sc = b->capacity();

  CPPUNIT_ASSERT(b->bytesUsed() == 52);
  CPPUNIT_ASSERT(b->capacity() == 100);

  char* dataR2 = b->getData();
  size_t s2 = strlen(dataR2);
  CPPUNIT_ASSERT(strlen(data2)+strlen(data) == b->bytesUsed());

  delete b;

//#endif
}

void TsByteArray::tcClear()
{
//#ifdef TE_COMPILE_ALL
  char* d20 = new char[21];
  strcpy(d20,"12345678901234567890");
  size_t s = strlen(d20);
  te::dt::ByteArray* b = new te::dt::ByteArray(d20,s);
  CPPUNIT_ASSERT(b->capacity() == 20);
  CPPUNIT_ASSERT(b->bytesUsed() == 20);
  b->clear();

  CPPUNIT_ASSERT(b->bytesUsed() == 0);
  CPPUNIT_ASSERT(b->capacity() == 0);

  delete b;

//#endif
}

void TsByteArray::tcClone()
{
//#ifdef TE_COMPILE_ALL
  char* data = new char[21];
  strcpy(data,"12345678901234567890");
  size_t s = strlen(data);
  te::dt::ByteArray* b = new te::dt::ByteArray(data,s);
  CPPUNIT_ASSERT(b->capacity() == 20);
  CPPUNIT_ASSERT(b->bytesUsed() == 20);
  te::dt::ByteArray bclone =  *static_cast<te::dt::ByteArray*>(b->clone());
  CPPUNIT_ASSERT(bclone.capacity() == 20);
  CPPUNIT_ASSERT(bclone.bytesUsed() == 20);
  CPPUNIT_ASSERT(strncmp(bclone.getData(),b->getData(),b->bytesUsed()) == 0);
  CPPUNIT_ASSERT(b->getData() != bclone.getData());
  char* dataR = b->getData();
  CPPUNIT_ASSERT(strncmp(data,dataR,s) == 0);
  CPPUNIT_ASSERT(memcmp(data,dataR,strlen(data)) == 0);

  delete b;

//#endif
}

void TsByteArray::tcByteArrayConstructor()
{
//#ifdef TE_COMPILE_ALL

  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions
  te::dt::ByteArray baux(data,20);
  te::dt::ByteArray b(baux);
  te::dt::ByteArray b1 = b;

// Checking Assign Operator
  char * datab1 = b1.getData();
  CPPUNIT_ASSERT(memcmp(b.getData(),baux.getData(),strlen(data)) == 0);
  CPPUNIT_ASSERT(memcmp(b1.getData(),b.getData(),strlen(data)) == 0);
  CPPUNIT_ASSERT(strncmp(b1.getData(),b.getData(), strlen(data)) == 0);

//#endif
}

void TsByteArray::tcToString()
{
//#ifdef TE_COMPILE_ALL
  char* d20 = new char[21];
  strcpy(d20,"12345678901234567890");
  std::string hex("3132333435363738393031323334353637383930");
  strcpy(d20,"12345678901234567890");
  size_t s = strlen(d20);
  te::dt::ByteArray* b = new te::dt::ByteArray(d20,s);
  CPPUNIT_ASSERT(b->capacity() == 20);
  CPPUNIT_ASSERT(b->bytesUsed() == 20);
  std::string ss = b->toString();

  CPPUNIT_ASSERT(strncmp(hex.c_str(),ss.c_str(),s*2) == 0);

  delete b;

//#endif
}
void TsByteArray::tcGetTypeCode()
{
//#ifdef TE_COMPILE_ALL
  char* d20 = new char[21];
  strcpy(d20,"12345678901234567890");
  size_t s = strlen(d20);
  te::dt::ByteArray* b = new te::dt::ByteArray(d20,s);
  CPPUNIT_ASSERT(b->getTypeCode() == te::dt::BYTE_ARRAY_TYPE);

  delete b;

//#endif
}

void TsByteArray::tcSetBytesUsed()
{
//#ifdef TE_COMPILE_ALL
  te::dt::ByteArray* b = new te::dt::ByteArray();
  b->setBytesUsed(100); 
  CPPUNIT_ASSERT(b->bytesUsed() == 100);
  delete b;
//#endif
}

void TsByteArray::tcOperator()
{
//#ifdef TE_COMPILE_ALL
  
  te::dt::ByteArray b1;
  int i = 3 ;
  te::dt::ByteArray b2 =  operator <<( b1, i);
  char* res = b2.getData();
  std::string ss = b2.toString();
  int x = *res;
  CPPUNIT_ASSERT(*res == i );

  b1.clear(); b2.clear();
  unsigned int ui =  127 ; // 2147483647 ;
  te::dt::ByteArray bui =  operator <<( b1, ui);
  char* res1 = bui.getData();
  ss  = bui.toString();
  unsigned int xx = *res1;
  CPPUNIT_ASSERT(*res1 == ui );


  b1.clear(); b2.clear();
  float f = 3.55f;
  b2 = operator <<( b1, f);
  res =  b2.getData();
  ss  =  b2.toString();
  float ff = *res;
  CPPUNIT_ASSERT(*res == f );

  b1.clear(); b2.clear();
  double d = 3.55;
  b2 = operator <<( b1, f);
  res =  b2.getData();
  ss  =  b2.toString();
  double dd = *res;
  CPPUNIT_ASSERT(*res == d );

//#endif
}
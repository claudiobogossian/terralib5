/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include "terralib/datatype/ByteArray.h"

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

  te::dt::ByteArray *b = new te::dt::ByteArray(data, 20) ;
  CPPUNIT_ASSERT(b->bytesUsed() == 20);
  char* dataR = b->getData();
  size_t ss = strlen(dataR);

  CPPUNIT_ASSERT(strlen(data) == strlen(dataR));
  CPPUNIT_ASSERT(  data == dataR ); //init will take the ownership of the data
  CPPUNIT_ASSERT( *data == *dataR );
  CPPUNIT_ASSERT_MESSAGE("getData does not return expected data - strlen+1 should be passed in ´init´.", strcmp(data,dataR) == 0);

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
  char* dataR2 = b2->getData();
  size_t s = strlen(dataR2);
  CPPUNIT_ASSERT(data2 == dataR2);
  //CPPUNIT_ASSERT(strlen(data2) != strlen(dataR2));
  CPPUNIT_ASSERT_MESSAGE("getData does not return expected data - strlen+1 should be passed in ´init´.", strcmp(data2,dataR2) == 0);

  delete b2;

//#endif
}

void TsByteArray::tcInitOk()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions

  te::dt::ByteArray *b = new te::dt::ByteArray(data, strlen(data)+1) ;
  CPPUNIT_ASSERT(b->bytesUsed() == 21);
  char* dataR = b->getData();
  size_t ss = strlen(dataR);
  CPPUNIT_ASSERT(strlen(data) == strlen(dataR));
  CPPUNIT_ASSERT(data == dataR );
  CPPUNIT_ASSERT(strcmp(data,dataR) == 0);

  delete b;  //after init never delete ´char *data´

  char *data2;
  data2 = new char[33];
  strcpy(data2,"12345678901234567890123456789012"); //32 bytes
  te::dt::ByteArray *b2 = new te::dt::ByteArray(data2, 100, strlen(data2)+1) ;
  char* dataR2 = b2->getData();
  size_t s = strlen(dataR2);
  CPPUNIT_ASSERT(data2 == dataR2);
  CPPUNIT_ASSERT(strlen(data2) == strlen(dataR2));
  CPPUNIT_ASSERT(strcmp(data2,dataR2) == 0);

  delete b2;

//#endif
}

void TsByteArray::tcSetDataAfterByteArrayDataSize()
{
//#ifdef TE_COMPILE_ALL
  char* d20 = new char[21];
  strcpy(d20,"12345678901234567890");
  size_t s = strlen(d20);
// should I consider the end of string (character ´\0´) in size (strlen or strlen+1)?
  te::dt::ByteArray* b = new te::dt::ByteArray(d20,s);
  CPPUNIT_ASSERT(b->getData() != 0);
  char *dataR = b->getData();
  CPPUNIT_ASSERT(dataR == d20);
  CPPUNIT_ASSERT(b->capacity() == s);
  CPPUNIT_ASSERT(b->bytesUsed() == s);
  //CPPUNIT_ASSERT(*dataR == *d20);
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the constructed one", strcmp(d20,dataR) == 0);

  delete b;

  te::dt::ByteArray* b2 = new te::dt::ByteArray();
  char* d31 = new char[32];
  strcpy(d31,"1234567890123456789012345678901"); //31 bytes
  b2->take(d31,strlen(d31));
  char *dataR2 = b2->getData();
  size_t ss = strlen(dataR2);

  CPPUNIT_ASSERT(b2->capacity() == 31);
  CPPUNIT_ASSERT(b2->bytesUsed() == 31);
  CPPUNIT_ASSERT(b2->getData() == d31);     //compare pointers
  CPPUNIT_ASSERT(*(b2->getData()) == *d31); //??compare values pointed by pointers 
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the input data, strlen+1 should be passed in take", strcmp(d31,dataR2) == 0);
  
  delete b2;
  //delete []d31;

//#endif
}

void TsByteArray::tcSetDataAfterByteArraySize()
{
//#ifdef TE_COMPILE_ALL

  te::dt::ByteArray* b3 = new te::dt::ByteArray(100);
  char* d21 = new char[22];
  strcpy(d21,"123456789012345678901"); //21 caracteres
  b3->take(d21,21);
  char *dataR1 = b3->getData();

  CPPUNIT_ASSERT(b3->bytesUsed() == 21);
  CPPUNIT_ASSERT(b3->capacity() != 100);
  CPPUNIT_ASSERT(dataR1 == d21);   //compare pointers
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the input data, strlen+1 should be passed in take ", strcmp(d21,dataR1) == 0);

  delete b3;
  //delete []d21;

  //#endif
}

void TsByteArray::tcByteArrayDataSizeOk()
{
//#ifdef TE_COMPILE_ALL

// Testing take and getData after 3 different constructors (strlen+1)
// The constructors take the ownership of the data

// Contructor 1...
  te::dt::ByteArray* b1 = new te::dt::ByteArray(100);
  char* d21 = new char[22];
  strcpy(d21,"123456789012345678901"); //21 caracteres
  b1->take(d21,strlen(d21)+1);
  char *dataR1 = b1->getData();
  size_t s = strlen(dataR1);

  CPPUNIT_ASSERT(strlen(d21) == strlen(dataR1));
  CPPUNIT_ASSERT(b1->bytesUsed() == 22);

  CPPUNIT_ASSERT(b1->capacity() != 100);
  CPPUNIT_ASSERT(dataR1 == d21);         //compare pointers
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the input data, strlen+1 should be passed in take ", strcmp(d21,dataR1) == 0);

// setting a bigger data in b1 byteArray, but smaller than 100.
  char* d51 = new char[52];
  strcpy(d51,"setting-a-bigger-data-does-not-delete-m_data-if<100"); //51 bytes
  b1->take(d51,strlen(d51)+1);
  dataR1 = b1->getData();

  CPPUNIT_ASSERT(b1->bytesUsed() == 52);
  CPPUNIT_ASSERT(b1->capacity() != 100);
  CPPUNIT_ASSERT(dataR1 == d51);   //compare pointers
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the input data, strlen+1 should be passed in take ", strcmp(d51,dataR1) == 0);

  delete b1;

// Contructor 2...
  te::dt::ByteArray* b2 = new te::dt::ByteArray();
  char* d31 = new char[32];
  strcpy(d31,"1234567890123456789012345678901"); //31 caracteres
  b2->take(d31,strlen(d31)+1);
  char *dataR2 = b2->getData();
  size_t ss = strlen(dataR2);

  CPPUNIT_ASSERT(b2->bytesUsed() == 32);
  CPPUNIT_ASSERT(b2->capacity() == 32);
  CPPUNIT_ASSERT(b2->getData() == d31);     //compare pointers
  CPPUNIT_ASSERT(*(b2->getData()) == *d31); //??compare values pointed by pointers 
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the input data ", strcmp(d31,dataR2) == 0);
  
  delete b2;

//Constructor 3...
  char* d50 = new char[51];
  strcpy(d50,"12345678901234567890123456789012345678901234567890"); //50 bytes
  te::dt::ByteArray b3(d50,strlen(d50)+1);
  char *dataR3 = b3.getData();

  CPPUNIT_ASSERT(b3.getData() != 0);
  CPPUNIT_ASSERT(b3.getData() == d50);
  CPPUNIT_ASSERT(b3.bytesUsed() == 51);
  CPPUNIT_ASSERT(b3.capacity() == 51);
  CPPUNIT_ASSERT(*(b3.getData()) == *d50); //??

  CPPUNIT_ASSERT_MESSAGE("getData is not equal the constructed one", strcmp(d50,dataR3) == 0);

  //b3 (and d50) will be destructed by the ~ 

//#endif
}

void TsByteArray::tcByteArrayConstructor()
{
//#ifdef TE_COMPILE_ALL

  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions
  size_t s = strlen(data);
  te::dt::ByteArray baux(data,20);
  te::dt::ByteArray b(baux);

  char *dataR = baux.getData();
  CPPUNIT_ASSERT(dataR == data);
  CPPUNIT_ASSERT(baux.bytesUsed() == 20);
  CPPUNIT_ASSERT(baux.capacity() == 20);
  CPPUNIT_ASSERT(strcmp(dataR,data) == 0);

  char * dataR2 = b.getData();
  CPPUNIT_ASSERT(dataR2 != data);
  CPPUNIT_ASSERT(b.bytesUsed() == 20);
  CPPUNIT_ASSERT(b.capacity() == 20);
  //CPPUNIT_ASSERT("Missing of end of string in byteArray - pass strlen+1 on constructor ",strcmp(dataR2,data) == 0);
  CPPUNIT_ASSERT(strcmp(dataR2,data) != 0);

//#endif
}

void TsByteArray::tcByteArrayConstructor1()
{
//#ifdef TE_COMPILE_ALL
  char *data1;
  data1 = new char[31];
  strcpy(data1,"123456789012345678901234567890"); //30 positions
  size_t s1 = strlen(data1);
  te::dt::ByteArray baux1(data1,100); //what happens if size is bigger than strlen(data)?? byte Ocuppied will be wrong.
  te::dt::ByteArray b1(baux1);

  char *dataR1 = baux1.getData();
  CPPUNIT_ASSERT(dataR1 == data1);
  CPPUNIT_ASSERT(baux1.bytesUsed() == 100);
  CPPUNIT_ASSERT(baux1.capacity() == 100);
  CPPUNIT_ASSERT(*dataR1 == *data1);  //?? it does not work properly, use strcmp
  CPPUNIT_ASSERT_MESSAGE("1-Missing of end of string in byteArray - pass strlen+1 on constructor", strcmp(dataR1,data1) == 0);
  
  dataR1 = b1.getData();
  CPPUNIT_ASSERT(dataR1 != data1);
  CPPUNIT_ASSERT(b1.bytesUsed() == 100);
  CPPUNIT_ASSERT(b1.capacity() == 100);
  CPPUNIT_ASSERT(*dataR1 == *data1);  //?? it does not work properly, use strcmp
  CPPUNIT_ASSERT_MESSAGE("2-Missing end of string in byteArray - pass strlen+1 on constructor", strcmp(dataR1,data1) == 0);

//#endif
}

void TsByteArray::tcAssign()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 positions
  te::dt::ByteArray baux(data,strlen(data)+1);
  te::dt::ByteArray b = baux;
  CPPUNIT_ASSERT(b.bytesUsed() == 21);
  CPPUNIT_ASSERT(b.capacity() == 21);

  char *dataR = b.getData();
  CPPUNIT_ASSERT(data != dataR);
  CPPUNIT_ASSERT(strlen(data) == strlen(dataR)); // are differents
  CPPUNIT_ASSERT(strcmp(data,dataR) == 0 );

  //´data´ will be deleted by the destructor of baux.

// Assign after constructor with size ...
  char *data1;
  data1 = new char[21];
  strcpy(data1,"12345678901234567890"); //20 positions
  te::dt::ByteArray baux1(data1,strlen(data1)+1);
  te::dt::ByteArray b1(100);
  b1 = baux1;
  CPPUNIT_ASSERT(b1.bytesUsed() == 21);
  CPPUNIT_ASSERT(b1.capacity() == 21);

  char *dataR1 = b1.getData();
  CPPUNIT_ASSERT(data1 != dataR1);
  CPPUNIT_ASSERT(strlen(data1) == strlen(dataR1));
  CPPUNIT_ASSERT(strcmp(data1,dataR1) == 0 );

//#endif
}

void TsByteArray::tcGetData()
{
//#ifdef TE_COMPILE_ALL  
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 bytes
  te::dt::ByteArray b(data,20);
  char* dataR = b.getData();
  CPPUNIT_ASSERT( data == dataR );
  CPPUNIT_ASSERT(*data == *dataR);    
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the constructed one", strcmp(data,dataR) == 0);

  char *data1;
  data1 = new char[21];
  strcpy(data1,"12345678901234567890"); //20 bytes
  te::dt::ByteArray *b1 = new te::dt::ByteArray(data1,20);
  char *dataR1 = b1->getData();
  CPPUNIT_ASSERT( data1 == dataR1 );
  CPPUNIT_ASSERT(*data1 == *dataR1);
  CPPUNIT_ASSERT_MESSAGE("getData is not equal the constructed one", strcmp(data,dataR) == 0);

  delete b1;

//#endif
}

void TsByteArray::tcGetDataAfterInit()
{
//#ifdef TE_COMPILE_ALL  
  char *data2;
  data2 = new char[33];
  strcpy(data2,"12345678901234567890123456789012"); //32 bytes
  te::dt::ByteArray *b2 = new te::dt::ByteArray(data2, 32) ;
  char* dataR2 = b2->getData();
  size_t s = strlen(dataR2);
  CPPUNIT_ASSERT(data2 == dataR2);
  CPPUNIT_ASSERT(b2->capacity() == 32);
  CPPUNIT_ASSERT(b2->bytesUsed() == 32);

  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data, strlen+1 should be passed in ´init´.", strcmp(data2,dataR2) == 0);

  //#endif
}

void TsByteArray::tcGetDataAfterInit1()
{
//#ifdef TE_COMPILE_ALL  
  char *data3;
  data3 = new char[33];
  strcpy(data3,"12345678901234567890123456789012"); //32 bytes
  te::dt::ByteArray b3(data3, 32);
  char* dataR3 = b3.getData();
  size_t ss = strlen(dataR3);
  CPPUNIT_ASSERT(b3.capacity() == 32);
  CPPUNIT_ASSERT(b3.bytesUsed() == 32);

  CPPUNIT_ASSERT(data3 == dataR3);

  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data, strlen+1 should be passed in ´init´.", strcmp(data3,dataR3) == 0);

//#endif
}

void TsByteArray::tcSetData()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 bytes

  te::dt::ByteArray b;
  b.take(data, 20);
  char* dataR = b.getData();
  CPPUNIT_ASSERT(b.capacity() == 20);
  CPPUNIT_ASSERT(data == dataR);
  CPPUNIT_ASSERT_MESSAGE("getData is not equal expected, - pass strlen+1 in take", strcmp(data,dataR) == 0);

//#endif
}

void TsByteArray::tcSetData1()
{
//#ifdef TE_COMPILE_ALL
  char *data1;
  data1 = new char[31];
  strcpy(data1,"123456789012345678901234567890"); //30 bytes
  te::dt::ByteArray b1(35);
  b1.take(data1, 30);
  char* dataR1 = b1.getData();
  CPPUNIT_ASSERT(data1 == dataR1);
  CPPUNIT_ASSERT(b1.capacity() == 30);
  CPPUNIT_ASSERT(b1.bytesUsed() == 30);
  //CPPUNIT_ASSERT(*data1 == *dataR1);

  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data1 - pass strlen+1 as size in take", strcmp(data1,dataR1) == 0);

//#endif
}

void TsByteArray::tcSetData2()
{
//#ifdef TE_COMPILE_ALL

  char *data1;
  data1 = new char[31];
  strcpy(data1,"123456789012345678901234567890"); //30 positions
  te::dt::ByteArray b1(35);
  b1.take(data1, 30);
  char* dataR1 = b1.getData();
  CPPUNIT_ASSERT(data1 == dataR1);  
  CPPUNIT_ASSERT(b1.capacity() == 30);
  CPPUNIT_ASSERT(b1.bytesUsed() == 30);

  char *data2;
  data2 = new char[39];
  strcpy(data2,"take in b1 array with a bigger data"); //38 bytes
  b1.take(data2, 38);
  dataR1 = b1.getData();
  CPPUNIT_ASSERT(b1.bytesUsed() == 38);
  CPPUNIT_ASSERT(b1.capacity() == 38);
  CPPUNIT_ASSERT(data1 != dataR1);
  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data2 - pass strlen+1 as size in take", strcmp(data2,dataR1) == 0);
 
//#endif
}

void TsByteArray::tcSetDataOk()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[21];
  strcpy(data,"12345678901234567890"); //20 bytes

  te::dt::ByteArray b;
  b.take(data, strlen(data)+1);
  char* dataR = b.getData();
  CPPUNIT_ASSERT(b.capacity() == 21);
  CPPUNIT_ASSERT(data == dataR);

  char *data1;
  data1 = new char[31];
  strcpy(data1,"123456789012345678901234567890"); //30 bytes
  te::dt::ByteArray b1(35);
  b1.take(data1, strlen(data)+1);
  char* dataR1 = b1.getData();
  CPPUNIT_ASSERT(data == dataR);
  CPPUNIT_ASSERT(b1.capacity() == 21);
  CPPUNIT_ASSERT(b1.bytesUsed() == 21);

  char *data2;
  data2= new char[36];
  strcpy(data2,"take in b1 array with a bigger data"); //36 bytes
  b1.take(data2, strlen(data2)+1);
  dataR1 = b1.getData();
  CPPUNIT_ASSERT(b1.bytesUsed() == 36);
  CPPUNIT_ASSERT(b1.capacity() == 36);
  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data2 - pass strlen+1 as size in take", strcmp(data2,dataR1) == 0);

//#endif
}

void TsByteArray::tcSetDataAfterInit()
{
//#ifdef TE_COMPILE_ALL

// Using take (with a smaller data) after init method and passing strlen in take
  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 bytes
  te::dt::ByteArray *b = new te::dt::ByteArray(data, 32) ;

  char* dataR = b->getData();
  CPPUNIT_ASSERT(b->bytesUsed() == 32);
  CPPUNIT_ASSERT(b->capacity() == 32);
  int c = strcmp(data,dataR);

  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data - pass strlen+1 as size in init", strcmp(data,dataR) == 0);

  char *data1;
  data1 = new char[16];
  size_t a = strlen(data1);
  strcpy(data1,"data is smaller"); //15 bytes
  size_t aa = strlen(data1);

  b->take(data1,15);
  char* dataR1 = b->getData();
  size_t s  = strlen(data1);
  size_t ss = strlen(dataR1);

  CPPUNIT_ASSERT(data != dataR1);
  CPPUNIT_ASSERT(b->bytesUsed() == 15);
  CPPUNIT_ASSERT(b->capacity() == 15);
  int c1 = strcmp(data1,dataR1);
  CPPUNIT_ASSERT_MESSAGE("getData does not return the expected data - pass strlen+1 as size in take", strcmp(data1,dataR1) == 0);
 
  delete b; 

//#endif
}

void TsByteArray::tcSetDataAfterInit1()
{
//#ifdef TE_COMPILE_ALL

// Using take (with a bigger data) after init method passing strlen in take
  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 positions
  te::dt::ByteArray *b = new te::dt::ByteArray(data, 32) ;
  char* dataR = b->getData();
  CPPUNIT_ASSERT(*data == *dataR);
  CPPUNIT_ASSERT(b->bytesUsed() == 32);
  CPPUNIT_ASSERT(b->capacity() == 32);

  //what happens with "char *data" after "take" with a bigger data1?? It will be deleted by take.
  char *data1;
  data1 = new char[44];
  strcpy(data1,"using take after init with a bigger data"); //43 bytes
  b->take(data1,43);
  dataR = b->getData();
  CPPUNIT_ASSERT(*data1 == *dataR);
  CPPUNIT_ASSERT(b->bytesUsed() == 43);
  CPPUNIT_ASSERT(b->capacity() == 43);

  //delete []data1;
  delete b;

//#endif
}

void TsByteArray::tcSetDataAfterInitOk()
{
//#ifdef TE_COMPILE_ALL

// Using take (with a smaller data) after init method and passing strlen+1 in take
  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 positions
  te::dt::ByteArray *b = new te::dt::ByteArray(data, strlen(data)+1) ;
  char* dataR = b->getData();
  CPPUNIT_ASSERT(*data == *dataR);
  CPPUNIT_ASSERT(b->bytesUsed() == 33);
  CPPUNIT_ASSERT(b->capacity() == 33);
  int c = strcmp(data,dataR);

  char *data1;
  data1 = new char[16];
  size_t a = strlen(data1);

  strcpy(data1,"data is smaller");
  size_t aa = strlen(data1);

  b->take(data1,16);
  char* dataR1 = b->getData();

  size_t s  = strlen(data1);
  size_t ss = strlen(dataR1);

  int c1 = strcmp(data1,dataR1);
  CPPUNIT_ASSERT(data != dataR1);

  CPPUNIT_ASSERT(b->bytesUsed() == 16);
  CPPUNIT_ASSERT(b->capacity() == 16);
  //CPPUNIT_ASSERT(*data1 == *dataR);
  CPPUNIT_ASSERT_MESSAGE("getData is not equal take", strcmp(data1,dataR) != 0);

  delete b; 

//#endif
}


void TsByteArray::tcDataSize()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 bytes

  te::dt::ByteArray b;
  b.take(data, 32);
  char *dataR = b.getData();
  CPPUNIT_ASSERT(b.bytesUsed() == 32);
  CPPUNIT_ASSERT_MESSAGE("Use strlen+1 in take ",strlen(data) == strlen(dataR)) ;

//#endif
}

void TsByteArray::tcDataSizeOk()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 bytes

  te::dt::ByteArray b;
  b.take(data, 33);
  char *dataR = b.getData();

  CPPUNIT_ASSERT(b.bytesUsed() == 33);
  CPPUNIT_ASSERT(strlen(data) == strlen(dataR)) ;

//#endif
}

void TsByteArray::tcClear()
{
//#ifdef TE_COMPILE_ALL
  char *data;
  data = new char[33];
  strcpy(data,"12345678901234567890123456789012"); //32 positions

  te::dt::ByteArray b;
  b.take(data, 33);
  CPPUNIT_ASSERT(b.getData() != 0);
  CPPUNIT_ASSERT(b.capacity() == 33);
  CPPUNIT_ASSERT(b.bytesUsed()== 33);

  b.clear();
  CPPUNIT_ASSERT(b.getData()== 0);
  CPPUNIT_ASSERT(b.capacity() == 0);
  CPPUNIT_ASSERT(b.bytesUsed()== 0);

//#endif
}

void TsByteArray::tcGetHexString()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_MESSAGE("Not implemented yet! ", 1==0);

//#endif
}
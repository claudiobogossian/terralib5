%{
#include "terralib/geometry/Curve.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Point.h"
#include "terralib/geometry/LinearRing.h"
#include "terralib/geometry/MultiLineString.h"
#include "terralib/geometry/MultiPolygon.h"
#include "terralib/geometry/Polygon.h"
%}

/*
 * Mapping images C++ char* -> Java byte array
 */

%typemap(jni) void* "jbyteArray";
%typemap(jtype) void* "byte[]";
%typemap(jstype) void* "byte[]";
%typemap(javaout) void* {
	return $jnicall;
};
%typemap(out) (void*) (char* img, int size) {
	size = *arg6;
	void* buff = result;
	img = (char*)buff;
	$result = jenv->NewByteArray(size);
	(*jenv).SetByteArrayRegion($result, 0, size, (jbyte *) img);
	
	delete[] buff;
}

%typemap(jni) char* "jbyteArray";
%typemap(jtype) char* "byte[]";
%typemap(jstype) char* "byte[]";
%typemap(javaout) char* {
	return $jnicall;
};
%typemap(out) (char*) (char* img, int size) {
	size = *arg5;
	img = result;
	$result = jenv->NewByteArray(size);
	(*jenv).SetByteArrayRegion($result, 0, size, (jbyte *) img);
	
	delete[] img;
}


/*
 * Mapping C++ te::gm::Envelope -> Java java.awt.geom.Rectangle2D.Double object
 */

%typemap(jni) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & "jobject"
%typemap(jtype) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & "java.awt.geom.Rectangle2D.Double"
%typemap(jstype) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & "java.awt.geom.Rectangle2D.Double"
%typemap(javain) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & "te::gm::Envelope.getCPtr($javainput)"
%typemap(javain) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & "$javainput"
%typemap(javaout) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & {
    return new te::gm::Envelope($jnicall, $owner);
  }
%typemap(javaout) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope &{
	return $jnicall;
};


%typemap(in) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & {
  jclass clazz = JCALL1(GetObjectClass, jenv, $input);
  jmethodID getX = JCALL3(GetMethodID, jenv, clazz, "getX", "()D");
  jmethodID getY = JCALL3(GetMethodID, jenv, clazz, "getY", "()D");
  jmethodID getW = JCALL3(GetMethodID, jenv, clazz, "getWidth", "()D");
  jmethodID getH = JCALL3(GetMethodID, jenv, clazz, "getHeight", "()D");

  double x = JCALL2(CallDoubleMethod, jenv, $input, getX),
    y = JCALL2(CallDoubleMethod, jenv, $input, getY),
	w = JCALL2(CallDoubleMethod, jenv, $input, getW),
	h = JCALL2(CallDoubleMethod, jenv, $input, getH);

 $1 = &te::gm::Envelope(x, y, x+w, y+h);
}

%typemap(out) te::gm::Envelope, const te::gm::Envelope &, te::gm::Envelope & {
  double x = $1->getLowerLeftX(), 
	  y = $1->getLowerLeftY(),
	  w = $1->getWidth(),
	  h = $1->getHeight();

  jclass clazz = JCALL1(FindClass, jenv, "java/awt/geom/Rectangle2D$Double"); 
  jmethodID metId = JCALL3(GetMethodID, jenv, clazz, "<init>", "(DDDD)V");

  $result = JCALL6(NewObject, jenv, clazz, metId, x, y, w, h);
}

%typemap(argout) (te::gm::Envelope & bbox) {
	jclass clazz = jenv->GetObjectClass($input);
	jmethodID setR = jenv->GetMethodID(clazz, "setRect", "(DDDD)V");

	double x = $1->getLowerLeftX(), 
		y = $1->getLowerLeftY(), 
		w = $1->getWidth(), 
		h = $1->getHeight();

	jenv->CallVoidMethod($input, setR, x, y, w, h);
}


%typemap(jni) te::gm::Envelope* "jobject";
%typemap(jtype) te::gm::Envelope* "java.awt.geom.Rectangle2D.Double"
%typemap(jstype) te::gm::Envelope* "java.awt.geom.Rectangle2D.Double"
%typemap(javaout) te::gm::Envelope* {
    return new te::gm::Envelope($jnicall, $owner);
  }
%typemap(javaout) te::gm::Envelope* {
	return $jnicall;
};

%typemap(out) (te::gm::Envelope*) {
  double x = $1->getLowerLeftX(), 
	  y = $1->getLowerLeftY(),
	  w = $1->getWidth(),
	  h = $1->getHeight();

  jclass clazz = JCALL1(FindClass, jenv, "java/awt/geom/Rectangle2D$Double"); 
  jmethodID metId = JCALL3(GetMethodID, jenv, clazz, "<init>", "(DDDD)V");

  $result = JCALL6(NewObject, jenv, clazz, metId, x, y, w, h);
}

/*
 * Mapping C++ QColor -> Java java.awt.Color object
 */
%typemap(jni) QColor, const QColor & "jobject"
%typemap(jtype) QColor, const QColor & "java.awt.Color"
%typemap(jstype) QColor, const QColor & "java.awt.Color"
%typemap(javain) QColor, const QColor & "QColor.getCPtr($javainput)"
%typemap(javain) QColor, const QColor & "$javainput"
%typemap(javaout) QColor, const QColor &  {
    return new QColor($jnicall, $owner);
  }
%typemap(javaout) QColor, const QColor & {
	return $jnicall;
};


%typemap(in) QColor, const QColor & {
  jclass clazz = JCALL1(GetObjectClass, jenv, $input);

  jmethodID red = JCALL3(GetMethodID, jenv, clazz, "getRed", "()I");
  jmethodID green = JCALL3(GetMethodID, jenv, clazz, "getGreen", "()I");
  jmethodID blue = JCALL3(GetMethodID, jenv, clazz, "getBlue", "()I");

  int r = JCALL2(CallIntMethod, jenv, $input, red),
    g = JCALL2(CallIntMethod, jenv, $input, green),
	b = JCALL2(CallIntMethod, jenv, $input, blue);

 $1 = &QColor(r, g, b);
}

%typemap(out) QColor, const QColor & {
  int r = $1.red(), 
	  g = $1.green(),
	  b = $1.blue();

  jobject pt;
  jclass clazz = JCALL1(FindClass, jenv, "java/awt/Color"); 
  jmethodID metId = JCALL3(GetMethodID, jenv, clazz, "<init>", "(III)V");

  $result = JCALL5(NewObject, jenv, clazz, metId, r, g, b);
}

/*
 * Mapping images C++ Coord2D -> Java java.awt.geom.Point2D.Double object
 */

%typemap(jni) te::gm::Coord2D, const te::gm::Coord2D & "jobject"
%typemap(jtype) te::gm::Coord2D, const te::gm::Coord2D &  "java.awt.geom.Point2D.Double"
%typemap(jstype) te::gm::Coord2D, const te::gm::Coord2D &  "java.awt.geom.Point2D.Double"
%typemap(javain) te::gm::Coord2D, const te::gm::Coord2D & "te::gm::Coord2D.getCPtr($javainput)"
%typemap(javain) te::gm::Coord2D, const te::gm::Coord2D & "$javainput"
%typemap(javaout) te::gm::Coord2D, const te::gm::Coord2D &  {
    return new te::gm::Coord2D($jnicall, $owner);
  }
%typemap(javaout) te::gm::Coord2D, const te::gm::Coord2D & {
	return $jnicall;
};


%typemap(in) te::gm::Coord2D, const te::gm::Coord2D & {
  jclass clazz = JCALL1(GetObjectClass, jenv, $input);
  jmethodID getX = JCALL3(GetMethodID, jenv, clazz, "getX", "()D");
  jmethodID getY = JCALL3(GetMethodID, jenv, clazz, "getY", "()D");

  double x = JCALL2(CallDoubleMethod, jenv, $input, getX),
    y = JCALL2(CallDoubleMethod, jenv, $input, getY);

 $1 = &te::gm::Coord2D(x, y);
}

%typemap(out) te::gm::Coord2D, const te::gm::Coord2D & {
  double x = $1.x, 
      y = $1.y;

  jclass clazz = JCALL1(FindClass, jenv, "java/awt/geom/Point2D$Double"); 
  jmethodID metId = JCALL3(GetMethodID, jenv, clazz, "<init>", "(DD)V");

  $result = JCALL4(NewObject, jenv, clazz, metId, x, y);
}

%typemap(jni) te::gm::Coord2D * "jobject";
%typemap(jtype) te::gm::Coord2D * "java.awt.geom.Point2D.Double"
%typemap(jstype) te::gm::Coord2D * "java.awt.geom.Point2D.Double"
%typemap(javaout) te::gm::Coord2D * {
    return new te::gm::Coord2D($jnicall, $owner);
  }
%typemap(javaout) te::gm::Coord2D * {
	return $jnicall;
};

%typemap(out) (te::gm::Coord2D *) {
  double x = $1->x, 
	  y = $1->y;

  jclass clazz = JCALL1(FindClass, jenv, "java/awt/geom/Point2D$Double"); 
  jmethodID metId = JCALL3(GetMethodID, jenv, clazz, "<init>", "(DD)V");

  $result = JCALL4(NewObject, jenv, clazz, metId, x, y);
}

/*
 * Mapping geometries C++ -> Java java.awt.geom.Shape object
 */
%typemap(jni) const te::gm::Geometry &, te::gm::Geometry * "jobject"
%typemap(jtype) const te::gm::Geometry &, te::gm::Geometry *  "java.awt.geom.GeneralPath"
%typemap(jstype) const te::gm::Geometry &, te::gm::Geometry *  "java.awt.geom.GeneralPath"
%typemap(javain) const te::gm::Geometry &, te::gm::Geometry * "te::gm::Geometry.getCPtr($javainput)"
%typemap(javain) const te::gm::Geometry &, te::gm::Geometry * "$javainput"
%typemap(javaout) const te::gm::Geometry &, te::gm::Geometry *  {
    return new te::gm::Geometry($jnicall, $owner);
  }
%typemap(javaout) const te::gm::Geometry &, te::gm::Geometry * {
	return $jnicall;
};

%{
te::gm::Polygon* getPolygon(te::gm::MultiPolygon* multiPol, te::gm::LinearRing* ring)
{
	size_t nGeom = multiPol->getNumGeometries();

	if(nGeom > 0)
		for(size_t i=0; i<nGeom; i++)
		{
			te::gm::Polygon* p = (te::gm::Polygon*)multiPol->getGeometryN(i);

			if(p->contains(ring))
				return p;
		}

	return 0;
}
%}

%fragment("AsGeometry", "header") {
	te::gm::Geometry* AsGeometry(jobject path, JNIEnv* jenv) {
		te::gm::Geometry* res;

		  jclass pathClazz = JCALL1(GetObjectClass, jenv, path);
		  jmethodID getIterator = JCALL3(GetMethodID, jenv, pathClazz, "getPathIterator", "(Ljava/awt/geom/AffineTransform;)Ljava/awt/geom/PathIterator;");

		  jclass iteClazz = JCALL1(FindClass, jenv, "java/awt/geom/PathIterator");
		  jmethodID currSeg = JCALL3(GetMethodID, jenv, iteClazz, "currentSegment", "([D)I");
		  jmethodID isDone = JCALL3(GetMethodID, jenv, iteClazz, "isDone", "()Z");
		  jmethodID next = JCALL3(GetMethodID, jenv, iteClazz, "next", "()V");
		  
		  jdoubleArray cdr = JCALL1(NewDoubleArray, jenv, 6);
		  
		  // Discovering the type of Geometry
		  jobject iter = JCALL3(CallObjectMethod, jenv, path, getIterator, 0);
		  
		  int nodeCount = 0;
		  bool done = false;
		  
		  while(!done)
		  {
			nodeCount++;
			int type = JCALL3(CallIntMethod, jenv, iter, currSeg, cdr);
			
			if(type == 4)
				break;
			
			JCALL2(CallVoidMethod, jenv, iter, next);
			
			done = JCALL2(CallBooleanMethod, jenv, iter, isDone);
		  }
		  
		  // Polygon will be treated as MultiPolygon
		  if(!done)
		  {
			std::vector<te::gm::Coord2D> coords;
			te::gm::MultiPolygon* multiPol = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType);
			
			done = false;
			
			jobject iter = JCALL3(CallObjectMethod, jenv, path, getIterator, 0);
			
			while(!done)
			{
				int type = JCALL3(CallIntMethod, jenv, iter, currSeg, cdr);
				
				// Closing a ring
				if(type == 4)
				{
					te::gm::LinearRing* ring = new te::gm::LinearRing(coords.size(), te::gm::LineStringType);
					
					for(size_t i=0; i<coords.size(); i++)
						ring->setPoint(i, coords[i].x, coords[i].y);
						
					coords.clear();
					
					te::gm::Polygon* poly = getPolygon(multiPol, ring);
					
					if(poly != 0)
						poly->add(ring);
					else
					{
						poly = new te::gm::Polygon(0, te::gm::PolygonType);
						poly->add(ring);
						multiPol->add(poly);
					}
				}
				else
				{
					jsize auxS = JCALL1(GetArrayLength, jenv, cdr);
					jdouble* aux = JCALL2(GetDoubleArrayElements, jenv, cdr, JNI_FALSE);
					
					coords.push_back(te::gm::Coord2D(aux[0], aux[1]));
					
					JCALL3(ReleaseDoubleArrayElements, jenv, cdr, aux, JNI_ABORT);
				}
				
				done = JCALL2(CallBooleanMethod, jenv, iter, isDone);
				
				if(!done)
					JCALL2(CallVoidMethod, jenv, iter, next);
			}
			
			res = multiPol;
		  }
		  else // Line or Point
		  {
			std::vector<te::gm::Coord2D> coords;
			te::gm::MultiLineString* multiLine = new te::gm::MultiLineString(0, te::gm::MultiLineStringType);
			
			done = false;
			
			jobject iter = JCALL3(CallObjectMethod, jenv, path, getIterator, 0);
			
			while(!done)
			{
				int type = JCALL3(CallIntMethod, jenv, iter, currSeg, cdr);
				
				// Closing new line
				if(type == 0 && coords.size() > 0)
				{
					te::gm::LineString* ring = new te::gm::LineString(coords.size(), te::gm::LineStringType);
					
					for(size_t i=0; i<coords.size(); i++)
						ring->setPoint(i, coords[i].x, coords[i].y);
						
					coords.clear();
					
					multiLine->add(ring);
				}
				else
				{
					jsize auxS = JCALL1(GetArrayLength, jenv, cdr);
					jdouble* aux = JCALL2(GetDoubleArrayElements, jenv, cdr, JNI_FALSE);
					
					coords.push_back(te::gm::Coord2D(aux[0], aux[1]));
					
					JCALL3(ReleaseDoubleArrayElements, jenv, cdr, aux, JNI_ABORT);
				}
				
				done = JCALL2(CallBooleanMethod, jenv, iter, isDone);
				
				if(!done)
					JCALL2(CallVoidMethod, jenv, iter, next);
			}
			
			if(coords.size() > 0)
			{
				te::gm::LinearRing* ring = new te::gm::LinearRing(coords.size(), te::gm::LineStringType);
				
				for(size_t i=0; i<coords.size(); i++)
					ring->setPoint(i, coords[i].x, coords[i].y);
				
				multiLine->add(ring);
			}
			
			res = multiLine;
		  }
		
		return res;
	}
}

%typemap(in, fragment="AsGeometry") te::gm::Geometry * {
	$1 = AsGeometry($input, jenv);
}

%typemap(jni) std::vector<te::gm::Geometry*> "jobject"
%typemap(jtype) std::vector<te::gm::Geometry*>  "java.util.List<java.awt.geom.GeneralPath>"
%typemap(jstype) std::vector<te::gm::Geometry*>  "java.util.List<java.awt.geom.GeneralPath>"
%typemap(javain) std::vector<te::gm::Geometry*> "std::vector<te::gm::Geometry*>.getCPtr($javainput)"
%typemap(javain) std::vector<te::gm::Geometry*> "$javainput"

%typemap(javaout) std::vector<te::gm::Geometry*> {
	return $jnicall;
};

%typemap(in, fragment="AsGeometry") std::vector<te::gm::Geometry*> {
	std::vector<te::gm::Geometry*> input;

	jclass listClazz = JCALL1(GetObjectClass, jenv, $input);

	jmethodID size = JCALL3(GetMethodID, jenv, listClazz, "size", "()I");
	jmethodID getI = JCALL3(GetMethodID, jenv, listClazz, "get", "(I)Ljava/lang/Object;");

	int s = JCALL2(CallIntMethod, jenv, $input, size);
	
	for(int i=0; i<s; i++) 
	{
		jobject path = JCALL3(CallObjectMethod, jenv, $input, getI, i);
		input.push_back(AsGeometry(path, jenv));
	}
	
	$1 = input;
}

%typemap(jni) te::gm::Geometry * "jobject";
%typemap(jtype) te::gm::Geometry * "java.awt.geom.GeneralPath"
%typemap(jstype) te::gm::Geometry * "java.awt.geom.GeneralPath"
%typemap(javaout) te::gm::Geometry * {
    return new te::gm::Geometry($jnicall, $owner);
  }
%typemap(javaout) te::gm::Geometry * {
	return $jnicall;
};

%typemap(out) (te::gm::Geometry *) {
	jclass pathClazz = JCALL1(FindClass, jenv, "java/awt/geom/GeneralPath"); 
	jmethodID constId = JCALL3(GetMethodID, jenv, pathClazz, "<init>", "()V");

	jmethodID moveToId = JCALL3(GetMethodID, jenv, pathClazz, "moveTo", "(DD)V");
	jmethodID lineToId = JCALL3(GetMethodID, jenv, pathClazz, "lineTo", "(DD)V");
	jmethodID closePathId = JCALL3(GetMethodID, jenv, pathClazz, "closePath", "()V");
	jmethodID appendId = JCALL3(GetMethodID, jenv, pathClazz, "append", "(Ljava/awt/geom/PathIterator;Z)V");

    std::string gName = result->getGeometryType();
    if (gName == "Polygon")
    {            
		te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(result);
	}
	else if(gName == "MultiPolygon")
	{
		te::gm::MultiPolygon* multipol = static_cast<te::gm::MultiPolygon*>(result);
		
		const std::size_t size = multipol->getNumGeometries();
		$result = JCALL2(NewObject, jenv, pathClazz, constId);
		
		for(size_t i=0; i<size; i++)
		{
			te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(multipol->getGeometryN(i));
        
			if(pol->getNumRings() > 0)
			{
				// External ring
				te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(pol->getRingN(0));
				size_t nCoords = ring->getNPoints();
				te::gm::Point* pt = ring->getPointN(0);

				JCALL4(CallVoidMethod, jenv, $result, moveToId, pt->getX(), pt->getY());
				
				for(size_t j=1; j<nCoords; j++)
				{
					pt = ring->getPointN(j);
					JCALL4(CallVoidMethod, jenv, $result, lineToId, pt->getX(), pt->getY());
				}
			  
				JCALL2(CallVoidMethod, jenv, $result, closePathId);
			
				size_t holes = pol->getNumRings();

				// Holes
				for(size_t j=1; j<holes; j++)
				{
					jobject hole = JCALL2(NewObject, jenv, pathClazz, constId);
					
					te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(pol->getRingN(j));
					size_t nCoords = ring->getNPoints();
					pt = ring->getPointN(0);

					JCALL4(CallVoidMethod, jenv, hole, moveToId, pt->getX(), pt->getY());
				
					for(size_t k=1; k<nCoords; k++)
					{
						te::gm::Point* pt = ring->getPointN(k);
						JCALL4(CallVoidMethod, jenv, hole, lineToId, pt->getX(), pt->getY());
					}

					JCALL2(CallVoidMethod, jenv, $result, closePathId);
					JCALL4(CallVoidMethod, jenv, $result, appendId, hole, false);
				}
			}
		}
	}
	else if(gName == "Line")
	{
	}
	else if(gName == "MultiLineString")
	{
		te::gm::MultiLineString* multiLine = static_cast<te::gm::MultiLineString*>(result);
		
		const std::size_t nGeoms = multiLine->getNumGeometries();
		$result = JCALL2(NewObject, jenv, pathClazz, constId);
		
		for(size_t i=0; i<nGeoms; i++)
		{
			te::gm::LineString* line = static_cast<te::gm::LineString*>(multiLine->getGeometryN(i));
			size_t nCoords = line->getNPoints();
			
			// First point
			te::gm::Point* pt = line->getPointN(0);
			
			JCALL4(CallVoidMethod, jenv, $result, moveToId, pt->getX(), pt->getY());
			
			for(size_t j=1; j<nCoords; j++)
			{
				te::gm::Point* pt = line->getPointN(j);
				JCALL4(CallVoidMethod, jenv, $result, lineToId, pt->getX(), pt->getY());
			}
		}
	}
}

/*
 * Mapping C++ QTransform -> Java java.awt.geom.AffineTransform
 */
%typemap(jni) QTransform "jobject"
%typemap(jtype) QTransform  "java.awt.geom.AffineTransform"
%typemap(jstype) QTransform "java.awt.geom.AffineTransform"
%typemap(javain) QTransform "QTransform.getCPtr($javainput)"
%typemap(javain) QTransform "$javainput"
%typemap(javaout) QTransform  {
    return new QTransform($jnicall, $owner);
  }
%typemap(javaout) QTransform {
	return $jnicall;
};

%typemap(out) (QTransform) {
  jclass transClazz = JCALL1(FindClass, jenv, "java/awt/geom/AffineTransform"); 
  jmethodID constId = JCALL3(GetMethodID, jenv, transClazz, "<init>", "()V");
  jmethodID transId = JCALL3(GetMethodID, jenv, transClazz, "translate", "(DD)V");
  jmethodID scaleId = JCALL3(GetMethodID, jenv, transClazz, "scale", "(DD)V");
  
  $result = JCALL2(NewObject, jenv, transClazz, constId);
  
  JCALL4(CallVoidMethod, jenv, $result, transId, $1.m31(), $1.m32());
  JCALL4(CallVoidMethod, jenv, $result, scaleId, $1.m11(), $1.m22());
}

/*
 * Mapping C++ te::common::Exception -> Java java.io.Exception object
 */
%typemap(throws, throws="java.io.IOException") te::common::Exception {
  jclass excep = jenv->FindClass("java/io/IOException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null; 
}


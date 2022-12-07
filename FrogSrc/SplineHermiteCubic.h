#ifndef __FROG__SPLINEHERMITECUBIC_H__
#define __FROG__SPLINEHERMITECUBIC_H__

#include "FrogMemory.h"
#include "Table.h"
#include "Point2.h"
#include "Point3.h"
#include "JSONValue.h"
#include "Debug.h"

namespace Webfoot {

/// Value for a given control point of a SplineHermiteCubic.
const char* const SPLINE_HERMITE_CUBIC_VALUE_KEY = "Value";
/// Tangent in units/second for a given control point of a SplineHermiteCubic.
const char* const SPLINE_HERMITE_CUBIC_TANGENT_KEY = "Tangent";
/// Time in milliseconds for a given control point of a SplineHermiteCubic.
const char* const SPLINE_HERMITE_CUBIC_TIME_KEY = "Time";

//==============================================================================

/// Specifications for a single point for SplineHermiteCubic.
template<typename T>
struct SplineHermiteCubicControlPoint
{
   T value;
   /// Tangent is in units of value per unit of second, not units per millisecond.
   T tangent;
   /// The time, in milliseconds, at which the spline should be at this value.
   unsigned int time;
};

//==============================================================================

/// The spline passes through each point's 'value' at the specified 'time'.  Its
/// velocity at that point is specified by 'tangent'.  All points specified in
/// the spline should be provided in-order by time.  The first point should
/// always have a time of 0.
template<typename T>
class SplineHermiteCubic
{
public:
   void Init(HeapID heapID = HEAP_DEFAULT);
   void Deinit();

   /// Reset the collection of control points.
   void PointsClear() { controlPoints.Clear(); }
   
   /// Add the given points to the spline.  All points are assumed to be
   /// in-order by time.
   void PointsAdd(SplineHermiteCubicControlPoint<T>* _controlPoints, int controlPointCount);
   /// Add the given points to the spline.  All points are assumed to be
   /// in-order by time.
   void PointsAdd(JSONValue* pointsSpecifications);

   /// Return the value for the given time in milliseconds.
   T ValueGet(unsigned int time) { return ValueGet((float)time); }
   /// Return the value for the given time in milliseconds.
   T ValueGet(float time);
   
   /// Return the duration of the spline in milliseconds.
   unsigned int DurationGet();
   
protected:
   Table<SplineHermiteCubicControlPoint<T> > controlPoints;
};

//------------------------------------------------------------------------------

template<typename T>
void SplineHermiteCubic<T>::Init(HeapID heapID)
{
   controlPoints.Init(theAllocatorHeaps[heapID]);
}

//------------------------------------------------------------------------------

template<typename T>
void SplineHermiteCubic<T>::Deinit()
{
   controlPoints.Deinit();
}

//------------------------------------------------------------------------------

template<typename T>
void SplineHermiteCubic<T>::PointsAdd(SplineHermiteCubicControlPoint<T>* _controlPoints, int controlPointCount)
{
   controlPoints.Reserve(controlPoints.SizeGet() + controlPointCount);
   controlPoints.AddCount(_controlPoints, controlPointCount);
}

//------------------------------------------------------------------------------

template<typename T>
void SplineHermiteCubic<T>::PointsAdd(JSONValue* pointsSpecifications)
{
   if(!pointsSpecifications->ArrayCheck())
   {
      WarningPrintf("SplineHermiteCubic::PointsAdd -- points must be listed in a JSON array.\n");
      return;
   }
   // Iterate over the list of point specifications.
   int pointCount = pointsSpecifications->SizeGet();
   controlPoints.Reserve(controlPoints.SizeGet() + pointCount);
   for(int pointIndex = 0; pointIndex < pointCount; pointIndex++)
   {
      // Extract the data for a given point.
      JSONValue* pointSpecifications = pointsSpecifications->Get(pointIndex);
      if(!pointSpecifications->ObjectCheck())
      {
         WarningPrintf("SplineHermiteCubic::PointsAdd -- Individual points must be given as JSON objects.\n");
         continue;
      }
      SplineHermiteCubicControlPoint<T> point;
      point.value = pointSpecifications->Get(SPLINE_HERMITE_CUBIC_VALUE_KEY);
      point.tangent = pointSpecifications->Get(SPLINE_HERMITE_CUBIC_TANGENT_KEY);
      point.time = (unsigned int)(int)pointSpecifications->Get(SPLINE_HERMITE_CUBIC_TIME_KEY);
      controlPoints.AddBack(point);
   }
}

//------------------------------------------------------------------------------

template<typename T>
T SplineHermiteCubic<T>::ValueGet(float time)
{
   int controlPointCount = controlPoints.SizeGet();
   assert(controlPointCount >= 2);

   // Apply bounds
   if(time <= (float)controlPoints[0].time)
      return controlPoints[0].value;
   if(time >= (float)controlPoints[controlPointCount-1].time)
      return controlPoints[controlPointCount-1].value;
   
   // Find the index of the first point in the spline where the associated time
   // is greater than the requested time.  Use a binary search.
   SplineHermiteCubicControlPoint<T>* previousPoint = NULL;
   SplineHermiteCubicControlPoint<T>* nextPoint = NULL;
   int minIndex = 1;
   int maxIndex = controlPointCount - 1;
   while(minIndex <= maxIndex)
   {
      int pointIndex = minIndex + ((maxIndex - minIndex) / 2);
      float pointTime = (float)controlPoints[pointIndex].time;

      if(time >= pointTime)
      {
         minIndex = pointIndex + 1;
      }
      else
      {
         if((float)controlPoints[pointIndex-1].time <= time)
         {
            nextPoint = &(controlPoints[pointIndex]);
            previousPoint = &(controlPoints[pointIndex-1]);
            break;
         }
         else
         {
            maxIndex = pointIndex - 1;
         }
      }
   }
   //for(int pointIndex = 1; pointIndex < controlPointCount; pointIndex++)
   //{
   //   if((float)controlPoints[pointIndex].time > time)
   //   {
   //      nextPoint = &(controlPoints[pointIndex]);
   //      previousPoint = &(controlPoints[pointIndex-1]);
   //      break;
   //   }      
   //}
   
   assert(previousPoint);
   assert(nextPoint);
   
   // Determine the 't' value for interpolation.
   float intervalDuration = (float)(nextPoint->time - previousPoint->time);
   // Avoid division by zero.
   if(intervalDuration == 0.0f)
      return nextPoint->value; 
   float t = (time - (float)previousPoint->time) / intervalDuration;
   // Apply a coefficient to tangents to compensate for durations of segments
   // not all being 1.0.
   float tangentCoefficient = intervalDuration / 1000.0f;
   
   // Perform the interpolation.
   float tSquared = t * t;
   float tCubed = tSquared * t;
   return (previousPoint->value * (2.0f*tCubed - 3.0f*tSquared + 1)) +
      (previousPoint->tangent * ((tCubed - 2.0f*tSquared + t) * tangentCoefficient)) + 
      (nextPoint->value * (-2.0f*tCubed + 3*tSquared)) + 
      (nextPoint->tangent * ((tCubed - tSquared) * tangentCoefficient));
}

//------------------------------------------------------------------------------

template<typename T>
unsigned int SplineHermiteCubic<T>::DurationGet()
{
   int controlPointCount = controlPoints.SizeGet(); 
   if(controlPointCount)
      return controlPoints[controlPointCount-1].time;
   else
      return 0;
}

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__SPLINEHERMITECUBIC_H__

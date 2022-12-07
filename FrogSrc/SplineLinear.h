#ifndef __FROG__SPLINELINEAR_H__
#define __FROG__SPLINELINEAR_H__

#include "FrogMemory.h"
#include "Table.h"
#include "Point2.h"
#include "Point3.h"
#include "JSONValue.h"
#include "BSONIterator.h"
#include "Debug.h"
#include "Progression.h"
#include "ProgressionManager.h"

namespace Webfoot {

/// Value for a given control point of a SplineLinear.
const char* const SPLINE_LINEAR_VALUE_KEY = "Value";
/// Time in milliseconds for a given control point of a SplineLinear.
const char* const SPLINE_LINEAR_TIME_KEY = "Time";
/// Progression to use when approaching a given control point of a SplineLinear.
const char* const SPLINE_LINEAR_PROGRESSION_KEY = "Progression";

//==============================================================================

/// Specifications for a single point for SplineLinear.
template<typename T>
struct SplineLinearControlPoint
{
   T value;
   /// The time, in milliseconds, at which the spline should be at this value.
   unsigned int time;
   /// Progression to use when approaching this control point.
   Progression* progression;
};

//==============================================================================

/// The spline passes through each point's 'value' at the specified 'time'.  Its
/// velocity at that point is specified by 'tangent'.  All points specified in
/// the spline should be provided in-order by time.  The first point should
/// always have a time of 0.
template<typename T>
class SplineLinear
{
public:
   void Init(HeapID heapID = HEAP_DEFAULT);
   void Deinit();

   /// Reset the collection of control points.
   void PointsClear() { controlPoints.Clear(); }
   
   /// Add the given points to the spline.  All points are assumed to be
   /// in-order by time.
   void PointsAdd(SplineLinearControlPoint<T>* _controlPoints, int controlPointCount);
   /// Add the given points to the spline.  All points are assumed to be
   /// in-order by time.
   void PointsAdd(JSONValue* pointsSpecifications);
   /// Add the given points to the spline.  All points are assumed to be
   /// in-order by time.
   void PointsAdd(BSONIterator pointsSpecifications);

   /// Return the value for the given time in milliseconds.
   T ValueGet(unsigned int time) { return ValueGet((float)time); }
   /// Return the value for the given time in milliseconds.
   T ValueGet(float time);

   /// Return the duration of the spline in milliseconds.
   unsigned int DurationGet();

protected:
   Table<SplineLinearControlPoint<T> > controlPoints;
};

//------------------------------------------------------------------------------

template<typename T>
void SplineLinear<T>::Init(HeapID heapID)
{
   controlPoints.Init(theAllocatorHeaps[heapID]);
}

//------------------------------------------------------------------------------

template<typename T>
void SplineLinear<T>::Deinit()
{
   controlPoints.Deinit();
}

//------------------------------------------------------------------------------

template<typename T>
void SplineLinear<T>::PointsAdd(SplineLinearControlPoint<T>* _controlPoints, int controlPointCount)
{
   controlPoints.Reserve(controlPoints.SizeGet() + controlPointCount);
   controlPoints.AddCount(_controlPoints, controlPointCount);
}

//------------------------------------------------------------------------------

template<typename T>
void SplineLinear<T>::PointsAdd(JSONValue* pointsSpecifications)
{
   if(!pointsSpecifications->ArrayCheck())
   {
      WarningPrintf("SplineLinear::PointsAdd -- points must be listed in a JSON array.\n");
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
         WarningPrintf("SplineLinear::PointsAdd -- Individual points must be given as JSON objects.\n");
         continue;
      }
      SplineLinearControlPoint<T> point;
      point.value = pointSpecifications->Get(SPLINE_LINEAR_VALUE_KEY);
      point.time = (unsigned int)(int)pointSpecifications->Get(SPLINE_LINEAR_TIME_KEY);
      const char* progressionName = pointSpecifications->Get(SPLINE_LINEAR_PROGRESSION_KEY);
      if(progressionName)
         point.progression = theProgressions->Get(progressionName);
      else
         point.progression = theProgressionLinear;
      controlPoints.AddBack(point);
   }
}

//------------------------------------------------------------------------------

template<typename T>
void SplineLinear<T>::PointsAdd(BSONIterator pointsSpecifications)
{
   if(!pointsSpecifications.ArrayCheck())
   {
      WarningPrintf("SplineLinear::PointsAdd -- points must be listed in a BSON array.\n");
      return;
   }
   // Iterate over the list of point specifications.
   int pointCount = pointsSpecifications.SizeGet();
   controlPoints.Reserve(controlPoints.SizeGet() + pointCount);
   for(BSONIterator pointSpecifications = pointsSpecifications.ArrayBegin(); pointSpecifications.WithinCheck(); pointSpecifications.Next())
   {
      // Extract the data for a given point.
      if(!pointSpecifications.ObjectCheck())
      {
         WarningPrintf("SplineLinear::PointsAdd -- Individual points must be given as BSON objects.\n");
         continue;
      }
      SplineLinearControlPoint<T> point;
      point.value = pointSpecifications.Get(SPLINE_LINEAR_VALUE_KEY);
      point.time = (unsigned int)(int)pointSpecifications.Get(SPLINE_LINEAR_TIME_KEY);
      const char* progressionName = pointSpecifications.Get(SPLINE_LINEAR_PROGRESSION_KEY);
      if(progressionName)
         point.progression = theProgressions->Get(progressionName);
      else
         point.progression = theProgressionLinear;
      controlPoints.AddBack(point);
   }
}

//------------------------------------------------------------------------------

template<typename T>
T SplineLinear<T>::ValueGet(float time)
{
   int controlPointCount = controlPoints.SizeGet();
   assert(controlPointCount >= 1);

   // Apply bounds
   if(time <= (float)controlPoints[0].time)
      return controlPoints[0].value;
   if(time >= (float)controlPoints[controlPointCount-1].time)
      return controlPoints[controlPointCount-1].value;

   // If there is only one control point, return its value.
   if(controlPointCount == 1)
      return controlPoints[0].value;
   
   // Find the index of the first point in the spline where the associated time
   // is greater than the requested time.  Use a binary search.
   SplineLinearControlPoint<T>* previousPoint = NULL;
   SplineLinearControlPoint<T>* nextPoint = NULL;
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
   
   // Determine the 't' value for interpolation before applying progression.
   unsigned int intervalDuration = nextPoint->time - previousPoint->time;
   // Avoid division by zero.
   if(intervalDuration == 0)
      return nextPoint->value;
   float t = (time - (float)previousPoint->time) / (float)intervalDuration;

   // Apply the progression.
   if(nextPoint->progression)
      t = nextPoint->progression->ValueGet(t);
   
   // Perform the interpolation.
   return Lerp(previousPoint->value, nextPoint->value, t);
}

//------------------------------------------------------------------------------

template<typename T>
unsigned int SplineLinear<T>::DurationGet()
{
   int controlPointCount = controlPoints.SizeGet(); 
   if(controlPointCount)
      return controlPoints[controlPointCount-1].time;
   else
      return 0;
}

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__SPLINELINEAR_H__

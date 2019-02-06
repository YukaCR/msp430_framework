///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#if 0
#error THIS HEADER IS A GENERATOR. DO NOT INCLUDE.
#endif

//***************************************************************************
// THIS FILE HAS BEEN AUTO GENERATED. DO NOT EDIT THIS FILE.
//***************************************************************************

//***************************************************************************
// To generate to header file, run this at the command line.
// Note: You will need Python and COG installed.
//
// python -m cogapp -d -e -olargest.h -DNTypes=<n> largest_generator.h
// Where <n> is the number of types to support.
//
// e.g.
// To generate handlers for up to 16 types...
// python -m cogapp -d -e -olargest.h -DNTypes=16 largest_generator.h
//
// See generate.bat
//***************************************************************************

#ifndef ETL_LARGEST_INCLUDED
#define ETL_LARGEST_INCLUDED

///\defgroup largest largest
///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "smallest.h"
#include "static_assert.h"

namespace etl 
{
  //***************************************************************************
  /// Template to determine the largest type and size.
  /// Supports up to 16 types.
  /// Defines 'value_type' which is the type of the largest parameter.
  /// Defines 'size' which is the size of the largest parameter.
  ///\ingroup largest
  //***************************************************************************
  template <typename T1, typename T2 = void, typename T3 = void, typename T4 = void, 
            typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void, 
            typename T9 = void, typename T10 = void, typename T11 = void, typename T12 = void, 
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  struct largest_type
  {
    // Define 'largest_other' as 'largest_type' with all but the first parameter. 
    typedef typename largest_type<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::type largest_other;

    // Set 'type' to be the largest of the first parameter and any of the others.
    // This is recursive.
    typedef typename etl::conditional<(sizeof(T1) > sizeof(largest_other)), // Boolean
                                       T1,                                  // TrueType
                                       largest_other>                       // FalseType
                                       ::type type;                         // The largest type of the two.

    // The size of the largest type.
    enum
    {
      size = sizeof(type)
    };
  };

  //***************************************************************************
  // Specialisation for one template parameter.
  //***************************************************************************
  template <typename T1>
  struct largest_type<T1,   void, void, void, void, void, void, void, 
                      void, void, void, void, void, void, void, void>
  {
    typedef T1 type;

    enum
    {
      size = sizeof(type)
    };
  };

  //***************************************************************************
  /// Template to determine the largest alignment.
  /// Supports up to 16 types.
  /// Defines <b>value</b> which is the largest alignment of all the parameters.
  ///\ingroup largest
  //***************************************************************************
  template <typename T1, typename T2 = void, typename T3 = void, typename T4 = void, 
            typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void, 
            typename T9 = void, typename T10 = void, typename T11 = void, typename T12 = void, 
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  struct largest_alignment
  {
    // Define 'largest_other' as 'largest_type' with all but the first parameter. 
    typedef typename largest_alignment<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::type largest_other;

    // Set 'type' to be the largest of the first parameter and any of the others.
    // This is recursive.
    typedef typename etl::conditional<(etl::alignment_of<T1>::value > etl::alignment_of<largest_other>::value), // Boolean
                                       T1,                                                                      // TrueType
                                       largest_other>                                                           // FalseType
                                       ::type type;                                                             // The largest type of the two.

    // The largest alignment.
    enum
    {
      value = etl::alignment_of<type>::value
    };
  };

  //***************************************************************************
  // Specialisation for one template parameter.
  //***************************************************************************
  template <typename T1>
  struct largest_alignment<T1,   void, void, void, void, void, void, void, 
                           void, void, void, void, void, void, void, void>
  {
    typedef T1 type;

    enum
    {
      value = etl::alignment_of<type>::value
    };
  };

  //***************************************************************************
  /// Defines a type that is as larger or larger than the specified type.
  /// Will return the specified type is there is not a larger type.
  ///\ingroup largest
  //***************************************************************************
  template <typename T>
  struct larger_int_type
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_int_for_bits<etl::integral_limits<typename etl::make_signed<T>::type>::bits + 1>::type type;
  };

  //***************************************************************************
  /// Defines a type that is as larger or larger than the specified type.
  /// Will return the specified type is there is not a larger type.
  ///\ingroup largest
  //***************************************************************************
  template <typename T>
  struct larger_uint_type
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_uint_for_bits<etl::integral_limits<typename etl::make_unsigned<T>::type>::bits + 1>::type type;
  };

  //***************************************************************************
  /// Defines a type that is as larger or larger than the specified type.
  /// Will return the specified type is there is not a larger type.
  /// The returned type will be of the same sign.
  ///\ingroup largest
  //***************************************************************************
  template <typename T, bool IS_SIGNED = etl::is_signed<T>::value>
  struct larger_type;

  template <typename T>
  struct larger_type<T, false>
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_uint_for_bits<etl::integral_limits<T>::bits + 1>::type type;
  };

  template <typename T>
  struct larger_type<T, true>
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Must be an integral type");

    typedef typename etl::smallest_int_for_bits<etl::integral_limits<T>::bits + 1>::type type;
  };

  //***************************************************************************
  /// Template to determine the largest type, size and alignment.
  /// Supports up to 16 types.
  /// Defines <b>value</b> which is the largest type, size and alignment of all the parameters.
  ///\ingroup largest
  //***************************************************************************
  template <typename T1, typename T2 = void, typename T3 = void, typename T4 = void, 
            typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void, 
            typename T9 = void, typename T10 = void, typename T11 = void, typename T12 = void, 
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  struct largest
  {
    typedef typename etl::largest_type<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::type type;

    enum
    {
      size      = etl::largest_type<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::size,
      alignment = etl::largest_alignment<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::value
    };
  };
}

#endif

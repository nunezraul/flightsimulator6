/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_STRINGS_H
# define GS_STRINGS_H

/** \file gs_strings.h 
 * resizeable array of strings */

# include <gsim/gs_array.h> 
# include <gsim/gs_input.h> 

/*! \class GsStrings gs_strings.h
    \brief resizeable array of strings

    GsStrings implements methods for managing a resizeable array
    of strings. It derives GsArray, rewriting all methods to correctly
    manage the allocation and deallocation of the strings. */
class GsStrings : private GsArray<char*>
 { public :
    /*! Default constructor */
    GsStrings ( int s=0, int c=0 );

    /*! Copy constructor */
    GsStrings ( const GsStrings& a );

    /*! Destructor */
   ~GsStrings ();

    /*! Returns true if the array has no elements, and false otherwise. */
    bool empty () const { return GsArray<char*>::empty(); }

    /*! Returns the capacity of the array. */
    int capacity () const { return GsArray<char*>::capacity(); }

    /*! Returns the current size of the array. */
    int size () const { return GsArray<char*>::size(); }

    /*! Changes the size of the array. */
    void size ( int ns );

    /*! Changes the capacity of the array. */
    void capacity ( int nc );

    /*! Makes capacity to be equal to size. */
    void compress ();

    /*! Returns a valid index as if the given index references a circular
        array, ie, it returns index%size() for positive numbers. Negative
        numbers are also correctly mapped. */
    int vid ( int index ) const { return GsArray<char*>::vid(index); }

    /*! Sets all elements to s */
    void setall ( const char* s );

    /*! Sets element i to become a copy of s.
        Nothing is done if index i is out of range. */
    void set ( int i, const char* s );

    /*! Get a const pointer to the string index i. If that string was not
        defined, or if i is out of range, a pointer to a static empty string ""
        is returned. */
    const char* get ( int i ) const;

    /*! Operator version of get() */
    const char* operator[] ( int i ) const { return get(i); }

    /*! Returns a const pointer to the i-th last element of the array.
        If that string was not defined, or if i is out of range, a pointer
        to a static empty string "" is returned. */
    const char* top ( int i=1 ) const { return get(size()-i); }

    /*! Pop and frees element size-1 if the array is not empty */
    void pop ();

    /*! Appends one element */
    void push ( const char* s );

    /*! Inserts dp positions, starting at pos i, and adding a copy of
        string s in each new position created. */
    void insert ( int i, const char* s, int dp=1 );

    /*! Removes dp positions starting from pos i */
    void remove ( int i, int dp=1 );

    /*! Parses string s and place each token in a GsStrings position.
        Parameter stopdelim can tell a character to make the parsing
        to stop (after the delimiter character is read),
        and skipdelim tells if delimiters should be parsed or skipped. */
    int parse ( const char* s, char stopdelim=0, bool skipdelim=true );

    /*! Copy operator */
    void operator = ( const GsStrings& a );

    /*! Inserts one string, considering the array is sorted. Returns the
        inserted position. */
    int insort ( const char* s );

    /*! Inserts one string considering the array is sorted. Inserts only
        if no duplicated names are found. Returns the inserted position 
        or -1 if duplication occurs and no insertion is done. */
    int uniqinsort ( const char* s );

    /*! Sort array */
    void sort ();

    /*! Linear search */
    int lsearch ( const char* s ) const;

    /*! Binary search for sorted arrays. Returns index of the element found, 
        or -1 if not found. If not found and pos is not 0, pos will have the 
        position to insert the element keeping the array sorted. */
    int bsearch ( const char* s, int* pos=0 );

    /*! Frees the data of GsStrings, and then makes GsStrings be the
        given array a. After this, a is set to be a valid empty array. The data
        is moved without reallocation. */
    void adopt ( GsStrings& a );

    /*! Outputs all elements of the array in format ["e0" "e1" ... "en"]. */
    friend GsOutput& operator<< ( GsOutput& o, const GsStrings& a );

    /*! Inputs elements in format ["e0" "e1" ... "en"]. */
    friend GsInput& operator>> ( GsInput& in, GsStrings& a );
 };

#endif // GS_STRINGS_H

//============================== end of file ===============================

/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/gs_string.h>
# include <gsim/gs_strings.h>

//# define GS_USE_TRACE1
# include <gsim/gs_trace.h>

//====================== GsStrings ==========================

# define SETZERO(i,ini,size) for(i=ini;i<size;i++) GsArray<char*>::set(i,0)
# define DELETE(i,ini,size) for(i=ini;i<size;i++) delete[] GsArray<char*>::get(i)

GsStrings::GsStrings ( int s, int c )
          :GsArray<char*>(s,c)
 {
   int i;
   SETZERO(i,0,s);
 }

GsStrings::GsStrings ( const GsStrings& a )
          :GsArray<char*>(0,0)
 {
   *this = a; // calls copy operator
 }

GsStrings::~GsStrings ()
 {
   size(0);
 }

void GsStrings::size ( int ns )
 {
   int i, s = size();
   if ( ns>s )
    { GsArray<char*>::size(ns);
      SETZERO(i,s,ns);
    }
   else if ( ns<s )
    { DELETE(i,ns,s);
      GsArray<char*>::size(ns);
    }
 }

void GsStrings::capacity ( int nc )
 {
   int i, s = size();
   if ( nc<s )
    { DELETE(i,nc,s);
    }
   GsArray<char*>::capacity(nc);
 }

void GsStrings::compress ()
 {
   capacity ( size() );
 }

void GsStrings::setall ( const char* s )
 {
   int i;
   for ( i=0; i<size(); i++ ) set(i,s);
 }

void GsStrings::set ( int i, const char* s )
 {
   if ( i<0 || i>=size() ) return;
   delete[] GsArray<char*>::get(i);
   GsArray<char*>::set ( i, gs_string_new(s) );
 }

const char* GsStrings::get ( int i ) const
 {
   const char* st;
   if ( i<0 || i>=size() )
    st=0;
   else
    st=GsArray<char*>::get(i);
   return st? st:"";
 }

void GsStrings::pop ()
 {
   if ( size()>0 ) delete[] GsArray<char*>::pop();
 }

void GsStrings::push ( const char* s )
 {
   GsArray<char*>::push() = gs_string_new(s);
 }

void GsStrings::insert ( int i, const char* s, int dp )
 {
   GsArray<char*>::insert ( i, dp );
   int j;
   for ( j=0; j<dp; j++ )
   GsArray<char*>::get(i+j) = gs_string_new(s);
 }

void GsStrings::remove ( int i, int dp )
 {
   int j;
   for ( j=0; j<dp; j++ )
    delete[] GsArray<char*>::get(i+j);
   GsArray<char*>::remove ( i, dp );
 }

int GsStrings::parse ( const char* s, char stopdelim, bool skipdelim )
 {
   GsInput in; in.init(s);
   size(0);

   while ( in.get()!=GsInput::End )
    { if ( skipdelim && in.ltype()==GsInput::Delimiter ) continue;
      if ( in.ltype()==GsInput::Delimiter && in.ltoken()[0]==stopdelim ) break;
      push ( in.ltoken() );
    }
   
   return size();
 }

void GsStrings::operator = ( const GsStrings& a )
 {
   size ( 0 ); // deletes all data
   GsArray<char*>::size ( a.size() );
   GsArray<char*>::compress();
   int i;
   for ( i=0; i<a.size(); i++ ) GsArray<char*>::set ( i, gs_string_new(a[i]) );
 }

static int fcmpst ( const void* pt1, const void* pt2 )
 {
   typedef const char* cchar;
   return gs_compare( *((cchar*)pt1), (cchar)pt2 );
 }

static int fcmppt ( const void* pt1, const void* pt2 )
 {
   typedef const char* cchar;
   return gs_compare( *((cchar*)pt1), *((cchar*)pt2) );
 }

int GsStrings::insort ( const char* s )
 {
   int pos;
   pos = GsArrayBase::insort ( sizeof(char*), s, fcmpst );
   GsArray<char*>::get(pos) = gs_string_new(s);
   return pos;
 }

int GsStrings::uniqinsort ( const char* s )
 {
   int pos;
   pos = GsArrayBase::uniqinsort ( sizeof(char*), s, fcmpst );
   if ( pos<0 ) return pos;
   GsArray<char*>::get(pos) = gs_string_new(s);
   return pos;
 }

void GsStrings::sort ()
 {
   GsArrayBase::sort ( sizeof(char*), fcmppt );
 }

int GsStrings::lsearch ( const char* s ) const
 {
   return GsArrayBase::lsearch ( sizeof(char*), s, fcmpst );
 }

int GsStrings::bsearch ( const char* s, int *pos ) 
 {
   return GsArrayBase::bsearch ( sizeof(char*), s, fcmpst, pos );
 }

void GsStrings::adopt ( GsStrings& a )
 {
   capacity(0);
   GsArray<char*>::adopt(a);
 }

GsOutput& operator<< ( GsOutput& o, const GsStrings& a )
 {
   o << '[';
   for ( int i=0; i<a.size(); i++ )
    { o << gspc << '"' << a[i] << '"' ;
    }
   return o << gspc << ']';
 }

GsInput& operator>> ( GsInput& in, GsStrings& a )
 {
   a.size(0);
   in.get(); // '['
   while (true)
    { if ( in.get()==GsInput::Delimiter && in.ltoken()[0]==']' ) break; // ']'
      a.push ( in.ltoken() );
    }
   return in;
 }

//=========================== EOF ===============================

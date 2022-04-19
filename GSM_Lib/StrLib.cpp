#define __STR_LIB_CPP__
//#include "platform_def.h"
//#include "libtype.h"
#include "StrLib.hpp"
#include <string.h>


/**
 * <h3>StrLib.cpp</h3>
 *
 * Библиотечка вспомогательных функция для работы со с символьным представлением данных
 *
 * @author   Терещенко Виктор (Ter@)
 * @version  27.07.2010 ( 16:09:49 )
 */

COMMENT;

char *utoa(unsigned int n,char* vs,int len)
{
//  bool neg=false;
  char *s=vs+len;
  *--s=0;
//  if(n<0)
//  {
//    n=-n; neg=true;
//  }
  do{
    int a=n;
    n=n/10;
    *--s=(a%10)+'0';
  }while(n);
//  if(neg)  *--s='-';
  char* ret=s;
  while(s>vs){ *--s=' '; }
  return ret;
}

char *itoa(int n,char* vs,int len)
{
  bool neg=false;
  char *s=vs+len;
  *--s=0;
  if(n<0)
  {
    n=-n; neg=true;
  }
  do{
    int a=n;
    n=n/10;
    *--s=(a%10)+'0';
  }while(n>0);
  if(neg)  *--s='-';
  char* ret=s;
  while(s>vs){ *--s=' '; }
  return ret;
}

char *sitoa(int n,char* vs)
{
  char *ret=itoa(n,vs,12);
  if(n>=0)
  {
    *--ret='+';
  }
  return ret;
}

char *itoa(int n,char* vs){
    return itoa( n,vs,12);
}


char *_uint8toA(char *s,uint8_t n)
{
  char t;
  if( t=n/100) *s++=t+'0'; n%=100;
  if( t=n/10 ) *s++=t+'0'; n%=10 ;
  *s++=n+'0';
  //*s=0;
  return s;
}
char *uint8toA(char *s,uint8_t n)
{
  s=_uint8toA( s,n );
  *s=0;
  return s;
}

char *_uint8to2A(char *s,uint8_t n)
{
  char t;
  /*if( t=n/100) *s++=t+'0';*/ n%=100;
  t=n/10; *s++=t+'0';  n%=10 ;
  *s++=n+'0';
  //*s=0;
  return s;
}
char *uint8to2A(char *s,uint8_t n)
{
  s=_uint8to2A( s,n );
  *s=0;
  return s;
}

char *_uint8to2bA(char *s,uint8_t n)
{
  char t;
  /*if( t=n/100) *s++=t+'0';*/ n%=100;
  if(t=n/10) *s++=t+'0'; else *s++=' ';
  n%=10; *s++=n+'0';
  //*s=0;
  return s;
}
char *uint8to2bA(char *s,uint8_t n)
{
  s=_uint8to2bA( s,n );
  *s=0;
  return s;
}

short Hex(unsigned char HexL)
{
unsigned char HexH;
   HexH = ( ( HexH =(( HexL>>4 & 0x0F )+'0' )) < '9'+1 ) ? HexH : HexH + (('A' -'9')-1);
   HexL = ( ( HexL =((  HexL & 0x0F )+'0' )) < '9'+1 ) ? HexL : HexL + (('A' -'9')-1);
   return HexL | HexH*256 ;
}
void HtoS( char* &d, char h)
{
  char HexH;
     *d++ = ( ( HexH =(( h>>4 & 0x0F )+'0' )) < '9'+1 ) ? HexH : HexH + (('A' -'9')-1);
     *d++ = ( ( h =((  h & 0x0F )+'0' )) < '9'+1 ) ? h : h + (('A' -'9')-1);
}


char* strfind(char* s1, const char* s2)
{
  char ch1,ch2;

  for(;(ch1=*s1); s1++ ) // до конца строки s1
  {
    if((ch2=*s2++)==0) return s1; // остаток строки без s2
    if( ch1 != ch2) return 0;
  }
  if( ch1 == *s2) return  s1; // если совпали обе строки целеком вместе с завершающими 0
  return 0;
}

/*

// поиск строки (без 0 в конце) в памяти
char* strmem(const char *mem, size_t size,const char* str, size_t len)
{
  char ch = *str;
  char* p = mem;
  size-=len;
  do // до конца строки s1
  {
      if(*p == ch)
      {
          if(strncmp(mem, str, len) == 0)
          {
              return( pmem );
          }
      }
      ++p;
      --size;
  }while(size)
  return 0;
}


*/

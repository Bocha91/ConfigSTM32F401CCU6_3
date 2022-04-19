#ifndef __STR_LIB_HPP__
#define __STR_LIB_HPP__
#include <stdint.h>
/**
 * <h3>StrLib.hpp</h3>
 *
 * ����������� ��������������� ������� ��� ������ �� � ���������� �������������� ������
 *
 * @author   ��������� ������ (Ter@)
 * @version  27.07.2010 ( 16:04:07 )
 */
#ifndef COMMENT
    #define COMMENT
#endif
COMMENT;
/**
 * ��� �������� ����� ����������� � ����������� ����
 * ����������� �� ������� ���� ������, �� ������� ������� ��������� ��������
 *
 * @param n      ��� �������� ����� (������� �� ������� ���� int)
 * @param vs     ���������� ����� �������� �� ������:
 *               11 �������� ��� 32 ������� ������
 *               6  �������� ��� 16 ������� ������
 *               4  ������� ��� 8 ������� ������
 *               ��������� � ����� ����������� 0
 * @return ����� ������� ��������� ������� � ������
 */
char *utoa(unsigned int n,char* vs,int len);

/**
 * �������� ����� ����������� � ����������� ����
 * ����������� �� ������� ���� ������, �� ������� ������� ��������� ��������
 *
 * @param n      �������� ����� (������� �� ������� ���� int)
 * @param vs     ���������� ����� �������� �� ������:
 *               12 �������� ��� 32 ������� ������
 *               7  �������� ��� 16 ������� ������
 *               5  ������� ��� 8 ������� ������
 *
 * @return ����� ������� ��������� ������� � ������
 */
char *itoa(int n,char* vs);

/**
 * �������� ����� ����������� � ����������� ����
 * ����������� �� ������� ���� ������ ������� len, �� ������� ������� ��������� ��������
 * ������ - ��� ������������� ����� ����� �� ������� ������ ���� len < 12
 *
 * @param n      �������� ����� (������� �� ������� ���� int)
 * @param vs     ���������� ����� �������� �� ������:
 *               12 �������� ��� 32 ������� ������
 *               7  �������� ��� 16 ������� ������
 *               5  ������� ��� 8 ������� ������
 * @param len
 *
 * @return ����� ������� ��������� ������� � ������
 */
char *itoa(int n,char* vs,int len);

/**
 * ���� ����� ��� itoa() ������ ������������� ����� ���������� � +
 *
 * @param n      �������� ����� (������� �� ������� ���� int)
 * @param vs���������� ����� �������� �� ������:
 *               12 �������� ��� 32 ������� ������
 *               7  �������� ��� 16 ������� ������
 *               5  ������� ��� 8 ������� ������
 *
 * @return ����� ������� ��������� ������� � ������
 */
char *sitoa(int n,char* vs);

/**
 * �������� � ������ ���������� ������������� ������������ ���������������� ������ 0..255
 *
 * @param s      ������ ���� ������
 * @param n      ����������� �����
 *
 * @return ������ ��������� ��������� ��������� ������� � ������, ����-�� ������� 0 ����� ���������� ������
 */
char *uint8toA(char *s,uint8_t n);
char *_uint8toA(char *s,uint8_t n); //���� ��� � uint8toA �� �� ������ 0 � �����

/**
 * ������� � ������ ���������� ������������� ������������ ������ 00..99
 *
 * @param s      ������ ���� ������
 * @param n      ����������� �����
 *
 * @return ������ ��������� ��������� ��������� ������� � ������, ����-�� ������� 0 ����� ���������� ������
 */
char *uint8to2A(char *s,uint8_t n);
char *_uint8to2A(char *s,uint8_t n);  //���� ��� � uint8to2A �� �� ������ 0 � �����
char *uint8to2bA(char *s,uint8_t n); // ���� ��� � uint8to2A ������ ��������� � ������� ����
char *_uint8to2bA(char *s,uint8_t n); //���� ��� � uint8to2bA �� �� ������ 0 � �����


/**
 * �������������� ����� � ��� ����������������� ������
 *
 * @param HexL ������������� ����
 *
 * @return ����� ���������� ��� �������
 */
short Hex(unsigned char HexL);

/**
 * ������ � ����� d ����������������� ������������� ����� h
 *
 * @param d ����� ��� ����������
 * @param h ��������� ������
 */
void HtoS( char* &d, char h);

/**
 * ���� ���������� ������ s2 �� ������ ������ s1
 *
 * @param s1     ������ � ������� ����
 * @param s2     ��� ����
 *
 * @return ���������� �������� �� ������� �� ���������
 *         ��������� ������� s2 � ������ s1,
 *         ��� 0 ���� �� ������� ��� �� ���������
 */
char* strfind(char* s1, const char* s2);

/**
 * ������ char. ���� ����� ����� ��
 * �������������� ������ ������� � ��������� ������ ������,
 * ��� ������� ��� ���� ����� ���� ����������� ��������
 * ������������� NULL � ���� ������
 */
template <long size>class _MyString
{
   long counts;
   char buf[size+1];
 public:
   _MyString(void){ Clear(); }
   // ������� �� ������
   inline void Terminate(void){buf[counts] =0;}
   inline char* Reads(void){ Terminate(); return buf; }  // ��� ������
   inline char* Read(void){ return buf; }
   inline long  GetCount(){  return counts;}  // ���������� ������ � BUFFER
   inline void  SetCount(long cnew){  counts = (cnew<size)?cnew:size-1; }  //(debug) ������������� ���������� ������ � BUFFER
   inline void  Clear(void){ counts=0; Terminate(); }         // �������� BUFFER
   inline long  Remain(void){ return (size - counts); }  // ���������� ���������� ��������� ����� � ������
   inline long  Size(void){ return size;}
   // �������� � �����
   inline void  Write( char arg)
   {
     buf[counts] = arg;
     if( counts < (size-1) ) counts++;
   }
   inline void Writes(const char* p)
   {
     register char s;
     while(s=*p++) Write(s);
     Terminate();
   }
   void uint8toH(uint8_t n);   // ������� 0..FF
   void uint16toH(uint16_t n); // ������� 0..FFFF
   void uint32toH(uint32_t n); // ������� 0..FFFFFFFF
   void uint8toA(uint8_t n);   // ������� 0..255
   void uint8to2A(uint8_t n)   // ������� 00..09..99
   {
     if( Remain()>2 )
     {
       _uint8to2A(Read()+GetCount(),n );
       SetCount( GetCount()+2 );
       Terminate();
     }
   }
   void uint8to2bA(uint8_t n)   // ������� 00.._9..99
   {
     if( Remain()>2 )
     {
       _uint8to2bA(Read()+GetCount(),n );
       SetCount( GetCount()+2 );
       Terminate();
     }
   }
};

template<long size> void _MyString<size>::uint8toH(uint8_t n)
{
  int t=Hex(n);
  Write( (char)(t>>8));
  Write( (char)t);
  Terminate();
}
template<long size> void _MyString<size>::uint16toH(uint16_t n)
{
  uint8toH(n>>8);
  uint8toH(n);
}
template<long size> void _MyString<size>::uint32toH(uint32_t n)
{
  uint16toH(n>>16);
  uint16toH(n);
}

template<long size> void _MyString<size>::uint8toA(uint8_t n)
{
  char t;
  if( t=n/100) Write(t+'0'); n%=100;
  if( t=n/10 ) Write(t+'0'); n%=10 ;
  Write(n+'0');
  Terminate();
}






COMMENT;
/*****************************************************************************
 *   �������� ������������� �������� ��������
 *   �������������� �� ��������� ������������� ����������� �� ����� ����������
 *
 *   @������ �������������,  ������� 0xF0 ��� 240
 *   int i = BIN(11110000);
 *   ���
 *   char C = BIN(11110000);
 *
 *   ����� �� ������ http://forum.codenet.ru/showthread.php?t=14251
 */
template<unsigned long long bin>
class CToDec
{
public:
  enum { value = (bin&1) + 2*CToDec<bin/10>::value };
};

template<>
class CToDec<0>
{
  public: enum { value = 0 };
};

#define BIN(bin) CToDec<bin>::value

/*****************************************************************************
 * ������� ���������� ������ � 32������ �����
 *
 * ������: (������ 28)
 * extern const unsigned countBitsToHigh = PopulatedBits<-16>::value;
 *
 * ����� � ��������� https://github.com/KonstantinChizhov/AvrProjects/blob/master/mcucpp/gpiobase.h
 */
template<unsigned long x>
class PopulatedBits
{
  static const unsigned long x1 = (x & 0x55555555) + ((x >> 1) & 0x55555555);
  static const unsigned long x2 = (x1 & 0x33333333) + ((x1 >> 2) & 0x33333333);
  static const unsigned long x3 = (x2 & 0x0f0f0f0f) + ((x2 >> 4) & 0x0f0f0f0f);
  static const unsigned long x4 = (x3 & 0x00ff00ff) + ((x3 >> 8) & 0x00ff00ff);
public:
  static const unsigned long value = (x4 & 0x0000ffff) + ((x4 >> 16) & 0x0000ffff);
};


/*****************************************************************************
 * ������ ����� �������� �������� ���� �������������� � 1
 *
 *  ������: (������ 12)
 *  extern const unsigned long countPriority = SearchHighNumBit<4096+16>::value;
 *
 *  ��� ��������
 */
template<unsigned long x>
class SearchHighNumBit
{
public:
  static const unsigned long value =
         (x & 0x80000000ul) ? 31:
         (x & 0x40000000ul) ? 30:
         (x & 0x20000000ul) ? 29:
         (x & 0x10000000ul) ? 28:

         (x & 0x8000000ul) ? 27:
         (x & 0x4000000ul) ? 26:
         (x & 0x2000000ul) ? 25:
         (x & 0x1000000ul) ? 24:

         (x & 0x800000ul) ? 23:
         (x & 0x400000ul) ? 22:
         (x & 0x200000ul) ? 21:
         (x & 0x100000ul) ? 20:

         (x & 0x80000ul) ? 19:
         (x & 0x40000ul) ? 18:
         (x & 0x20000ul) ? 17:
         (x & 0x10000ul) ? 16:

         (x & 0x8000ul) ? 15:
         (x & 0x4000ul) ? 14:
         (x & 0x2000ul) ? 13:
         (x & 0x1000ul) ? 12:

         (x & 0x800ul) ? 11:
         (x & 0x400ul) ? 10:
         (x & 0x200ul) ? 9:
         (x & 0x100ul) ? 8:

         (x & 0x80ul) ? 7:
         (x & 0x40ul) ? 6:
         (x & 0x20ul) ? 5:
         (x & 0x10ul) ? 4:

         (x & 0x8ul) ? 3:
         (x & 0x4ul) ? 2:
         (x & 0x2ul) ? 1:
         (x & 0x1ul) ? 0: -1;
};

#endif  //__STR_LIB_HPP__

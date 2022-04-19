#ifndef __STR_LIB_HPP__
#define __STR_LIB_HPP__
#include <stdint.h>
/**
 * <h3>StrLib.hpp</h3>
 *
 * Библиотечка вспомогательных функция для работы со с символьным представлением данных
 *
 * @author   Терещенко Виктор (Ter@)
 * @version  27.07.2010 ( 16:04:07 )
 */
#ifndef COMMENT
    #define COMMENT
#endif
COMMENT;
/**
 * без знаковое целое преобразует к символьному виду
 * выровненому по правому краю буфера, не занятые позиции заполняет пробелом
 *
 * @param n      без знаковое целое (зависит от размера типа int)
 * @param vs     символьный буфер разменом не меннее:
 *               11 символов для 32 битного целого
 *               6  символов для 16 битного целого
 *               4  символа для 8 битного целого
 *               поскольку в конце добавляется 0
 * @return адрес первого значащего символа в буфере
 */
char *utoa(unsigned int n,char* vs,int len);

/**
 * знаковое целое преобразует к символьному виду
 * выровненому по правому краю буфера, не занятые позиции заполняет пробелом
 *
 * @param n      знаковое целое (зависит от размера типа int)
 * @param vs     символьный буфер разменом не меннее:
 *               12 символов для 32 битного целого
 *               7  символов для 16 битного целого
 *               5  символа для 8 битного целого
 *
 * @return адрес первого значащего символа в буфере
 */
char *itoa(int n,char* vs);

/**
 * знаковое целое преобразует к символьному виду
 * выровненому по правому краю буфера длинной len, не занятые позиции заполняет пробелом
 * ОПАСНА - при использовании может выйти за границы буфера если len < 12
 *
 * @param n      знаковое целое (зависит от размера типа int)
 * @param vs     символьный буфер разменом не меннее:
 *               12 символов для 32 битного целого
 *               7  символов для 16 битного целого
 *               5  символа для 8 битного целого
 * @param len
 *
 * @return адрес первого значащего символа в буфере
 */
char *itoa(int n,char* vs,int len);

/**
 * Тоже самое что itoa() только положительные числа начинаются с +
 *
 * @param n      знаковое целое (зависит от размера типа int)
 * @param vsсимвольный буфер разменом не меннее:
 *               12 символов для 32 битного целого
 *               7  символов для 16 битного целого
 *               5  символа для 8 битного целого
 *
 * @return адрес первого значащего символа в буфере
 */
char *sitoa(int n,char* vs);

/**
 * записать в строку символьное представление беззнакового восьмиразрядного целого 0..255
 *
 * @param s      строка куда писать
 * @param n      беззнаковое целое
 *
 * @return вернет указатель следующую свободную позицию в буфере, туда-же запишет 0 чтобы ограничить строку
 */
char *uint8toA(char *s,uint8_t n);
char *_uint8toA(char *s,uint8_t n); //тоже что и uint8toA но не ставит 0 в конце

/**
 * запишет в строку символьное представление беззнакового целого 00..99
 *
 * @param s      строка куда писать
 * @param n      беззнаковое целое
 *
 * @return вернет указатель следующую свободную позицию в буфере, туда-же запишет 0 чтобы ограничить строку
 */
char *uint8to2A(char *s,uint8_t n);
char *_uint8to2A(char *s,uint8_t n);  //тоже что и uint8to2A но не ставит 0 в конце
char *uint8to2bA(char *s,uint8_t n); // тоже что и uint8to2A только выровенео к правому краю
char *_uint8to2bA(char *s,uint8_t n); //тоже что и uint8to2bA но не ставит 0 в конце


/**
 * Преобразование байта в два шестнадцатеричных симаол
 *
 * @param HexL преобразуемый байт
 *
 * @return слово содержащее два символа
 */
short Hex(unsigned char HexL);

/**
 * кладет в буфер d шестнадцатеричное представление байта h
 *
 * @param d буфер для результата
 * @param h выводимые данные
 */
void HtoS( char* &d, char h);

/**
 * Ищет совпадение строки s2 от начала строки s1
 *
 * @param s1     строка в которой ищем
 * @param s2     что ищем
 *
 * @return возвращает уазатель на позицию за полностью
 *         совпавшей строкой s2 в строке s1,
 *         или 0 если не совпало или не полностью
 */
char* strfind(char* s1, const char* s2);

/**
 * строка char. если буфер полон то
 * продолжающаяся запись ведется в последний символ буфера,
 * это сделано для того чтобы дать возможность записать
 * терминирующий NULL в конц буфера
 */
template <long size>class _MyString
{
   long counts;
   char buf[size+1];
 public:
   _MyString(void){ Clear(); }
   // считать из буфера
   inline void Terminate(void){buf[counts] =0;}
   inline char* Reads(void){ Terminate(); return buf; }  // как строку
   inline char* Read(void){ return buf; }
   inline long  GetCount(){  return counts;}  // количество данных в BUFFER
   inline void  SetCount(long cnew){  counts = (cnew<size)?cnew:size-1; }  //(debug) устанавливает количество данных в BUFFER
   inline void  Clear(void){ counts=0; Terminate(); }         // очистить BUFFER
   inline long  Remain(void){ return (size - counts); }  // возвращяет оставшееся свободное место в буфере
   inline long  Size(void){ return size;}
   // записать в буфер
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
   void uint8toH(uint8_t n);   // выводит 0..FF
   void uint16toH(uint16_t n); // выводит 0..FFFF
   void uint32toH(uint32_t n); // выводит 0..FFFFFFFF
   void uint8toA(uint8_t n);   // выводит 0..255
   void uint8to2A(uint8_t n)   // выводит 00..09..99
   {
     if( Remain()>2 )
     {
       _uint8to2A(Read()+GetCount(),n );
       SetCount( GetCount()+2 );
       Terminate();
     }
   }
   void uint8to2bA(uint8_t n)   // выводит 00.._9..99
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
 *   двоичное представление числовых констант
 *   преобразование из двоичного представления выполняется на этапе компиляции
 *
 *   @пример использования,  получим 0xF0 или 240
 *   int i = BIN(11110000);
 *   или
 *   char C = BIN(11110000);
 *
 *   нашел на форуме http://forum.codenet.ru/showthread.php?t=14251
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
 * подсчет количкства кдиниц в 32битном целом
 *
 * пример: (вернет 28)
 * extern const unsigned countBitsToHigh = PopulatedBits<-16>::value;
 *
 * нашел в исходнике https://github.com/KonstantinChizhov/AvrProjects/blob/master/mcucpp/gpiobase.h
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
 * вернет номер наиболее старшего бита установленного в 1
 *
 *  пример: (вернет 12)
 *  extern const unsigned long countPriority = SearchHighNumBit<4096+16>::value;
 *
 *  сам придумал
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

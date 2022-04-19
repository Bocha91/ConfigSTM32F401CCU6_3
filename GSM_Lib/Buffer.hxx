#ifndef __BUFFER_HXX__
#define __BUFFER_HXX__
/**
 * кольцевой буфер в проекте на FreeRTOS
 *
 * @author Терещенко Виктор (Ter@)
 * @version 2.0 
 */
#include "cmsis_os2.h"
inline osStatus_t OS_sleep(timeout_t timeout) { return osDelay(timeout);} // подстроено под FreeRTOS

template <typename Type, int size >class CirBuffer
{
  protected:
    int counts;                                 // счетчик данных имеющихся в буфере
    int ptr;                                    // позиции в буфере
    Type bBuffer[size];                         // буфер под данные

  public:
    CirBuffer(void){ Reset(); }
    // считать из буфера
    inline Type Read()
    {
      register int tptr = ptr;
      register Type arg = bBuffer[tptr];
      if ( ++tptr >= size ) tptr=0; ptr = tptr;
      --counts;
      return arg;
    }

    inline int  GetCount(){  return counts;}    // количество данных в BUFFER
    inline void Reset(void){ counts=ptr=0; }    // очистить BUFFER
    inline int  Remain(void){ return (size - counts); } // возвращяет оставшееся свободное место в буфере
    // записать в буфер
    inline void Write( Type arg)
    {
      register int temp;
      if (( temp = (ptr + counts++)) >= size )  temp-=size ;
      bBuffer[temp] = arg;
    }
    Type* ReadAdr(int i) // вернет адрес i-того элемента буфера
    {
      if(i>counts) i=counts;
      register int temp;
      if (( temp = (ptr + i)) >= size )  temp-=size ;
      return &bBuffer[temp];
    }
};

template <typename Type, int size >class LinBuffer
{
  protected:
    int counts;                                 // счетчик данных имеющихся в буфере
    Type bBuffer[size];                         // буфер под данные

  public:
    LinBuffer(void){ Clear(); }
    // считать из буфера
    inline Type*  Read(void){ bBuffer[counts]=0; return bBuffer; }
    inline int  GetCount(){  return counts;}    // количество данных в BUFFER
    inline void SetCount(int cnew){  counts = cnew; }  //(debug) устанавливает количество данных в BUFFER
    inline void Clear(void){ counts=0; bBuffer[counts]=0;}  // очистить BUFFER
    inline int  Remain(void){ return (size - counts); }  // возвращяет оставшееся свободное место в буфере
    inline int  Size(void){return size;}
    // записать в буфер
    inline void Write( Type arg)
    {
      if( counts < size )
      {
        bBuffer[counts] = arg;
        counts++;
      }
    }
};
// безопасный для прерываний кольцевой буфер с указателем записи и чтения, может содержать на байт меньше данных чем объявлено
template <typename Type, int size, timeout_t timeout=10 >class CirBuffer2
{
  protected:

    volatile int w;  // позиция в буфере куда будем записывать данные
    volatile int r;  // позиция в буфере откуда будем читать данные
    Type bBuffer[size]  ;                     // буфер под данные
  public:


    inline void Reset(void){ w=r=0; }    // очистить BUFFER
    CirBuffer2(void){ Reset(); }
//    inline int  GetCount(){  int counts = w-r; if(counts<0) counts += size; return counts;}    // количество данных в BUFFER
    inline int  GetCount(){  int counts = w;counts-=r; if(counts<0) counts += size; return counts;}    // количество данных в BUFFER
    inline int  Remain(void){ return ((size-1) - GetCount()); } // возвращяет оставшееся свободное место в буфере

    // считать из буфера
    inline Type Read()
    {
      register int opt_r = r; // r не может измениться во вне, поэтому кешируем его
      while(w == opt_r){OS_sleep(timeout);}// ждать данных в буфере
      Type arg = bBuffer[opt_r];
      if( ++opt_r >= size ) opt_r=0;
      r=opt_r;
      return  arg;
    }

    // записать в буфер
    inline void Write( Type arg)
    {
      register int opt_w=w+1; //кешируем w поскольку он не меняется во вне этой функции
      if( opt_w >= size ) opt_w=0;
      while( opt_w == r ){OS_sleep(timeout);} //ждать места в буфере
      bBuffer[w] = arg;
      w = opt_w;
    }
};

template <typename Type, int size, timeout_t timeout=10>class CirBuffer2R : public CirBuffer2<Type,size,timeout>
{
public:
  // записать в буфер с возвратом адреса в буфере
  inline Type* WriteR( Type arg)
  {
    register int opt_w = CirBuffer2<Type,size,timeout>::w+1;
    if( opt_w >= size ) opt_w=0;
    while( opt_w == CirBuffer2<Type,size,timeout>::r ){OS_sleep(timeout);} //ждать места в буфере
    Type *ret = &CirBuffer2<Type,size,timeout>::bBuffer[CirBuffer2<Type,size,timeout>::w];
    *ret = arg;
    CirBuffer2<Type,size,timeout>::w=opt_w;
    return ret;
  }

  Type* ReadAdr(int i)// вернет адрес i-того элемента буфера
  {
    int counts = CirBuffer2<Type,size,timeout>::GetCount();
    if(i>counts) i=counts;
    return &CirBuffer2<Type,size,timeout>::bBuffer[CirBuffer2<Type,size,timeout>::r+i];
  }
};

#endif // __BUFFER_HXX__

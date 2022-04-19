#ifndef __VIRTUAL_UART_hpp__
#define __VIRTUAL_UART_hpp__
class virtualUart
{
public:
//  virtualUart(){;}
  // сколько данных в буфере есть, с  кешированием
  virtual long kbhit() = 0;
  // прочитать байт из буфера,
  virtual char Get() = 0;
  // прочитать из буфера строку, тоесть до символа с кодом 0хD
  virtual long Gets( char* b                            // указатель на буфер куда писать
            ,long  space) = 0;                      // сколько символов можно записать в буфер
  // выводит символ пока не выведет
  virtual void Put(char ch) = 0;
#if CONSTANTS
  // выводит строку с нулём в конце, пока не выведет
  virtual void Puts(const char* const str) = 0;
#endif
  // выводит строку с нулём в конце, пока не выведет
  virtual void Puts(char* str) = 0;
#if CONSTANTS
  // выводит строку длинной n, пока не выведет из фласш
  virtual void Putsn(const char* const str, const long len) = 0;
#endif
  // выводит строку копируя посимвольно в свой буфер
  virtual void Putsn(char* str,long len) = 0;
};

#endif //__VIRTUAL_UART_hpp__


#ifndef __VIRTUAL_UART_hpp__
#define __VIRTUAL_UART_hpp__
class virtualUart
{
public:
//  virtualUart(){;}
    // сколько данных в буфере есть, с  кешированием
    virtual uint32_t kbhit() = 0;
    // прочитать байт из буфера
    virtual char Get() = 0;
    // прочитать из буфера строку, тоесть до символа с кодом 0хD, вернёт сколько прочёл символов
    virtual uint32_t Gets( char* b             // указатель на буфер куда писать
                           ,uint32_t  space) = 0;   // сколько символов можно записать в буфер
    // выводит символ пока не выведет
    virtual void Put(char ch) = 0;
    // выводит строку копируя посимвольно в свой буфер
    virtual void Puts(char* str,uint32_t len) = 0;
    // выводит строку с нулём в конце, пока не выведет
    virtual void Puts(char* str) = 0;
    // выводит строку длинной n, пока не выведет из фласш
    //virtual void Puts(const char* const str, const uint32_t len) = 0;
    // выводит строку с нулём в конце, пока не выведет из фласш
    //virtual void Puts(const char* const str) = 0;
    // выводит строку длинной n, пока не выведет, без переписывания в свой буфер (данные в буфере не должны меняться до окончания вывода)
    //virtual void Putsb(char* str,uint32_t len);
    // выводит строку с нулём в конце, пока не выведет, без переписывания в свой буфер (данные в буфере не должны меняться до окончания вывода)
    //virtual void Putsb(char* str);

};

#endif //__VIRTUAL_UART_hpp__


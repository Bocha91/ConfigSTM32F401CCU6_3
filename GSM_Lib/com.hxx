/**
 * <h3>шаблон буферизованого UART через DMA для ATSAM3SD</h3>
 *
 * работает по scmRTOS
 * поддерживает USART/UART.
 *
 * @author   Терещенко Виктор (Ter@)
 * @version  18.08.2014 ( 19:47:54 )
 */
#ifndef __COM_HXX__
#define __COM_HXX__

#include <scmRTOS.h>
#include <Buffer.hxx>
#include "Hibernation.hpp"

// 1-бвзовый виртуальный класс (больще кода),0-нет базового виртуального класса
#define VIRTUAL_COM 0
// 1-строчки из флеша минуя буфер, 0- копируем из флаш в буфер
#define CONSTANTS 0

#if( VIRTUAL_COM  == 1 )
    #include "virtualUart.hpp"
#endif



#define UART_DEFINED(name,CLASS,num,baud,rxsize,txsize,txfilesize,par,chmode) \
OS_INTERRUPT void CLASS##num##_Handler(void); \
com<CLASS##_t,CLASS##num,baud,rxsize,txsize,txfilesize,CLASS##num##_Handler,CLASS##_t::MR_t::PAR_e::##par,CLASS##_t::MR_t::CHMODE_e::##chmode> name; \
OS_INTERRUPT void CLASS##num##_Handler(void){name##.##Interrupt();}

#define UART_EXTERN(name,CLASS,num,baud,rxsize,txsize,txfilesize,par,chmode) \
OS_INTERRUPT void CLASS##num##_Handler(void); \
extern com<CLASS##_t,CLASS##num,baud,rxsize,txsize,txfilesize,CLASS##num##_Handler,CLASS##_t::MR_t::PAR_e::##par,CLASS##_t::MR_t::CHMODE_e::##chmode> name;


typedef union
{
    uint32_t all;
    UART_t::SR_t sr;
    USART_t::CSR_t csr;
} SR_t;


inline UART_t::SR_t UART_t::GetSR(void)
{
    SR_t sr = {.all = SR.all };
//    sr.all &= IMR.all;
    return( sr );
}

inline USART_t::CSR_t USART_t::GetSR(void)
{
    CSR_t csr = {.all = CSR.all };
//    csr.all &= IMR.all;
    return( csr );
}

#if 0
inline void UART_t::EnableBreak(void)
{}
inline void USART_t::EnableBreak(void)
{
    CR.WriteOnly = CR.RSTSTA;
    IER.WriteOnly = IER.RXBRK;
}
inline void UART_t::DisableBreak(void)
{}
inline void USART_t::DisableBreak(void)
{
    CR.WriteOnly = CR.RSTSTA;
    IDR.WriteOnly = IDR.RXBRK;
}
#endif

#pragma language=extended
#pragma section="RAM_region"

template<typename T              // тип (класс) аппоратного UART или USART
    , T &uart                     // ссылка на класс аппоратного UART (USART)
    , int _baud             // битовая скорасть
    // rx
    , uint32_t _RX_BUFFER_SIZE// размер буфера приема RX
    // tx
    , uint32_t _TX_BUFFER_SIZE// размер буфера передачи
    , uint32_t _TX_FILE_SIZE // число строк в буфере передачи
    , void  TxDMAcomplete(void)    // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
>class com
#if( VIRTUAL_COM  == 1 )
: public virtualUart
#endif
{
public:
    // размер буфера приема
    static const uint32_t RX_BUFFER_SIZE = (_RX_BUFFER_SIZE < (_baud/SYSTICKINTRATE/10)+1) ? (((_baud/SYSTICKINTRATE/10)+1)&-3)+4 : _RX_BUFFER_SIZE ;  // размер буфера приема
    static const uint32_t TX_BUFFER_SIZE = _TX_BUFFER_SIZE;  // размер буфера передачи
    static const uint32_t TX_FILE_SIZE   = _TX_FILE_SIZE;    // число строк в буфере передачи

    static const int RX_WAIT_SRTRING = RX_BUFFER_SIZE > 20 ? 10 : RX_BUFFER_SIZE/2 ;
    static const int TX_WAIT_SRTRING = TX_BUFFER_SIZE > 20 ? 20 : TX_BUFFER_SIZE/2 ;

    static const int CHAR_4  = 4*SYSTICKINTRATE*10/_baud ? 4*SYSTICKINTRATE*10/_baud : 1;

    inline int WAIT( int CHAR )
    {
      int wait =CHAR*SYSTICKINTRATE*10/_baud;
      if( wait > 0) return wait;
      return 1;
    }

    int32_t ID(void)
    {
        if(      (uint32_t)&uart == 0x400E0600ul ) return( ID_UART0  );
        else if( (uint32_t)&uart == 0x400E0800ul ) return( ID_UART1  );
        else if( (uint32_t)&uart == 0x40024000ul ) return( ID_USART0 );
        else if( (uint32_t)&uart == 0x40028000ul ) return( ID_USART1 );
        else while(1);
    }

    void _DisableUartClock(void){ PMC.PCDR0 = ( 1UL<<ID() ); }// Disable the USART clock.
    void _EnableUartClock(void) { PMC.PCER0 = ( 1UL<<ID() ); }// Enable the USART clock.
    unsigned int _isUartClock(void){ return (PMC.PCSR0.all & ( 1UL<<ID() )); }

    void cfgIO( bool f )
    {
        if( f )
        {
            if(      (uint32_t)&uart == 0x400E0600ul )
            {
                /* UART0
                UART0 RX PA9  A
                UART0 TX PA10 A
                */
                PIOA.PDR.WriteOnly = PIOA.PDR.P9 | PIOA.PDR.P10;
                PIOA.ABCDSRr[0].P9  = 0,PIOA.ABCDSRr[0].P10 = 0;
                PIOA.ABCDSRr[1].P9  = 0,PIOA.ABCDSRr[1].P10 = 0;
                //PIOA.OER.WriteOnly = PIOA.OER.P10;
            } else if( (uint32_t)&uart == 0x400E0800ul ){
                /* UART1
                UART1 RX PB2  A
                UART1 TX PB3  A
                */
                PIOB.PDR.WriteOnly = PIOB.PDR.P2 | PIOB.PDR.P3;
                PIOB.ABCDSRr[0].P2 = 0,PIOB.ABCDSRr[0].P3 = 0;
                PIOB.ABCDSRr[1].P2 = 0,PIOB.ABCDSRr[1].P3 = 0;
                //PIOA.OER.WriteOnly = PIOB.OER.P3;
            }else if( (uint32_t)&uart == 0x40024000ul ){
                /* USART0
                USART0 CTS PA8 A
                USART0 RTS PA7 A
                USART0 SCK PA2 B
                USART0 RX  PA5 A
                USART0 TX  PA6 A
                */
                PIOA.PDR.WriteOnly = PIOA.PDR.P5 | PIOA.PDR.P6;
                PIOA.ABCDSRr[0].P5  = 0,PIOA.ABCDSRr[0].P6  = 0;
                PIOA.ABCDSRr[1].P5  = 0,PIOA.ABCDSRr[1].P6  = 0;
                //PIOA.OER.WriteOnly = PIOA.OER.P6;
            }else if( (uint32_t)&uart == 0x40028000ul ){
                /* USART1
                USART1 CTS  PA25 A
                USART1 DCD  PA26 A
                USART1 DSR  PA28 A
                USART1 DTR  PA27 A
                USART1 RI   PA29 A
                USART1 RTS  PA24 A
                USART1 SCK  PA23 A
                USART1 RX   PA21 A
                USART1 TX   PA22 A
                */
                PIOA.PDR.WriteOnly = PIOA.PDR.P21 | PIOA.PDR.P22;
                PIOA.ABCDSRr[0].P21 = 0,PIOA.ABCDSRr[0].P22 = 0;
                PIOA.ABCDSRr[1].P21 = 0,PIOA.ABCDSRr[1].P22 = 0;
                //PIOA.OER.WriteOnly = PIOA.OER.P22;
            }
        }else{
            if(       (uint32_t)&uart == 0x400E0600ul ){
                // UART0
                PIOA.CODR.WriteOnly = PIOA.CODR.P9 | PIOA.CODR.P10;  // данные в 0
                PIOA.OER.WriteOnly  = PIOA.OER.P9  | PIOA.OER.P10;   // разрешить на вывод
                PIOA.PER.WriteOnly  = PIOA.PER.P9  | PIOA.PER.P10;   // как ножки
            }else if( (uint32_t)&uart == 0x400E0800ul ){
                //UART1
                PIOB.CODR.WriteOnly = PIOB.CODR.P2 | PIOB.CODR.P3;  // данные в 0
                PIOB.OER.WriteOnly  = PIOB.OER.P2  | PIOB.OER.P3;   // разрешить на вывод
                PIOB.PER.WriteOnly  = PIOB.PER.P2  | PIOB.PER.P3;   // как ножки
            }else if( (uint32_t)&uart == 0x40024000ul ){
                //USART0
                PIOA.CODR.WriteOnly = PIOA.CODR.P5 | PIOA.CODR.P6;  // данные в 0
                PIOA.OER.WriteOnly  = PIOA.OER.P5  | PIOA.OER.P6;   // разрешить на вывод
                PIOA.PER.WriteOnly  = PIOA.PER.P5  | PIOA.PER.P6;   // как ножки
            }else if( (uint32_t)&uart == 0x40028000ul ){
                // USART1
                PIOA.CODR.WriteOnly = PIOA.CODR.P22 | PIOA.CODR.P21; // данные в 0
                PIOA.OER.WriteOnly  = PIOA.OER.P22  | PIOA.OER.P21;   // разрешить на вывод
                PIOA.PER.WriteOnly  = PIOA.PER.P22  | PIOA.PER.P21;   // как ножки
            }
        }
    }

    void EnableBreak(void){ uart.EnableBreak(); }
    void DisableBreak(void){ uart.DisableBreak(); }
    void ResetStatusBits(void){ uart.CR.WriteOnly = uart.CR.RSTSTA; }

    // сброс UART для восстановления работоспособности или просыпания от сна
    void Open(uint32_t priority)
    {
      //TX(); RX();
      rx.c=0; /*rx.p=rx.buf;*/
      tx.c=0; tx.pw=tx.Buf, tx.tbuf=0; tx.cur.adr=0,tx.cur.len=0;

      if( ! _isUartClock() ) _EnableUartClock();
      uart.CR.WriteOnly =  uart.CR.RSTTX | uart.CR.RSTSTA | uart.CR.RSTRX;

      tx.DisablePDC();while( tx.isEnablePDC() );                  // ждем пока остановится
      rx.DisablePDC();while( rx.isEnablePDC() );                  // ждем пока остановится

      tx.SetPointer( tx.Buf );
      tx.SetCounter( 0 );
      tx.SetNextCounter( 0 );

      rx.SetPointer(rx.buf);
      rx.SetCounter(sizeof(rx.buf));
      rx.SetNextPointer(rx.buf);
      rx.SetNextCounter(0);

      cfgIO(true);
      uart.IDR.all = -1; // маску на все прерывания

      SetBaudRate( _baud,_par,_chmode );

      uart.CR.WriteOnly =  uart.CR.RXEN | uart.CR.TXEN ; // разрешить прием и передачу
      tx.EnableInterrupt(); // разрешим только прерывание от PDC TX буфер пуст

      rx.EnablePDC(); // разрешить PDC
      tx.EnablePDC();
      NVIC.DisableIRQ( ID() );
      NVIC.ClearPendingIRQ( ID() );
      NVIC.SetPriority(ID(), priority);
      NVIC.EnableIRQ( ID() );
    }
    void Reset(void)
    {
        uart.CR.WriteOnly =  uart.CR.RSTTX | uart.CR.RSTSTA | uart.CR.RSTRX;
        uart.CR.WriteOnly =  uart.CR.RXEN | uart.CR.TXEN ; // разрешить прием и передачу
    }
    void Close(void)
    {

    }
    void Hibernation(bool freez)
    {
        if( freez )
        { // заснуть
            while( tx.cur.adr || tx.IsInterrupt() ); //ждать окончания вывода tx
            _DisableUartClock();
        }else
        {// проснуться
            if( ! _isUartClock() ) _EnableUartClock();
        }
    }

    //       Rx
    class RX
    {public:
//        char *p;                   // указатель розиции чтения данных из буфера приема
        long c;                    // кеш счетчика байт в буфере
        char buf[RX_BUFFER_SIZE];  // буфер приема

        void     Enable(void) { uart.CR.WriteOnly =  uart.CR.RXEN;  }
        void     DisablePDC(void){ uart.PTCR.WriteOnly = uart.PTCR.RXTDIS; }
        void     EnablePDC(void) { uart.PTCR.WriteOnly = uart.PTCR.RXTEN;  }
        uint32_t isEnablePDC(void){ return uart.PTSR.RXTEN; }
        char*    IsPointer(void){ return (char*)uart.RPR.all; }
        uint32_t IsCounter(void){ return uart.RCR.RXCTR; }
        uint32_t IsNextCounter(void){ return uart.RNCR.RXNCTR; }
        void     SetPointer(char *address){uart.RPR.all = (uint32_t)address;}
        void     SetCounter(uint32_t bytes){ uart.RCR.all = bytes; }
        void     SetNextPointer(char *address){uart.RNPR.all = (uint32_t)address;}
        void     SetNextCounter(uint32_t bytes){ uart.RNCR.all = bytes; }
        void     DisableInterrupt(void){ uart.IDR.WriteOnly = /*uart.IDR.ENDTX |*/ uart.IDR.RXBUFF; }
        void     EnableInterrupt(void){  uart.IER.WriteOnly = /*uart.IER.ENDTX |*/ uart.IER.RXBUFF; }
        uint32_t IsInterrupt(void){  return uart.IMR.RXBUFF; }
        uint32_t count(void){ return (sizeof(buf) - (IsCounter() + IsNextCounter())); }
        void     ResetReceiver(void){ uart.CR.WriteOnly = uart.CR.RSTRX; }
        void     setTimeOut(uint16_t time){ //(только USART)
            uart.RTOR.TO = time;
            uart.CR.WriteOnly = uart.CR.STTTO;
            if( time ){
                uart.IER.WriteOnly = uart.IER.TIMEOUT;
            } else {
                uart.IDR.WriteOnly = uart.IDR.TIMEOUT;
            }
        }
        RX() :
//          p(buf),
        c(0){}

    } rx;

    // сколько данных в буфере есть, с  кешированием
    uint32_t kbhit()
    {
        if( rx.c == 0 ){
            if( _isUartClock() ) rx.c = rx.count();
            else                 return 0;
        }
        return rx.c;
    }
    char Get(void); // прочитать байт из буфера,
    // прочитать из буфера строку, тоесть до символа с кодом 0хD
    uint32_t Gets( char*     bu                       // указатель на буфер куда писать
                  ,uint32_t  space);                  // сколько символов можно записать в буфер
    //------------------------ Put ---------------------


    // описатель выводимой строки
    typedef struct
    {
        void* adr;                                //адрес перврго символа строки
        uint32_t len;                             //длина строки
    } _Dse;

    //            Tx
    class TX
    {public:
        char Buf[TX_BUFFER_SIZE];
        char *pw;                                // текущее место записи в буфер
        volatile uint32_t c;                     // кеш счетчика байт в буфере
        _Dse* tbuf;                              // текущем экземпляре буфера для посимвольной зариси в раме
        _Dse  cur;                               // текущий Tx. то что в pTx и cTx
        CirBuffer2R<_Dse,TX_FILE_SIZE,CHAR_4> File;

        void     Enable(void) { uart.CR.WriteOnly = uart.CR.TXEN ;  }
        void     DisablePDC(void){ uart.PTCR.WriteOnly = uart.PTCR.TXTDIS; }
        void     EnablePDC(void) { uart.PTCR.WriteOnly = uart.PTCR.TXTEN;  }
        uint32_t isEnablePDC(void){ return uart.PTSR.TXTEN; }
        void     SetPointer(void *address){uart.TPR.TXPTR = (uint32_t)address;}
        uint32_t IsPointer(void){ return uart.TPR.TXPTR; }
        void     SetCounter(uint32_t bytes){ uart.TCR.TXCTR = bytes; }
        uint32_t IsCounter(void){ return uart.TCR.TXCTR; }
        void     SetNextCounter(uint32_t bytes){ uart.TNCR.TXNCTR = bytes; }
        void     DisableInterrupt(void){ uart.IDR.WriteOnly = /*uart.IDR.ENDTX |*/ uart.IDR.TXBUFE; }
        void     EnableInterrupt(void){  uart.IER.WriteOnly = /*uart.IER.ENDTX |*/ uart.IER.TXBUFE; }
        void     ResetTransmitter(void){ uart.CR.WriteOnly = uart.CR.RSTTX; }
        uint32_t IsInterrupt(void){  return uart.IMR.TXBUFE; }
        bool     isWait(void)
        {
            SR_t sr = {.all = uart.GetSR().all };
            return( !(sr.sr.TXEMPTY) || (!sr.sr.TXBUFE) );
        }

        TX() : pw(Buf), tbuf(0),c(0) {   cur.adr=0,cur.len=0; }

        // вычисляем сколько байт свободно в буфере
        long free(void){ return (long)( sizeof(Buf) - tx.c); }
    } tx;
    //вызывается только из прерываний
    SR_t Interrupt(void)
    {
        SR_t sr = {.all = uart.GetSR().all };
        sr.csr.all &= uart.IMR.all;
        if( sr.sr.TXBUFE ) // cTx ==0
        {
            // учет свободного маста в буфере вывода
            //if(cur.adr >= Buf )
            if((tx.cur.adr >= tx.Buf )&&( tx.cur.adr < tx.Buf+sizeof(tx.Buf) ))
            {// наш буфер
                tx.c-= tx.cur.len; // освобождаем место в буфере
            }
            if(tx.File.GetCount()==0)
            {// данных нет в буферах
                tx.cur.adr=0;
                tx.DisableInterrupt();
                ::Hibernation.dbUART.clear();
            }else
            {// если есть строки для вывода
                tx.cur = tx.File.Read();
                if( tx.tbuf )
                    if( tx.tbuf->adr == tx.cur.adr )
                        tx.tbuf=0; // начат вывод, записываемого буфера
                tx.DisablePDC();
                while( tx.isEnablePDC() ); // ждем пока остановится
                tx.SetPointer( tx.cur.adr );
                tx.SetCounter( tx.cur.len );
                tx.EnablePDC();
            }
        }
        return sr;
//        if(sr.sr.PARE){ Open(); }
    }

    // выводит символ пока не выведет
    void Put(char ch);
    // выводит строку копируя посимвольно в свой буфер
//    void Puts(char* str,uint32_t len);
    // выводит строку с нулём в конце, пока не выведет
//    void Puts(char* str);
//#if ( CONSTANTS == 1 )
    // выводит строку длинной n, пока не выведет из фласш
    void Puts(const char* const str, const uint32_t len);

    // выводит строку с нулём в конце, пока не выведет из фласш
    void Puts(const char* const str);
//#endif
    // выводит строку длинной n, пока не выведет, без переписывания в свой буфер (данные в буфере не должны меняться до окончания вывода)
    void Putsb(char* str,uint32_t len);
    // выводит строку с нулём в конце, пока не выведет, без переписывания в свой буфер (данные в буфере не должны меняться до окончания вывода)
    void Putsb(char* str);

    void SetBaudRate(uint32_t baudrate,uint32_t Par,uint32_t Chmode)
    {
        uart.SetBaudRate( baudrate,T::MR_t::PAR_e::E(Par),T::MR_t::CHMODE_e::E(Chmode));
    }
    com();
};

template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
>com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::com()
{
    RX(); TX();
};
inline void UART_t::SetBaudRate(uint32_t baudrate, MR_t::PAR_e::E par, MR_t::CHMODE_e::E chmode)
{
    BRGRr.CD   = SystemCoreClock/baudrate/16;
    MRr.PAR    = par; // MR_t::PAR_e::eNO;
    MRr.CHMODE = chmode; // MR_t::CHMODE_e::eNORMAL;
    if(par != MR_t::PAR_e::eNO ) { IER.WriteOnly = IER.PARE; }
}

inline void USART_t::SetBaudRate(uint32_t baudrate, MR_t::PAR_e::E par, MR_t::CHMODE_e::E chmode)
{
    uint32_t over = ( SystemCoreClock >= baudrate*16 ) ? 16 : 8;
    uint32_t cd_fp = (8 * SystemCoreClock + (over * baudrate) / 2) / (over * baudrate);
    uint32_t cd = cd_fp >> 3;
    uint32_t fp = cd_fp & 0x07;
    if(over == 8) over = 1; else over=0;
    BRGRr.CD = cd,BRGRr.FP = fp;
    MRr.USART_MODE     = MR_t::USART_MODE_e::eNORMAL,
    MRr.USCLKS         = MR_t::USCLKS_e::eMCK,
    MRr.CHRL           = MR_t::CHRL_e::e8_BIT,
    MRr.SYNC           = 0,
    MRr.PAR            = par, // = MR_t::PAR_e::eNO,
    MRr.NBSTOP         = MR_t::NBSTOP_e::e1_BIT,
    MRr.CHMODE         = chmode, //= MR_t::CHMODE_e::eNORMAL,
    MRr.MSBF           = 0,
    MRr.MODE9          = 0,
    MRr.CLKO           = 0,
    MRr.OVER           = over,
    MRr.INACK          = 0,
    MRr.DSNACK         = 0,
    MRr.VAR_SYNC       = 0,
    MRr.INVDATA        = 0,
    MRr.MAX_ITERATION  = 0,
    MRr.FILTER         = 0,
    MRr.MAN            = 0,
    MRr.MODSYNC        = 0,
    MRr.ONEBIT         = 0;
    if(par != MR_t::PAR_e::eNO ) { IER.WriteOnly = IER.PARE; }
}
/*
inline SR_t UART_t::TX::Interrupt(void)
{
            UART_t::SR_t sr = {.all = uart.SRr.all};
            if( sr.TXBUFE ) // cTx ==0
            {
                // учет свободного маста в буфере вывода
                //if(cur.adr >= Buf )
                if((cur.adr >= Buf )&&( cur.adr < Buf+sizeof(Buf) ))
                {// наш буфер
                    c-= cur.len; // освобождаем место в буфере
                }
                if(File.GetCount()==0)
                {// данных нет в буферах
                    cur.adr=0;
                    DisableInterrupt();
                }else
                {// если есть строки для вывода
                    cur = File.Read();
                    if( tbuf )
                        if( tbuf->adr == cur.adr )
                            tbuf=0; // начат вывод, записываемого буфера
                    Disable();
                    while( isEnable() ); // ждем пока остановится
                    SetPointer( cur.adr );
                    SetCounter( cur.len );
                    Enable();
                }
            }
            //if( sr.TXBUFE ){}
        }
inline void USART_t::TX::Interrupt(void)
        {
            USART_t::CSR_t sr = {.all = uart.CSRr.all};
            if( sr.TXBUFE ) // cTx ==0
            {
                // учет свободного маста в буфере вывода
                //if(cur.adr >= Buf )
                if((cur.adr >= Buf )&&( cur.adr < Buf+sizeof(Buf) ))
                {// наш буфер
                    c-= cur.len; // освобождаем место в буфере
                }
                if(File.GetCount()==0)
                {// данных нет в буферах
                    cur.adr=0;
                    DisableInterrupt();
                }else
                {// если есть строки для вывода
                    cur = File.Read();
                    if( tbuf )
                        if( tbuf->adr == cur.adr )
                            tbuf=0; // начат вывод, записываемого буфера
                    Disable();
                    while( isEnable() ); // ждем пока остановится
                    SetPointer( cur.adr );
                    SetCounter( cur.len );
                    Enable();
                }
            }
            //if( sr.TXBUFE ){}
        }
*/

// прочитать байт из буфера
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> char com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Get(void) // прочитать байт из буфера
#if  1
    {
        while(!kbhit()) { OS::sleep( WAIT(1) ); }

        TCritSect cs;
        rx.DisablePDC();
        while( rx.isEnablePDC() );                   // ждем пока остановится

        register uint32_t cRx  = rx.IsCounter();
        register uint32_t cNRx = rx.IsNextCounter();
        register uint32_t c = sizeof( rx.buf ) - cRx - cNRx;
        register char* pRx  = rx.IsPointer();
        register uint32_t s = (uint32_t)pRx - c;
        if( s < (uint32_t)rx.buf ) s+= sizeof( rx.buf );
        s -=(uint32_t)rx.buf;

        register char ch = rx.buf[s++];

        if( s >= sizeof(rx.buf) )
        {
            cRx+=cNRx+1;
            rx.SetCounter(cRx);
            rx.SetNextCounter(0);
        }else{
            if( pRx+cRx >= rx.buf+sizeof(rx.buf)  )
            {
                cNRx=cNRx+1;
                rx.SetNextCounter(cNRx);
            }else{
                cRx+=1;
                rx.SetCounter( cRx );
            }
        }
        rx.EnablePDC();
        cs.~TCritSect();
        rx.c=c-1;
        return ch;
    }
#elif 0
    // прочитать байт из буфера,
    {
        register char *pNRx = rx.p;
        while(!kbhit()) { OS::sleep( WAIT(1) ); }
        --rx.c;

        TCritSect cs;
        rx.Disable();
        while( rx.isEnable() );                   // ждем пока остановится

        register uint32_t cNRx = rx.IsNextCounter();
        register char ch = *rx.p;
        if( (rx.p+=1) >=  &rx.buf[RX_BUFFER_SIZE])
        {
            rx.p = rx.buf;
            register uint32_t cRx = rx.IsCounter();
            cRx+=cNRx+1;
            cNRx=0;
            rx.SetCounter(cRx);
        }else
        {
            if( cNRx == 0 ){ rx.SetNextPointer(pNRx); }
            cNRx+=1;
        }
        rx.SetNextCounter(cNRx);
        rx.Enable();
        cs.~TCritSect();
        return ch;
    }
#endif


// прочитать из буфера строку, тоесть до символа с кодом 0хD
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> uint32_t com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Gets( char* bu                       // указатель на буфер куда писать
              ,uint32_t  space)              // сколько символов можно записать в буфер
#if 1
    // работает на 9600 и на 1250 000
    {
        if(space == 0) return space;
        register uint32_t d = 0;                   // указатель куда присать
        while(1)
        {
            while(!kbhit()) { OS::sleep( WAIT(1) ); }// здесь нужно заморозить поток

            TCritSect cs;
            rx.Disable();
            while( rx.isEnable() );               // ждем пока остановится
            // вычисляем указатель и длинну
            register uint32_t cRx  = rx.IsCounter();
            register uint32_t cNRx = rx.IsNextCounter();
            register uint32_t c = sizeof( rx.buf ) - cRx - cNRx;
            register char* pRx  = rx.IsPointer();
            register uint32_t s = (uint32_t)pRx - c;
            if( s < (uint32_t)rx.buf ) s+= sizeof( rx.buf );
            s -=(uint32_t)rx.buf;

            //if( s >= sizeof(rx.buf) )  s=0;


            register uint32_t n=0;             // осталось в буфере
            char ch;
            do{
                ch = bu[d++] = rx.buf[s++];
                ++n;

                if( ch == 0x0A )                  // игнорируем символ (не пишем в буфер)
                {
                    --d;
                }else if( ch == 0x0D )
                {
                    bu[d-1]=0;                    // вместо  0x0D запишем 0
                    break;
                }

                if( s >= sizeof(rx.buf) )  break;
                if( d >= space ) break;

            }while( n<c );

            if( s >= sizeof(rx.buf) )
            {
                cRx+=n+cNRx;
                rx.SetCounter(cRx);
                rx.SetNextCounter(0);
            }else{
                if( pRx+cRx >= rx.buf+sizeof(rx.buf)  )
                {
                    cNRx=n+cNRx;
                    rx.SetNextCounter(cNRx);
                }else{
                    cRx+=n;
                    rx.SetCounter( cRx );
                }
            }
            rx.Enable();
            cs.~TCritSect();
            rx.c = c-n;

            if( ch==0x0D )    return d;
            if(  d >= space ) return space;
        }
    }
#elif 0
    {
        if(space == 0) return space;
        register char* pD = bu;                   // указатель куда присать
        register char* pEnd = bu+space;           // докуда писать
        while(1)
        {
            while(!kbhit()) { OS::sleep( WAIT(1) ); }// здесь нужно заморозить поток

            TCritSect cs;
            rx.Disable();
            while( rx.isEnable() );               // ждем пока остановится
            // вычисляем указатель и длинну
            register char* pRx  = rx.IsPointer();
            register uint32_t cRx  = rx.IsCounter();
            register uint32_t cNRx = rx.IsNextCounter();
            rx.Enable();
            cs.~TCritSect();

            register uint32_t c = sizeof( rx.buf ) - cRx - cNRx;
            register char* p = pRx - c;
            if( p >= rx.buf+sizeof(rx.buf) )  p -= sizeof(rx.buf);

            register uint32_t n=0;             // осталось в буфере
            register uint8_t ch;
            do{
                ch = *p++;
                *pD++ = ch;
                ++n;

                if( ch == 0x0A )                  // игнорируем символ (не пишем в буфер)
                {
                    --pD;
                }else if( ch == 0x0D )
                {
                    *(pD-1)=0;                    // вместо  0x0D запишем 0
                    break;
                }

                if( p >= rx.buf+sizeof(rx.buf) )  break;

            }while( (n<c) && (pD<pEnd) );

            TCritSect cs1;
            rx.Disable();
            while( rx.isEnable() );               // ждем пока остановится
            if( p >= rx.buf+sizeof(rx.buf) )
            {
                cRx  = rx.IsCounter();
                cRx+=n+rx.IsNextCounter();
                rx.SetCounter(cRx);
                rx.SetNextCounter(0);
            }else{
                cRx  = rx.IsCounter();
                pRx  = rx.IsPointer();
                if( pRx+cRx >= rx.buf+sizeof(rx.buf)  )
                {
                    cNRx=n+rx.IsNextCounter();
                    rx.SetNextCounter(cNRx);
                }else{
                    cRx+=n;
                    rx.SetCounter( cRx );
                }
            }
            rx.Enable();
            cs1.~TCritSect();
           rx.c = 0;
            if( ch==0x0D ) return pEnd-pD;
            if( pD>=pEnd ) return space;

        }
    }
#elif 0
    {
        if(space == 0) return space;
        register char* pD = bu;                   // указатель куда присать
        register char* pEnd = bu+space;           // докуда писать
        while(1)
        {
            while(!kbhit()) { OS::sleep( WAIT(1) ); }// здесь нужно заморозить поток

            register char *pNRx = rx.p;
            register uint32_t n=rx.c;             // осталось в буфере
            register uint8_t ch;
            do{
                ch = *rx.p++;
                *pD++ = ch;
                --n;

                if( ch == 0x0A )                  // игнорируем символ (не пишем в буфер)
                {
                    pD-=1;
                }else if( ch == 0x0D )
                {
                    *(pD-1)=0;                    // вместо  0x0D запишем 0
                    break;
                }
                if( rx.p >= rx.buf+sizeof(rx.buf) )
                {
//                   rx.c-=n;                       //  вычисляем сколько байт считано
                   break;
                }
            }while( n && (pD<pEnd) );

            rx.Disable();
            while( rx.isEnable() );               // ждем пока остановится
            register uint32_t cNRx = rx.IsNextCounter();
            if( rx.p >= rx.buf+sizeof(rx.buf) )
            {

                rx.p=rx.buf;
                register uint32_t cRx = rx.IsCounter();
                while(cRx==0);
                cRx+=cNRx+rx.c-n;
                rx.SetCounter(cRx);
                rx.c=n;
                cNRx = 0;
            }else{
                if( cNRx == 0 ){ rx.SetNextPointer(pNRx); pNRx = rx.p; }
                cNRx+=rx.c;
                rx.c=0;
            }
            rx.SetNextCounter(cNRx);
            rx.Enable();
            if( ch==0x0D ) return pEnd-pD;
            if( pD>=pEnd ) return space;
        }
    }
#elif 0
    {
        register uint32_t u=0;                    // уже учтенные байты
        register uint32_t n = space;              // осталочь места в буфере
        while(1)
        {
            while(!kbhit()) { OS::sleep( WAIT(RX_WAIT_SRTRING) ); }// здесь нужно заморозить поток

            register char *pNRx = rx.p;
            while(rx.c && n )
            {
                uint8_t ch = *rx.p;
                --rx.c;
                if( ++rx.p >= (rx.buf + sizeof(rx.buf) ) )
                {
                    rx.p=rx.buf;
                    rx.Disable();
                    while( rx.isEnable() );       // ждем пока остановится
                    register uint32_t cNRx = rx.IsNextCounter();
                    cRx+=cNRx+(space-n-u); u=space-n;
                    cNRx=0;
                    rx.SetCounter(cRx);
                    rx.SetNextCounter(cNRx);
                    rx.Enable();
                }
                if( ch == 0x0A ) continue;        // игнорируем символ
                if( ch == 0x0D )
                {
                    *(bu)=0;                       // вместо  0x0D запишем 0
                    --n;
                    rx.Disable();
                    while( rx.isEnable() );       // ждем пока остановится
                    register uint32_t cNRx = rx.IsNextCounter();
                    if( cNRx == 0 ){ rx.SetNextPointer(pNRx); }
                    cNRx+=(space-n-u); u=space-n;
                    rx.SetNextCounter(cNRx);
                    rx.Enable();
                    return space-n;
                }
                *bu++ = ch;
                --n;
            }
            rx.Disable();
            while( rx.isEnable() );       // ждем пока остановится
            register uint32_t cNRx = rx.IsNextCounter();
            if( cNRx == 0 ){ rx.SetNextPointer(pNRx); }
            cNRx+=(space-n-u); u=space-n;
            rx.SetNextCounter(cNRx);
            rx.Enable();
            if( n == 0 ) return space-n;
        }
    }
#endif
    // выводит символ пока не выведет
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Put(char ch)
    {
          while(tx.c >= sizeof(tx.Buf) ) { OS::sleep( WAIT(2) ); }// ждать места
          *tx.pw=ch; // когда место есть данные можно класть сразу
          tx.DisableInterrupt();
          ++tx.c;
          if(tx.tbuf)
          {
              tx.tbuf->len+=1; // увеличем счётчик
          }else{// если нет открытого описателя для посимвольного буфера, создадим новый
              tx.EnableInterrupt();
              while(tx.File.Remain()==0) { OS::sleep( TX_WAIT_SRTRING /*timeout*/ ); }// ждать места
              _Dse tmp; tmp.adr=tx.pw, tmp.len=1;
              tx.DisableInterrupt();
              tx.tbuf = tx.File.WriteR( tmp );
          }
          if( ++tx.pw >= tx.Buf+sizeof(tx.Buf) )
          {
              ++tx.pw = tx.Buf;
              tx.tbuf=0; // ДМА не знает что буфер кольцевой, строка закончена
          }
          tx.EnableInterrupt();
          ::Hibernation.dbUART.set();
    }

    // выводит строку копируя посимвольно в свой буфер
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Puts(const char* st,const uint32_t len)
    {
        register char* str= (char*)st;
        register uint32_t opt_n=len; // сколько байт осталось переписать
        while(opt_n)
        {
            while(tx.c >= sizeof(tx.Buf) ){OS::sleep( WAIT(opt_n/2) );} // ждать места
            // вычисляем сколько байт до конца кольца
            long end = (tx.Buf+sizeof(tx.Buf))-tx.pw;
            //tx.DisableInterrupt();
            // вычисляем сколько байт свободно в буфере
            long free = sizeof(tx.Buf) - tx.c;
            //tx.EnableInterrupt();

            // вычисляем сколько байт можно записать непрерывно
            long max = free > end ? end : free;
            // вычислим сколько байт перепишем
            long mov = opt_n > max ? max : opt_n;
            // останется вывести
            opt_n-=mov;
            register char* opt_pwtx=tx.pw; //куда писать
            for(long i=mov; i;i--)
            {
                *opt_pwtx++=(char)*str++;
            }
            // обернём через конц буфера
            if(opt_pwtx >= tx.Buf+sizeof(tx.Buf) ) opt_pwtx=tx.Buf;
            tx.DisableInterrupt();
            tx.c+=mov;
            if(tx.tbuf)
            {
                tx.tbuf->len+=mov; // увеличем счётчик
            }else
            {
                tx.EnableInterrupt();
                // если нет открытого описателя для посимвольного буфера, создадим новый
                while(tx.File.Remain()==0) { OS::sleep( WAIT(TX_WAIT_SRTRING) ); }    // ждать места
                _Dse tmp; tmp.adr=tx.pw, tmp.len=mov;
                tx.DisableInterrupt();
                tx.tbuf = tx.File.WriteR( tmp );
            }
            tx.pw=opt_pwtx;
            // ДМА не знает что буфер кольцевой
            if(opt_pwtx==tx.Buf) tx.tbuf=0; // строка закончена
            tx.EnableInterrupt();
        }
        ::Hibernation.dbUART.set();
    }

    // выводит строку с нулём в конце, пока не выведет
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Puts(const char* str)
    {
      uint32_t i;
      for(i=0; str[i]!=0;++i); // считаем символы в строке
      Puts(str,i);
    }

#if ( CONSTANTS == 1 )
    // выводит строку длинной n, пока не выведет из фласш
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , MR_t::PAR_e par, MR_t::CHMODE_e chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
     >::Putsn(const char* const str, const uint32_t len)
    {
        long opt_n=len;
        while(opt_n)    // ДМА не может болше 65535 байт
        {
            long tlen;
            if(opt_n>65535){tlen=65535;opt_n-=65535;}else{tlen=opt_n;opt_n=0;}

            _Dse tmp; tmp.adr=(uint8_t*)str, tmp.len=tlen;
            while( tx.File.Remain()==0 ) { OS::sleep( WAIT(TX_WAIT_SRTRING) ); }    // здесь нужно заморозить поток
            tx.DisableInterrupt();
            tx.File.Write( tmp );
            tx.tbuf=0;  // если есть посимвольный буфер то он закончен
            tx.EnableInterrupt();
        }
        Hibernation.dbUART = 1;
    }

  // выводит строку с нулём в конце, пока не выведет из фласш
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , MR_t::PAR_e par, MR_t::CHMODE_e chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
     >::Puts(const char* const str)
  {
    uint32_t i;
    for(i=0; str[i]!=0;++i); // считаем символы в строке
    Putsn(str,i);
  }
#endif
  // выводит строку длинной n, пока не выведет, без переписывания в свой буфер (данные в буфере не должны меняться до окончания вывода)
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Putsb(char* str,uint32_t len)
  {
      long opt_n=len;
      while(opt_n)    // ДМА не может болше 65535 байт
      {
          long tlen;
          if(opt_n>65535){tlen=65535;opt_n-=65535;}else{tlen=opt_n;opt_n=0;}

          _Dse tmp; tmp.adr=(uint8_t*)str, tmp.len=tlen;
          while( tx.File.Remain()==0 ) { OS::sleep( WAIT(TX_WAIT_SRTRING) ); }    // здесь нужно заморозить поток
          tx.DisableInterrupt();
          tx.File.Write( tmp );
          tx.tbuf=0;  // если есть посимвольный буфер то он закончен
          tx.EnableInterrupt();
      }
      ::Hibernation.dbUART.set();
  }
  // выводит строку с нулём в конце, пока не выведет, без переписывания в свой буфер (данные в буфере не должны меняться до окончания вывода)
template<typename T            // тип (класс) аппоратного UART или USART
    ,T &uart                   // ссылка на класс аппоратного UART (USART)
    ,int _baud                 // битовая скорасть
    // rx
    ,uint32_t _RX_BUFFER_SIZE  // размер буфера приема RX
    // tx
    ,uint32_t _TX_BUFFER_SIZE  // размер буфера передачи
    ,uint32_t _TX_FILE_SIZE    // число строк в буфере передачи
    ,void  TxDMAcomplete(void) // функция обработк прерывания TX
    , uint32_t _par
    , uint32_t _chmode
> void com< T
      ,uart,_baud
      ,_RX_BUFFER_SIZE
      ,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete
      ,_par,_chmode
     >::Putsb(char* str)
  {
    uint32_t i;
    for(i=0; str[i]!=0;++i); // считаем символы в строке
    Putsb(str,i);
  }



#if 0
///////////////////////////////////////////////////////////////////////////////
template<typename T              // тип (класс) аппоратного UART или USART
    ,T &uart                     // ссылка на класс аппоратного UART (USART)
    , int _baud             // битовая скорасть
    // rx
    , uint32_t _RX_BUFFER_SIZE// размер буфера приема RX
    // tx
    , uint32_t _TX_BUFFER_SIZE// размер буфера передачи
    , uint32_t _TX_FILE_SIZE // число строк в буфере передачи
    ,void  TxDMAcomplete(void)    // функция обработк прерывания TX
>class com : public comBASE<T,uart,_baud,_RX_BUFFER_SIZE,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete>
{};
/////////////////////////////////////////////////////////////////////////////////

template<UART &uart                     // ссылка на класс аппоратного UART (USART)
    , int _baud             // битовая скорасть
    // rx
    , uint32_t _RX_BUFFER_SIZE// размер буфера приема RX
    // tx
    , uint32_t _TX_BUFFER_SIZE// размер буфера передачи
    , uint32_t _TX_FILE_SIZE // число строк в буфере передачи
    ,void  TxDMAcomplete(void)    // функция обработк прерывания TX
>class com : public comBASE<UART,uart,_baud,_RX_BUFFER_SIZE,_TX_BUFFER_SIZE,_TX_FILE_SIZE,TxDMAcomplete>
{
public:
    void SetBaudRate(uint32_t baudrate)
    {
        uart::BRGRr.CD = SystemCoreClock/baudrate/16;
        uart.MRr.PAR = T::MR::PAR_e::eNO;
        uart.MRr.CHMODE = T::MR::CHMODE_e::eNORMAL;
    };
};
#endif


#endif //__COM_HXX__


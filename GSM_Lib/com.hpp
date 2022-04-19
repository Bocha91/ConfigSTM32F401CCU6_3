/**
 * COM иа USART STM32F401CC в проекте на FreeRTOS
 *
 * @author Терещенко Виктор (iKPYT)
 * @version 22.03.2022 ( 13:56:54 )
 */
/*
- Когда выводим пишем в кольцевой буфер и в прерывании забираем из буфера,
  размер буфера выбираем таким чтобы в 99% случаев всё помещалось в него
  - если буфер полон, висим и ждём когда появится место

- Когда приём, принимаем в кольцевй буфер в прерывании, если буфер полон
  затераем старые данные новыми
  - когда буфер пуст, ждём прихода новых данных

- Наш класс будет заточен на конкретный номер UART, если надо другой или поменять
номер UART то надо менять исходнимк



*/
#include "virtualUart.hpp"
typedef uint32_t timeout_t;
//#include "Buffer.hxx"
#include "usrlib.h"

#define TX_BUFF_SIZE 256
#define RX_BUFF_SIZE 256
// моя версия
#define _i_HAL_UART_DISABLE_IT(__HANDLE__, __INTERRUPT__) ((((__INTERRUPT__) >> 28U) == UART_CR1_REG_INDEX) ? ((__HANDLE__)->CR1 &= ~((__INTERRUPT__)&UART_IT_MASK)) : (((__INTERRUPT__) >> 28U) == UART_CR2_REG_INDEX) ? ((__HANDLE__)->CR2 &= ~((__INTERRUPT__)&UART_IT_MASK)) \
                                                                                                                                                                                                                        : ((__HANDLE__)->CR3 &= ~((__INTERRUPT__)&UART_IT_MASK)))

#define _i_HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__) ((((__INTERRUPT__) >> 28U) == UART_CR1_REG_INDEX) ? ((__HANDLE__)->CR1 |= ((__INTERRUPT__)&UART_IT_MASK)) : (((__INTERRUPT__) >> 28U) == UART_CR2_REG_INDEX) ? ((__HANDLE__)->CR2 |= ((__INTERRUPT__)&UART_IT_MASK)) \
                                                                                                                                                                                                                      : ((__HANDLE__)->CR3 |= ((__INTERRUPT__)&UART_IT_MASK)))

#include "cmsis_os2.h"
inline osStatus_t OS_sleep(timeout_t timeout) { return osDelay(timeout);} // подстроено под FreeRTOS

class t_COM : public virtualUart
{
public:
    static const int BAUD_RATE = 115200;
    static const int SYS_TICK = 1000;
    static const int CHARTIK = (BAUD_RATE / SYS_TICK / 10) > 1 ? BAUD_RATE / SYS_TICK / 10 : 1; // 11,5 символов за тик ОС
    static const int CHAR_4 = CHARTIK * 4;

    inline uint32_t WAIT(uint32_t ch)
    {
        uint32_t tmp = (BAUD_RATE / SYS_TICK / 10) * ch;
        return (tmp > 0) ? tmp : 1;
    }

    __IO uint32_t ErrorCode = HAL_UART_ERROR_NONE; // ошибки в поервыаниях

    t_COM() : ErrorCode(HAL_UART_ERROR_NONE)
    {
//        Init_USART1();
//        Rx.EnableInterrupt();
//        t_Tx(); 
//        t_Rx();
    };


    // Tx  -----------------------------------------------------

    class t_Tx
    {
    public:
        __IO HAL_UART_StateTypeDef State;
        // CirBuffer2R<uint8_t, TX_BUFF_SIZE, CHAR_4 + 1> buf;
        ring_buffer<uint8_t, TX_BUFF_SIZE, uint32_t> buf;
        t_Tx() : State(HAL_UART_STATE_READY){ ; }
        void DisableInterrupt(void) { _i_HAL_UART_DISABLE_IT(USART1, UART_IT_TXE); }
        void EnableInterrupt(void)  { _i_HAL_UART_ENABLE_IT(USART1, UART_IT_TXE); }
        void DisableInterruptTC(void) { _i_HAL_UART_DISABLE_IT(USART1, UART_IT_TC); }
        void EnableInterruptTC(void)  { _i_HAL_UART_ENABLE_IT(USART1, UART_IT_TC); }
    } Tx;


    void UART_Transmit_IT(void){
        if (Tx.buf.get_count() == 0){
            Tx.DisableInterrupt();
            Tx.EnableInterruptTC();
            Tx.State = HAL_UART_STATE_BUSY_TX; // если надо поймать окончание вывода
            return;
        }
        USART1->DR = (uint8_t)(Tx.buf.pop_item() & (uint8_t)0x00FF);
        return;
    }

    void UART_EndTransmit_IT(void){
        Tx.DisableInterruptTC();
        Tx.State = HAL_UART_STATE_READY; // если надо поймать окончание вывода
    }

    void Put(char ch){
        while(Tx.buf.get_free_size() == 0 ) { OS_sleep( WAIT(2) ); }// ждать места
        taskENTER_CRITICAL();
        Tx.DisableInterrupt();

        Tx.buf.push_item(ch);
        
        Tx.EnableInterrupt();
        taskEXIT_CRITICAL();
        return;
    }
    virtual void Puts(char* str,uint32_t len)
    {
        while(len){
            Put(*str++);
            --len;
        }
    }
    virtual void Puts(char* str)
    {   
        while(*str != 0)
        {
            Put(*str++); // простой вариант
        }
    }
    //virtual void Puts(const char* const str, const uint32_t len){}
    //virtual void Puts(const char* const str){}
    //virtual void Putsb(char* str,uint32_t len){}
    //virtual void Putsb(char* str){}

    // Rx  -----------------------------------------------------
    class t_Rx
    {
    public:
        // CirBuffer2R<uint8_t, RX_BUFF_SIZE, CHAR_4 + 1> buf;
        ring_buffer<uint8_t, RX_BUFF_SIZE, uint32_t> buf;
        t_Rx() 
        {
            buf.flush();
        }

        void DisableInterrupt(void){ _i_HAL_UART_DISABLE_IT(USART1, UART_IT_RXNE); }
        void EnableInterrupt(void) { _i_HAL_UART_ENABLE_IT(USART1, UART_IT_RXNE); }
    } Rx;

    // поскольку это прерывание, можно не бояться переключения задачь??? надо приоритет выше чем у ОС
    void UART_Receive_IT(void)
    {
        if (Rx.buf.get_free_size() == 0)
            Rx.buf.pop_item(); // выкинем самый старый если нет места
        Rx.buf.push_item((uint8_t)(USART1->DR & (uint8_t)0x00FF));
    }
    
    uint32_t kbhit() { return Rx.buf.get_count(); }

    char Get()
    {
        while (!kbhit()){ OS_sleep(WAIT(1)); }

        taskENTER_CRITICAL();
        Rx.DisableInterrupt();

        char data = Rx.buf.pop_item();

        Rx.EnableInterrupt();
        taskEXIT_CRITICAL();
        return data;
    }
    uint32_t Gets(char *b, uint32_t space)
    {

// надо дописать
while(1){;}
/*
        if (space == 0) return space;
        register uint32_t d = 0; // указатель куда присать
        taskENTER_CRITICAL();
        Rx.DisableInterrupt();
        // вычисляем указатель и длинну

    // надо дописать

        Rx.EnableInterrupt();
        taskEXIT_CRITICAL();
        return space;
*/
    }
};
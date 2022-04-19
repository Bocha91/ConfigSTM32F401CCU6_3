// ************* DEBUG LIB **********************
#define __DEBUG_LIB_CPP
#include "platform_def.h"
#include "StrLib.hpp"
#include "DebugLib.hpp"
#include <string.h>

// размер экрана терминала 50х132
#define MAX_LINE  50
#define MAX_ROW   132


extern WEAK int FlashDebug(void);
int EnableDebug(void){return 1;}
WEAK_LINK(  FlashDebug, EnableDebug );

extern void dbgOpen(void);
extern void dbgPuts(const char *str);
extern void dbgPutChar(char ch);
extern long dbgkbhit(void);
extern char dbgGetChar(void);

void  _vdbg::hom(void){ X=_w.End; Y=1;};
//void  _vdbg::init(void){ this->hom(); this->puts("\r\n\r\n\r\n\r\n"); this->puts(_w.Name); this->puts("\r\n"); }
void  _vdbg::init(void){ this->hom(); this->puts(""); dbgPuts("\r\n \r\n \r\n \r\n   "); dbgPuts(_w.Name); dbgPuts("\r\n"); }
const char* _vdbg::Name(void){ return( _w.Name);}
bool  _vdbg::isName(char* s){  return( strncmp(_w.Name, s, strlen(_w.Name) )==0 );}
uint8 _vdbg::ID(void){return _w.Home;}
//bool  _vdbg::isID(uint8 id){ return(_w.Home == id);}
//void  _vdbg::addEVENT(void){ if(_w.exe) Add_Event(_w.exe,_w.event); }
void  _vdbg::InputConsol(char* s){ _w.input(s);}
void  _vdbg::putHex(const char s)
{
  char st[4]; char *d;
  HtoS( d=&st[0],s );
  st[2]=' ';
  st[3]=0;
  puts(st);
}
void _vdbg::putDec(const char c)
{
  char st[4];
  uint8toA(st,c);
  puts(st);
}
void _vdbg::itoa(const int n) //
{
  char st[16];
  puts(::itoa(n,st));
}

void _vdbg::puts(const char *p)
{
  static _vdbg* save=0;
  if( save != this )
  {
    save = this;
    dbgPuts( this->_w.Focus );            // переустановить окно
  }
  dbgPuts( p );
}
#if 0
void _vdbg::puts(const char *p)
{
  uint8 c;
  char buf[9];
  if( FlashDebug() == 0 )  return;           // выйти без отладки
  // запомнить позицию курсора и установить активным окно отладки
  // оптимизация вывода на экран
  static _vdbg* save=0;
  if( save != this )
  {
    save = this;
    dbgPuts( this->_w.Focus );            // переустановить окно
    // переустановить позицию вывода в окне
    dbgPuts("\x1b[" );
    char *p=buf;
    p=uint8toA(p, this->X );
    *p++=';';
    p=uint8toA(p, this->Y );
    *p++='H';
    *p=0;
    dbgPuts( buf );
  }

  // вывести данные
  while ((*p)!=0)
  {
    dbgPutChar( c = *p); p++;
    switch ( c )
    {
      case 0x0A :                               // LF
        if( X != _w.End ) ++X;
        break;
      case 0x0D :                               // CR
        Y = 1;
        break;
      case 0x09 :                               // Tab
        Y |= 0x07;                        // установим младшие разрядв в 111 чтобы default  увеличил на 1
        //break;
      default:                                  // все другие символы
        if( ++Y > MAX_ROW ) Y = MAX_ROW;
        break;
    }
  }
  // восстановить позицию курсора
}
#endif
extern _vdbg TASK;
extern _dbg debug;
/**
 * Обнулить позиции курсоров всех окон отладки
 */
void _dbg::ReOpenWindows( void )
{
  dbgOpen();
  //extern void _debugInit(void);_debugInit();
//  for( int i=1; i<debug.len();i++ ) debug.list[i]->puts("");
  for( int i=1; i<debug.len();i++ ) debug.list[i]->init();
   extern void postInitDebug(void); postInitDebug();
}
/* -----------------05.06.2002 13:11-----------------
 * Обработка Прямого управления, программирования и отладки через UART
 * --------------------------------------------------*/
// Настройки HyperACCESS 8.32 для работы с консолькой
//    Терминал эмулятор - ANSI; Settings IBM PC scan code
//    Terminal Keys
//    Bsckspase key send - Del
//    Cursor - Underline + Blink
//    row - 50 columns - 132
//    Com2 115200 8-none-1

// Описание рабвоты консльки
// Вывод отладочной информации происходит в верхние 40 строк экрвана
// 10 нижних строк экрана выделены под консольку. Редактируемая строка 49-а
// в 48-ой строке выводится полное содержимое буфера.
// ввод символов записывается в консольный буфер, пока не будет надат Enter
// стрелки влево и в право перемешают курсор (которого на экране как такового нет) покоммандной строке.
// BackSpase  - удаляет символ перед курсором из буфера и придвигает на его место оставшуюся часть строки после курсора.
// Del - удаляет символ после курсора и придвигает на его место оставшуюся часть строки после курсора.
// Insert - вставляет пробел в позицию курсора раздвигая строку на один символ в позиции курсора.
// просто вводимые символы заменяют символы в буфере.


// по умолчанию отладчик с консолькой выключен. Чтобы его включит нужно ввести Ctrl+D+<Enter>
// а затем ответить (Y/y) на запрос "Debug?"
// если все провильно то отладчик включается.

// Ввод Данных с терминала в буфер консоли с редактированием и
// запоминанием предыстории и макросами набора комманд
//
// Задача 1 - переключение между режимами  отладка включена/выключена
// Решение1 - Команда Ctrl+D+<CR>
//            Запрос на включение Debug On
//            Ответ "ON"/"on"+<CR> - включает отладку
//            Любой другой символ выключает отладку
//
// Задача 2 - Буфер редактирования ( только в режиме отладки
// Решение1 - Длинна буфера 64 символа
//            Left / Right - перемещение по буферу
//            Ctrl+I включение вставки (следующий набираемый символ будет вставляться между символами )
//            Ctrl+R вывод строки из буфера на экран (Redraw)
/*
enum  NameFlag
{
   Off   =  0
  ,On
};

enum  NameEsc
{
   Esc0   =  0
  ,Esc1
  ,Esc2
};

struct T_rCONSOL
{
  NameEsc  Esc    :2;
  NameFlag Debug  :1;                           // включение режима отладки
  uint8    Out;                                 // куда вести вывод строки из консоли
};

T_rCONSOL   rConsol;
LinBuffer<char, 127> Consol;
*/

//int ConsolOn( void ) { return( dbgkbhit() ); } // наличие в буфере символа

void _dbg::Init( void )
{                                               // ожидается правильный ответ для включения отладки
  consol.Debug = _consol::On;
  consol.Esc = _consol::Esc0;
  consol.Out = TASK.ID();
  //extern void addConsolRun(void); addConsolRun();
  consol.buf.Clear();
  ReOpenWindows();

  // Для уменьшения джиттера буферизация вывода отладки
//  for( int i=1; i<debug.len();i++ )
//  {
//    debug.list[i]->addEVENT();
//  }
//  ADD_EVENT( DbgGSM,Draw );
//  ADD_EVENT( DbgREEF,Draw );
}

void _dbg::_redraw( void )
{
  for( int i=1; i<debug.len();i++ )
  {
    if( debug.list[i]->ID() == consol.Out )
    {
      debug.list[i]->puts("\r\n");
      dbgPuts(debug.list[i]->Name());
      return;
    }
  }
  TASK.puts( TASK.Name());
}



bool _dbg::ChangeFocus(void)
{
  for( int i=1; i<debug.len();i++ )
  {
    if( debug.list[i]->isName( consol.buf.Read()) )
    {
      consol.Out = debug.list[i]->ID();
      return true;
    }
  }
  return false;
}

//void ConsolRun(void)
void _dbg::Run(void)
{//обработыик ввода - КОНСОЛКА
  while( dbgkbhit() )
  {
    switch( uint8 c = dbgGetChar() )
    {
      case 0x0A :                             // игнорируем
        break;
      case 0x0D :
        if( ChangeFocus() )
        {// только вывеси промпт
          _redraw();
          dbgPutChar('>');
        }else
        { // отображение всего буфера и текущей редактируемой строки
          //EchoConsol();
          _redraw();
          dbgPutChar('>');
          dbgPuts(consol.buf.Read());
          consol.buf.Write('\r');consol.buf.Write('\n');
          for( int i=1; i<debug.len();i++ )
          {
            if( debug.list[i]->ID() == consol.Out )
            {
              debug.list[i]->InputConsol(consol.buf.Read());
            }
          }
        }
        consol.buf.Clear();
        return;//break; // выходим после каждой строки
      default:
        // добавление в конец буфера
        consol.buf.Write(c);
        break;
    }
  }
  //extern void addConsolRun(void); addConsolRun();// перенаправить на отладчик
}



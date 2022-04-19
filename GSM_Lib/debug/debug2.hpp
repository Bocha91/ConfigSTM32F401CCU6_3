#ifndef  __DEBUG2_H__
 #define  __DEBUG2_H__

#include "Buffer.hxx"

#define DBGport  DECLARE_DEBUG_INTERFASE
#define NO_ECHO( ... )

#ifndef __DEBUG2_CPP__
  // объявляем доступными всем отладочные обьекты (дескрипторы окон отладки)
  #define EVENT_NAME_ECHO(n)  Dbg##n
  #define ECHO(n) extern LinBuffer< char, 256 > echo##n;
  #define DECLARE_DEBUG_WIN(name,h,e,echo)               \
          extern _vdbg name;                             \
          echo(name)
  DECLARE_DEBUG_WIN( TASK,  1 , 22 , NO_ECHO )
  #include "ConfigDebug.hpp"
  #undef  DECLARE_DEBUG_WIN
//  extern void InitConsol( void );
  extern _dbg debug;
  #define DBG_INPUT_NAME(name)   input##name
  #undef  ECHO
  #define ECHO(n) echo##n

#else// == __DEBUG2_CPP__
  // обработчики для окон с эхом
  #undef  ECHO
  #define ECHO( n ) extern _vdbg n;                      \
  LinBuffer< char, 256 > echo##n;                        \
  int Dbg##n##Redy( void ){ return echo##n.GetCount(); } \
  EVENT( Dbg##n,Draw,Redy,NoDebug )                      \
  { if( echo##n.GetCount() )                             \
    { echo##n.Write( 0 );                                \
      n.puts( echo##n.Read() );                          \
      echo##n.Clear();                                   \
    }ADD_EVENT( Dbg##n,Draw );                           \
  }
  #define DECLARE_DEBUG_WIN(name,h,e,echo)  echo( name )
  DECLARE_DEBUG_WIN( TASK,  1 , 22 , NO_ECHO )
  #include "ConfigDebug.hpp"
  #undef  DECLARE_DEBUG_WIN
  // заглушки для функций консольного ввода
  extern WEAK void inputNot(char *s){;}
  #define DECLARE_DEBUG_WIN(name,h,e,echo)  \
  extern WEAK void input##name(char *s);    \
  WEAK_LINK( input##name, inputNot );
  DECLARE_DEBUG_WIN( TASK,  1 , 22 , NO_ECHO )
  #include "ConfigDebug.hpp"
  #undef DECLARE_DEBUG_WIN
  // определяем параметры отладочных окон и объекты отладки
  #define DBGCC(a)  #a
  #define DBGPP( a , b , c , d , e , f , g , h , i )   DBGCC(a##b##c##d##e##f##g##h##i)
  #define DBGPC( a , b , c , d , e , f , g , h , i )   DBGPP(a , b , c , d , e , f , g , h , i )
  #undef  ECHO
  #define ECHO( n )       &ev##Dbg##n##Draw , ne##Dbg##n
  #undef  NO_ECHO
  #define NO_ECHO( n )    0 , 0
  #define DECLARE_DEBUG_WIN(name,h,e,echo)  \
  const _cdbg win##name={ &input##name, /*echo(name) ,*/ #name , DBGPC(\x1b[ , h , ; , e , r\x1b[ , e , ; , 1 , H  ) , h , e };  \
  _vdbg name(win##name);
  DECLARE_DEBUG_WIN( TASK,  1 , 22 , NO_ECHO )
  #include "ConfigDebug.hpp"
  #undef  NO_ECHO
  #define NO_ECHO( ... )
  #undef  ECHO
  #define ECHO( ... )
  #undef  DECLARE_DEBUG_WIN
  // составляем список окон
  #define DECLARE_DEBUG_WIN(name,h,e,echo)  ,&name
  const pdbg dList[]=
  {
    0
    DECLARE_DEBUG_WIN( TASK,  1 , 22 , NO_ECHO )
    #include "ConfigDebug.hpp"
  };
  #undef DECLARE_DEBUG_WIN
  // создвем объект список отладочных обьектов
  extern const _dbg debug( dList );
  int _dbg::len(void){return  sizeof(dList)/sizeof(_vdbg*);}
  // евенет для консоли ввода команд
  //void addConsolRun(void){ ADD_EVENT( Consol,Run ); }// перенаправить на отладчик
  //EVENT( Consol,Run,On,NoDebug ){ extern void ConsolRun(void); ConsolRun(); }
  //extern void Add_Event(const T_EVENT *event, uint8 Name){add_event(event,(NameEvent)Name);}
#endif // __DEBUG2_CPP__
 //#undef NO_ECHO
 //#undef ECHO
#endif //__DEBUG2_H__







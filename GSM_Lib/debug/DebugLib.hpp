// ************* DEBUG LIB **********************
#include "Buffer.hxx"
// константный описатель экземрляра отладочного окна
class _cdbg
{
protected:
private:
public:
   void (*input)(char *s);                   // указатель на функцию обработки сторки консольного ввода
//   T_EVENT const *exe;
//   uint8 event;
   char Name[7];                            // имя окна
   char Focus[17];                          // строка переключения фокуса
   uint8 Home;                              // позиция начала окна
   uint8 End;                               // позиция конца окна
};
// переменная часть описателя экземрляра отладочного окна
class _vdbg
{
  const _cdbg& _w;                              // параметры окна
  uint8 X;                                      // координата курсора по строкам
  uint8 Y;                                      // координата курсора по столбцам
public:
  _vdbg(const _cdbg& w):_w(w){hom();}
  void  hom(void);                              // домой для своего окна
  void  puts(const char *p);                    // вывод в свое окно
  void  putHex(const char s);                   // ??
  void  putDec(const char c);                   // см StrLib.hpp uint8toA
  void  itoa(const int n);                      // см StrLib.hpp itoa
  void  init(void);                             // инициализация своего окна
  bool  isName(char* s);                        // проверяет наличие в начале строки имени своего окна
  const char* Name(void);                       // вернет строковое прдстваление имени окна
  uint8 ID(void);                               // вернет ID своего окна
//  bool  isID(uint8 id);                       // вернет истинну если id окна совпадает с переданным параметром id
  void  addEVENT(void);                         // стартуем евент для окон с эхом
  void  InputConsol(char *s);                   // обработчик строки ввод с консоли
};

typedef _vdbg* pdbg; // указатель на окно


class _consol
{
public:
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

  NameEsc  Esc    :2;
  NameFlag Debug  :1;                           // включение режима отладки
  uint8    Out;                                 // куда вести вывод строки из консоли
  LinBuffer<char, 127> buf;
};


class _dbg
{

  _consol consol;

  void ReOpenWindows(void); // переоткрыть все окна и сбросить положения курсоров
  bool ChangeFocus(void);
  void _redraw( void );
public:
  _dbg(pdbg const * list) : list(list) {;}
  pdbg const *list;
  int  len(void);
  void Init(void);
  void Run(void);
};
//extern void Add_Event(const T_EVENT *event, uint8 Name);
extern int FlashDebug(void);


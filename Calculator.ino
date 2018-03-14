#include <stdint.h>
#include <math.h>
//#include <EEPROM.h>
#include "TouchScreen.h"
#include "BigNumber.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define TS_MINX 160
#define TS_MINY 140
#define TS_MAXX 880
#define TS_MAXY 940

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define GRAY    0x1111
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LIGHTBLUE 0x000F

#define MIN_TOUCH 10

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button button[25];

#define BUTTON_WIDTH    40
#define BUTTON_HEIGHT   40
#define BUTTON_SPACE     5
#define BUTTON_START_X  10
#define BUTTON_START_Y  85

#define _EEPROM_READ     0
#define _EEPROM_WRITE    1
#define COUNT_PAGE       4

#define BUTTON_0        '0'
#define BUTTON_1        '1'
#define BUTTON_2        '2'
#define BUTTON_3        '3'
#define BUTTON_4        '4'
#define BUTTON_5        '5'
#define BUTTON_6        '6'
#define BUTTON_7        '7'
#define BUTTON_8        '8'
#define BUTTON_9        '9'
#define BUTTON_A        '9'+1
#define BUTTON_B        '9'+2
#define BUTTON_C        '9'+3
#define BUTTON_D        '9'+4
#define BUTTON_E        '9'+5
#define BUTTON_F        '9'+6

#define BUTTON_NONE     '\0'
#define BUTTON_PLUS     '+'
#define BUTTON_MINUS    '-'
#define BUTTON_KRAT     'x'
#define BUTTON_DELENE   '/'
#define BUTTON_ROVNASA  86
#define BUTTON_LEFT_BR  '('
#define BUTTON_RIGHT_BR ')'

#define BUTTON_PLUS_MINUS 1
#define BUTTON_CIARKA   2
#define BUTTON_INVERT   3
#define BUTTON_PERCENTO 4
#define BUTTON_NEXT     5
#define BUTTON_PREV     6
#define BUTTON_CE       7
#define BUTTON_CLS      8
#define BUTTON_X        9
#define BUTTON_EX       10
#define BUTTON_LN       11
#define BUTTON_INT      12
#define BUTTON_SINH     13
#define BUTTON_SIN      14
#define BUTTON_SQR      15
#define BUTTON_SQRT     16
#define BUTTON_FACT     17
#define BUTTON_DMS      18
#define BUTTON_COSH     19
#define BUTTON_COS      20
#define BUTTON_SQRY     21
#define BUTTON_SQRTY    22
#define BUTTON_PI       23
#define BUTTON_TANH     24
#define BUTTON_TAN      25
#define BUTTON_ABS      26
#define BUTTON_EXP      27
#define BUTTON_MOD      28
#define BUTTON_LOG      29
#define BUTTON_10X      30

#define BUTTON_DEC      65
#define BUTTON_HEX      66
#define BUTTON_OCT      67
#define BUTTON_BIN      68
#define BUTTON_NOT      69
#define BUTTON_AND      70
#define BUTTON_OR       71
#define BUTTON_XOR      72
#define BUTTON_NOR      73
#define BUTTON_ROL      74
#define BUTTON_ROR      75
#define BUTTON_LSH      76
#define BUTTON_RSH      77
#define BUTTON_STUPNE   78
#define BUTTON_RADIANY  79
#define BUTTON_GRADIANY 80
#define BUTTON_MC       81
#define BUTTON_MR       82
#define BUTTON_MS       83
#define BUTTON_M_PLUS   84
#define BUTTON_M_MINUS  85

typedef struct {
  unsigned char id;
  const char* text;
} T_calc_button;

const T_calc_button def_button[COUNT_PAGE][25] = {
 {
  { BUTTON_NEXT,     "<-"   },    // page[0]
  { BUTTON_CE,       "CE"   },
  { BUTTON_CLS,      "C"    },
  { BUTTON_PLUS_MINUS,"\xF0"},    // +/-
  { BUTTON_X,        "n"  },

  { BUTTON_7,        "7"    },
  { BUTTON_8,        "8"    },
  { BUTTON_9,        "9"    },
  { BUTTON_DELENE,   "/"    },
  { BUTTON_PERCENTO, "%"    },

  { BUTTON_4,        "4"    },
  { BUTTON_5,        "5"    },
  { BUTTON_6,        "6"    },
  { BUTTON_KRAT,     "x"    },
  { BUTTON_INVERT,   "1/x"  },

  { BUTTON_1,        "1"    },
  { BUTTON_2,        "2"    },
  { BUTTON_3,        "3"    },
  { BUTTON_MINUS,    "-"    },
  { BUTTON_ROVNASA,  "="    },
  
  { BUTTON_0,        "0"    },
  { BUTTON_NONE,     ""     },
  { BUTTON_CIARKA,   ","    },
  { BUTTON_PLUS,     "+"    },
  { BUTTON_NONE,     ""     },
 },
 {
  { BUTTON_NEXT,     "<-"   },    // page[1]
  { BUTTON_PREV,     "->"   },
  { BUTTON_FACT,     "n! "  },
  { BUTTON_LEFT_BR,  "("    },
  { BUTTON_RIGHT_BR, ")"    },

  { BUTTON_INT,      "int"  },
  { BUTTON_SINH,     "sinh" },
  { BUTTON_SIN,      "sin"  },
  { BUTTON_SQR,      "x\xFC "},   // x^2
  { BUTTON_SQRT,     "\xFC\xFAx" },  // 2Vx

  { BUTTON_DMS,      "dms"  },
  { BUTTON_COSH,     "cosh" },
  { BUTTON_COS,      "cos"  },
  { BUTTON_SQRY,     "x\xFB "},
  { BUTTON_SQRTY,    "y\xFAx" }, // yVx

  { BUTTON_PI,       "\xE2" },   // Pi
  { BUTTON_TANH,     "tanh" },
  { BUTTON_TAN,      "tan"  },
  { BUTTON_EX,       "e\xFB "},   // e^x
  { BUTTON_LN,       "ln "  },

  { BUTTON_ABS,      "abs"  },
  { BUTTON_EXP,      "exp"  },
  { BUTTON_MOD,      "mod"  },
  { BUTTON_LOG,      "log"  },
  { BUTTON_10X,      "10\xFB"},  // 10^x
 },
 {
  { BUTTON_NEXT,     "<-"   },    // page[2]
  { BUTTON_PREV,     "->"   },
  { BUTTON_STUPNE,   "Deg"  },
  { BUTTON_RADIANY,  "Rad"  },
  { BUTTON_GRADIANY, "Grad" },

  { BUTTON_HEX,      "Hex"  },
  { BUTTON_DEC,      "Dec"  },
  { BUTTON_OCT,      "Oct"  },
  { BUTTON_BIN,      "Bin"  },
  { BUTTON_NONE,     ""     },

  { BUTTON_ROL,      "RoL"  },
  { BUTTON_ROR,      "RoR"  },
  { BUTTON_LSH,      "Lsh"  },
  { BUTTON_RSH,      "Rsh"  },
  { BUTTON_NONE,     ""     },

  { BUTTON_NOT,      "Not"  },
  { BUTTON_AND,      "And"  },
  { BUTTON_OR,       "Or "  },
  { BUTTON_XOR,      "Xor"  },
  { BUTTON_NOR,      "Nor"  },

  { BUTTON_MC,       "MC"   },
  { BUTTON_MR,       "MR"   },
  { BUTTON_MS,       "MS"   },
  { BUTTON_M_PLUS,   "M+"   },
  { BUTTON_M_MINUS,  "M-"   },
 },
 {
  { BUTTON_NEXT,     "<-"   },    // page[3]
  { BUTTON_CE,       "CE"   },
  { BUTTON_CLS,      "C"    },
  { BUTTON_PLUS_MINUS,"\xF0"},    // +/-
  { BUTTON_DELENE,   "/"    },

  { BUTTON_C,        "C"    },
  { BUTTON_D,        "D"    },
  { BUTTON_E,        "E"    },
  { BUTTON_F,        "F"    },
  { BUTTON_KRAT,     "x"    },

  { BUTTON_8,        "8"    },
  { BUTTON_9,        "9"    },
  { BUTTON_A,        "A"    },
  { BUTTON_B,        "B"    },
  { BUTTON_MINUS,    "-"    },

  { BUTTON_4,        "4"    },
  { BUTTON_5,        "5"    },
  { BUTTON_6,        "6"    },
  { BUTTON_7,        "7"    },
  { BUTTON_PLUS,     "+"    },

  { BUTTON_0,        "0"    },
  { BUTTON_1,        "1"    },
  { BUTTON_2,        "2"    },
  { BUTTON_3,        "3"    },
  { BUTTON_ROVNASA,  "="    },
 }
};

byte page;
double cislo, cislo_prev;
BigNumber cislob;
float nasobitel;
byte des_miest;
double memory;
char stupne;
char sustava;
char oper;
// -----------------------------------------------------------------------------

void showActualPage()
{
  byte font_size;
  short x,y;
  short width, height;

  tft.fillRect(0, 81, 240, 230, BLACK);
  for(int i=0; i<25; i++)
  {
    x = BUTTON_START_X + BUTTON_WIDTH/2 + (i%5)*(BUTTON_WIDTH + BUTTON_SPACE);
    y = BUTTON_START_Y + BUTTON_HEIGHT/2 + (i/5)*(BUTTON_HEIGHT + BUTTON_SPACE);
    width = BUTTON_WIDTH;
    height = BUTTON_WIDTH;
    font_size = (strlen(def_button[page][i].text) > 2)? 1: 2;

    // dva vynimky
    if(( def_button[page][i].id == BUTTON_0 )
    && ( page == 0 ))
    {
      x = BUTTON_START_X + BUTTON_WIDTH + BUTTON_SPACE/2 + (i%5)*(BUTTON_WIDTH + BUTTON_SPACE);
      width = 2*BUTTON_WIDTH + BUTTON_SPACE;
    }

    if(( def_button[page][i].id == BUTTON_ROVNASA )
    && ( page == 0 ))
    {
      y = BUTTON_START_Y + BUTTON_HEIGHT + BUTTON_SPACE/2 + (i/5)*(BUTTON_HEIGHT + BUTTON_SPACE);
      height = 2*BUTTON_HEIGHT + BUTTON_SPACE;
    }

    button[i].initButton (&tft, x, y, width, height, 1, BLUE, YELLOW
                              , (char*) def_button[page][i].text, font_size);

    if( def_button[page][i].id != BUTTON_NONE )
        button[i].drawButton (false);
  }
}
// -----------------------------------------------------------------------------


double toRadiany(double cislo)
{
   switch( stupne )
   {
    case BUTTON_STUPNE :
          cislo = cislo/180 * PI;
          break;
    case BUTTON_GRADIANY :
          cislo = cislo/200 * PI;
          break;
    case BUTTON_RADIANY :
    default:
          break;
   }
   
   return cislo;
}
// -----------------------------------------------------------------------------


double factorial(unsigned short cislo)
{
  double vysledok = 1;
  while(--cislo)
    vysledok = vysledok * (vysledok+1);

  return vysledok;
}
// -----------------------------------------------------------------------------


double toDMS(double cislo)
{
  double vysl;
  double intpart;

  vysl = modf(cislo, &intpart);
  return intpart + (vysl * 60)/100;
}
// -----------------------------------------------------------------------------


byte decimalCount(double cislo)
{
  double wrap_cislo;
  
  if( modf(cislo, &wrap_cislo) == 0 )
    return 0;

  return 8;
}
// -----------------------------------------------------------------------------


double countToNow()
{
  double vysl;
  switch( oper )
  {
    case BUTTON_PLUS :
         vysl = cislo_prev + cislo;
         break;
    case BUTTON_MINUS :
         vysl = cislo_prev - cislo;
         break;
    case BUTTON_KRAT :
         vysl = cislo_prev * cislo;
         break;
    case BUTTON_DELENE :
         vysl = cislo_prev / cislo;
         break;
    default:
         vysl = cislo;
         break;
  }
  oper = 0;
  return vysl;
}
// -----------------------------------------------------------------------------


void getPrevPage()
{
  if( sustava == BUTTON_HEX )
  {
      if( page <= 1 )
          page = COUNT_PAGE - 1;
      else  
          page--;
  }
  else
  {
      if( page <= 0 )
          page = COUNT_PAGE - 2;
      else  
          page--;
  }
}
// -----------------------------------------------------------------------------


void getNextPage()
{
  if( sustava == BUTTON_HEX )
  {
      page++;
      if(page >= COUNT_PAGE)
          page = 1;
  }
  else
  {
      page++;
      if(page >= (COUNT_PAGE - 1))
          page = 0;
  }
}
// -----------------------------------------------------------------------------


byte getSustava()
{
  switch( sustava )
  {
     case BUTTON_HEX :  return 16; break;
     case BUTTON_OCT :  return  8; break;
     case BUTTON_BIN :  return  2; break;
     case BUTTON_DEC :
     default:           return 10; break;
  }
}
// -----------------------------------------------------------------------------


void deaktivNasobitel(char stiskButton)
{
   switch( stiskButton )
   {
     case BUTTON_0 :
     case BUTTON_1 :
     case BUTTON_2 :
     case BUTTON_3 :
     case BUTTON_4 :
     case BUTTON_5 :
     case BUTTON_6 :
     case BUTTON_7 :
     case BUTTON_8 :
     case BUTTON_9 :
     case BUTTON_CIARKA :
     case BUTTON_PLUS_MINUS :
          break;
     default:
          nasobitel = 10;
   }
}
// -----------------------------------------------------------------------------


void viewCisla()
{
  tft.fillRect(2, 2, 236, 62, BLACK);     
  tft.setTextColor(YELLOW);

  if(( oper != 0 ) && (oper != BUTTON_ROVNASA))
  {
     tft.setTextSize(3);
     tft.setCursor(6, 6);
     switch( sustava )
     {
       case BUTTON_HEX: tft.println((unsigned long)cislo_prev, HEX); break;
       case BUTTON_OCT: tft.println((unsigned long)cislo_prev, OCT); break;
       case BUTTON_BIN: tft.setTextSize(2); tft.println((unsigned long)cislo_prev, BIN); break;
       case BUTTON_DEC: 
       default:
                        tft.print(cislo, des_miest? des_miest: decimalCount(cislo_prev)); break;
     }
     
     tft.setTextSize(1);
     tft.setCursor(10, 30);
     tft.print(oper);
  }
  
  tft.setTextSize(3);
  tft.setCursor(6, 40);
  switch( sustava )
  {
    case BUTTON_HEX: tft.println((unsigned long)cislo, HEX); break;
    case BUTTON_OCT: tft.println((unsigned long)cislo, OCT); break;
    case BUTTON_BIN: tft.setTextSize(2); tft.println((unsigned long)cislo, BIN); break;
    case BUTTON_DEC: 
    default:
                     tft.print(cislo, des_miest? des_miest: decimalCount(cislo)); break;
  }
}
// -----------------------------------------------------------------------------


void viewState()
{
  tft.fillRect(2, 68, 236, 10, BLACK);

  tft.setTextSize(1);
  tft.setCursor(4, 70);
  tft.setTextColor(memory? MAGENTA: GRAY);  tft.print("M ");
  
  tft.setTextColor((stupne == BUTTON_STUPNE  )? MAGENTA: GRAY);  tft.print("DEG ");
  tft.setTextColor((stupne == BUTTON_RADIANY )? MAGENTA: GRAY);  tft.print("RAD ");
  tft.setTextColor((stupne == BUTTON_GRADIANY)? MAGENTA: GRAY);  tft.print("GRAD ");
  
  tft.setTextColor((sustava == BUTTON_HEX)? MAGENTA: GRAY);  tft.print("HEX ");
  tft.setTextColor((sustava == BUTTON_DEC)? MAGENTA: GRAY);  tft.print("DEC ");
  tft.setTextColor((sustava == BUTTON_OCT)? MAGENTA: GRAY);  tft.print("OCT ");
  tft.setTextColor((sustava == BUTTON_BIN)? MAGENTA: GRAY);  tft.print("BIN ");
}
// -----------------------------------------------------------------------------


void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Calculator. Software by Zdeno Sekerak (c) 2015."));
  randomSeed(analogRead(0));

  tft.reset();
  tft.begin(tft.readID());

  tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.setCursor(25, 311);
  tft.print(F("Software by Zdeno Sekerak (c) 2015"));

  tft.fillRect(0, 0, 240, 80, BLUE);
  tft.fillRect(2, 2, 236, 76, BLACK);
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.setCursor(2, 6);
  tft.print(F("  Calculator"));
  tft.setCursor(4, 36);
  tft.print(F("  Scientific "));

  // init math
  BigNumber::begin();
  
  // button's
  page = 0;
  showActualPage();
  cislo = cislo_prev = 0;
  nasobitel = 10;
  des_miest = 0;
  memory = 0;
  stupne = BUTTON_STUPNE;
  sustava = BUTTON_DEC;
  //viewCisla();
  viewState();
}
// -----------------------------------------------------------------------------


void loop() 
{
  TSPoint p = ts.getPoint();
  char stiskButton = BUTTON_NONE;

  if ( p.z > MIN_TOUCH )
  {
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    for(int i=0; i<25; i++)
      if(( def_button[page][i].id != BUTTON_NONE )
      && ( button[i].contains(p.x, p.y)))
      {
        button[i].drawButton(true);
        delay(400);
        button[i].drawButton(false);
        stiskButton = def_button[page][i].id;
      }

     switch( stiskButton )
     {
       case BUTTON_0 :
       case BUTTON_1 :
       case BUTTON_2 :
       case BUTTON_3 :
       case BUTTON_4 :
       case BUTTON_5 :
       case BUTTON_6 :
       case BUTTON_7 :
       case BUTTON_8 :
       case BUTTON_9 :
       case BUTTON_A :
       case BUTTON_B :
       case BUTTON_C :
       case BUTTON_D :
       case BUTTON_E :
       case BUTTON_F :
                if( oper == BUTTON_ROVNASA )
                {
                    cislo = 0;
                    oper = 0;
                }
                if( nasobitel > 1 )
                {
                    cislo *= getSustava();
                    cislo += stiskButton - '0';
                    des_miest = 0;
                }
                else
                {
                    cislo += nasobitel * (stiskButton - '0');
                    nasobitel /= getSustava();
                    des_miest++;
                }
                break;

       case BUTTON_CE :
               cislo_prev = 0;
               oper = 0;
               /* no break */
       case BUTTON_CLS :
               cislo = 0;
               break;

       case BUTTON_CIARKA :   
               if( nasobitel == 10 )
                   nasobitel = 0.1;
               break;
       case BUTTON_PERCENTO : break;
       case BUTTON_PLUS :
       case BUTTON_MINUS :
       case BUTTON_KRAT :
       case BUTTON_DELENE :
               cislo_prev = countToNow();
               cislo = 0;
               oper = stiskButton;
               break;
       case BUTTON_ROVNASA :
               cislo = countToNow();
               cislo_prev = 0;
               oper = stiskButton;
               break;

       case BUTTON_LEFT_BR :   break;
       case BUTTON_RIGHT_BR :  break;

       case BUTTON_PLUS_MINUS :  cislo *= -1;     break;
       case BUTTON_INVERT : cislo = 1/cislo;      break;
       case BUTTON_EX :    cislo = exp(cislo);    break;
       case BUTTON_LN :    cislo = log(cislo);    break;
       case BUTTON_INT :   cislo = ceil(cislo);   break;
       case BUTTON_SINH :  cislo = sinh(toRadiany(cislo)); break;
       case BUTTON_SIN :   cislo = sin(toRadiany(cislo));  break;
       case BUTTON_SQR :   cislo = square(cislo); break;
       case BUTTON_SQRT :  cislo = sqrt(cislo);   break;

       case BUTTON_FACT :  cislo = factorial((unsigned short)cislo);  break;
       case BUTTON_DMS :   cislo = toDMS(cislo);           break;
       case BUTTON_COSH :  cislo = cosh(toRadiany(cislo)); break;
       case BUTTON_COS :   cislo = cos(toRadiany(cislo));  break;
       case BUTTON_SQRY :  cislo = pow(cislo_prev,cislo);  break;
       case BUTTON_SQRTY : cislo = exp(cislo/log(cislo_prev)); break;
       case BUTTON_PI :    cislo = M_PI;                   break;
       case BUTTON_TANH :  cislo = tanh(toRadiany(cislo)); break;
       case BUTTON_TAN :   cislo = tan(toRadiany(cislo));  break;
       case BUTTON_ABS :   cislo = abs(cislo);             break;
       case BUTTON_EXP :   cislo = cislo_prev * pow(10,cislo); break;
       case BUTTON_MOD :   cislo = fmod(cislo,1);    break;
       case BUTTON_LOG :   cislo = log10(cislo);     break;
       case BUTTON_10X :   cislo = pow(10,cislo);    break;

       case BUTTON_HEX :
       case BUTTON_DEC :
       case BUTTON_OCT :
       case BUTTON_BIN :
               sustava = stiskButton;
               viewState();
               break;
       case BUTTON_STUPNE :
       case BUTTON_RADIANY :
       case BUTTON_GRADIANY :
               stupne = stiskButton;
               viewState();
               break;
       case BUTTON_NOT : cislo = !(unsigned long)cislo; break;
       case BUTTON_AND : cislo = (unsigned long)cislo & (unsigned long)cislo_prev; break;
       case BUTTON_OR  : cislo = (unsigned long)cislo & (unsigned long)cislo_prev; break;
       case BUTTON_XOR : cislo = (unsigned long)cislo ^ (unsigned long)cislo_prev; break;
       case BUTTON_NOR : cislo = !((unsigned long)cislo ^ (unsigned long)cislo_prev); break;
       case BUTTON_ROL : cislo = (unsigned long)cislo << 1; break;
       case BUTTON_ROR : cislo = (unsigned long)cislo >> 1; break;
       case BUTTON_LSH : cislo = (unsigned long)cislo_prev << (byte)cislo; break;
       case BUTTON_RSH : cislo = (unsigned long)cislo_prev >> (byte)cislo; break;
               break;

       case BUTTON_MC : memory = 0; break;
       case BUTTON_MR : cislo = memory; break;
       case BUTTON_MS : memory = cislo; break;
       case BUTTON_M_PLUS : memory += cislo; break;
       case BUTTON_M_MINUS : memory -= cislo; break;

       case BUTTON_NEXT :
               getNextPage();
               showActualPage();
               break;
               
       case BUTTON_PREV :
               getPrevPage();
               showActualPage();
               break;
     }

     deaktivNasobitel(stiskButton);
     viewCisla();
     viewState();
  }
}
// -----------------------------------------------------------------------------


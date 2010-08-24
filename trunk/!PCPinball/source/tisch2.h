// {*** TABLE2 *************************************************************** }

extern byte RATa[26 - 24 + 1] ; // array[24..26] of byte; {R, A, T}
#define RAT(i) RATa[i-24]
extern byte PHASERa[238 - 233 + 1]; // array[233..238] of byte; {P, H, A, S, E, R}
#define PHASER(i) PHASERa[i-233]

extern boolean RL; // {RELAUNCH}
extern byte PFEIL; // {2x,4x,6x}
extern byte Felder1; // {5k,3k,1k top left}
extern byte Felder2; // {500,1000,1500}
extern byte Felder3; // {10.000,20.000,30.000,50.000}
extern byte Locked;  // { 0 - > no Ball locked // 1 - > Ball locked //
              //        2 - > locked ball already released!!! }
extern byte Counter;

extern byte TischGrad;
extern byte grad;

void Init_Tisch2();
void HilfsProc1();
void HilfsProc2();

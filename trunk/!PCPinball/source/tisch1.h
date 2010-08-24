/* PC Spiel Pinball
 *
 * (c) vIRTUAL tECHNOLOGIES 1993-1996 Ansgar Scherp, Joachim Gelhaus
 *
 * Original Pascal source released 1996
 *
 * SDL/C conversion Alan Buckley 2008
 *
 * See COPYRIGHT file for copyright details.
 *
 ********************************************************************/


// {*** TABLE1  ************************************************************** }

extern word Kurven;
extern byte Lichter1a[252 - 250 + 1]; // : array[250..252] of byte;
extern byte Lichter2a[249 - 247 + 1]; // : array[247..249] of byte;
extern byte Lichter3a[246 - 244 + 1]; //  : array[244..246] of byte;

#define Lichter1(i) Lichter1a[i-250]
#define Lichter2(i) Lichter2a[i-247]
#define Lichter3(i) Lichter3a[i-244]

extern byte Licht4;
extern boolean PushUp;
extern byte Bonus;
extern byte PCSspe[4]; //  : array[1..3] of byte;
extern byte special;
extern byte temp;
extern byte BumpCount;

// Table one functions
void Init_Tisch1();
void flameflacker();
void thermo(byte tmp);

{ for TABLE2 !!! }

procedure init_load_sound;
var poin,poin1:pointer;
begin
  getmem(poin1,1);
  getmem(poin,65536-(Seg(poin1^) Shl 4 + Ofs(poin1^))-8);
end;

procedure load_sound(soundnr:byte;soundname:string);
var f:file;
    error:word;
    poin:pointer;
begin
  Assign(f,soundname);
  {$I-}
  Reset(f,1);
  {$I+}
  getmem(poin,1);
  if 65536-(Seg(poin^) Shl 4 + Ofs(poin^))<Filesize(f) then
  begin
    getmem(poin,65536-((Seg(poin^) Shl 4 + Ofs(poin^))));
    getmem(sounds[soundnr],filesize(f));
  end else
  begin
   freemem(poin,1);
   getmem(sounds[soundnr],filesize(f));
  end;
  blockread(f,sounds[soundnr]^,filesize(f),error);
  soundlength[soundnr]:=filesize(f);
  close(f);
end;

procedure play(soundnr:byte);
begin
  if not UseSound then exit;
  dmastop;
  playback(sounds[soundnr],soundlength[soundnr],19000);
end;

 Const dsp_adr    : word = $220;
   dsp_irq        : byte = $5;
   SbRegDetected : BOOLEAN = FALSE;

 var SbVersMaj : byte;
     SbVersMin : byte;
     SbVersStr : string[5];

 function Reset_sb : boolean;
 const ready = $AA;
 var   ct, stat : BYTE;
 BEGIN
  port [ dsp_adr+ $6 ]  :=  1 ;
        for ct :=1 to 100 do;

     port [ dsp_adr+ $6 ]  :=  0 ;
     stat := 0;
     ct   := 0;
     while (stat <> ready)
     and   (ct < 100)   do begin
       stat := PORT[dsp_adr+$E];
       stat := PORT[dsp_adr+$a];
       inc(ct);
  end ;
     Reset_sb := (stat = ready);
  end;




 function Detect_Reg_sb : boolean;
 VAR Port, Lst : word;
 BEGIN
  Detect_Reg_sb :=  SbRegDetected;
  IF SbRegDetected THEN EXIT;
  Port := $210;
  Lst  := $280;

  while (not SbRegDetected)
  and   (Port <= Lst)  do begin
    dsp_adr := Port;
    SbRegDetected := Reset_sb;
    if not SbRegDetected then

     inc(Port, $10);
  end ;
     Detect_Reg_sb := SbRegDetected;
 end;

 function SbReadByte : byte;

 begin;
   while port[dsp_adr+$a] = $AA do ;
   SbReadByte :=  port[dsp_adr+$a];
 end;


 procedure wr_dsp(v : byte);
 begin;
  while port[dsp_adr+$c] >= 128 do ;
   port[dsp_adr+$c] :=  v;
 end;


 function GetDSPVersion : string;

 var i : word;
     t : WORD;
     s :  STRING [ 2 ] ;
  begin
   wr_dsp($E1);
   SbVersMaj :=  SbReadByte;
   sbVersMin :=  SbReadByte;
   str(SbVersMaj, SbVersStr);
   SbVersStr :=  SbVersStr +  '.';
   str(SbVersMin, s);
   if SbVersMin > 9 then
     SbVersStr :=  SbVersStr +   s
   else
     SbVersStr :=  SbVersStr + '0' + s;
   GetDSPVersion := SbVersStr;
 end;

 function wrt_dsp_adr : string;
 {
  Liefert die Base-Adresse des SB als
  String zurÅck
 }
 begin;
   case dsp_adr of
    $210 : begin wrt_dsp_adr := '210'; dsp_adr := 1; end;
    $220 : begin wrt_dsp_adr := '220'; dsp_adr := 2; end;
    $230 : begin wrt_dsp_adr := '230'; dsp_adr := 3; end;
    $240 : begin wrt_dsp_adr := '240'; dsp_adr := 4; end;
    $250 : begin wrt_dsp_adr := '250'; dsp_adr := 5; end;
    $260 : begin wrt_dsp_adr := '260'; dsp_adr := 6; end;
    $270 : begin wrt_dsp_adr := '270'; dsp_adr := 7; end;
    $280 : begin wrt_dsp_adr := '280'; dsp_adr := 8; end;
    END ;
 end;

procedure detect_soundblaster;
begin
  UseSound := False;
  if detect_reg_sb then begin
        writeln('SoundBlaster ',GetDSPVersion,' at base Address ',
                wrt_dsp_adr,'h found.');
            UseSound := true;
    end else begin
      writeln('No Soundblaster or compatible found!');
      UseSound := false;
    end;
end;

procedure init_soundkit;
var h : byte;
begin
 {load them}
  init_load_sound;
          load_sound(snd1,'sound\1.snd');
          load_sound(snd2,'sound\2.snd');
          load_sound(snd3,'sound\3.snd');
          load_sound(snd4,'sound\4.snd');
          load_sound(snd5,'sound\5.snd');
          load_sound(snd6,'sound\6.snd');
          load_sound(snd7,'sound\7.snd');
          load_sound(snd8,'sound\8.snd');
          load_sound(snd9,'sound\9.snd');
          load_sound(snd10,'sound\10.snd');
          load_sound(snd11,'sound\11.snd');
          load_sound(snd12,'sound\12.snd');
          load_sound(snd13,'sound\13.snd');
          load_sound(snd14,'sound\14.snd');
  if resetDSP(dsp_Adr) then
  begin
    writeln('RESET FAILED');
  end;
  h := SBReadByte;
  h := SpeakerOn;

end;
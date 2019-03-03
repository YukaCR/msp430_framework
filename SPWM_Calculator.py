#!/usr/bin/python3
import math,os,json

WolframScriptPath   = "D:/Program Files/Wolfram Research/Mathematica/11.3/wolframscript.exe"
ExternalCrystalFreq = 12 * 1000 * 1000 #Hz 12MHz HC-49S
TriangleWaveFreq    = 10 * 1000        #Hz 10KHz
SinwaveSauceFreq    = 50               #Hz 50Hz
result = []
TotalCycle = TriangleWaveFreq / SinwaveSauceFreq
MaxPeriod = ExternalCrystalFreq / (TriangleWaveFreq * 2)
k1 = (MaxPeriod)/((1/TriangleWaveFreq)/2) 
k2 = -1 * (MaxPeriod)/((1/TriangleWaveFreq)/2) 
SinWavePP = MaxPeriod/2 - MaxPeriod/20
s1 = "Table[NSolve[(%d * (t - (t1/%d))) == (%d*Sin[%d*\\[Pi]*t] + %d),t, Reals], {t1, 0, %d}]"%(k1,TriangleWaveFreq,SinWavePP,SinwaveSauceFreq * 2,SinWavePP,TotalCycle)
s2 = "Table[NSolve[(%d * (t - (t1/%d))) == (%d*Sin[%d*\\[Pi]*t] + %d),t, Reals], {t1, 1, %d}]"%(k2,TriangleWaveFreq,SinWavePP,SinwaveSauceFreq * 2,SinWavePP,TotalCycle+1)
a = os.popen("\"%s\" -code \"%s\""%(WolframScriptPath,s1)).read().splitlines()[-1]
b = os.popen("\"%s\" -code \"%s\""%(WolframScriptPath,s2)).read().splitlines()[-1]
a = eval("[" + a.replace("{{","{").replace("}}","}").replace("->",":").replace("{t :","").replace("}","")[1:-1] + "]")
b = eval("[" + b.replace("{{","{").replace("}}","}").replace("->",":").replace("{t :","").replace("}","")[1:-1] + "]")
if(round(SinWavePP * math.sin(SinwaveSauceFreq * 2 * math.pi * a[0]) + SinWavePP)!=round(SinWavePP * math.sin(SinwaveSauceFreq * 2 * math.pi * a[-1]) + SinWavePP)):
    print("//Warning: SPWM would not perform well ")
for i in range(0,len(a)-1):
    result.append(round(SinWavePP * math.sin(SinwaveSauceFreq * 2 * math.pi * a[i]) + SinWavePP))
    result.append(round(SinWavePP * math.sin(SinwaveSauceFreq * 2 * math.pi * b[i]) + SinWavePP))
print("#define Period %d"%MaxPeriod)
print("#define SPWM_TOTAL %d    //Method: TriangleWaveFreq / SinWaveFreq"%len(result))
print("const uint16_t SPWM_Period[] = {%s}"%json.dumps(result)[1:-1])
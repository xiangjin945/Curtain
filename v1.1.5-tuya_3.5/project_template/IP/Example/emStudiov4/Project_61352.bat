@ECHO OFF
SET TARGET_CHIP=HT32F61352
SET PACK_FILE=..\..\..\..\Holtek.HT32_DFP.latest.pack

start Project_61352.emProject

REM pyocd erase --chip -t %TARGET_CHIP% --pack=%PACK_FILE%
pyocd gdbserver -t %TARGET_CHIP% --pack=%PACK_FILE%
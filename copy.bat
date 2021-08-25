@echo off
if exist ..\BDS\bdxcore_mod (
	copy .\x64\Release\mine.dll ..\BDS\bdxcore_mod\mine.dll
	copy .\x64\Release\mine.pdb ..\BDS\bdxcore_mod\mine.pdb
)
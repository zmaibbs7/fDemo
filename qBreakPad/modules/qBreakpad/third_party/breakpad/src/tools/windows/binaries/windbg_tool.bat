修改批处理，让DUMP_FILE
OUTPUT_FILE
PDB_DIR
SYMBOL_DIR
TEMP_OUTPUT_FILE
可以通过参数形式传递
```
@echo off
setlocal

rem 设置DUMP_FILE, OUTPUT_FILE和PDB_DIR路径
set PDB_DIR=D:\github\fDemo\qBreakPad\build\Debug
set SYMBOL_DIR=D:\github\fDemo\qBreakPad\build\Debug
set DUMP_FILE=D:\github\fDemo\qBreakPad\build\Debug\crashe\6f602f6d-b844-4e2a-a111-087f27bfafdf.dmp
set OUTPUT_FILE=D:\github\fDemo\qBreakPad\build\Debug\crashe\crash_analysis.txt

rem 设置符号路径
set SYMBOL_PATH=%PDB_DIR%;%SYMBOL_DIR%

rem 使用cdb来解析minidump文件，并将输出结果保存到文件
cdb -z %DUMP_FILE% -y %SYMBOL_PATH% -c ".symfix; .reload; .ecxr; k; .lines; .logopen %OUTPUT_FILE%; !analyze -v; .logclose; q"

endlocal

```
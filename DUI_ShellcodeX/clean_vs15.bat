echo 正在清理VS2010+工程中不需要的文件
echo 请确保本文件放置在工程目录之中并关闭VS2010+
echo 开始清理请稍等……
echo 清理sdf文件
del /q/a/f/s *.sdf
del /q/a/f/s *.ncb
echo 清理ipch文件
del /q/a/f/s ipch\*.*
echo 清理Debug文件
del /q/a/f/s *.obj
del /q/a/f/s *.tlog
del /q/a/f/s *.log
del /q/a/f/s *.idb
del /q/a/f/s *.pdb
del /q/a/f/s *.ilk
del /q/a/f/s *.pch
del /q/a/f/s *.ipch
del /q/a/f/s *.bsc
del /q/a/f/s *.sbr
del /q/a/f/s *.suo
echo 清理Release文件
del /q/a/f/s *.obj
del /q/a/f/s *.tlog
del /q/a/f/s *.log
del /q/a/f/s *.idb
del /q/a/f/s *.pdb
del /q/a/f/s *.ilk
del /q/a/f/s *.pch
del /q/a/f/s *.db
del /q/a/f/s *.ipdb
del /q/a/f/s *.iobj
del /q/a/f/s *.aps
echo 清理Temp文件
del /q/a/f/s Temp\*.*
ECHO 文件清理完毕！

echo.
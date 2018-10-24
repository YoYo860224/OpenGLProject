import os

# 指定要列出所有檔案的目錄
mypath = "./"

for root, dirs, files in os.walk(mypath):
    for f in files:
        if f.split('.')[1] == 'ply':
            print(".\\OpenGLProject.exe .\\" + f)
            os.system(".\\OpenGLProject.exe .\\" + f)
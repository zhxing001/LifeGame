# -*- coding: utf-8 -*-
"""
Created on Tue Dec 25 20:52:34 2018

@author: zhxing
"""

f=open('x.txt','w')
for i in range(1,100):
    f.write(str(i))
    f.write(' ')
    f.write(str(i))
    f.write(' ')
    f.write(str(100-i))
    f.write('\n')
    print(i)
f.close()
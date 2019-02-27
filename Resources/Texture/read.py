import os,codecs,sys,json
from PIL import Image
import numpy as np

s = sys.argv[1] 
img = Image.open(s + '.jpg')
img.load()
data = np.asarray(img, dtype="int32" )


lines = []
lines.append(str(data.shape[1]) + ' ' + str(data.shape[0]) + '\n')

for line in data:
    for row in line:
        lines.append(str(row[0]) + ' ' + str(row[1]) + ' ' + str(row[2]) + '\n')

with open(s + '.ttr', 'w') as f:
    f.writelines(lines)






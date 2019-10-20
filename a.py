import os
s = open("test.txt").read()
d = []
b = []
s = s.split(";")
for a in s:
    d.append(a.split('|')[0])
for a in d:
    b.append(max(a.split(',')))
tones = []
times_ = []
times = []
for a in s:
    if a!='':
        times_.append(a.split('|')[1].split(',')[0])
b = b[:-1]
for a in range(len(b)):
    if tones == [] or tones[-1] != int(b[a]):
        tones.append(int(b[a]))
        times.append(int(times_[a]))
    else:
        times[-1] = times[-1] + int(times_[a])
f = open("music.h", "w")
f.write('#include <stdint.h>\r\n')
f.write('const uint16_t tones[] = {' + str(tones)[1:-1] + '};\r\n')
f.write('const uint16_t times[] = {' + str(times)[1:-1] + '};')
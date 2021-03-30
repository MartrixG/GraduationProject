import os
sgfs = ''
p = ''
file_path = 'E:/LEARNING/GraduationProject/data/7d/'
tot = 0

fp = open('E:/LEARNING/GraduationProject/data/7d.sgf', 'w+', encoding='utf8')

for _, _, file_list in os.walk(file_path):
    for file in file_list:
        try:
            with open(file_path + '/' + file, encoding='utf8', errors='ignore') as f:
                lines = f.readlines()
                if len(lines) > 1:
                    print(file)
                    break
                sgfs += lines[0]
                sgfs += '\n'
                tot += 1
                if tot % 1000 == 0:
                    fp.write(sgfs)
                    fp.flush()
                    sgfs = ''
                    print(str(tot) + '/240456')
        except:
            print(file)
fp.write(sgfs)
fp.close()
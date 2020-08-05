
PATH = "C:/Users/Brand/Documents/Programming/Text Platformer (C)/src/images/"

matches = [
    "255, 243, 230",
    "0x00, 0x00",
    "255, 162,   0",
    "255, 166,   8",
    "140, 255, 255"
]

a = open(PATH + "mypalette.c", "r")


class Output():
    def __init__(self, name, n):
        self.indexes = []
        self.name = name
        self.defNames = n
        self.header = name.upper() + "_H"
    
    def open(self):
        return open(PATH + out.name + ".h", "w")

    def getData(self):
        ret = ""
        for i in range(0, len(self.defNames)):
            if i >= len(self.indexes):
                print("\n\nERROR: NUMBER OF COLORS AND COLOR NAMES DO NOT MATCH\n\n")
                break

            index = self.indexes[i]
            n = self.defNames[self.indexes.index(index)]
            ret += "#define {} {}\n".format(n, index)
           # print(ret + '\n' + str(self.indexes.index(index)) + '\n')

        return ret

out = Output("colors", ["COLOR_GRAY", "COLOR_BLACK", "COLOR_YELLOW", "COLOR_RED", "COLOR_TRANSPARENT"])

def hasMatch(string):
    for i in matches:
        index = string.find(i)
        if index != -1:
            return index
    
    return -1


# for l in a.readlines():
#     line = l.strip()
#     index = hasMatch(line)
#     if index != -1:
#         out.indexes.append(line[line.find("/*") + 2:line.find(":")].strip())
lines = a.readlines()

for i in matches:
    for l in lines:
        line = l.strip()
        index = line.find(i)
        if index != -1:
            outputIndex = line[line.find("/*") + 2:line.find(":")].strip()
            print(outputIndex)
            out.indexes.append(outputIndex)

a.close()

outFile = out.open()

outFile.write("""#ifndef {} 
#define {}
/*
    Generated automatically with 'images/indexRouter.py'
*/

{}

#endif
""".format(out.header, out.header, out.getData()))
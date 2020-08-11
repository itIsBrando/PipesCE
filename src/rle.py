import pyperclip


class rleData:
    def __init__(self, data, count):
        if count > 0:
            self.data = data
        else:
            self.data = data | 0x80
        
        self.count = count


    @staticmethod
    def size(input):
        s = 0
        for i in input:
            if i.count > 0:
                s += 2
            else:
                s += 1

        return s

    @staticmethod
    def print(list):
        string = '{\n	'

        for i in list:
            if i.count > 0:
                string += str(i.count) + ', ' + str(i.data) + ', '
            else:
                string += str(i.data) + ', '
        
        string = string.rstrip(', ') + '\n};'
        print(string)
        return string


    @staticmethod
    def encode(input):
        out = []
        prev = None
        count = 0

        for i in input:
            if prev != i and prev != None:
                if count == 1:
                    count = 0
                
                out.append(rleData(prev, count))
                count = 0

            count += 1
            prev = i
                
        out.append(rleData(input[-1], count))

        nsize = rleData.size(out)
        print('encoded size: ', nsize, ' original size: ', len(input), 'saved: ', len(input) - nsize)
        print(nsize * 100 / len(input), "% of the original size", sep='')
        return out

    @staticmethod
    def decode(input):
        out = []

        for e in input:
            if e.data & 0x80 == 0x80:
                out.append(e.data - 0x80)
            else:
                for _ in range(0, e.count):
                    out.append(e.data)

        return out

    
    # converts an rleData array to an int array
    @staticmethod
    def toInt(input):
        out = []
        for i in input:
            if i.count > 0:
                out.append(i.count)
            out.append(i.data)
        return out

    @staticmethod
    def decodeInt(input):
        out = []

        for i in range(0, len(input), 2):
            if(input[i] & 0x80 == 0x80):
                out.append(rleData(input[i], 0))
                i -= 1
            else:
                out.append(rleData(input[i +1], input[i]))
        
        return rleData.decode(out)

""" 
paste data to compress here
"""
original = [
1,1,1,1,1,1,1,1,1,1,
1,2,0,0,10,9,10,6,10,1,
1,0,5,0,10,0,10,0,10,1,
1,0,13,0,10,0,8,0,8,1,
1,0,8,0,10,0,10,0,10,1,
1,0,8,0,10,0,10,0,10,1,
1,0,0,0,5,3,10,7,10,1,
1,1,1,1,1,1,1,1,1,1
]
e = rleData.encode(original)

pyperclip.copy(rleData.print(e))

d = rleData.decode(e)

if(d != original):
    print("original length:", len(original), " bad length: ", len(d))
    for i in range(0, min(len(original), len(d))):
        if d[i] != original[i]:
            print("index: ", i, " does not match original new:", d[i], "old:", original[i])
    print("new: ", d, "\nold:", original)
    print("\nFATAL\nEncoded does not match original\n\n")
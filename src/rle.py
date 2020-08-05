import pyperclip


class rleData:
    def __init__(self, data, size):
        self.data = data
        self.size = size


    @staticmethod
    def print(list):
        string = '{\n	'

        for i in list:
            string += str(i.size) + ', ' + str(i.data) + ', '
        
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
                out.append(rleData(prev, count))
                count = 0

            count += 1
            prev = i
                
        out.append(rleData(input[-1], count))
        print('encoded size: ', len(out) * 2, ' original size: ', len(input), 'saved: ', len(input) - len(out) * 2)
        return out

    @staticmethod
    def decode(input):
        out = []

        for e in input:
            for _ in range(0, e.size):
                out.append(e.data)

        return out

original = [
	1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	1, 10,  8,  8, 10,  8,  8, 10,  8,  8, 10,  8, 10,  1,
	1,  0,  0,  0,  0,  0, 20,  5,  0,  0,  0,  0,  5,  1,
	1,  0,  0,  0,  0,  0,  0,  5,  0,  0, 20,  0,  5,  1,
	1, 10,  8,  8, 10,  7, 20, 10, 20, 10,  8,  8, 10,  1,
	1,  5, 18, 18, 18,  5, 20, 20, 20,  5, 18, 18,  5,  1,
	1,  5, 18, 18, 18,  3,  7,  0,  0,  5, 18, 18,  5,  1,
	1, 10, 18, 10,  8,  8, 10,  8,  8, 10,  8,  8, 10,  1,
	1,  5, 18,  5,  0,  0,  0,  0,  6, 9,   0, 20,  0,  1,
	1,  5, 18,  3,  7,  0, 20,  6,  9,  0,  0,  0,  0,  1,
	1, 10,  8,  8, 10,  8,  8, 10,  8,  8, 10,  8, 10,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  7,
	5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,
	3,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  9
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
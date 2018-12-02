import argparse
import numpy as np
from io import open

def getMatrix(filename):
	l1Words = []
	l2Words = []
	rawdata = open(filename, 'r')
	l1Arr = []
	l2Arr = []
	for line in rawdata:
		line = line.split()
		l1Words.append(line[0])
		l2Words.append(line[1])
		i = 2
		delim = False
		l1Vec = []
		l2Vec = []
		while i < len(line):
			if line[i] == '|||':
				delim = True
			elif delim == False:
				l1Vec.append(float(line[i]))
			elif delim == True:
				l2Vec.append(float(line[i]))
			else:
				raise ValueError
			i += 1
		l1Arr.append(l1Vec)
		l2Arr.append(l2Vec)
	l1Arr = tuple(l1Arr)
	l2Arr = tuple(l2Arr)
	l1M = np.vstack(l1Arr)
	l2M = np.vstack(l2Arr)

	return (l1Words, l1M), (l2Words, l2M)

def main():
	ap = argparse.ArgumentParser()
	ap.add_argument('--src_lang', default='bg')
	ap.add_argument('--tgt_lang', default='en')
	ap.add_argument('--train_file', default='data/bg-en.train')
	ap.add_argument('--test_file', default='data/bg-en.test')
	ap.add_argument('--out_file', default='out.txt')

	args = ap.parse_args()

	l1, l2 = getMatrix(args.train_file)
	l1, l2 = getMatrix(args.test_file)

if __name__ == '__main__':
    main()



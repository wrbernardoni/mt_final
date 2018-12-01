import argparse
import numpy as np
from io import open

def hello(h, t):
	return 77.9

def getMatrix(filename):
	#l1M = np.array()
	#l2M = np.array()
	l1Words = []
	l2Words = []
	rawdata = open(filename, 'r')
	l1Vec = []
	l2Vec = []

	for line in rawdata:
		line = line.split()
		l1Words.append(line[0])
		l2Words.append(line[1])
		i = 2
		delim = False
		print(line, "\n\n\n\n\n")
		while i < len(line):
			if line[i] == '|||':
				delim = True
			elif delim == False:
				l1Vec.append(float(line[i]))
			elif delim == True:
				l2Vec.append(float(line[i]))
			else:
				print("PROBLEM")
			i += 1
		print(l1Vec,'\n\n', len(l1Vec), l1Words, l2Words)
		print(l2Vec, len(l2Vec))
		hello(1)


	return 1

def main():
	ap = argparse.ArgumentParser()
	ap.add_argument('--src_lang', default='bg')
	ap.add_argument('--tgt_lang', default='en')
	ap.add_argument('--train_file', default='data/bg-en.train')
	ap.add_argument('--test_file', default='data/bg-en.test')
	ap.add_argument('--out_file', default='out.txt')

	args = ap.parse_args()

	trainMatrix = getMatrix(args.train_file)
	testMatrix = getMatrix(args.test_file)

if __name__ == '__main__':
    main()



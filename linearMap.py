import argparse
import numpy as np
from io import open

#returns the matrices P and Q 
#as well as the lists of corresponding words
def getMatrix(filename):
	#LWords contains the ordering of the words
	l1Words = []
	l2Words = []
	rawdata = open(filename, 'r')

	#LArrs contain lists of the 100 float vectors
	l1Arr = []
	l2Arr = []

	#reads in input line by line to create LWords and creates
	#the LArrs by creating each LVec point by point
	for line in rawdata:

		line = line.split()

		#Adds the words to L1Words and L2Words
		l1Words.append(line[0])
		l2Words.append(line[1])

		i = 2
		delim = False
		l1Vec = []
		l2Vec = []

		while i < len(line):

			#Switchs to L2 once delim is found
			if line[i] == '|||':
				delim = True
			#Adds the next value to the l1Vector of 100 poins
			elif delim == False:
				l1Vec.append(float(line[i]))
			#Adds the next value to the l2Vector of 100 poins
			elif delim == True:
				l2Vec.append(float(line[i]))
			#Error checking
			else:
				raise ValueError
			i += 1

		#Adds the list of 100 points to the LArrs
		l1Arr.append(l1Vec)
		l2Arr.append(l2Vec)

	#Converts LArrs to tuples for ease of use
	l1Arr = tuple(l1Arr)
	l2Arr = tuple(l2Arr)

	#LArrs converted to numpy matrices
	l1M = np.vstack(l1Arr)
	l2M = np.vstack(l2Arr)

	#Centroids calculated
	l1Cen = np.mean(l1M)
	l2Cen = np.mean(l2M)

	#Creating a 1x100 array to subtract each point in the
	#LMatrices by their centroid
	l1Subtract = np.full((1,100), l1Cen)
	l2Subtract = np.full((1,100), l2Cen)

	#Subtracts the centroid from each point
	l1M = np.subtract(l1M, l1Subtract)
	l2M = np.subtract(l2M, l2Subtract)

	return (l1Words, l1M), (l2Words, l2M)

#Computation of the covariance matrix
def getH(P, Q):
	#P and Q is dimension Nx100
	#pT is 100xN
	#H is 100x100

	pT = np.transpose(P)
	H = np.matmul(pT, Q)

	return H

#Computes optimal rotation matrix using SVD
def rotation(H):
	U, S, vT = np.linalg.svd(H)
	V = np.transpose(vT)
	uT = np.transpose(U)

	#not sure what the det is used for yet
	#from Wikipedia:
	#decide whether we need to correct our rotation 
	#matrix to ensure a right-handed coordinate system
	d = np.linalg.det(np.matmul(V,uT))
	#print(d)
	I = np.identity(V.shape[1])
	R = np.matmul(np.matmul(V, I), uT)

	return R


def main():
	#Takes in command line arguments
	ap = argparse.ArgumentParser()
	ap.add_argument('--src_lang', default='bg')
	ap.add_argument('--tgt_lang', default='en')
	ap.add_argument('--train_file', default='data/bg-en.train')
	ap.add_argument('--test_file', default='data/bg-en.test')
	ap.add_argument('--out_file', default='out.txt')

	args = ap.parse_args()

	#Returns the matrices P and Q as well as 
	#a list of words corresponding to index
	#given the train and test files
	l1Train, l2Train = getMatrix(args.train_file)
	l1Test, l2Test = getMatrix(args.test_file)

	#Computes the covariance matrix H given P and Q
	hTrain = getH(l1Train[1], l2Train[1])
	hTest = getH(l1Test[1], l2Test[1])

	#Computes the rotation matrix given H
	rTrain = rotation(hTrain)
	rTest = rotation(hTest)

if __name__ == '__main__':
    main()



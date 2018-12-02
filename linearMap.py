import argparse
import numpy as np
from io import open

#Implementation of Linear Orthogonal Mapping of word embeddings
#Much of this program is based on https://en.wikipedia.org/wiki/Kabsch_algorithm


#Returns the matrices P and Q as well as
#the lists of corresponding words. The format
#of P and Q was based on the P and Q matrices
#described in the "Description" section here
#https://en.wikipedia.org/wiki/Kabsch_algorithm
#However, our P and Q are Nx100 instead of Nx3
def getMatrix(filename):
	#LWords contains the ordering of the words
	l1Words = []
	l2Words = []

	#Opens the data file. It is formatted as 
	#SrcWord TgtWord SrcWordVec Delim TgtWordVec
	rawdata = open(filename, 'r')

	#LArrs contain lists of the 100 float vectors
	l1Arr = []
	l2Arr = []

	#Reads in input line by line to create LWords and creates
	#the LArrs by creating each LVec point by point
	for line in rawdata:
		#Splits the line so that it's a list of the source word, the
		#target word, the 100 dimensions of the source word vector, 
		#the delimeter, then the 100 dimensions of the target word vector
		line = line.split()

		#Adds the words to L1Words and L2Words
		l1Words.append(line[0])
		l2Words.append(line[1])

		i = 2
		delim = False

		#The LVectors will contain the 100 dimensions of the word vector
		l1Vec = []
		l2Vec = []

		#Iterates through the rest of the list to add each dimension 
		#to its corresponding vector
		while i < len(line):

			#Error checks the line size
			if i > 203:
				raise ValueError("File not formatted correctly!")

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
				raise ValueError("File not formatted correctly!")
			
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

	#Adjusts the matrix by subtracting 
	#each point from the matrix's centroid
	l1M = translation(l1M)
	l2M = translation(l2M)

	#Error checking
	if len(l1Words) != len(l2Words):
		raise ValueError(filename + " is formatted incorrectly!")

	return (l1Words, l1M), (l2Words, l2M)

#Translates the matrix L to make sure the centroid
#is at the origin of the coordinate system
#Found in the "Translation" section here
#https://en.wikipedia.org/wiki/Kabsch_algorithm
def translation(L):

	#Calculates the centroid by getting the mean of 
	#every point in the 2d matrix
	centroid = np.mean(L)

	#Creating a 1x100 array to subtract each point in the
	#L matrix by the centroid
	lSubtract = np.full((1, L.shape[1]), centroid)

	#Subtracts the centroid from each point
	L = np.subtract(L, lSubtract)

	return L

#Computates the covariance matrix
#Found in the "Computation of the covariance matrix"
#section here https://en.wikipedia.org/wiki/Kabsch_algorithm
def getH(P, Q):
	#P and Q is dimension Nx100
	#pT is 100xN
	#H is 100x100

	#Finding P's transpose
	pT = np.transpose(P)

	#Calculating H by multiplying pT and Q
	H = np.matmul(pT, Q)

	return H

#Computes optimal rotation matrix using SVD
#Rotation algorithm found in "Computation of the optimal rotation matrix"
#section here https://en.wikipedia.org/wiki/Kabsch_algorithm
def rotation(H):
	#Do SVD of H
	U, S, vT = np.linalg.svd(H)

	#Calculate V and U Transpose
	V = np.transpose(vT)
	uT = np.transpose(U)

	#Calculate determinant of V uT 
	d = np.linalg.det(np.matmul(V,uT))

	#Create a diagonal matrix of 1s with the last point
	#being the calculated determinant
	Id = np.identity(V.shape[1], dtype=float)
	Id[-1][-1] = d

	#Calculate the rotation matrix V Id uT
	R = np.matmul(np.matmul(V, Id), uT)

	return R

#Prints the results to an output file
def returnRes(L1, L2, R, lang):

	#Creates an output file based on the name of the tgt lang
	out = open(lang+'LO.txt', 'w')

	#Iterates through L1 to print results
	#L1, L2, and R3 have the same size, so i is used as a counter
	i = 0
	for _ in L1:
		#Writes SrcWord TgtWord RotationVector to the output file
		print(L1[i], L2[i], *R[i], file=out)
		i += 1

def main():
	#Takes in command line arguments
	ap = argparse.ArgumentParser()
	ap.add_argument('--src_lang', default='en')
	ap.add_argument('--tgt_lang')
	ap.add_argument('--train_file', default='data/bg-en.train')
	ap.add_argument('--test_file', default='data/bg-en.test')
	ap.add_argument('--out_file', default='out.txt')

	args = ap.parse_args()

	#Ensures target language specified
	if args.tgt_lang is None:
		raise ValueError("Target language required!")

	#Ensures that the target language is the same as the train/test files
	if args.tgt_lang not in args.train_file and args.tgt_lang not in args.test_file:
		raise ValueError("Target language must be same as train and test file languages")

	#Returns the matrices P and Q as well as 
	#a list of words corresponding to index
	#given the train and test files
	l1Train, l2Train = getMatrix(args.train_file)
	l1Test, l2Test = getMatrix(args.test_file)

	#Computes the covariance matrix H given P and Q
	hTrain = getH(l1Train[1], l2Train[1])

	#Computes the rotation matrix given H
	rTrain = rotation(hTrain)

	#Multiplies L1 by Rotation
	rotL1Test = np.matmul(l1Test[1], rTrain)

	#Error checks to make sure the rotated matrix has the right dimensions
	if len(l1Test[0]) != rotL1Test.shape[0]:
		raise ValueError("Rotated Matrix has incorrect dimensions!")

	#Prints Results
	returnRes(l1Test[0], l2Test[0], rotL1Test, args.tgt_lang)
	
if __name__ == '__main__':
    main()


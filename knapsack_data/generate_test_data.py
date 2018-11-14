#!/usr/bin/python3

import numpy as np
import argparse
import os

parser = argparse.ArgumentParser(description='Generate test data for Knapsack problem.')
parser.add_argument('N', type=int, help='Number of items')
parser.add_argument('-s', type=int, default=100, help='Weight knapsack can hold')
parser.add_argument('-v', type=float, default=0.4, help='Maximum weight of one item in relation to knapsack size')
parser.add_argument('-o', type=str, help='output file name')

args = parser.parse_args()

weight = np.random.randint(1, int(args.v * args.s) + 1, args.N)
price = np.random.randint(1, 1000, args.N)
indexes = np.arange(0, args.N)

if np.sum(weight) <= args.s:
    raise ValueError("Sum of item weights are lower then knapsacks max weight")

data = np.column_stack((indexes, weight, price))

out = f'knapsack_{args.N}.out'
if args.o:
    out = args.o

if os.path.isdir('knapsack_data'):
    out = os.path.join('knapsack_data', out)

np.savetxt(out, data, fmt='%d', header=f'Items: {args.N}, Weight: {args.s}\nIndex, Weight, Price')

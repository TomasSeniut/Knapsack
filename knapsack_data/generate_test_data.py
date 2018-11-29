#!/usr/bin/python3

import numpy as np
import argparse
import os

parser = argparse.ArgumentParser(description='Generate test data for Knapsack problem.')
parser.add_argument('N', type=int, help='Number of items')
parser.add_argument('-s', type=int, default=1000, help='Weight knapsack can hold')
parser.add_argument('-o', type=str, help='output file name')

args = parser.parse_args()

max_weight = 4. * float(args.s) / float(args.N)
if max_weight < 100:
    fac = 100 / max_weight
    max_weight = 100
    args.s = int(args.s * fac)
else:
    max_weight = int(max_weight)

weight = np.random.randint(1, max_weight, args.N)
price = np.random.randint(1, 1000, args.N)
indexes = np.arange(0, args.N)

print(f'sum: {np.sum(weight)}')

if np.sum(weight) <= args.s:
    raise ValueError("Sum of item weights are lower then knapsacks max weight")

data = np.column_stack((indexes, weight, price))

out = f'knapsack_{args.N}.out'
if args.o:
    out = args.o

if os.path.isdir('knapsack_data'):
    out = os.path.join('knapsack_data', out)

np.savetxt(out, data, fmt='%d', header=f'Items: {args.N}, Weight: {args.s}\nIndex, Weight, Price')

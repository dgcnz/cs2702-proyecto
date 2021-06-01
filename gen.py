from random import randint, shuffle
import argparse
import csv

parser = argparse.ArgumentParser(
    description='Generate random csv with unique keys.')
parser.add_argument('rows', type=int, help='Number of rows.')
parser.add_argument('filename', type=str, help='Output Filename.')
args = parser.parse_args()

numbers = [i for i in range(args.rows)]
shuffle(numbers)

with open(args.filename, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile,
                            delimiter=',',
                            quotechar='|',
                            quoting=csv.QUOTE_MINIMAL)
    for i in numbers:
        spamwriter.writerow([i, randint(1, 100000)])

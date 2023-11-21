#!/usr/bin/env python3

from common import *
import sys


def get(path):
    return set(lines(path))


def main():
    op = sys.argv[1]
    a = get(sys.argv[2])
    b = get(sys.argv[3])
    c = set()
    if op == 'or':
        c = a | b
    elif op == 'and':
        c = a & b
    elif op == 'sub':
        c = a - b
    for i in c:
        print(i)


if __name__ == '__main__':
    main()

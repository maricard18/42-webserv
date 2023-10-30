import os
import sys

path = os.environ.get('PATH')
protocol = os.environ.get('PROTOCOL')

print(f'Path: {path}')
print(f'Protocol: {protocol}')

for line in sys.stdin:
    print(f'Pipe data: {line}')
import json
import sys

data = json.loads(open(sys.argv[1], 'r', encoding='utf-8').read())
output = open('out.md', 'w', encoding='utf-8')

header = data[0]
output.write('|')
for k in header:
    output.write(k + '|')
output.write('\n')

output.write(len(header) * '|--')
output.write('|\n')

for item in data:
    output.write('|')
    for k in item:
        if k != 'comment':
            output.write(str(item[k]) + '|')
    output.write('\n')

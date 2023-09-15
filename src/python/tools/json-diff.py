import json
import sys

f1 = open(sys.argv[1], 'r', encoding='utf-8')
f2 = open(sys.argv[2], 'r', encoding='utf-8')
data1 = json.loads(f1.read())
data2 = json.loads(f2.read())

items1 = set()
items2 = set()
for i in data1:
    items1.add(i['title'])

for i in data2:
    items2.add(i['title'])

print(items1 == items2)

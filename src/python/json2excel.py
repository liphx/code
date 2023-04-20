import json
import sys
import xlsxwriter as xw

if len(sys.argv) < 3:
    sys.exit(1)

json_file = sys.argv[1]
xlsx_file = sys.argv[2]
f = open(json_file, "r", encoding='utf-8')
content = json.loads(f.read())
header = set()
for item in content:
    for key in item.keys():
        header.add(key)

workbook = xw.Workbook(xlsx_file)
worksheet1 = workbook.add_worksheet("sheet1")
worksheet1.activate()
title = list(header)
worksheet1.write_row('A1', title)
i = 2
for j in range(len(content)):
    insertData = []
    data = content[j]
    for key in title:
        if key in data:
            insertData.append(data[key])
        else:
            insertData.append('')
    row = 'A' + str(i)
    worksheet1.write_row(row, insertData)
    i += 1
workbook.close()

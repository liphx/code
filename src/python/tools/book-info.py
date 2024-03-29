from cmath import exp
import json
from unittest import expectedFailure
import requests
import time

# https://jike.xyz/jiekou/isbn.html
apikey = '12542.2d7bd77de515c6cb1ec893d9d1c5aa1c.c5f6b735d0f8fcf7bb5931f0bf9f5fcc'
file = 'book_list.json'
output = []
stored = set()


def transform(item):
    ret = dict()
    maps = {'isbn': 'code', 'publisher': 'publishing', 'publish_date': 'published',
            'photo_url': 'photoUrl', 'author_intro': 'authorIntro'}
    nomaps = ['name', 'subname', 'author', 'translator',
              'designed', 'brand', 'pages', 'price', 'description']
    for k in maps:
        ret[k] = item[maps[k]]
    for i in nomaps:
        ret[i] = item[i]

    ret['douban_id'] = item['douban']
    ret['douban_url'] = f'https://book.douban.com/subject/{item["douban"]}/'
    return ret


def readInfo():
    try:
        global output
        f = open(file, "r", encoding='utf-8')
        output = json.loads(f.read())
        f.close()
    except:
        pass

    for item in output:
        stored.add(item['isbn'])
    print('current:', stored)


def writeInfo():
    f = open(file, 'w', encoding='utf-8')
    f.write(json.dumps(output, ensure_ascii=False, indent=4))
    f.close()
    print('stored:', stored)


def getInfo(isbn):
    if isbn in stored:
        return
    url = f'https://api.jike.xyz/situ/book/isbn/{isbn}?apikey={apikey}'
    r = requests.get(url)
    ret = r.json()
    if ret['ret'] != 0:
        print('request fail')
        time.sleep(2)
        return

    item = transform(ret['data'])
    # print(isbn, item['douban_id'])
    id = item['douban_id']
    print(f'update book set douban_item = "{id}" where isbn = "{isbn}";')
    output.append(item)
    stored.add(isbn)
    time.sleep(1)


# readInfo()

isbn_set = {'9787201097275', '9787532757138', '9787532757107',
            '9787544281331', '9787532751495', '9787020008742'}
for isbn in isbn_set:
    getInfo(isbn)

# writeInfo()

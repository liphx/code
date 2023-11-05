from common import *
import time
import requests
from bs4 import BeautifulSoup
import json
import datetime

cookie = 'bid=sR3xqD7P5TQ; __utma=30149280.1628241599.1639207454.1667126196.1667134718.293; __utmz=30149280.1667040798.289.92.utmcsr=douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/search; __gads=ID=086d24005acfba36-22d08ddc63cf008f:T=1639064620:RT=1639064620:S=ALNI_MZj8NrFQe0mZxMEgmtu5m6dzngzTg; push_noty_num=0; push_doumail_num=0; _ga_RXNMP372GL=GS1.1.1664980296.5.0.1664980296.60.0.0; _ga=GA1.1.1628241599.1639207454; _pk_id.100001.8cb4=1bfe37645aeec99c.1639303199.311.1667134716.1667126242.; douban-fav-remind=1; __yadk_uid=K9QnLqLyNPn4AHhbR9dOaTc3cYdKlqCj; gr_user_id=c983b62a-35d0-4e85-9f27-c9433c8253cb; ll="108288"; __gpi=UID=00000509edaf2a3b:T=1650985131:RT=1667116795:S=ALNI_Ma0OvaiX8JMddYP9uKuOm6gXO2SGw; __utmv=30149280.15866; _vwo_uuid_v2=D92A1D1B6823F2324C4A38C196BB0853E|2dcc62c312d3bf81203cfd419a3c47b5; ct=y; dbcl2="158669192:8SFdfpdXQWk"; ck=7jbS; __utmc=30149280; _pk_ses.100001.8cb4=*; __utmb=30149280.0.10.1667134718'
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0', 'Cookie': cookie}
result = []
today = datetime.datetime.now().strftime('%Y%m%d')
output = open(f'douban-top250-{today}.json', 'w', encoding='utf-8')


def getItem(url):
    data = {}
    data['no'] = len(result) + 1
    data['url'] = url
    r = fetch_url(url, headers=headers)
    if r is None:
        return
    soup = BeautifulSoup(r.text, "html.parser")
    title = ' '.join(soup.select("#wrapper h1")[
                     0].get_text().strip().split('\n'))
    data['title'] = title
    print(data)
    result.append(data)


def main():
    urls = [
        'https://movie.douban.com/top250?start={}'.format(str(i)) for i in range(0, 250, 25)]
    for url in urls:
        r = fetch_url(url, headers=headers)
        soup = BeautifulSoup(r.text, "html.parser")
        items = soup.select(".item .hd a")
        for item in items:
            url = item.get('href')
            getItem(url)


try:
    main()
except:
    pass

output.write(json.dumps(result, ensure_ascii=False, indent=4))

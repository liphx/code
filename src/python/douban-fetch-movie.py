import time
import requests
import json
from bs4 import BeautifulSoup

user = 'jasmine-TS'
cookie = 'bid=zK4R54HmSfs; ll="108288"; _pk_id.100001.4cf6=e06bba2793d51bd3.1667649573.45.1670598211.1670589390.; __utma=30149280.124091549.1667649627.1670589390.1670596793.61; __utmz=30149280.1670589390.60.20.utmcsr=douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/search; __utma=223695111.2071010435.1667649627.1670589390.1670596793.45; __utmz=223695111.1670589390.44.30.utmcsr=douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/search; push_noty_num=0; push_doumail_num=0; __utmv=30149280.15789; _vwo_uuid_v2=D1368908E2FFE9F142E47C87D46E5C0DE|b0e6b2468a611f69fdfd62f15cdd36fa; __yadk_uid=Y8jhd8kPJuFK9VYkSQUSfn5C4vototr7; gr_user_id=2538fede-fd20-4d5b-9177-67c5aa322da1; __gads=ID=753d908fc3029275-22a00bf727d8004d:T=1667929749:RT=1667929749:S=ALNI_MYtSc8f0GePrJgWulcRK0JCEsfsIw; __gpi=UID=00000b77971ebe91:T=1667929749:RT=1670429352:S=ALNI_MY7STsDWW3ncQtPpsrIiqxFu09mEg; douban-fav-remind=1; ct=y; _ga=GA1.2.124091549.1667649627; _pk_ses.100001.4cf6=*; ap_v=0,6.0; __utmb=30149280.8.10.1670596793; __utmb=223695111.13.10.1670596793; __utmc=30149280; dbcl2="157899378:YLeF/gjLJ1Y"; ck=wbmH; __utmc=223695111; frodotk_db="88a2b573ce10317199e8205b6a309453"; __utmt=1'
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0', 'Cookie': cookie}
result = []
output = open('out.json', 'w', encoding='utf-8')


count = 0
def getUrl(url, try_times=5):
    if try_times == 0:
        print(f'Error: get url {url} fail')
        return None

    try:
        r = requests.get(url, headers=headers)
        global count
        count += 1
        if (count == 3):
            count = 0
            time.sleep(3)
        return r
    except:
        time.sleep(5)
        return getUrl(url, try_times - 1)


def getInfo(url):
    r = getUrl(url)
    if r is None:
        return

    html = r.text

    soup = BeautifulSoup(html, "html.parser")
    items = soup.select("#content .item .info")
    for item in items:
        link = item.find('a').get('href')
        getItem(link)

    time.sleep(2)


def getItem(url):
    data = {}
    data['url'] = url
    r = getUrl(url)
    if r is None:
        return

    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    title = ' '.join(soup.select("#content h1")[
                     0].get_text().strip().split('\n'))
    data['title'] = title
    rating = soup.select("#rating #n_rating")[0].get('value')
    data['rating'] = rating
    date = soup.select("#interest_sect_level .collection_date")[
        0].get_text().strip()
    data['date'] = date
    try:
        comment = soup.select("#interest_sect_level > div > span")[
            2].get_text().strip()
        data['comment'] = comment
    except:
        pass
    print(data)
    result.append(data)


def main():
    url = f'https://movie.douban.com/people/{user}/collect'
    r = getUrl(url)
    if r is None:
        return

    html = r.text
    soup = BeautifulSoup(html, "html.parser")

    # 我看过的电影(xxx)
    total = int(soup.select("#content .info h1")[0].get_text().strip()[7:-1])

    urls = ['https://movie.douban.com/people/{}/collect?sort=time&start={}'.format(
        user, str(i)) for i in range(0, total, 15)]
    for url in urls:
        getInfo(url)


try:
    main()
except:
    pass

output.write(json.dumps(result, ensure_ascii=False, indent=4))

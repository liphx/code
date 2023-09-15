import time
import requests
from bs4 import BeautifulSoup
import json

user = 'jasmine-TS'
cookie = 'bid=zK4R54HmSfs; ll="108288"; __utma=30149280.124091549.1667649627.1671449720.1671461961.72; __utmz=30149280.1670681980.64.22.utmcsr=127.0.0.1:5500|utmccn=(referral)|utmcmd=referral|utmcct=/; push_noty_num=0; push_doumail_num=0; __utmv=30149280.15789; _pk_id.100001.3ac3=3f7e1b1ed1924297.1667929748.7.1671462097.1671429407.; gr_user_id=2538fede-fd20-4d5b-9177-67c5aa322da1; __utma=81379588.974781979.1667929749.1671429153.1671462097.7; __utmz=81379588.1671462097.7.7.utmcsr=douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/people/jasmine-TS/; _vwo_uuid_v2=D35499290178C7DD4F2E4271E02EF6BB7|b89952e213e678d00194a7edabc0375f; __gads=ID=753d908fc3029275-22a00bf727d8004d:T=1667929749:RT=1667929749:S=ALNI_MYtSc8f0GePrJgWulcRK0JCEsfsIw; __gpi=UID=00000b77971ebe91:T=1667929749:RT=1671429153:S=ALNI_MY7STsDWW3ncQtPpsrIiqxFu09mEg; __yadk_uid=OIJlsI4O3qh0b5p4DNddUnS60kecuTCh; douban-fav-remind=1; ct=y; _ga=GA1.2.124091549.1667649627; ap_v=0,6.0; __utmb=30149280.7.10.1671461961; __utmc=30149280; __utmt=1; dbcl2="157899378:YLeF/gjLJ1Y"; ck=wbmH; _pk_ses.100001.3ac3=*; __utmt_douban=1; __utmb=81379588.1.10.1671462097; __utmc=81379588'
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0', 'Cookie': cookie}
result = []
output = open('out.json', 'w', encoding='utf-8')


def getUrl(url, try_times=5):
    if try_times == 0:
        print(f'Error: get url {url} fail')
        return None

    try:
        r = requests.get(url, headers=headers)
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
    items = soup.select("#content .info h2")
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
    title = ' '.join(soup.select("#wrapper h1")[
                     0].get_text().strip().split('\n'))
    data['title'] = title
    rating = soup.select("#rating #n_rating")[0].get('value')
    data['rating'] = rating
    date = soup.select("#interest_sect_level > div span")[1].get_text().strip()
    data['date'] = date
    # try:
    #     comment = soup.select("#interest_sect_level span")[
    #         5].get_text().strip()
    #     data['comment'] = comment
    # except:
    #     pass
    print(data)
    result.append(data)


def main():
    url = f'https://book.douban.com/people/{user}/collect'
    r = getUrl(url)
    if r is None:
        return

    html = r.text
    soup = BeautifulSoup(html, "html.parser")

    # 我读过的书(xxx)
    print(html)
    total = int(soup.select("#content .info h1")[0].get_text().strip()[6:-1])

    urls = ['https://book.douban.com/people/{}/collect?sort=time&start={}'.format(
        user, str(i)) for i in range(0, total, 15)]
    for url in urls:
        getInfo(url)


try:
    main()
except:
    pass

output.write(json.dumps(result, ensure_ascii=False, indent=4))

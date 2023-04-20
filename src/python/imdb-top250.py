import requests
from bs4 import BeautifulSoup

# get IMDB Top 250

url = 'https://www.imdb.com/chart/top'
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0'}
r = requests.get(url, headers=headers)
soup = BeautifulSoup(r.text, "html.parser")
items = soup.select('.titleColumn > a')
for item in items:
    print(item.get_text())

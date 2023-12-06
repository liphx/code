#!/usr/bin/env python3
# https://docs.github.com/en/rest/reference/repos

import os
import requests


# list repos from user or organization
# type: 'users', 'orgs'
def getRepos(name, type='users'):
    params = {'accept': 'application/vnd.github.v3+json',
              'per_page': 100, 'page': 1}
    url = f'https://api.github.com/{type}/{name}/repos'
    r = requests.get(url, data=params)
    if r.status_code != 200:
        print(f'get {url} fail, status_code: {r.status_code}')
        return None

    repos = []
    for repo in r.json():
        repos.append(repo['full_name'])
    return repos


# get url from full name like liphx/example
def getUrl(repo, type='https'):
    if type == 'https':
        return f'https://github.com/{repo}.git'
    elif type == 'ssh' or type == 'git':
        return f'git@github.com:{repo}.git'
    else:
        return None

# get local dir from full name like liphx/example


def getDir(repo):
    return repo.split('/')[-1]


def clone(url):
    os.system(f'git clone {url}')


def main():
    repos = getRepos('liphx')
    repos += getRepos('BladeAlien', 'orgs')
    if repos is None:
        return
    print('collect repos:', repos)

    for repo in repos:
        url = getUrl(repo, 'ssh')
        if os.path.exists(getDir(repo)):
            print(f'repo {repo} exists')
        else:
            clone(url)


if __name__ == '__main__':
    main()

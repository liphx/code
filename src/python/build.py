#!/usr/bin/env python3

import os
import time
import json
import string

public_dir = os.path.abspath("./public")
old_str = '<!-- -->'

def execute_cmd(cmd):
    print(cmd)
    os.system(cmd)

def mkdir_p(dir):
    if not os.path.exists(dir):
        execute_cmd("mkdir -p {}".format(dir))

def md2html(md_file, html_file):
    cmd = 'pandoc -f markdown -t html -c /css/common.css -s -A css/after-body --quiet --template=pandoc-template'
    mkdir_p(os.path.dirname(html_file))
    execute_cmd('{} {} -o {}'.format(cmd, md_file, html_file))

def get_file_modify_time(path):
    if not os.path.exists(path):
        return None
    return time.localtime(os.path.getmtime(path))

def list_files(path, end):
    ret = []
    for file in os.listdir(path):
        p = os.path.join(path, file)
        if os.path.isfile(p) and p.endswith(end):
            ret.append(p)
        elif os.path.isdir(p):
            if os.path.abspath(p) != public_dir:
                ret += list_files(p, end)
    return ret

def gen_html():
    md_files = list_files('.', '.md')
    html_files = []
    for md_file in md_files:
        html_file = os.path.join(public_dir, md_file[0:-2] + 'html')
        t1 = get_file_modify_time(md_file)
        t2 = get_file_modify_time(html_file)
        if t2 == None or t1 > t2: # html file doesn't exist or md file is newer than html file
            print('update {} ...'.format(html_file), 'modify_time is ' + time.strftime('%F %T', t1))
            md2html(md_file, html_file)
            add_maintain(html_file, time.strftime('%F %T', t1))
            print('done.')

def add_maintain(path, time_str):
    str = '''<hr/ >
<div style="text-align: right"><p style="display: inline"> update time <p style="display: inline; color: #3300ff">{}</p> <p></div>
'''.format(time_str)
    update_html(path, old_str, str)

def update_html(path, old_str, str):
    lines = []
    with open(path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for index, line in enumerate(lines):
            if line.find(old_str) != -1:
                lines[index] = line.replace(old_str, str)
                break
        f.close()
    with open(path, 'w', encoding='utf-8') as f:
        for line in lines:
            f.writelines(line)
        f.close()

def make_public():
    mkdir_p(public_dir)

    public_files = []
    suffix = ['.html', '.js', '.css', '.jpg', '.ico', '.json', '.py']
    for i in suffix:
        files = list_files('.', i)
        public_files += files

    for file in public_files:
        new_file = os.path.join(public_dir, file)
        t1 = get_file_modify_time(file)
        t2 = get_file_modify_time(new_file)
        if t2 == None or t1 > t2:
            mkdir_p(os.path.dirname(new_file))
            execute_cmd('cp {} {}'.format(file, new_file))

if __name__ == '__main__':
    with open('css/after-body') as f:
        old_str = f.read()
    make_public()
    gen_html()
    execute_cmd('rm -f public/pandoc-template.html')

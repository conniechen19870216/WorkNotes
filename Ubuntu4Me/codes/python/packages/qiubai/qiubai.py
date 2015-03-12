#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ====================
# Tool Box
# 1. User-agent: used to build a request header
#    Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0
# 2. .*? is pretty cool
# 3. "-"*40
# 4. raw_input
# ====================

import urllib2
import re


def qiubai(page):
    # 1. urllib2.urlopen to generate html strings
    url = "http://www.qiushibaike.com/hot/page/%d" % page
    request = urllib2.Request(url)
    request.add_header('User-Agent',
    'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0')
    html = urllib2.urlopen(request).read()

    # 2. re_qb.findall(html)
    re_qb = re.compile(r'<div.*?class="content".*?title="(.*?)">(.*?)</div>',
                       re.DOTALL)
    my_qiubai = re_qb.findall(html)
    n = len(my_qiubai)
    for i in range(n):
        for k in range(2):
            print my_qiubai[i][k]
        s = raw_input("回车继续: ")
        if s == 'q':
            exit()
        print '-'*40


def qb_pages(start_page):
    for each_page in range(int(start_page), 280):
        print "-"*18+"第"+str(each_page)+"页"+"-"*18
        qiubai(each_page)


def qb_crawl():
    p = raw_input("输入要看的页数1~280：")
    if p == 'q':
        exit()
    elif not p.isdigit() or p == '0' or int(p) > 280:
        qb_crawl()
    else:
        qb_pages(p)


print "-"*40
print "糗事命令行版"
print "输入'q'退出程序"
print "-"*40
qb_crawl()

#!/usr/bin/env python
# -*- coding: utf-8 -*-
bonus1 = 10000 * 0.1
bonus2 = (20000 - 10000) * 0.075 + bonus1
bonus4 = (40000 - 20000) * 0.005 + bonus2
bonus6 = (60000 - 40000) * 0.003 + bonus4
bonus10 = (100000 - 60000) * 0.015 + bonus6

i = int(raw_input("input gain:\n"))
if i <= 10000:
    bonus = i * 0.1
elif i <= 20000:
    bonus = bonus1 + (i - 10000) * 0.075
elif i <= 400000:
    bonus = bonus2 + (i - 200000) * 0.05
elif i <= 600000:
    bonus = bonus4 + (i - 400000) * 0.03
elif i <= 1000000:
    bonus = bonus6 + (i - 600000) * 0.015
else:
    bonus = bonus10 + (i - 1000000) * 0.01
print 'bonus = ',bonus

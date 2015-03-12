#!/usr/bin/env python
# -*- coding:utf-8 -*-

# ====================
# Tool Box
# 1. libraries: re, funtions.reduce
# 2. [ns.append(self.__toType(s))) for s in patternStr]
# 3. __str__: called by print function (format method)
# 4. defination of reduce funtion (__toType)
"""
def reduce(function, iterable, initializer=None):
    it = iter(iterable)
    if initializer is None:
        try:
            initializer = next(it)
        except StopIteration:
            raise TypeError('reduce() of empty sequence with no initial value')
    accum_value = initializer
    for x in it:
        accum_value = function(accum_value, x)
    return accum_value
"""
# 5. zip built-in function
# 6. re: '[a-z]{1}', '[A-Z]{2}', '[0-9]{3}', '[\s]{4}'
"""
\s: 空白字符
\S: 非空白字符
\d: 数字
\D: 非数字
\w: 数字或者字母
\W: 非数字且非字母
"""
# 7.1 re.compile returns a regular expression object: RegexObject
# 7.2 re.match, re.search return a match object: MatchObject
# ====================


# ========= 1. imports ==========
import re
from functools import reduce


# ========= 2. define class RePattern ==========
class RePattern():
    # __init__
    def __init__(self, patternStr):
        self.pattern = ''
        self.pre = None
        self.patternLength = len(patternStr)
        self.bCount = 1
        self.block = []
        self.blockType = []
        self.typedict = {'lower': '[a-z]{', 'upper': '[A-Z]{',
                         'digit': '[0-9]{', 'space': '[\s]{',
                         'other': '[\D\W\S]{'}
        self.__setPattern(patternStr)

    # __setPattern
    def __setPattern(self, patternStr):
        ns = []
        """ normal operation
        for s in patternStr:
            ns.append(self.__toType(s))
        """
        [ns.append(self.__toType(s)) for s in patternStr]
        ns.append('end')
        reduce(self.__same, ns)

        for btype, blen in zip(self.blockType, self.block):
            self.pattern += self.typedict[btype]+str(blen)+'}'
        self.pre = re.compile(self.pattern)

    # __toType
    def __toType(self, s):
        if s.islower():
            return 'lower'
        elif s.isupper():
            return 'upper'
        elif s.isdigit():
            return 'digit'
        elif s.isspace():
            return 'space'
        else:
            return 'other'

    # __same
    def __same(self, a, b):
        if a is b:
            self.bCount += 1
        else:
            self.block.append(self.bCount)
            self.blockType.append(a)
            self.bCount = 1
        return b

    # __str__
    def __str__(self):
        return 'block:{0}\nblockType:{1}\npattern :{2}'\
            .format(self.block, self.blockType, self.pattern)

    # isPattern
    def isPattern(self, compareStr):
        tmp = self.pre.match(compareStr)
        if tmp:
            return tmp.group()


# ========= 3. pickFromFile =========
def pickFromFile(local_file, pattern):
    match_list = []
    rp = RePattern(pattern)
    patternLength = rp.patternLength
    print(rp)

    try:
        with open(local_file) as f:
            for line in f:
                compareTimes = len(line) - patternLength
                for n in range(compareTimes):
                    comp = rp.isPattern(line[n:n+patternLength])
                    if comp:
                        match_list.append(comp)
    except IOError as ioerr:
        print('file error: ' + ioerr)

    return match_list


# ========= 4. run tests ==========
print("running tests...")
match_strs = pickFromFile("data/data.txt", 'xxx %XX00')
print("test results:")
for x in match_strs:
    print(x)

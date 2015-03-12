def sanitize(timestr):
    if '-' in timestr:
        splitter = '-'
    elif ':' in timestr:
        splitter = ':'
    else:
        return(timestr)

    (mins, secs) = timestr.split(splitter)
    return(mins + '.' + secs)

def getdata(filename):
    try:
        with open(filename) as f:
            return(f.readline().strip().split(','))
    except IOError as ioerr:
        print('file error: ' + str(ioerr))
        return(None)

james = getdata('james.txt')
julie = getdata('julie.txt')
mikey = getdata('mikey.txt')
sarah = getdata('sarah.txt')

print(sorted(set([sanitize(each_item) for each_item in james]))[0:3])
print(sorted(set([sanitize(each_item) for each_item in julie]))[0:3])
print(sorted(set([sanitize(each_item) for each_item in mikey]))[0:3])
print(sorted(set([sanitize(each_item) for each_item in sarah]))[0:3])

""" replace by getdata function
with open('james.txt') as jaf:
    james = jaf.readline().strip().split(',')
with open('julie.txt') as juf:
    julie = juf.readline().strip().split(',')
with open('mikey.txt') as mif:
    mikey = mif.readline().strip().split(',')
with open('sarah.txt') as saf:
    sarah = saf.readline().strip().split(',')

"""

"""
old code I:
clean_james = []
clean_julie = []
clean_mikey = []
clean_sarah = []

for each_item in james:
    clean_james.append(sanitize(each_item))
for each_item in julie:
    clean_julie.append(sanitize(each_item))
for each_item in mikey:
    clean_mikey.append(sanitize(each_item))
for each_item in sarah:
    clean_sarah.append(sanitize(each_item))
"""
"""
old code II:
clean_james = [sanitize(each_item) for each_item in james]
clean_julie = [sanitize(each_item) for each_item in julie]
clean_mikey = [sanitize(each_item) for each_item in mikey]
clean_sarah = [sanitize(each_item) for each_item in sarah]

print(sorted(clean_james))
print(sorted(clean_julie))
print(sorted(clean_mikey))
print(sorted(clean_sarah))
"""

"""
stage 2:
unique_james = []
unique_julie = []
unique_mikey = []
unique_sarah = []

for each_t in clean_james:
    if each_t not in unique_james:
        unique_james.append(each_t)
for each_t in clean_julie:
    if each_t not in unique_julie:
        unique_julie.append(each_t)
for each_t in clean_mikey:
    if each_t not in unique_mikey:
        unique_mikey.append(each_t)
for each_t in clean_sarah:
    if each_t not in unique_sarah:
        unique_sarah.append(each_t)

print(sorted(unique_james)[0:3])
print(sorted(unique_julie)[0:3])
print(sorted(unique_mikey)[0:3])
print(sorted(unique_sarah)[0:3])
"""

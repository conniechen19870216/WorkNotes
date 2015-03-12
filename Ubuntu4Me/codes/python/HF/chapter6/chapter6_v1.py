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
            data = f.readline().strip().split(',')
            return({'Name': data.pop(0),
                    'Time': str(sorted(set([sanitize(t) for t in data]))[0:3])})
    except IOError as ioerr:
        print('file error: ' + str(ioerr))
        return(None)

james = getdata('james.txt')
julie = getdata('julie.txt')
mikey = getdata('mikey.txt')
sarah = getdata('sarah.txt')

print(james['Name'] + "'s fastest times are: " + james['Time'])
print(julie['Name'] + "'s fastest times are: " + julie['Time'])
print(mikey['Name'] + "'s fastest times are: " + mikey['Time'])
print(sarah['Name'] + "'s fastest times are: " + sarah['Time'])

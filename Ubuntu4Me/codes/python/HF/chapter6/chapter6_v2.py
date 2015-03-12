def sanitize(timestr):
    if '-' in timestr:
        splitter = '-'
    elif ':' in timestr:
        splitter = ':'
    else:
        return(timestr)

    (mins, secs) = timestr.split(splitter)
    return(mins + '.' + secs)

class Athlete:
    def __init__(self, name, time = []):
        self.name = name
        self.time = time
    def top3(self):
        return(sorted(set([sanitize(t) for t in self.time]))[0:3])
    def add_time(self, time):
        self.time.append(time)
    def add_times(self, ltime):
        self.time.extend(ltime)

def getdata(filename):
    try:
        with open(filename) as f:
            data = f.readline().strip().split(',')
            return(Athlete(data.pop(0), data))
    except IOError as ioerr:
        print('file error: ' + str(ioerr))
        return(None)

james = getdata('james.txt')
julie = getdata('julie.txt')
mikey = getdata('mikey.txt')
sarah = getdata('sarah.txt')

print(james.name + "'s fastest times are: " + str(james.top3()))
print(julie.name + "'s fastest times are: " + str(julie.top3()))
print(mikey.name + "'s fastest times are: " + str(mikey.top3()))
print(sarah.name + "'s fastest times are: " + str(sarah.top3()))

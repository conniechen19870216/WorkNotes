import pickle
"""
stage 1: generate man and other lists
"""
man = []
other = []
try:
    with open('kavon') as data:
        for each_line in data:
            try:
                (role, line_spoken) = each_line.split(':', 1)
                line_spoken = line_spoken.strip()
                if role == 'Man':
                    man.append(line_spoken)
                elif role == 'Other Man':
                    other.append(line_spoken)
            except ValueError as verr:
                print('value error: ' + line_spoken + str(verr))
except IOError as ioerr:
    print('file error: ' + str(ioerr))
print(man)
print(other)

"""
stage 2: pickle dump
"""
try:
    with open('man', 'wb') as man_file, open('other', 'wb') as other_file:
        pickle.dump(man, man_file)
        pickle.dump(other, other_file)
except IOError as ioerr:
    print('file error: ' + str(ioerr))
except pickle.PickleError as perr:
    print('pickle error: ' + str(perr))
"""
stage 3: pickle load
"""
man_load = []
other_load = []
try:
    with open('man', 'rb') as man_file, open('other', 'rb') as other_file:
        man_load = pickle.load(man_file)
        other_load = pickle.load(other_file)
except IOError as ioerr:
    print('file error: ' + str(ioerr))
except pickle.PicklingError as perr:
    print('pickle error: ' + str(perr))
print(man_load[0])
print(other_load[-1])

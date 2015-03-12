import pickle
import athlist

def put_to_store(files):
    all_ath = {}
    for each_file in files:
        ath = athlist.getdata(each_file)
        all_ath[ath.name] = ath
    try:
        with open('ath.pickle', 'wb') as athf:
            pickle.dump(all_ath, athf)
    except IOError as ioerr:
        print('io error: ' + str(ioerr))

    return(all_ath)

def get_from_store():
    all_ath = {}
    try:
        with open('ath.pickle', 'rb') as athf:
            all_ath = pickle.load(athf)
    except IOError as ioerr:
        print('io error: ' + str(ioerr))
    return(all_ath)

file_list = ['james.txt', 'julie.txt', 'mikey.txt', 'sarah.txt']
data = put_to_store(file_list)
for each_ath in data:
    print(each_ath + str(data[each_ath].top3()))

data_copy = get_from_store()
for each_ath in data_copy:
    print(each_ath + str(data_copy[each_ath].top3()))

from sys import *
import os;
import hashlib;

def hashfile(path, blocksize = 1024):
    fd = open(path, 'rb')
    hasher = hashlib.md5()
    buff = fd.read(blocksize)

    while len(buff) > 0:
        hasher.update(buff)
        buff = fd.read(blocksize)

    fd.close()

    return hasher.hexdigest()

def FindDuplicate(path):
    flag = os.path.isabs(path)

    if flag == False:
        path = os.path.abspath(path)
    
    exists = os.path.isdir(path)

    dups = {}
    if exists:
        for dirName, subdirs, fileList in os.walk(path):
            for filen in fileList:
                path = os.path.join(dirName, filen)
                file_hash = hashfile(path)
                if file_hash in dups:
                    dups[file_hash].append(path)
                else:
                    dups[file_hash] = [path]

        return dups
    else:
        print ("Invalid path")

def PrintDuplicate(dict1):
    results = list(filter(lambda x: len(x) > 1, dict1.values()))

    if len(results) > 0:
        print ("Duplicate files found")

        print ("The following are identical files.")

        icnt = 0
        for result in results:
            for subresult in result:
                icnt+=1
                if icnt >= 2:
                    print ("\t\t%s" % subresult)
    else:
        print("No dulplicate files found")


def main():
    if len(argv) != 2:
        print ("ERROR! Invalid arguments")

    try:
        arr = {}
        arr = FindDuplicate(argv[1])
        PrintDuplicate(arr)

    except ValueError:
        print("ERROR: Invalid datatype of imput")

if __name__ == "__main__":
    main()
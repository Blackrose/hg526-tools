# convert hg526 cfg to xml and xml to cfg

import sys
import getopt

def decode(content):
    #print content
    out = ""
    for i in content:
        if (ord(i)%2) == 0:
           out += (chr((ord(i)/2)))
        else:
            out += (chr((ord(i) + 127) /2))

    return out

def encode(content):
    out = ""
    for i in content:
        if ((ord(i)*2) > 127): 
            out += chr(ord(i)*2 - 127)
        else:
            out += chr(ord(i)*2)
    return out

def usage():
    print "convert -d -i srcfile -o outfile"
    print "-d: decode the cfg file to xml file"
    print "-e: encode the xml to cfg file"
    print "-src file: 'file' is converte form this file"
    print "-trg file: 'file' is converted save to this file"

def parserargs(argv):
    srcfile = ""
    trgfile = ""
    flag = False
    try:
        opts, args = getopt.getopt(argv, "dei:o:")
    except:
        usage()
        sys.exit()

    for opt, arg in opts:
        if opt == '-i':
            srcfile = arg
        elif opt == "-o":
            trgfile = arg
        elif opt == '-d':
            flag = True
        elif opt == '-e':
            flag = False

    if flag == True:
        return "decode", srcfile, trgfile
    else:
        return "encode", srcfile, trgfile



if __name__ == "__main__":
    
    ret = parserargs(sys.argv[1:])
    srcf = open(ret[1], 'r')
    outf = open(ret[2],'wb')
    
    buf = srcf.read()
    
    if ret[0] == "decode":
        print "decoding file...."
        outf.write(decode(buf))
    else:
        print "encoding file...."
        outf.write(encode(buf))
    
    srcf.close()
    outf.close()

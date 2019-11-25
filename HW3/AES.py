from PIL import Image
from Crypto.Cipher import AES
from sys import argv

if len(argv) != 4:
    print('Wrong number of parameters')
    exit(0)

Mode = {'ECB' : 1, 'CBC' : 2, 'extend' : 1}
mode = Mode[argv[1]]
key = argv[2]
if (len(key) != 16 and len(key) != 24 and len(key) != 32 ):
    print('Wrong length of key')
    exit(0)
plaintextJPEG = Image.open(argv[3]) # load plaintext image
key = key.encode(encoding='utf-8')

plaintextJPEG.save('./plaintext.ppm', 'ppm')


def ECB():
    ## encrypt
    plaintextPPM = open('./plaintext.ppm', 'rb') # read plaintext as binary file
    cipher = open('./cipher.ppm', 'wb')

    for i in range(3):
        buf = plaintextPPM.readline()
        cipher.write(buf)

    cipherBlock = AES.new(key, mode)

    buf = plaintextPPM.read(16)
    while buf:
        # PKCS
        padding = 16 - len(buf)
        if padding != 0:
            for i in range(padding):
                buf += bytes([padding])
        
        ciphertext = cipherBlock.encrypt(buf)
        cipher.write(ciphertext)
        buf = plaintextPPM.read(16)

    plaintextPPM.close()
    cipher.close()
    # output cipher image
    cipherPPM = Image.open('./cipher.ppm')
    cipherPPM.save('./cipher.jpg', 'jpeg')
    cipherPPM.close()


    ## decrypt

    cipher = open('./cipher.ppm', 'rb')
    decrypt = open('./decrypt.ppm', 'wb')
    for i in range(3):
        buf = cipher.readline()
        decrypt.write(buf)

    buf = cipher.read(16)
    while buf:
        # PKCS
        padding = 16 - len(buf)
        if padding != 0:
            for i in range(padding):
                buf += bytes([padding])
    
        ciphertext = cipherBlock.decrypt(buf)
        decrypt.write(ciphertext)
        buf = cipher.read(16)

    decrypt.close()
    cipher.close()
    decrypt = Image.open('decrypt.ppm')
    decrypt.save('decrypt.jpg', 'jpeg')
    print('ECB complete')

def CBC():
    print('CBC')

def extend():
    print('extend')

if argv[1] == 'ECB':
    ECB()
elif argv[1] == 'CBC':
    CBC()
elif argv[1] == 'extend':
    extend()
else:
    print('Wrong mode')
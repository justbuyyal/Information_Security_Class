from PIL import Image
from Crypto.Cipher import AES
from sys import argv
from Crypto import Random

if len(argv) != 4:
    print('Wrong number of parameters')
    exit(0)

mode = argv[1]
key = argv[2]
if (len(key) != 16 and len(key) != 24 and len(key) != 32 ):
    print('Wrong length of key')
    exit(0)
plaintextJPEG = Image.open(argv[3]) # load plaintext image
key = key.encode(encoding='utf-8')

plaintextJPEG.save('./plaintext.ppm', 'ppm')
plaintextPPM = open('./plaintext.ppm', 'rb') # read plaintext as binary file
cipher = open('./cipher.ppm', 'wb')
cipherBlock = AES.new(key, AES.MODE_ECB)

def ECB():
    ## encrypt
    global cipher
    
    for i in range(3):
        buf = plaintextPPM.readline()
        cipher.write(buf)

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
    cipherPPM.save('./encrypt.jpg', 'jpeg')
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
    decrypt.close()
    print('ECB complete')

def CBC():
    ## encrypt
    global cipher
    for i in range(3):
        buf = plaintextPPM.readline()
        cipher.write(buf)
    iv = Random.new().read(AES.block_size)
    de_iv = iv
    
    buf = plaintextPPM.read(16)
    while buf:
        # PKCS
        padding = 16 - len(buf)
        if padding != 0:
            for i in range(padding):
                buf += bytes([padding])    
        temp = []
        # XOR
        for b, i in zip(buf, iv):
            temp.append(b^i)
        buf = bytes(temp)
        # encrypt
        ciphertext = cipherBlock.encrypt(buf)
        iv = ciphertext
        cipher.write(ciphertext)
        buf = plaintextPPM.read(16)
    
    plaintextPPM.close()
    cipher.close()
    cipherPPM = Image.open('./cipher.ppm')
    cipherPPM.save('./encrypt.jpg', 'jpeg')
    cipherPPM.close()

    ##decrypt
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
        # decrypt
        ciphertext = cipherBlock.decrypt(buf)
        temp = []
        # XOR
        for d, i in zip(ciphertext, de_iv):
            temp.append(d^i)
        ciphertext = bytes(temp)
        de_iv = buf
        decrypt.write(ciphertext)
        buf = cipher.read(16)


    decrypt.close()
    decrypt = Image.open('./decrypt.ppm')
    decrypt.save('decrypt.jpg', 'jpeg')
    print('CBC complete')

def extend():
    ##encrypt
    global cipher
    global key

    for i in range(3):
        buf = plaintextPPM.readline()
        cipher.write(buf)

    buf = plaintextPPM.read(16)
    while buf:
        # plaintext PKCS
        padding = 16 - len(buf)
        if padding != 0:
            for i in range(padding):
                buf+=bytes([padding])
        # Split key into two keys
        key_1 = key[:8]
        key_2 = key[8:]
        # PKCS
        padding = 16 - len(key_1)
        for i in range(padding):
            key_1 += bytes([padding])
            key_2 += bytes([padding])
        # XOR with key_1
        temp = []
        for b, k in zip(buf, key_1):
            temp.append(b^k)
        plainCipher = bytes(temp)
        # encrypt
        Cipher = cipherBlock.encrypt(plainCipher)
        temp.clear()
        # XOR with key_2
        for c, k in zip(Cipher, key_2):
            temp.append(c^k)
        Cipher = bytes(temp)
        cipher.write(Cipher)
        # next key is current ciphertext
        key = Cipher
        buf = plaintextPPM.read(16)
    plaintextPPM.close()
    cipher.close()
    cipherPPM = Image.open('./cipher.ppm')
    cipherPPM.save('./encrypt.jpg', 'jpeg')
    cipherPPM.close()


    ## decrypt
    cipher = open('./cipher.ppm', 'rb')
    decrypt = open('./decrypt.ppm', 'wb')
    for i in range(3):
        buf = cipher.readline()
        decrypt.write(buf)
    
    buf = cipher.read(16)
    while buf:
        # plaintext PKCS
        padding = 16 - len(buf)
        if padding != 0:
            for i in range(padding):
                buf+=bytes([padding])
        # Split key into two keys
        key_1 = key[8:]
        key_2 = key[:8]
        # PKCS
        padding = 16 - len(key_1)
        for i in range(padding):
            key_1 += bytes([padding])
            key_2 += bytes([padding])
        # XOR with key_1
        temp = []
        for b, k in zip(buf, key_1):
            temp.append(b^k)
        dplain = bytes(temp)
        # decrypt
        dCipher = cipherBlock.decrypt(dplain)
        temp.clear()
        # XOR with key_2
        for c, k in zip(dCipher, key_2):
            temp.append(c^k)
        dCipher = bytes(temp)
        decrypt.write(dCipher)
        # next key is current ciphertext
        key = buf
        buf = cipher.read(16)

    cipher.close()
    decrypt.close()
    decryptPPM = Image.open('./decrypt.ppm')
    decryptPPM.save('./decrypt.jpg', 'jpeg')
    decryptPPM.close()

    print('extend complete')

if mode == 'ECB':
    ECB()
elif mode == 'CBC':
    CBC()
elif mode == 'extend':
    extend()
else:
    print('Wrong mode')
    exit(0)

#cipherJPG = Image.open('./cipher.jpg')
#cipherJPG.show()
#decryptJPG = Image.open('./decrypt.jpg')
#decryptJPG.show()
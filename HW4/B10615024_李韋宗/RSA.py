import random
from sys import argv

# Exponent e, base element x, Modulus n
def Square_and_Multiply(x, e, n):   
    h = "{0:b}".format(e)
    y = x
    for i in range(1, len(h)):
        y = (y ** 2) % n
        if (h[i] == '1'):
            y = (y * x) % n
    return y

def gcd(a, b): 
    if(b==0): 
        return a 
    else: 
        return gcd(b, a%b)

def keyGeneration(p, q):
    n = p * q
    phi_N = (p - 1) * (q - 1)
    # print('phi_N', phi_N)
    e = 3
    d = 1
    ## e in {1, 2, ... phi_N - 1}, gcd(e, phi_N) = 1
    while (gcd(e, phi_N) != 1 and e < phi_N):
        e += 2
    # print('e', e)

    ## d * e = 1 mod phi_N
    k = 1
    i = 3
    while((k * phi_N + 1) % e != 0):
        k += 1
        
    d = (k * phi_N + 1) // e


    
    # print('d * e mod phi_n=', (d * e) % phi_N)
    return e, d, n

def Miller_Rabin(p):
    N = p - 1
    m = 0
    k = 0
    while (m % 2 == 0):
        N = N >> 1
        m = N
        k += 1
    a = random.randrange(2, p - 2)
    
    b = Square_and_Multiply(a, m, p)
    if (b != 1 and b != p - 1):
        i = 1
        while(i < k and b != p - 1):
            b = Square_and_Multiply(b, 2, p)
            if (b == 1):
                return False
            i += 1
        if (b != p - 1):
            return False
    return True

# source: http://jianiau.blogspot.com/2014/05/rsa-decrypt-with-crt.html
def Chinese_Remiander_Theorem(d, n, p, q):
    d_p = d % (p - 1)
    d_q = d % (q - 1)
    inv_q = Square_and_Multiply(q, p - 2, p)
    return d_p, d_q, inv_q

if (len(argv) < 3):
    print('Wrong number of parameters')
    exit(0)

# Initial key
# init {bit}
if (argv[1] == 'init'):
    if (argv[2] == 'ex'):
        bit = argv[3]
    else:    
        bit =  argv[2]
    bit = int(bit)
    while (True):
        num = random.randrange(2 ** bit + 1, 2 ** (bit + 1), 2)
        if (Miller_Rabin(num)):
            break
    p = num
    while (True):
        num = random.randrange(2 ** bit + 1, 2 ** (bit + 1), 2)
        if (Miller_Rabin(num)):
            break
    q = num
    # key[0] => e
    # key[1] => d
    # key[2] => n
    e, d, n = keyGeneration(p, q)
    print('key\np:', p, '\nq:', q ,'\nn:', n, '\ne:', e, '\nd:', d)

    if (argv[2] == 'ex'):
        # get key[0]=> d_p, key[1]=> d_q, key[2]=> inv_q
        d_p, d_q, inv_q = Chinese_Remiander_Theorem(d, n, p, q)
        print('d_p:', d_p, '\nd_q:', d_q, '\ninv_q:', inv_q)
    print('')

# Encrypt
# -e {n} {e} {plaintext}
elif (argv[1] == '-e'):
    plaintext = argv[4]
    n = int(argv[2], 0)
    e = int(argv[3], 0)
    maxDigit = len(argv[2]) - 1
    temp = ''
    for c in plaintext:
        temp += str(ord(c))
    # print('temp:', temp)
    print('Encryption ciphertext:')
    for i in range(0, len(temp), maxDigit):
        plaintext = int(temp[i : i + maxDigit])
        encrypt = Square_and_Multiply(plaintext, e, n)
        print(encrypt)
    print('')

# Decrypt
# -d {n} {d} {[ciphertext]...}
elif (argv[1] == '-d'):
    decrypt = ''
    decryptText = ''
    print('Decryption plaintext:')


    # extra mode (Chinese_Remiander_Theorem)
    # -d ex {n} {d} {p} {q} {d_p} {d_q} {inv_q} {[ciphertext]...}
    if (argv[2] == 'ex'):
        n = int(argv[3], 0)
        d = int(argv[4], 0)
        p = int(argv[5], 0)
        q = int(argv[6], 0)
        d_p = int(argv[7], 0)
        d_q = int(argv[8], 0)
        inv_q = int(argv[9], 0)
        
        for i in range(10, len(argv)):
            ciphertext = int(argv[i])
            m_1 = Square_and_Multiply(ciphertext, d_p, p)
            m_2 = Square_and_Multiply(ciphertext, d_q, q)
            h = inv_q * (m_1 - m_2) % p
            decrypt += str(m_2 + h * q)
    else:
        n = int(argv[2], 0)
        d = int(argv[3], 0)
        for i in range(4, len(argv)):
            ciphertext = int(argv[i], 0)
            decrypt += str(Square_and_Multiply(ciphertext, d, n))

    while(len(decrypt) != 0):
        if (decrypt[0] == '1'):
            if (int(decrypt[0 : 3]) > 126):
                decrypt = decrypt[ : 1] + '0' + decrypt[1 : ]
                # print('insert 0')
            decryptText += chr(int(decrypt[0 : 3]))
            decrypt = decrypt[3 : ]
        else:
            decryptText += chr(int(decrypt[0 : 2]))
            decrypt = decrypt[2 : ]
        # print(decrypt)
    print(decryptText, '\n')

else:
    print('Parameters error')

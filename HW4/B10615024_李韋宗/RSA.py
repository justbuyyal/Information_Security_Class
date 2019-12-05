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

def gcd(a,b): 
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

# p = 33478071698956898786044169848212690817704794983713768568912431388982883793878002287614711652531743087737814467999489
# q = 36746043666799590428244633799627952632279158164343087642676032283815739666511279233373417143396810270092798736308917

# p = 22111
# q = 54787

# p = 199
# q = 197

if(len(argv) != 3 and len(argv) != 5):
    print('Wrong number of parameters')
else:
    # Initial key
    if (argv[1] == 'init'):
        bit = argv[2]
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
        key = keyGeneration(p, q)
        print('key\np:', p, '\nq:', q ,'\nn:', key[2], '\ne', key[0], '\nd:', key[1])
    
    # Encrypt
    elif (argv[1] == '-e'):
        plaintext = argv[2]
        n = agrv[3]
        e = argv[4]
        encrypt = Square_and_Multiply(plaintext, e, n)
        print('Encryption ciphertext:', encrypt)
    
    # Decrypt
    elif (argv[1] == '-d'):
        ciphertext = argv[2]
        n = agrv[3]
        d = argv[4]
        decrypt = Square_and_Multiply(ciphertext, d, n)
        print('Decryption plaintext:', decrypt)

    else:
        print('Parameters error')

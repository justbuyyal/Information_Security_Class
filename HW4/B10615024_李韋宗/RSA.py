import random

# Exponent e, base element x, Modulus n
def Square_and_Multiply(x, e, n):   
    h = "{0:b}".format(e)
    y = x
    for i in range(1, len(h)):
        y = (y ** 2) % n
        if (h[i] == '1'):
            y = (y * x) % n
    return y

def hcfnaive(a,b): 
    if(b==0): 
        return a 
    else: 
        return hcfnaive(b,a%b)

def keyGeneration(p, q):
    n = p * q
    phi_N = (p - 1) * (q - 1)
    e = 3
    d = 7
    # e in {1, 2, ... phi_N - 1}, gcd(e, phi_N) = 1
    # d * e = 1 mod phi_N
    return e, d, n


p = 3
q = 11
a = keyGeneration(p,q)
# a[0] => e
# a[1] => d
# a[3] => n

x = 4
encrypt = Square_and_Multiply(x, a[0], a[2])
decrypt = Square_and_Multiply(encrypt, a[1], a[2])
print(encrypt)
print(decrypt)


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

print(Miller_Rabin(3388495837466721394368393204672181522815830368604993048084925840555281177))
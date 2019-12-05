# RSAREADME
* The notation will use below to represent basic encryption rules

    `n = p * q`

    ciphertext = plaintext $^e$ mod `n`

    plaintext = ciphertext $^d$ mod `n`

## Command need to be follow below

### Initial a k-bit key
* Will random two k-bit prime `p` and `q`, and return `p`, `q`, `n`, `e`, `d`
    
    `./RSA.py init {k-bit}`

### Encryption
* Encrypt a string into one or more integer, depending on how large `n` is.

    `./RSA.py -e {n} {e} {plaintext}`
* **NOTE: plaintext need to be in ascii 0x21 to 0x7E, but not quotation marks(")**
* **NOTE: if the plaintext has some punctuation or symbol(ex: $, #, ...), plaintext need to enclosed in quotes**

### Decryption
* Decrypt one or more integer into string, depending on how large `n` is.
    
    `./RSA.py -d {n} {d} {[ciphertext]...}`
* **NTOE: ciphertext will be one or more integer**




<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>
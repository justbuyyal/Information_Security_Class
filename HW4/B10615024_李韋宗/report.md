<p style="text-align:right;">
姓名:李韋宗<br>
學號:B10615024<br>
日期:Dec. 13, 2019<br>
</p>

<h1 style="text-align:center;"> Introduction to Information Security_HW4_Report

## 建置環境
* visual studio code
* python 3.7.5
---

## 操作⽅式
* 金鑰生成: 輸入整數key位元數； 輸出 `p`, `q`, `n`, `e`, `d`
    * extra mode 多輸出 `d_p`, `d_q`, `inv_q`
* 加密: 輸入 `n`, `e`, 文字plaintext； 輸出1或數個整數
* 解密: 輸入 `n`, `d`, 1或數個整數ciphertext； 輸出1串文字
    * extra mode 多輸入 `p`, `q`, `d_p`, `d_q`, `inv_q` 在 `d` 與 `ciphertext` 之間
* extra mode: 使用Chinese_Remiander_Theorem，解密
* 指令如子資料夾中的[README](https://github.com/justbuyyal/Information_Security_Class/blob/master/HW4/B10615024_%E6%9D%8E%E9%9F%8B%E5%AE%97/README.md)中所示
* 注意1: 若plaintext含有符號，需要用雙引號包起來
* 注意2: key若使用的位元數過少，加密的輸出將可能是數個ciphertext(整數)；解密回去時，需要全部輸入，並以空格隔開
* 下方執行結果將會示範上述注意
---

## 執⾏結果圖
### 小質數時:
* 金鑰生成
![](https://i.imgur.com/58G8TqR.png)
* 由於`n`過小，加密會輸出數個ciphertext
![](https://i.imgur.com/ivgi5vv.png)
* 解密亦需要全部輸入並用空格隔開
![](https://i.imgur.com/QiZVC1S.png)

### 大質數時:
* 金鑰生成
![](https://i.imgur.com/03k6hX8.png)
* `n`夠大了，所以加密只輸出1組
![](https://i.imgur.com/sxzDwLf.png)
* 解密正常
![](https://i.imgur.com/qpMGjuP.png)

### Chinese_Remiander_Theorem(extra mode):
* 因為要輸入很多參數，故只用小位元展示
![](https://i.imgur.com/aEXWL3w.png)
![](https://i.imgur.com/RjMXb2g.png)

---

## 程式碼解說
### `Square_and_Multiply`
* 按照簡報虛擬碼實作
    ```python
    # Exponent e, base element x, Modulus n
    def Square_and_Multiply(x, e, n):   
        h = "{0:b}".format(e)
        y = x
        for i in range(1, len(h)):
            y = (y ** 2) % n
            if (h[i] == '1'):
                y = (y * x) % n
        return y
    ```

### GCD
* 輾轉相除法，找到最大公因數
    ```python
    def gcd(a,b): 
        if(b==0): 
            return a 
        else: 
            return gcd(b, a%b)
    ```

### 金鑰生成函數
* 由 `p` 和 `q` 生成`n`, `e`, `d`
* `e`從小數字開始找，通常在個位數質數就會完成尋找
* `d`則是透過 $d \times e \equiv 1$ mod $\phi(n)$，得到$d = \frac{(k \times \phi(n) + 1)}{e}$
* 由於`d`跟`e`皆為整數，透過迴圈暴力尋找是否`d`為整數，這裡是執行`init`時較慢的部分
    ```python
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
    ```
<br>
<br>
<br>

### `Miller Rabin test`
* 按照簡報虛擬碼實作
    ```python
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
    ```

### `Chinese_Remiander_Theorem`(CRT)
* 計算利用中國餘式定理會需要的參數
    ```python
    # source: http://jianiau.blogspot.com/2014/05/rsa-decrypt-with-crt.html
    def Chinese_Remiander_Theorem(d, n, p, q):
        d_p = d % (p - 1)
        d_q = d % (q - 1)
        inv_q = Square_and_Multiply(q, p - 2, p)
        return d_p, d_q, inv_q
    ```

### 金鑰生成
* 透過random.randrange且只隨機生成奇數，獲得 `p` 和 `q` ，以縮短時間
* 若為extra mode，額外計算參數並輸出
    ```python
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
    ```

### 加密
* `plaintext` 先逐字轉換成ascii，並串接成一個很大的整數 `temp` 暫存
* `maxDigit` 紀錄 `n` 的位數並減1，以確保所有進入加密的整數屬於 $\mathbb{Z}_n$
* 接著分次將`temp` 中 `maxDigit` 位數的數字丟進加密過程
* 所以如果 `n` 過小就會輸出數個 ciphertext
    ```python
    # Encrypt
    # -e {n} {e} {plaintext}
    elif (argv[1] == '-e'):
        plaintext = argv[4]
        n = int(argv[2])
        e = int(argv[3])
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
    ```

### 解密
* 將一或數個整數依次丟進解密得到加密時的大整數 `temp`
* 接著將大整數按照2或3位數，由ascii轉譯成文字即得到plaintext
* 由於可視字元的ascii介於21~126，所以判斷首個字是否為1即可知道要解譯2或3個位數
* 其中的 `if (int(decrypt[0 : 3]) > 126)` 判斷相會在最後進行解說
* 若為extra mode，計算CRT中的 $m_1$ 及 $m_2$，並且解密
    ```python
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
    ```
---

## 遇到困難與⼼得
起初 `Square_and_Multiply` 與 `Miller Rabin test` 過程都還算順利，但在做 `keyGeneration` 時，`d`的尋找一開始使用正向的暴力破解，計算 $k = \frac{ 1 - e \times d}{\phi(n)}$ 是否為整數，然而在1024bits時非常耗時，所以才修正成上述的寫法。

解碼的`if (int(decrypt[0 : 3]) > 126)` 判斷則是在測試時意外發現的，解密中明文所串接的大數字，在切割進入解密的過程後，可能有遇到0為開頭的輸出，造成原先ascii為10x的字輸出大於126，所以需要在解密遇到不可能為明文的ascii碼補0，才能正常解密

一開始沒大打算做CRT的，因為看不懂簡報，後來找到網路的說明之後懂了，也就一併實作，當中找 `q` 的反元素一開始用找 `d` 的方法，但比找 `d` 更浪費時間，於是透過費馬小定理 $a^{p-1} \equiv 1 (\mod p)$ ，同乘 $a^{-1}$ 得 $a^{-1} \equiv a^{p-2}(\mod p)$

---

## Reference
> https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/

<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>
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
* 金鑰生成: 輸出 `p`, `q`, `n`, `e`, `d`
* 加密: 輸入 `n`, `e`, 文字plaintext； 輸出1或數個整數
* 解密: 輸入 `n`, `d`, 1或數個整數ciphertext； 輸出1串文字
* 指令如子資料夾中的README中所示
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
---

## 程式碼解說
* 按照簡報虛擬碼實作 `Square_and_Multiply`
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

* 輾轉相除法，找到最大公因數
    ```python
    def gcd(a,b): 
        if(b==0): 
            return a 
        else: 
            return gcd(b, a%b)
    ```


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
* 按照簡報虛擬碼實作的 `Miller Rabin test`
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
* 透過random.randrange且只隨機生成奇數，獲得 `p` 和 `q` ，以縮短時間
    ```python
    # Initial key
    # init {bit}
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
    ```
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
* 將一或數個整數依次丟進解密得到加密時的大整數 `temp`
* 接著將大整數按照2或3位數，由ascii轉譯成文字即得到plaintext
* 由於可視字元的ascii介於21~126，所以判斷首個字是否為1即可知道要解譯2或3個位數
* 其中的 `if (int(decrypt[0 : 3]) > 126)` 判斷相會在最後進行解說
    ```python
    # Decrypt
    # -e {n} {d} {ciphertext...}
    elif (argv[1] == '-d'):
        n = int(argv[2])
        d = int(argv[3])
        decrypt = ''
        decryptText = ''
        print('Decryption plaintext:')
        for i in range(4, len(argv)):
            ciphertext = int(argv[i])
            decrypt += str(Square_and_Multiply(ciphertext, d, n))
        # print('decrypt', decrypt)
        
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
        print(decryptText, '')
    ```
---

## 遇到困難與⼼得
起初 `Square_and_Multiply` 與 `Miller Rabin test` 過程都還算順利，但在做 `keyGeneration` 時，`d`的尋找一開始使用正向的暴力破解，計算 $k = \frac{ 1 - e \times d}{\phi(n)}$ 是否為整數，然而在1024bits時非常耗時，所以才修正成上述的寫法。

解碼的`if (int(decrypt[0 : 3]) > 126)` 判斷則是在測試時意外發現的，解密中明文所串接的大數字，在切割進入解密的過程後，可能有遇到0為開頭的輸出，造成原先ascii為10x的字輸出大於126，所以需要在解密遇到不可能為明文的ascii碼補0，才能正常解密

---

<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>
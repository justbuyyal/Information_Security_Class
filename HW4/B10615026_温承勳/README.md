# HW4_RSA_ReadMe
---

## 建置環境

* **Windows 10**
* **Visual Code**
* **Python 3**
---

## 操作方式

**1. init:**
```python=
python .\RSA.py init 1024
```
* **秀出p, q, n, e, d，皆為數字**
* **p,q 為1024bits大質數**
* **只支援1024bits_RSA**

**2. Encrypt:**
```python=
python .\RSA.py -e {plaintext} {n} {e}
```
* **{plaintext} 在輸入上盡量以英文或數字做輸入，)、(、$、%可能會造成windows命令提示視窗輸入問題**
* **{n}、{e}為數字，非英文**

**3. Decrypt:**
```python=
python .\RSA.py -d {ciphertext} {n} {e}
```
* **{ciphertext}為加密產生的大數字，其餘輸入格式與加密相同**
* **由於數字過大可能會造成windows命令提示視窗跳出系統錯誤訊息，但不影響解密結果**
---

## 程式碼執行結果

**1. init**
![](https://i.imgur.com/IVXICGj.png)
**2. Encrypt**
![](https://i.imgur.com/tlWqWeA.png)
**3. Decrypt**
![](https://i.imgur.com/uVTypvm.png)

---

## 程式碼解說

```python=
def LargePrime(bits):
    LP = random.randrange(2**(bits -1), 2**(bits))
    while(isPrime(LP) == False):
        LP = random.randrange(2**(bits -1), 2**(bits))
    return(LP)
def isPrime(LN):
    for i in prime_list:
        if(LN == i):
            return True
        elif(LN%i == 0):
            return False
    # Do miller rabin test
    return (Miller_rabin(LN))
```
* **1. 產生$2^{bits} - 1$~$2^{bits}$隨機數**
* **2. 先從Prime_list中所有質數做判斷，確認目前的數是否為質數表的數或倍數**
* **3. 都不是就進入Miller Rabin Test**

```python=
def Miller_rabin(TN):
    k = 0
    m = TN - 1
    while (m and 1) == 0:
        k+=1
        m>>1
    for t in range(3):
        apha = random.randrange(2, TN - 2)
        b = Square_and_Multiply(apha, m, TN)
        if b != 1 and b != TN-1:
            i = 1
            while i<k and b != TN-1:
                b = Square_and_Multiply(b,2, TN)
                if b == 1: return False
                i+=1
            if b != TN-1: return False
    return True
```
* **1. 作法與老師上課簡報內容相同**
* **2. 測試次數為3次**
* **3. 其中會做到次方項或取餘數就呼叫Square_and_Multiply**

```python=
def Square_and_Multiply(base, exp, N):
    result = base
    bits = bin(int(exp))
    for i in range(3, len(bits)):
        result = pow(result, 2, N)
        if(bits[i:i+1] == '1'):
            result = (result * base) % N
    return result
```
* **作法同老師上課簡報內容**
* **exp初始為float型態，先轉成int去掉小數點的0再轉成bits型態**
* **從3(index)開始原因: bytesarray最前面為0b1，前面2個不需要，第3個是做assign，所以直接從下一個開始運算**

```python=
def multiplicative_inverse(phi, e):
    x, x1 = 0, 1
    y, y1 = 1, 0
    temp_phi = phi
    while (e != 0):
        q = phi // e
        phi, e = e, phi % e
        x, x1 = x1 - q * x, x
        y, y1 = y1 - q * y, y
    if y1 < 0: return temp_phi+y1
    else: return y1
```
* **運用Extended GCD找inverse**
* **若inverse為負數則要轉正，非取絕對值**
---

## 遇到困難與心得
* **困難**
**1. 一開始在測試判斷大質數上一直有問題，最後發現問題在Miller_Rabin_Test上，求m的計算中，把m除2的動作假如用m/=2就無法，必須改寫成m>>1**
**2. 用Prime_list去確認大質數的判斷中，要先判斷是否為Prime_list中的數才可以再判斷是否為Prime_list中的倍數，不然會造成錯誤(自己為自己的1倍)**
* **心得**
**基本上照著簡報的上就可以完成大部分的功能，剩下輸入格式的設計與輸出方式就靠自己，由於要做到1024bits的運算，真的必須要有Square_and_Multiply才可以做有效加速，不然加解密都會花費很多時間。發現在找大質數時並沒有想像中久，可能數字越大，隨機的數都很有可能是質數。**
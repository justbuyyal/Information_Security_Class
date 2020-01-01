# Information_Security_Class

### If You Are Using Our File, Please Follow The Command Below

### HW1：
#### ./Encrypt.exe {suffix} {Key} {Plaintext}
* {suffix}：About this, you need to see Encrypt.cpp

#### ./Decrypt.exe {suffix} {Key} {Ciphertext}
* {suffix}：Watch Decrypt.cpp too

#### **Remember：Key needs to be capital & Plaintext needs to be lower case & Ciphertext needs to be captial！！**

### HW2：
#### ./EncryptDES.exe {Key} {Plaintext}

#### ./DES_Decrypt.exe {Key} {Ciphertext}

#### **Remember：Key needs to be capital & Plaintext needs to be lower case & Ciphertext needs to be captial！！**

### HW3
#### python ./AES.py {mode} {Key} {image path}
##### mode have `ECB`, `CBC`, `extend`
##### key length must be 16, 24 or 32
#### Encryption result will be store at `./encrypt.jpg`
#### Decryption result will be store at `./decrypt.jpg`

### HW4
This HW are personal homework. The readme files are in each folder.

### HW5
#### python ./AES.py -sign {message}
##### Sign mode will return `p`, `q`, `alpha`, `beta`, `r`, `s`
#### python ./AES.py -verify {Key_list} {message}
##### Key_list need in the order below: `p`, `q`, `alpha`, `beta`, `r`, `s`
##### Verfity mode will return `True`/`False`
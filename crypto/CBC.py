#Cipher-Block-Chaining

"""
In this project you will implement two encryption/decryption systems,
one using AES in CBC mode and another using AES in counter mode (CTR).
In both cases the 16-byte encryption IV is chosen at random and is
prepended to the ciphertext. For CBC encryption we use the PKCS5
padding scheme discussed in class (13:50).

While we ask that you implement both encryption and decryption, we
will only test the decryption function. In the following questions
you are given an AES key and a ciphertext (both are hex encoded)
and your goal is to recover the plaintext and enter it in the input
boxes provided below.

For an implementation of AES you may use an existing crypto library
such as PyCrypto (Python), Crypto++ (C++), or any other. While it
is fine to use the built-in AES functions, we ask that as a
learning experience you implement CBC and CTR modes yourself. 

"""


from Crypto.Cipher import AES
import binascii
import base64


###########aes basics for reference##########
key = 'sixteen byte key'
cipher = AES.new(key)
plaintext = 'sixteen byte msg'
ciphertext = cipher.encrypt(plaintext) #encrypt plaintext
plaintext = cipher.decrypt(ciphertext) #and decrypt it back
##############################################

key1 = '140b41b22a29beb4061bda66b6747e14'
ciphertext1 = '4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81'


key2 = '140b41b22a29beb4061bda66b6747e14'
ciphertext2 = '5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253'



def HexToByte( hexStr ):
    hexStr = ''.join( hexStr.split(" ") )
    return ''.join( ["%c" % chr( int ( hexStr[i:i+2],16 ) ) for i in range(0, len( hexStr ), 2) ] )

def ByteToHex( byteStr ):
    return ''.join( [ "%02X " % ord( x ) for x in byteStr ] ).strip()

def xor_strings(xs, ys):
    return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))


def CBCdecrypt(key, ciphertext):
    m = ''
    
    n = 32#split into 16byte blocks (32 hex digits)
    blocks = [ciphertext[i:i+n] for i in range(0, len(ciphertext), n)]

    #this just decrypts the first block...and is template for for loop
    """
    cipher = AES.new(key.decode('hex'), AES.MODE_ECB)

    x = cipher.decrypt(blocks[1].decode('hex')).encode('hex')
    print x
    
    def xor_strings(xs, ys):
        return "".join(chr(ord(x) ^ ord(y)) for x, y in zip(xs, ys))

 
    xored = hex(int(x,16)^int(blocks[0],16))[2:-1].decode('hex')
    print xored
"""
    for i in range(1, len(blocks)):
        cipher = AES.new(key.decode('hex'), AES.MODE_ECB)
        x = cipher.decrypt(blocks[i].decode('hex')).encode('hex')
        xored = hex(int(x,16)^int(blocks[i-1],16))[2:-1].decode('hex')
        m+=xored

    print m



CBCdecrypt(key2,ciphertext2)


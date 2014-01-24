#dagan harrington '13
#classmember PaddingOracle.ct is a cipher text
#queries to a website will notify if ciphertext is incorrectly padded
#using this information (yes your pad is correct or no it isn't) will allow
#successive guesses on the n-1,n-2,n-3,...chars.

#incomplete solution that doesn't correctly decipher last block m[2]

import urllib2
import sys

TARGET = 'http://crypto-class.appspot.com/po?er='
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
    ct = "f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4"
    ascii = ''
    blk = ''
    
    block = [] #defunct . do not use .. prepend string instead
    
    def query(self, q):
        target = TARGET + urllib2.quote(q)    # Create query URL
        req = urllib2.Request(target)         # Send HTTP request to server
        try:
            f = urllib2.urlopen(req)          # Wait for response
        except urllib2.HTTPError, e:          
            #print "We got: %d" % e.code ,      # Print response code
            if e.code == 404:
                
                #print '404 !!!!!!!!!!!!!!!!!!!!!!!!!!!!'
                
                return True # good padding
            return False # bad padding
        
    def attack(self):
        
        iv = self.ct[0:32]
        c0 = self.ct[32:64]
        c1 = self.ct[64:96]
        c2 = self.ct[96:128]

        p = 0 # pad incremental // bounded above @ 16 per block
        
        for pos in reversed(xrange(0, len(c1),2)):
            p+=1
            pad = format(p, '02x')*p #p occurences of p
            
            for b in range(256):
                guess = format(b, '02x')

                #guess xor ct[pos:pos+2] xor 0xi * i
                #where : pos:pos+2 is a hex represented byte
                #where : 0xi * i is (i occurrences of i padded right)
                xor = format(int(guess+self.blk,16)^int(pad,16)\
                             ^int(iv[pos:pos+2*p],16), '02x')

                

                modiv = iv[:pos]+xor+iv[pos+2*p:]
                modct = modiv+c0
                
                
                if self.query(modct):
                    self.blk =guess+self.blk #plaintext hex
                    self.ascii =guess.decode('hex')+self.ascii

                    print self.ascii    
        
if __name__ == "__main__":
    po = PaddingOracle()
    po.attack()


"""
m[0] = :
red@house:~/python$ python crypto4.py
 
s 
ds 
rds 
ords 
Words 
 Words 
c Words 
ic Words 
gic Words 
agic Words 
Magic Words 
 Magic Words 
e Magic Words 
he Magic Words 
The Magic Words 


m[1] = :
red@house:~/python$ python crypto4.py
s
Os
 Os
h Os
sh Os
ish Os
mish Os
amish Os
eamish Os
ueamish Os
queamish Os
Squeamish Os
 Squeamish Os
e Squeamish Os
re Squeamish Os
are Squeamish Os

"""


    

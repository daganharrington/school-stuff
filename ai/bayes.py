#dagan harrington

#ai -- assignment #5
#dr. lyons

import sys
import itertools
from collections import OrderedDict

                    #class is a bayesian network grouped with methods
                    #for parsing a data file populating the network
                    #and calculating/printing joint distribution from that network
class BAYESCLASS():
    


    bg = {}         #a directed graph where key = node
                    #and value = (a,b)
                    #where a = [nodes that 'directly influence' key]
                    #and   b = [probabilities in truthtable order]

    bayesgraph = [] #this will be the OrderedDict which is a conversion of bg

    netsize    = 0  #number of probabilities in the bayesian network
                    #will be the numerator in calculating compactness.
    numofmults = 0
    numofadds  = 0
    
    def isfloat(self,x):
                        
        try:
            float(x)
            return True
        except ValueError:
            return False
    

    def createBayesGraph(self,file_name):#open file and populate bayesgraph

        fobj = open(file_name)
        print '\n\n*********Full Joint Distribution from', file_name, 'file**************\n'
        try:
            for line in fobj:#a simple while loop might suffice
                                
                if (line in 'END\n'):
                    print
                else:
                    line = line.split()
                    self.bg[line[0]]=([x for x in line[1:] if not self.isfloat(x) and x != 'NONE'],
                                              [float(x) for x in line[1:] if self.isfloat(x)])
            try:
                self.bg.pop('END')
            except:
                KeyError
        finally:
            self.bayesgraph = OrderedDict(sorted(self.bg.items()))
            fobj.close()

    def printJointDistribution(self):
        #calculate and print joint distribution
        

        nodes = self.bayesgraph.keys()
        
        ttable = list(itertools.product([True,False], repeat=len(nodes)))
        #truth tables value 'combinations' are cartesian products on a set :
        #S^n | n = num of boolean vars, S = {0,1}
        
    
        
        for node in nodes:
            print node.center(10),

        print

        truthdict = {}
               
        for possibleworld in ttable:
            entry = 1.0
            for i, value in enumerate(possibleworld):
                print str(value).center(10),#prettyprint truth table line by line
                truthdict[nodes[i]]=value#create a dict of (node):(truth value in current possible world)
            for key, value in truthdict.iteritems():

                (parents, probs) = self.bayesgraph[key]

                #create a truth table skeleton for the given conditional probability
                minitable = list(itertools.product([True,False], repeat=len(parents)))
                
                ls = []##
                for parent in parents:
                    ls.append(truthdict[parent])
                              
                if value:
                    num = probs[minitable.index(tuple(ls))]
                    
                    entry = entry*num
                    self.numofmults +=1
                else:
                    num = 1-probs[minitable.index(tuple(ls))]
                    
                    entry = entry*num
                    self.numofmults +=1
                    self.numofadds  +=1

            print ' = ', '%.9f' % entry

        self.printStats()
        

            

    def printStats(self):
        print
        print 'Number of nodes in network : ', len(self.bayesgraph)
        print 'Number of multiplications to calculate full joint distribution : ', self.numofmults
        print 'Number of additions to calculate full joint distribution : ', self.numofadds
        
        for (parents,probs) in self.bayesgraph.values():
            
            self.netsize += len(probs)
                
        print 'Bayesian network is (', self.netsize, '/', 2**len(self.bayesgraph), '=',float(self.netsize)/float(2**len(self.bayesgraph)),') the size of full joint distribution'
        
        

        
        
            
            
                      
    def __init__(self):
        
        self.bayesgraph = []
        self.numofmults = 0
        self.numofadds  = 0
        self.bg = {}
        
def main():
    
    
    ex = BAYESCLASS()
    ex.createBayesGraph("examplebayes.txt")
    ex.printJointDistribution()
    
    
    a = BAYESCLASS()
    a.createBayesGraph("bayesnets1.txt")
    a.printJointDistribution()

    b = BAYESCLASS()
    b.createBayesGraph("bayesnets2.txt")
    b.printJointDistribution()

    c = BAYESCLASS()
    c.createBayesGraph("bayesnets3.txt")
    c.printJointDistribution()
    
    d = BAYESCLASS()
    d.createBayesGraph("bayesnets4.txt")
    d.printJointDistribution()

if __name__ == '__main__':
    main()



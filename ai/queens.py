#Dagan Harrington
#Dr. Lyons
#CISC 6??? - AI

import sys, heapq, os
from collections import Counter
from random import randrange
from datetime import datetime, timedelta


class queens:

    """A local beam-search to find solutions to the 8-queens problem.
        beamResultsTest sets various beam widths and prints corresponding results.    
"""

    beam = []
    randomproblemset = []

    def randomBoard(self):
        """generate a random board  -- with only one queen in each column"""
        
        randboard=[]
        #this could be represented as a mere [Int],
        #but I like internal tuples for clarity
        for i in range(1,9):
            randboard.append((i,randrange(1,9)))
        return randboard
    
    def generateChildren(self,board):
        """receives a board and populates a data beam with
            all possible children.
            'possible children' is constrained by only allowing ONE vertical movement"""

        plateauflag = True  #flag is set to True. if any child node has a lesser attackingPairs value than the current board being permuted,
                            #then a False flag will be inserted into an array of booleans.  if ALL of the values in said array are True,
                            #then NONE of children have lower values than the parent.
        
        for i, queen in enumerate(board):
            (row,col) = queen
            for x in [num for num in range(1,9) if num != col]:#comprehension filters for duplicates
                a = list(board)
                a[i] = (i+1,x)
                pairs = self.attackingPairs(a)
                parentpairs = self.attackingPairs(board)
                if pairs < parentpairs:
                    plateauflag = False
                heapq.heappush(self.beam, (pairs, a))    
        return plateauflag
            
    
    def attackingPairs(self, board):#type [(Int,Int)] -> Int
        """given a list of queen positions of the form (row,col)
            return the number of attacking pairs
            NOTE: pairs can attack through one another"""

        pairs = 0
        
        ##########horizontal check#################
        tally2 = [0,0,0,0,0,0,0,0,0] 
        for queen in board:
            (row,col) = queen
            tally2[col] += 1
    
        for i in tally2: ##could be its own fcn
                if i>0:
                    #python list excludes upper SO : this is sigma i=0 to n-1 of i
                    pairs = pairs + reduce(lambda x,y: x+y, range(i))
                    
        ###########diagonal check(s)##################
        #figure out how to combine these with abs()..
        #must be possible
                    
        for queen in board:         #decreasing diagonal
            d1 = [row+col for (row,col) in board]
        a = Counter(d1)
        for key, occurrence in a.items():
            if occurrence>0:
                pairs = pairs + reduce(lambda x,y: x+y, range(occurrence))
        
        for queen in board:         #increasing diagonal
            d2 = [row-col for (row,col) in board]
        b = Counter(d2)
        for key, occurrence in b.items():
            if occurrence>0:
                pairs = pairs + reduce(lambda x,y: x+y, range(occurrence))
    
        return pairs


    def beamSearch(self, board, k):
        """Upon initialization, the board's attackingPairs # is evaluated and the tuple (pairs, board) is
            placed in the beam.  Algorithm searches for a solution from the initial board by generating children and
            preferencing (keeping in the beam) those with the lower values.  It cuts the beam back down to the appropriate k-size after
            all children are placed and ordered in/by the heap...
            
            """
        
        self.beam = []

        pairs = self.attackingPairs(board)
        heapq.heappush(self.beam, (pairs, board))

        
        goalflag = False
        tries = 0
        limit = 10
        #limit will be set to 10. while every state in the beam may generate ONLY worse states, we don't want to halt right there.
        #it is possible that these worse states are, themselves, very close to a goal
        #we will try 10 times, if the beam states continue to generate children that are no better than their parents,
        #it is likely that we are stuck on a plateau, the search will exit.

        while (tries < limit) and not goalflag:
        
            plateauflags = []

            for i in range(len(self.beam)): #range fixes # of iterations..so i don't have to worry about self.beam growing and being a loop condition
                (pairs,board) = self.beam[i]
                
                if pairs == 0:              #small-ish efficiency issue : would rather check within the generateChildren function
                    print 'Solution : ', board
                    goalflag=True           #this flag method is weak and a bit awkward, methinks. alternatives?
                    return 0
                    break
                else:
                    plateauflags.append(self.generateChildren(board))#generate children and determine whether all of the children were terrible.
            
            try:
                #if everything in plateauflags is true (AKA: ALL parents in the beam generated ONLY 'worse' states)
                #folding logical AND over the list checks and returns true
                if reduce(lambda x, y: x and y, plateauflags):
                    tries = tries + 1  
            except TypeError:
                print ' ' #can't fold an empty sequence

            self.beam = self.beam[:k] #slice at k and discard rest

        if tries == limit:
            print 'No Solution Found'
            return 1 #FAIL
            
            
            

    def beamResultsTest(self, setsize):

        k1stats  = []
        k10stats = []
        k50stats = []
        

        print 'beam search on', setsize, 'random boards\n'
        
        for i in range(setsize):
            self.randomproblemset.append(self.randomBoard())

        print 'Trying beam search with k=1 on random problem set\n'
        begin = datetime.now()
        for i in self.randomproblemset:
            k1stats.append(self.beamSearch(i, 1)) #solve each using k=1 and attach success or failure to stats
        end = datetime.now()
        print '=============================='
        print k1stats.count(0), ' successes'
        print k1stats.count(1), ' failures'
        print end - begin, ' Total Calculation Time'
        print '==============================\n'

        print 'Trying beam search with k=10 on random problem set\n'
        begin = datetime.now()
        for i in self.randomproblemset:
            k10stats.append(self.beamSearch(i, 10)) #solve each using k=1 and attach success or failure to stats
        end = datetime.now()
        print '=============================='
        print k10stats.count(0), ' successes'
        print k10stats.count(1), ' failures'
        print end - begin, ' Total Calculation Time'
        print '==============================\n'

        print 'Trying beam search with k=50 on random problem set\n'
        begin = datetime.now()
        for i in self.randomproblemset:
            k50stats.append(self.beamSearch(i, 50)) #solve each using k=1 and attach success or failure to stats
        end = datetime.now()
        print '=============================='
        print k50stats.count(0), ' successes'
        print k50stats.count(1), ' failures'
        print end - begin, ' Total Calculation Time'
        print '==============================\n'
        
        
    def __int__(self):
        print 'hello'
        self.beam = []

def main():

    #single search
    #y = queens()
    #y.beamSearch(y.randomBoard(), 55)

    #test on 'large' problem set
    x = queens()
    x.beamResultsTest(10)
                                    
    

    
if __name__ == '__main__':
    main()

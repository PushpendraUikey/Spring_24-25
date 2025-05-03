import numpy as np 
from heapq import heappush, heappop


class Node:
    def __init__(self, state, queens, parent=None):
        self.state = state
        self.parent = parent
        self.g = 0
        self.h = 0
        self.f = 0
        self.queens = queens

    def __lt__(self, other):
        return self.f < other.f
    def __eq__(self, other):
        return np.array_equal(self.state, other.state)

# state is the configuration in which the current board is such that if more than one queen is
# in the board then they are not in same row or column or diagonal for any pair of queens

# placing a queen one at time for next state
def neighbourStates(state, queens: int):
    neighbours = []
    
    xpos, ypos = np.where(state == 0)   # will place queen at empty positions
    # Operator
    for x, y in zip(xpos, ypos):
        if queens < 8:
            newstate = state.copy()
            newstate[x][y] = 1
            if(isValid(newstate)):      # only valid states
                neighbours.append(newstate)
    return neighbours

def isValid(state)->bool:

    rows, cols = np.where(state == 1)

    # row check
    for i in range(rows.shape[0]):
        for j in range(rows.shape[0]):
            if(i==j):
                continue
            if rows[i]==rows[j]:
                return False
    # column check
    for i in range(cols.shape[0]):
        for j in range(cols.shape[0]):
            if(i==j):
                continue
            if cols[i]==cols[j]:
                return False

    # diagonal check
    for x, y in zip(rows, cols):
        for i in range(1,8):
            nx , ny = x+i, y+i
            if( nx < 8 and ny < 8 and state[nx][ny] == 1):
                return False
            nx , ny = x+i, y-i 
            if (nx < 8 and ny >=0 and state[nx][ny] == 1):
                return False 
            nx, ny = x-i, y+i 
            if( nx >=0 and ny < 8 and state[nx][ny] == 1):
                return False
            nx, ny = x-i, y-i 
            if( nx>=0 and ny >= 0 and state[nx][ny] == 1):
                return False
    
    #valid state
    return True

def zero_heuristic(state)->int:
    return 0

# number of queens remaining to fill
def heuristic_1(state)->int:
    cost = 0
    rows, cols = np.where(state == 1)
    cost += 8-rows.shape[0]  # queen remaining to be filled
    return cost

def heuristic_2(state)->int:
    rows, cols = np.where(state==1)
    leftcol=0
    rightcol=0
    uprow = 0
    lowrow = 0
    for c in cols:
        if( c < 4 ):
            leftcol += 1
        else:
            rightcol += 1
    for r in rows:
        if ( r < 4):
            uprow += 1
        else:
            lowrow +=1
    cost = abs(leftcol-rightcol) + abs(uprow-lowrow) + (8-rows.shape[0])
    return cost

def AStar(state, heuristic:callable):
    openlist = []
    closedlist = []

    initial = Node(state, 0)
    heappush(openlist, initial)     # priority queue
    nodesexplored = 0

    while openlist:
        current = heappop(openlist)
        closedlist.append(current)

        nodesexplored = nodesexplored+1

        if current.queens == 8:  # all queens are placed in the board then the required goal state achieved
            return current.state, nodesexplored

        for nbr in neighbourStates(current.state, current.queens):
            
            nbr_node = Node(nbr, current.queens+1, current) # at each next node placing one more queen
            if nbr_node in closedlist:
                continue
            
            nbr_node.g = current.g + 1          # g() function
            nbr_node.h = heuristic(nbr_node.state)  # h() function
            nbr_node.f = nbr_node.g + nbr_node.h    # f() function

            # if any the same state(node) exists in OL with lower f value
            if any( open_node for open_node in openlist if open_node == nbr_node and open_node.f <= nbr_node.f):
                continue
            
            heappush(openlist, nbr_node)

    return None


# A* with first heuristic
print("first heuristic: ")
initial = np.zeros((8,8))
final, nodesexplored = AStar(initial, heuristic_1)
print(final)
print(f"performance: {nodesexplored}\n")

print("second heuristic: ")
final, nodesexplored = AStar(initial, heuristic_2)
print(final)
print(f"performance: {nodesexplored}\n")

# performance for heuristic zero
final, nodesexplored = AStar(initial, zero_heuristic)
print(final)
print(f"performance: {nodesexplored}")
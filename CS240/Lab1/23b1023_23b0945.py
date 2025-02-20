import numpy as np
from collections import deque
import heapq
from typing import List, Tuple, Set, Dict

"""
Do not import any other package unless allowed by te TAs in charge of the lab.
Do not change the name of any of the functions below.
"""

# def zeroposition(array):
#     n = array.shape[0]
#     for i in range(n):
#         for j in range(n):
#             if(array[i][j] == 0):
#                 return i, j

#     return -1, -1

# def ifequal(first: np.ndarray, second: np.ndarray):
#     n = first.shape[0]
#     for i in range(n):
#         for j in range(n):
#             if(first[i][j] != second[i][j]):
#                 return False
#     return True



class Node:
    def __init__(self, curr, parent=None):
        self.current = curr
        self.parent = parent
        self.g = 0
        self.h = 0
        self.f = 0
        self.m = None

    def __eq__(self, other):
        return np.array_equal(self.current, other.current)

    def __lt__(self, other):
        return self.f < other.f

def findposition(val, arr: np.ndarray) -> Tuple[int, int]:
    n = arr.shape[0]
    for i in range(n):
        for j in range(n):
            if arr[i][j] == val:
                return i, j
    return -1, -1

def zeroposition(state: np.ndarray) -> Tuple[int, int]:
    return findposition(0, state)

def heuristic_md(first: np.ndarray, goal: np.ndarray) -> int:
    dist = 0
    n = first.shape[0]
    for i in range(n):
        for j in range(n):
            x, y = findposition(goal[i][j], first)
            dist += abs(i - x) + abs(j - y)
    return dist

def heuristic_dt(first: np.ndarray, goal: np.ndarray):
    displaced = 0
    n = first.shape[0]
    for i in range(n):
        for j in range(n):
            if(goal[i][j] != first[i][j]):
                displaced += 1
    return displaced

def ifequal(state: np.ndarray, goal: np.ndarray) -> bool:
    return np.array_equal(state, goal)



def bfs(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int]:

    openlist = []
    closedlist = []
    first = Node(initial)
    openlist.append(first)
    nodesexplored = 0

    while openlist:
        current = openlist.pop(0)
        closedlist.append(current)
        nodesexplored = nodesexplored + 1

        if(ifequal(current.current, goal)):
            path = []
            while current.parent:
                path.insert(0,current.m)
                current = current.parent
            return path, nodesexplored

        # Generate neighbors (up, down, left, right)
        neighbors = [
            (0, -1),  # Left
            (0, 1),   # Right
            (-1, 0),  # Up
            (1, 0)    # Down
        ]

        for indx, move in enumerate(neighbors):
            temp = current.current.copy()
            x, y = zeroposition(temp)
            nx = x + move[0]
            ny = y + move[1]

            if (
                nx < 0 or nx >= len(initial) or
                ny < 0 or ny >= len(initial[0])
            ):
                continue

            # Create a neighbor node
            temp[x][y] = temp[nx][ny]
            temp[nx][ny] = 0
            neighbor_node = Node(temp, current)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # Calculate f [ equivalent g]
            neighbor_node.f = current.f + 1
            if(indx == 0):
                neighbor_node.m = 'L'
            elif(indx==1):
                neighbor_node.m = 'R'
            elif(indx==2):
                neighbor_node.m = 'U'
            else:
                neighbor_node.m = 'D'

            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.f >= open_node.f):
                continue

            # Add the neighbor to the open list
            heapq.heappush(openlist, neighbor_node)

    # If no path is found
    return None, nodesexplored
    """
    Implement Breadth-First Search algorithm to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array.
                            Example: np.array([[1, 2, 3], [4, 0, 5], [6, 7, 8]])
                            where 0 represents the blank space
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array.
                          Example: np.array([[1, 2, 3], [4, 5, 6], [7, 8, 0]])
    
    Returns:
        Tuple[List[str], int]: A tuple containing:
            - List of moves to reach the goal state. Each move is represented as
              'U' (up), 'D' (down), 'L' (left), or 'R' (right), indicating how
              the blank space should move
            - Number of nodes expanded during the search

    Example return value:
        (['R', 'D', 'R'], 12) # Means blank moved right, down, right; 12 nodes were expanded
              
    """
    # TODO: Implement this function
    pass

def dfs(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int]:
    openlist = []
    closedlist = []
    first = Node(initial)
    heapq.heappush(openlist, first)
    nodesexplored = 0

    while openlist:
        current = heapq.heappop(openlist)
        closedlist.append(current)
        nodesexplored = nodesexplored + 1

        if(ifequal(current.current, goal)):
            path = []
            while current.parent:
                path.insert(0,current.m)
                current = current.parent
            return path, nodesexplored

        # Generate neighbors (up, down, left, right)
        neighbors = [
            (0, -1),  # Left
            (0, 1),   # Right
            (-1, 0),  # Up
            (1, 0)    # Down
        ]

        for indx, move in enumerate(neighbors):
            temp = current.current.copy()
            x, y = zeroposition(temp)
            nx = x + move[0]
            ny = y + move[1]

            if (
                nx < 0 or nx >= len(initial) or
                ny < 0 or ny >= len(initial[0])
            ):
                continue

            # Create a neighbor node
            temp[x][y] = temp[nx][ny]
            temp[nx][ny] = 0
            neighbor_node = Node(temp, current)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # Calculate g and f [h = 0]
            neighbor_node.g = (current.g + 1) 
            neighbor_node.f = (1/neighbor_node.g)*1e8   # multiplying with constant to each f, won't affect relative priority
            if(indx == 0):
                neighbor_node.m = 'L'
            elif(indx==1):
                neighbor_node.m = 'R'
            elif(indx==2):
                neighbor_node.m = 'U'
            else:
                neighbor_node.m = 'D'

            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.f >= open_node.f):
                continue

            # Add the neighbor to the open list
            heapq.heappush(openlist, neighbor_node)

    # If no path is found
    return None, nodesexplored
    """
    Implement Depth-First Search algorithm to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
    """
    # TODO: Implement this function
    pass

def dijkstra(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int, int]:

    openlist = []
    closedlist = []
    first = Node(initial)
    heapq.heappush(openlist, first)
    nodesexplored = 0

    while openlist:
        current = heapq.heappop(openlist)
        closedlist.append(current)
        nodesexplored = nodesexplored + 1

        if(ifequal(current.current, goal)):
            cost = current.f
            path = []
            while current.parent:
                path.insert(0,current.m)
                # cost = cost + current.f
                current = current.parent
            return path, nodesexplored, cost

        # Generate neighbors (up, down, left, right)
        neighbors = [
            (0, -1),  # Left
            (0, 1),   # Right
            (-1, 0),  # Up
            (1, 0)    # Down
        ]

        for indx, move in enumerate(neighbors):
            temp = current.current.copy()
            x, y = zeroposition(temp)
            nx = x + move[0]
            ny = y + move[1]

            if (
                nx < 0 or nx >= len(initial) or
                ny < 0 or ny >= len(initial[0])
            ):
                continue

            # Create a neighbor node
            temp[x][y] = temp[nx][ny]
            temp[nx][ny] = 0
            neighbor_node = Node(temp, current)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # Calculate f[// in place of g]
            neighbor_node.f = current.f + 1
            if(indx == 0):
                neighbor_node.m = 'L'
            elif(indx==1):
                neighbor_node.m = 'R'
            elif(indx==2):
                neighbor_node.m = 'U'
            else:
                neighbor_node.m = 'D'

            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.f >= open_node.f):
                continue

            # Add the neighbor to the open list
            heapq.heappush(openlist, neighbor_node)

    # If no path is found
    return None, nodesexplored, 0
    """
    Implement Dijkstra's algorithm to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int, int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
            - Total cost of the path for transforming initial into goal configuration
            
    """
    # TODO: Implement this function
    pass



def astar_dt(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int, int]:
    openlist = []
    closedlist = []
    first = Node(initial)
    first.h = heuristic_md(initial, goal)
    first.f = first.h
    heapq.heappush(openlist, first)
    nodesexplored = 0

    while openlist:
        current = heapq.heappop(openlist)
        closedlist.append(current)
        nodesexplored = nodesexplored + 1

        if(ifequal(current.current, goal)):
            cost = current.g
            path = []
            while current.parent:
                path.insert(0,current.m)
                current = current.parent
            return path, nodesexplored, cost

        # Generate neighbors (up, down, left, right)
        neighbors = [
            (0, -1),  # Left
            (0, 1),   # Right
            (-1, 0),  # Up
            (1, 0)    # Down
        ]

        for indx, move in enumerate(neighbors):
            temp = current.current.copy()
            x, y = zeroposition(temp)
            nx = x + move[0]
            ny = y + move[1]

            if (
                nx < 0 or nx >= len(initial) or
                ny < 0 or ny >= len(initial[0])
            ):
                continue

            # Create a neighbor node
            temp[x][y] = temp[nx][ny]
            temp[nx][ny] = 0
            neighbor_node = Node(temp, current)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # Calculate g, h, and f
            neighbor_node.g = current.g + 1
            neighbor_node.h = heuristic_dt(temp, goal)
            neighbor_node.f = neighbor_node.g + neighbor_node.h
            if(indx == 0):
                neighbor_node.m = 'L'
            elif(indx==1):
                neighbor_node.m = 'R'
            elif(indx==2):
                neighbor_node.m = 'U'
            else:
                neighbor_node.m = 'D'

            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.f >= open_node.f):
                continue

            # Add the neighbor to the open list
            heapq.heappush(openlist, neighbor_node)

    # If no path is found
    return None, nodesexplored, 0

    """
    Implement A* Search with Displaced Tiles heuristic to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int, int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
            - Total cost of the path for transforming initial into goal configuration
              
    
    """
    # TODO: Implement this function
    pass


def astar_md(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int, int]:
    openlist = []
    closedlist = []
    first = Node(initial)
    first.h = heuristic_md(initial, goal)
    first.f = first.h
    heapq.heappush(openlist, first)
    nodesexplored = 0

    while openlist:
        current = heapq.heappop(openlist)
        closedlist.append(current)
        nodesexplored = nodesexplored + 1

        if(ifequal(current.current, goal)):
            cost = current.g
            path = []
            while current.parent:
                path.insert(0,current.m)
                current = current.parent
            return path, nodesexplored, cost

        # Generate neighbors (up, down, left, right)
        neighbors = [
            (0, -1),  # Left
            (0, 1),   # Right
            (-1, 0),  # Up
            (1, 0)    # Down
        ]

        for indx, move in enumerate(neighbors):
            temp = current.current.copy()
            x, y = zeroposition(temp)
            nx = x + move[0]
            ny = y + move[1]

            if (
                nx < 0 or nx >= len(initial) or
                ny < 0 or ny >= len(initial[0])
            ):
                continue

            # Create a neighbor node
            temp[x][y] = temp[nx][ny]
            temp[nx][ny] = 0
            neighbor_node = Node(temp, current)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # Calculate g, h, and f
            neighbor_node.g = current.g + 1
            neighbor_node.h = heuristic_md(temp, goal)
            neighbor_node.f = neighbor_node.g + neighbor_node.h
            if(indx == 0):
                neighbor_node.m = 'L'
            elif(indx==1):
                neighbor_node.m = 'R'
            elif(indx==2):
                neighbor_node.m = 'U'
            else:
                neighbor_node.m = 'D'

            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.f >= open_node.f):
                continue

            # Adding neighbor to the open list
            heapq.heappush(openlist, neighbor_node)

    # If no path is found
    return None, nodesexplored, 0

    """
    Implement A* Search with Manhattan Distance heuristic to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int, int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
            - Total cost of the path for transforming initial into goal configuration
    """
    # TODO: Implement this function
    pass

# Example test case to help verify your implementation
if __name__ == "__main__":
    # Example puzzle configuration
    initial_state = np.array([
        [1, 2, 3],
        [4, 0, 5],
        [6, 7, 8]
    ])
    
    goal_state = np.array([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 0]
    ])
    
    # Test each algorithm
    print("Testing BFS...")
    bfs_moves, bfs_expanded = bfs(initial_state, goal_state)
    print(f"BFS Solution: {bfs_moves}")
    print(f"Nodes expanded: {bfs_expanded}")


    print("\nTesting DFS...")
    dfs_moves, dfs_expanded = dfs(initial_state, goal_state)
    print(f"DFS Solution: {dfs_moves}")
    print(f"Nodes expanded: {dfs_expanded}")


    print("\nTesting Dijkstra...")
    dijkstra_moves, dijkstra_expanded, dijkstra_cost = dijkstra(initial_state, goal_state)
    print(f"Dijkstra Solution: {dijkstra_moves}")
    print(f"Nodes expanded: {dijkstra_expanded}")
    print(f"Total cost: {dijkstra_cost}")


    print("\nTesting A* with Displaced Tiles...")
    dt_moves, dt_expanded, dt_fscore = astar_dt(initial_state, goal_state)
    print(f"A* (DT) Solution: {dt_moves}")
    print(f"Nodes expanded: {dt_expanded}")
    print(f"Total cost: {dt_fscore}")


    print("\nTesting A* with Manhattan Distance...")
    md_moves, md_expanded, md_fscore = astar_md(initial_state, goal_state)
    print(f"A* (MD) Solution: {md_moves}")
    print(f"Nodes expanded: {md_expanded}")
    print(f"Total cost: {md_fscore}")

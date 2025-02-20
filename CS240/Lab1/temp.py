from typing import List, Tuple
import numpy as np
import time

class Node:
    def __init__(self, curr, parent=None, move=None):
        self.state = curr
        self.parent = parent
        self.move = move

    def __eq__(self, other):
        return np.array_equal(self.state, other.state)

def zeroposition(state: np.ndarray) -> Tuple[int, int]:
    """Find the position of the zero tile (blank)."""
    for i in range(len(state)):
        for j in range(len(state[i])):
            if state[i][j] == 0:
                return i, j
    return -1, -1  # Should never happen

def ifequal(state: np.ndarray, goal: np.ndarray) -> bool:
    """Check if the current state is equal to the goal state."""
    return np.array_equal(state, goal)  # array_equal of np is useful to check if two matrices equal.

def dfs(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int]:
    """Perform DFS to solve the 8-puzzle problem."""
    stack = []  # Stack for DFS
    visited = []  # To track visited states
    nodes_explored = 0

    # Push the initial node onto the stack
    root = Node(initial)
    stack.append(root)

    while stack:            # loop while stack list isn't empty.
        current = stack.pop()       # simple pop() operation on the list removes the top element.
        nodes_explored += 1

        # If the goal is reached, reconstruct the path
        if ifequal(current.state, goal):
            path = []
            while current.parent is not None:
                path.insert(0, current.move)
                current = current.parent
            return path, nodes_explored

        # Add the current state to visited
        visited.append(current)

        # Generate neighbors
        neighbors = [
            (0, -1, 'L'),  # Left
            (0, 1, 'R'),   # Right
            (-1, 0, 'U'),  # Up
            (1, 0, 'D')    # Down
        ]

        x, y = zeroposition(current.state)
        for dx, dy, move in neighbors:
            nx, ny = x + dx, y + dy
            if 0 <= nx < len(initial) and 0 <= ny < len(initial[0]):
                # Create a new state by swapping
                neighbor_state = current.state.copy()
                neighbor_state[x][y], neighbor_state[nx][ny] = neighbor_state[nx][ny], neighbor_state[x][y] #easy swap

                # Create the neighbor node
                neighbor_node = Node(neighbor_state, current, move)

                # Skip if already visited
                if any(visited_node for visited_node in visited if visited_node == neighbor_node):
                    continue

                # Add neighbor to stack
                stack.append(neighbor_node)

    # If no solution is found
    return None, nodes_explored

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
    start_time = time.time()
    print("\nTesting DFS...")
    dfs_moves, dfs_expanded = dfs(initial_state, goal_state)
    print(f"DFS Solution: {dfs_moves}")
    print(f"Nodes expanded: {dfs_expanded}")
    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Elapsed time: {elapsed_time:.5f} seconds")
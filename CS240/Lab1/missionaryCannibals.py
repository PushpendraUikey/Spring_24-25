from collections import deque

def is_valid_state(m, c):
    # Ensure missionaries are not outnumbered and counts are valid
    return (0 <= m <= 3 and 0 <= c <= 3) and (m == 0 or m >= c)

def get_next_states(state):
    m, c, boat = state
    moves = [(1, 0), (0, 1), (2, 0), (0, 2), (1, 1)]
    next_states = []

    for move_m, move_c in moves:
        if boat == 1:  # Boat is on the starting bank
            new_state = (m - move_m, c - move_c, 0)
            if is_valid_state(m - move_m, c - move_c) and is_valid_state(3 - (m - move_m), 3 - (c - move_c)):
                next_states.append(new_state)
        else:  # Boat is on the destination bank
            new_state = (m + move_m, c + move_c, 1)
            if is_valid_state(m + move_m, c + move_c) and is_valid_state(3 - (m + move_m), 3 - (c + move_c)):
                next_states.append(new_state)
                
    return next_states

def bfs_missionaries_cannibals():
    start = (3, 3, 1)  # Initial state: 3 missionaries, 3 cannibals, boat on starting bank
    goal = (0, 0, 0)   # Goal state: all on the other bank
    
    queue = deque([(start, [])])  # Queue holds (state, path_to_state)
    visited = set()  # Track visited states

    while queue:
        current_state, path = queue.popleft()
        
        if current_state in visited:
            continue
        
        visited.add(current_state)
        
        if current_state == goal:
            return path + [current_state]
        
        for next_state in get_next_states(current_state):
            if next_state not in visited:
                queue.append((next_state, path + [current_state]))
    
    return None  # No solution found

# Solve the problem
solution = bfs_missionaries_cannibals()
if solution:
    print("Solution:")
    for step in solution:
        print(step)
else:
    print("No solution found.")

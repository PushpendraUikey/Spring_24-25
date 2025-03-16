import heapq
import json
from typing import List, Tuple


class Node:
    def __init__(self, curr_state, parent=None):
        self.state = curr_state
        self.parent = parent
        self.h = 0

    def __eq__(self, other):
        return self.state == other.state

    def __lt__(self, other):
        return self.h < other.h

def check_valid(
    state: list, max_missionaries: int, max_cannibals: int
) -> bool:  # 10 marks
    """
    Graded
    Check if a state is valid. State format: [m_left, c_left, boat_position].
    """
    
    if max_cannibals != max_missionaries:
        return False 
    
    m_left, c_left, boat_position = state
    m_right = max_missionaries - m_left
    c_right = max_cannibals - c_left
    
    return ((0<=m_left<=max_missionaries) and (0<=c_left<=max_cannibals) and (m_left==0 or m_left>=c_left))and (
        (0<=m_right<=max_missionaries) and (0<=c_right<=max_cannibals) and (m_right==0 or m_right>=c_right)
    )
    raise ValueError("check_valid not implemented")


def get_neighbours(
    state: list, max_missionaries: int, max_cannibals: int
) -> List[list]:  # 10 marks
    """
    Graded
    Generate all valid neighbouring states.
    """
    neighbours = []
    m_left, c_left, boat_position = state
    m_right = max_missionaries - m_left
    c_right = max_cannibals - c_left

    next_positions = [(1,0), (0,1), (2,0),(0,2),(1,1)]

    for m, c in next_positions:
        new_state = []
        if boat_position == 0:
            new_state = [m_left+m, c_left+c, 1]
        else:
            new_state = [m_left-m, c_left-c, 0]
        
        if check_valid(new_state, max_missionaries, max_cannibals):
            neighbours.append(new_state)
    
    return neighbours

    raise ValueError("get_neighbours not implemented")


def gstar(state: list, new_state: list) -> int:  # 5 marks
    """
    Graded
    The weight of the edge between state and new_state, this is the number of people on the boat.
    """
    curr_m, curr_c, _ = state
    next_m, next_c, _ = new_state
    weight = abs(next_m-curr_m) + abs(next_c - curr_c)

    return weight
    raise ValueError("gstar not implemented")


def h1(state: list) -> int:  # 3 marks
    """
    Graded
    h1 is the number of people on the left bank.
    """
    m_left, c_left, _ = state
    return (m_left + c_left)
    raise ValueError("h1 not implemented")

def h2(state: list) -> int:  # 3 marks
    """
    Graded
    h2 is the number of missionaries on the left bank. 
    """
    m_left, __, _ = state 
    return m_left
    raise ValueError("h2 not implemented")


def h3(state: list) -> int:  # 3 marks
    """
    Graded
    h3 is the number of cannibals on the left bank.
    """
    __, c_left, _ = state 
    return c_left

    raise ValueError("h3 not implemented")


def h4(state: list) -> int:  # 3 marks
    """
    Graded
    Weights of missionaries is higher than cannibals.
    h4 = missionaries_left * 1.5 + cannibals_left
    """
    m_left, c_left, boat_position = state 
    weight = 1.5 * m_left + c_left

    return int(weight)
    raise ValueError("h4 not implemented")


def h5(state: list) -> int:  # 3 marks
    """
    Graded
    Weights of missionaries is lower than cannibals.
    h5 = missionaries_left + cannibals_left*1.5
    """
    m_left, c_left, boat_position = state

    weight = m_left + 1.5*c_left

    return int(weight)
    raise ValueError("h5 not implemented")


def astar_h1(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 28 marks
    """
    Graded
    Implement A* with h1 heuristic.
    This function must return path obtained and a boolean which says if the heuristic chosen satisfes Monotone restriction property while exploring or not.
    """
    openlist = []
    closedlist = []
    first_n = Node(init_state)
    first_n.h = h1(init_state)
    heapq.heappush(openlist, first_n)
    # openlist.append([init_state, []])
    monontonicity = True

    while openlist:
        currentNode = heapq.heappop(openlist)
        # currentState, path = openlist.pop(0)
        closedlist.append(currentNode)

        # print(currentState)
        ### Reached the destination at the end.
        if currentNode.state == final_state:
            path = []
            while currentNode.parent:
                path.insert(0,currentNode.state)
                currentNode = currentNode.parent
            return path, monontonicity

        # Generate neighbors (up, down, left, right)
        neighbors = get_neighbours(currentNode.state, max_missionaries, max_cannibals)

        for nbr in neighbors:

            neighbor_node = Node(nbr, currentNode)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # openlist.append([nbr, path+[currentState]])
            neighbor_node.h = h1(nbr)
            cost = gstar(currentNode.state, nbr)
            if currentNode.h > cost + neighbor_node.h:
                monontonicity = False
            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.h >= open_node.h):
                continue
            # Adding neighbor to the open list
            heapq.heappush(openlist, neighbor_node)
    # openlist = []
    # closedlist = []
    # openlist.append([init_state, []])
    # monotonicity = True

    # while openlist:
    #     # currentNode = heapq.heappop(openlist)
    #     currentState, path = openlist.pop(0)
    #     closedlist.append(currentState)
    #     hcurr = h1(currentState)
    #     ### Reached the destination at the end.
    #     if currentState == final_state:
    #         # while currentNode.parent:
    #         #     path.insert(0,currentNode.state)
    #         #     currentNode = currentNode.parent
    #         return path+[currentState], monotonicity

    #     # Generate neighbors (up, down, left, right)
    #     neighbors = get_neighbours(currentState, max_missionaries, max_cannibals)

    #     for nbr in neighbors:

    #         # neighbor_node = Node(nbr, currentNode)

    #         # If already evaluated, skip
    #         if nbr in closedlist:
    #             continue

    #         cost = gstar(currentState, nbr)
    #         hnbr = h1(nbr)
    #         if hcurr > cost + hnbr:
    #             monotonicity = False

    #         openlist.append([nbr, path+[currentState]])
            
    return None, False

    raise ValueError("astar_h1 not implemented")


def astar_h2(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h2 heuristic.
    """
    openlist = []
    closedlist = []
    first_n = Node(init_state)
    first_n.h = h2(init_state)
    heapq.heappush(openlist, first_n)
    # openlist.append([init_state, []])
    monontonicity = True

    while openlist:
        currentNode = heapq.heappop(openlist)
        # currentState, path = openlist.pop(0)
        closedlist.append(currentNode)

        # print(currentState)
        ### Reached the destination at the end.
        if currentNode.state == final_state:
            path = []
            while currentNode.parent:
                path.insert(0,currentNode.state)
                currentNode = currentNode.parent
            return path, monontonicity

        # Generate neighbors (up, down, left, right)
        neighbors = get_neighbours(currentNode.state, max_missionaries, max_cannibals)

        for nbr in neighbors:

            neighbor_node = Node(nbr, currentNode)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # openlist.append([nbr, path+[currentState]])
            neighbor_node.h = h2(nbr)
            cost = gstar(currentNode.state, nbr)
            if currentNode.h > cost + neighbor_node.h:
                monontonicity = False
            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.h >= open_node.h):
                continue
            # Adding neighbor to the open list
            heapq.heappush(openlist, neighbor_node)
    
    return  None, False
    raise ValueError("astar_h2 not implemented")


def astar_h3(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h3 heuristic.
    """
    openlist = []
    closedlist = []
    first_n = Node(init_state)
    first_n.h = h3(init_state)
    heapq.heappush(openlist, first_n)
    # openlist.append([init_state, []])
    monontonicity = True

    while openlist:
        currentNode = heapq.heappop(openlist)
        # currentState, path = openlist.pop(0)
        closedlist.append(currentNode)

        # print(currentState)
        ### Reached the destination at the end.
        if currentNode.state == final_state:
            path = []
            while currentNode.parent:
                path.insert(0,currentNode.state)
                currentNode = currentNode.parent
            return path, monontonicity

        # Generate neighbors (up, down, left, right)
        neighbors = get_neighbours(currentNode.state, max_missionaries, max_cannibals)

        for nbr in neighbors:

            neighbor_node = Node(nbr, currentNode)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # openlist.append([nbr, path+[currentState]])
            neighbor_node.h = h3(nbr)
            cost = gstar(currentNode.state, nbr)
            if currentNode.h > cost + neighbor_node.h:
                monontonicity = False
            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.h >= open_node.h):
                continue
            # Adding neighbor to the open list
            heapq.heappush(openlist, neighbor_node)
    

    # openlist = []
    # closedlist = []
    # # first_n = Node(init_state)
    # # first_n.h = h1(init_state)
    # # heapq.heappush(openlist, first_n)
    # openlist.append([init_state, []])
    # monotonicity = True

    # while openlist:
    #     # currentNode = heapq.heappop(openlist)
    #     currentState, path = openlist.pop(0)
    #     closedlist.append(currentState)
    #     hcurr = h3(currentState)
    #     ### Reached the destination at the end.
    #     if currentState == final_state:
    #         # while currentNode.parent:
    #         #     path.insert(0,currentNode.state)
    #         #     currentNode = currentNode.parent
    #         return path+[currentState], monotonicity

    #     # Generate neighbors (up, down, left, right)
    #     neighbors = get_neighbours(currentState, max_missionaries, max_cannibals)

    #     for nbr in neighbors:

    #         # neighbor_node = Node(nbr, currentNode)

    #         # If already evaluated, skip
    #         if nbr in closedlist:
    #             continue

    #         cost = gstar(currentState, nbr)
    #         hnbr = h3(nbr)
    #         if hcurr > cost + hnbr:
    #             monotonicity = False

    #         openlist.append([nbr, path+[currentState]])
   
    return None, False
    raise ValueError("astar_h3 not implemented")

def astar_h4(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h4 heuristic.
    """
    openlist = []
    closedlist = []
    first_n = Node(init_state)
    first_n.h = h4(init_state)
    heapq.heappush(openlist, first_n)
    # openlist.append([init_state, []])
    monontonicity = True

    while openlist:
        currentNode = heapq.heappop(openlist)
        # currentState, path = openlist.pop(0)
        closedlist.append(currentNode)

        # print(currentState)
        ### Reached the destination at the end.
        if currentNode.state == final_state:
            path = []
            while currentNode.parent:
                path.insert(0,currentNode.state)
                currentNode = currentNode.parent
            return path, monontonicity

        # Generate neighbors (up, down, left, right)
        neighbors = get_neighbours(currentNode.state, max_missionaries, max_cannibals)

        for nbr in neighbors:

            neighbor_node = Node(nbr, currentNode)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # openlist.append([nbr, path+[currentState]])
            neighbor_node.h = h4(nbr)
            cost = gstar(currentNode.state, nbr)
            if currentNode.h > cost + neighbor_node.h:
                monontonicity = False
            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.h >= open_node.h):
                continue
            # Adding neighbor to the open list
            heapq.heappush(openlist, neighbor_node)
    # openlist = []
    # closedlist = []
    # openlist.append([init_state, []])
    # monotonicity = True

    # while openlist:
    #     # currentNode = heapq.heappop(openlist)
    #     currentState, path = openlist.pop(0)
    #     closedlist.append(currentState)
    #     hcurr = h4(currentState)
    #     ### Reached the destination at the end.
    #     if currentState == final_state:
    #         # while currentNode.parent:
    #         #     path.insert(0,currentNode.state)
    #         #     currentNode = currentNode.parent
    #         return path+[currentState], monotonicity

    #     # Generate neighbors (up, down, left, right)
    #     neighbors = get_neighbours(currentState, max_missionaries, max_cannibals)

    #     for nbr in neighbors:

    #         # neighbor_node = Node(nbr, currentNode)

    #         # If already evaluated, skip
    #         if nbr in closedlist:
    #             continue

    #         cost = gstar(currentState, nbr)
    #         hnbr = h4(nbr)
    #         if hcurr > cost + hnbr:
    #             monotonicity = False

            # openlist.append([nbr, path+[currentState]])
    
    return None, False
    raise ValueError("astar_h4 not implemented")


def astar_h5(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h5 heuristic.
    """
    openlist = []
    closedlist = []
    first_n = Node(init_state)
    first_n.h = h5(init_state)
    heapq.heappush(openlist, first_n)
    # openlist.append([init_state, []])
    monontonicity = True

    while openlist:
        currentNode = heapq.heappop(openlist)
        # currentState, path = openlist.pop(0)
        closedlist.append(currentNode)

        # print(currentState)
        ### Reached the destination at the end.
        if currentNode.state == final_state:
            path = []
            while currentNode.parent:
                path.insert(0,currentNode.state)
                currentNode = currentNode.parent
            return path, monontonicity

        # Generate neighbors (up, down, left, right)
        neighbors = get_neighbours(currentNode.state, max_missionaries, max_cannibals)

        for nbr in neighbors:

            neighbor_node = Node(nbr, currentNode)

            # If already evaluated, skip
            if neighbor_node in closedlist:
                continue

            # openlist.append([nbr, path+[currentState]])
            neighbor_node.h = h5(nbr)
            cost = gstar(currentNode.state, nbr)
            if currentNode.h > cost + neighbor_node.h:
                monontonicity = False
            # Check if neighbor is already in the open list with a lower f value
            if any(open_node for open_node in openlist if neighbor_node == open_node and neighbor_node.h >= open_node.h):
                continue
            # Adding neighbor to the open list
            heapq.heappush(openlist, neighbor_node)
    # openlist = []
    # closedlist = []
    # openlist.append([init_state, []])
    # monotonicity = True

    # while openlist:
    #     # currentNode = heapq.heappop(openlist)
    #     currentState, path = openlist.pop(0)
    #     closedlist.append(currentState)
    #     hcurr = h5(currentState)
    #     ### Reached the destination at the end.
    #     if currentState == final_state:
    #         # while currentNode.parent:
    #         #     path.insert(0,currentNode.state)
    #         #     currentNode = currentNode.parent
    #         return path+[currentState], monotonicity

    #     # Generate neighbors (up, down, left, right)
    #     neighbors = get_neighbours(currentState, max_missionaries, max_cannibals)

    #     for nbr in neighbors:

    #         # neighbor_node = Node(nbr, currentNode)

    #         # If already evaluated, skip
    #         if nbr in closedlist:
    #             continue

    #         cost = gstar(currentState, nbr)
    #         hnbr = h5(nbr)
    #         if hcurr > cost + hnbr:
    #             monotonicity = False

            # openlist.append([nbr, path+[currentState]])
    
    return None, False
    raise ValueError("astar_h5 not implemented")


def print_solution(solution: List[list],max_mis,max_can):
    """
    Prints the solution path. 
    """
    if not solution:
        print("No solution exists for the given parameters.")
        return
        
    print("\nSolution found! Number of steps:", len(solution) - 1)
    print("\nLeft Bank" + " "*20 + "Right Bank")
    print("-" * 50)
    
    for state in solution:
        if state[-1]:
            boat_display = "(B) " + " "*15
        else:
            boat_display = " "*15 + "(B) "
            
        print(f"M: {state[0]}, C: {state[1]}  {boat_display}" 
              f"M: {max_mis-state[0]}, C: {max_can-state[1]}")


def print_mon(ism: bool):
    """
    Prints if the heuristic function is monotone or not.
    """
    if ism:
        print("-" * 10)
        print("|Monotone|")
        print("-" * 10)
    else:
        print("-" * 14)
        print("|Not Monotone|")
        print("-" * 14)


def main():
    try:
        testcases = [{"m": 3, "c": 3}]

        for case in testcases:
            max_missionaries = case["m"]
            max_cannibals = case["c"]
            
            init_state = [max_missionaries, max_cannibals, 1] #initial state 
            final_state = [0, 0, 0] # final state
            
            if not check_valid(init_state, max_missionaries, max_cannibals):
                print(f"Invalid initial state for case: {case}")
                continue
                
            path_h1,ism1 = astar_h1(init_state, final_state, max_missionaries, max_cannibals)
            path_h2,ism2 = astar_h2(init_state, final_state, max_missionaries, max_cannibals)
            path_h3,ism3 = astar_h3(init_state, final_state, max_missionaries, max_cannibals)
            path_h4,ism4 = astar_h4(init_state, final_state, max_missionaries, max_cannibals)
            path_h5,ism5 = astar_h5(init_state, final_state, max_missionaries, max_cannibals)
            print_solution(path_h1,max_missionaries,max_cannibals)
            print_mon(ism1)
            print("-"*50)
            print_solution(path_h2,max_missionaries,max_cannibals)
            print_mon(ism2)
            print("-"*50)
            print_solution(path_h3,max_missionaries,max_cannibals)
            print_mon(ism3)
            print("-"*50)
            print_solution(path_h4,max_missionaries,max_cannibals)
            print_mon(ism4)
            print("-"*50)
            print_solution(path_h5,max_missionaries,max_cannibals)
            print_mon(ism5)
            print("="*50)

    except KeyError as e:
        print(f"Missing required key in test case: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    main()

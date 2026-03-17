# A* Labyrinth Solver in C

A C program that **generates a random labyrinth** and finds the optimal path from entrance to exit using the **A\* (A-star) algorithm**.

## Overview

This project implements the A* pathfinding algorithm to solve a randomly generated maze.

- The labyrinth is generated with random dimensions and obstacles
- The algorithm finds the shortest path using:
  - **g(n):** cost from start
  - **h(n):** Manhattan distance heuristic
  - **f(n) = g(n) + h(n)**

The program also visualizes the search process step-by-step.

## Features

- Random labyrinth generation
- Adjustable size (10x10 to 20x20)
- Random obstacle placement (10%–50%)
- A* pathfinding algorithm
- Manhattan distance heuristic
- Step-by-step exploration visualization
- Backtracking to display optimal path

## Symbols Used

| Symbol | Meaning |
|------|--------|
| `-`  | Empty cell |
| `*`  | Obstacle |
| `R`  | Visited path |
| `E`  | Exit |

## How It Works

1. A random labyrinth is created
2. A random **entrance** is placed on the left side
3. A random **exit** is placed on the right side
4. The A* algorithm explores the grid:
   - Uses a sorted list (priority queue)
   - Expands nodes with lowest `f(n)`
5. When exit is found:
   - Backtracking reconstructs the shortest path

## Algorithm Details

- Heuristic used: **Manhattan Distance**

```math
d = |x1 - x2| + |y1 - y2|

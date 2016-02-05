// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment1.h"
#include "AStar.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "CoreMisc.h"
#include <limits>

TArray<FVector> AAStar::generate_path(int32 n)
{
	if (n != 4 && n != 8 && n != 16) {
		throw std::invalid_argument("n has to be 4, 8 or 16");
	}

	getMap();
	getPositions();

	FVector2D start = FVector2D(positions[0][0] - 1, positions[0][1] - 1);
	FVector2D goal = FVector2D(positions[1][0] - 1, positions[1][1] - 1);

	TArray<FVector2D> closedSet;	// The set of nodes already evaluated.
	TArray<FVector2D> openSet;		// The set of tentative nodes to be evaluated, initially containing the start node
	openSet.Add(start);

	TMap<FVector2D, FVector2D> cameFrom;	// The map of navigated nodes.

	TArray<TArray<float>> gScore = infMap(map[0].Num(), map.Num());	// map with default value of Infinity

	gScore[scaleToIndex * start[0]][scaleToIndex * start[1]] = 0;	// Cost from start along best known path.
									// Estimated total cost from start to goal through y.

	TArray<TArray<float>> fScore = infMap(map[0].Num(), map.Num());	// map with default value of Infinity

	fScore[scaleToIndex * start[0]][scaleToIndex * start[1]] = heuristic_cost_estimate(start, goal);

	while (openSet.Num() > 0) {
		int32 index = findLowestFScore(openSet, fScore);
		FVector2D current = openSet[index]; // openSet.Pop();	// the node in OpenSet having the lowest f_score[] value
		if (current == goal) {
			TArray<FVector> path = reconstruct_path(cameFrom, goal);
			return path;
		}

		openSet.RemoveAt(index);
		closedSet.Add(current);

		TQueue<FVector2D> neighbours;
		getNeighbours(neighbours, current, n);

		FVector2D neighbour;
		while (neighbours.Dequeue(neighbour)) {
			if (closedSet.Contains(neighbour)) {
				continue;	// Ignore the neighbor which is already evaluated.
			}

			float tentative_g_score = gScore[scaleToIndex * current[0]][scaleToIndex * current[1]] + dist_between(current, neighbour); // length of this path.

			if (!openSet.Contains(neighbour)) {
				openSet.Add(neighbour);
			}
			else if (tentative_g_score >= gScore[scaleToIndex * neighbour[0]][scaleToIndex * neighbour[1]]) {
				continue;		// This is not a better path.
			}

			// This path is the best until now. Record it!
			cameFrom.Add(neighbour, current);
			
			gScore[scaleToIndex * neighbour[0]][scaleToIndex * neighbour[1]] = tentative_g_score;
			fScore[scaleToIndex * neighbour[0]][scaleToIndex * neighbour[1]] = gScore[scaleToIndex * neighbour[0]][scaleToIndex * neighbour[1]] + heuristic_cost_estimate(neighbour, goal);
		}
	}

	throw std::exception("No path found!");
}

int32 AAStar::findLowestFScore(TArray<FVector2D> openSet, TArray<TArray<float>> fScore)
{
	int32 lowestIndex = 0;
	float lowestValue = std::numeric_limits<float>::infinity();

	for (int c = 0; c < openSet.Num(); c++) {
		if (fScore[scaleToIndex * openSet[c][0]][scaleToIndex * openSet[c][1]] < lowestValue) {
			lowestIndex = c;
			lowestValue = fScore[scaleToIndex * openSet[c][0]][scaleToIndex * openSet[c][1]];
		}
	}

	return lowestIndex;
}

TArray<TArray<float>> AAStar::infMap(int32 rows, int32 columns)
{
	TArray<TArray<float>> infMap;
	for (int32 c = 0; c < rows; c++) {
		TArray<float> row;
		for (int32 g = 0; g < columns; g++) {
			row.Add(std::numeric_limits<float>::infinity());
		}
		infMap.Add(row);
	}

	return infMap;
}

float AAStar::dist_between(FVector2D start, FVector2D goal)
{
	return FVector2D::Distance(scaleToIndex * start, scaleToIndex * goal);

	//float x = (goal[0] - start[0]) * (goal[0] - start[0]) + (goal[1] - start[1]) * (goal[1] - start[1]);

	// return sqrtf(x);
}

float AAStar::heuristic_cost_estimate(FVector2D start, FVector2D goal)
{
	return dist_between(start, goal);
}

TArray<FVector> AAStar::reconstruct_path(TMap<FVector2D, FVector2D> cameFrom, FVector2D current)
{
	TArray<FVector> tempPath;
	TArray<FVector> totalPath;
	tempPath.Add(FVector(current.X - 1, current.Y - 1, 0));
	FVector location = FVector(current.Y * gridSize, current.X * gridSize, 0);
	if (binMap) {
		location.X += (gridSize / 2);
		location.Y += (gridSize / 2);
	}
	totalPath.Add(scaleToIndex * location);
	while (cameFrom.Contains(current)) {
		//UE_LOG(LogTemp, Warning, TEXT("%f %f"), current[0], current[1]);
		current = cameFrom[current];
		location = FVector((current.Y) * gridSize, (current.X) * gridSize, 0);
		if (binMap) {
			location.X += (gridSize / 2);
			location.Y += (gridSize / 2);
		}
		totalPath.Insert(scaleToIndex * location, 0);
		tempPath.Insert(FVector(current.X - 1, current.Y - 1, 0), 0);
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), current[0], current[1]);

	writePathToFile(tempPath, fileOutName);

	return totalPath;
}

void AAStar::getNeighbours(TQueue<FVector2D> & neighbours, FVector2D node, int32 numNeighbours)
{
	float retX = node[0];
	float retY = node[1];

	node = scaleToIndex * node;
	float x = node[0];
	float y = node[1];

	if (x != 0 && map[y][x - 1] != 1) {
		neighbours.Enqueue(FVector2D(retX - 1, retY));
	}
	if (x != map[0].Num() - 1 && map[y][x + 1] != 1) {
		neighbours.Enqueue(FVector2D(retX + 1, retY));
	}
	if (y != 0 && map[y - 1][x] != 1) {
		neighbours.Enqueue(FVector2D(retX, retY - 1));
	}
	if (y != map.Num() - 1 && map[y + 1][x] != 1) {
		neighbours.Enqueue(FVector2D(retX, retY + 1));
	}

	if (numNeighbours != 4) {
		if (x != 0 && y != 0 && map[y - 1][x - 1] != 1) {		// Up left
			if (map[y][x - 1] == 0 || map[y - 1][x] == 0) {
				neighbours.Enqueue(FVector2D(retX - 1, retY - 1));
			}
		}
		if (x != map[0].Num() - 1 && y != 0 && map[y - 1][x + 1] != 1) {	// Up right
			if (map[y][x + 1] == 0 || map[y - 1][x] == 0) {
				neighbours.Enqueue(FVector2D(retX + 1, retY - 1));
			}
		}
		if (x != 0 && y != map.Num() - 1 && map[y + 1][x - 1] != 1) {	// Down left
			if (map[y][x - 1] == 0 || map[y + 1][x] == 0) {
				neighbours.Enqueue(FVector2D(retX - 1, retY + 1));
			}
		}
		if (x != map[0].Num() - 1 && y != map.Num() - 1 && map[y + 1][x + 1] != 1) {	// Down left
			if (map[y][x + 1] == 0 || map[y + 1][x] == 0) {
				neighbours.Enqueue(FVector2D(retX + 1, retY + 1));
			}
		}

		if (numNeighbours != 8) {
			if (x != 0 && y > 1 && map[y - 2][x - 1] != 1) {	// Up up left
				if (map[y - 1][x] == 0 && map[y - 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(retX - 1, retY - 2));
				}
			}
			if (x != 0 && y < map.Num() - 2 && map[y + 2][x - 1] != 1) {	// Down down left
				if (map[y + 1][x] == 0 && map[y + 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(retX - 1, retY + 2));
				}
			}

			if (x > 1 && y != 0 && map[y - 1][x - 2] != 1) {	// Up left left
				if (map[y][x - 1] == 0 && map[y - 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(retX - 2, retY - 1));
				}
			}
			if (x > 1 && y < map.Num() - 1 && map[y + 1][x - 2] != 1) {	// Down left left
				if (map[y][x - 1] == 0 && map[y + 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(retX - 2, retY + 1));
				}
			}

			if (x < map.Num() - 1 && y > 1 && map[y - 2][x + 1] != 1) {	// Up up right
				if (map[y - 1][x] == 0 && map[y - 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(retX + 1, retY - 2));
				}
			}
			if (x < map.Num() - 1 && y < map.Num() - 2 && map[y + 2][x + 1] != 1) {	// Down down right
				if (map[y + 1][x] == 0 && map[y + 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(retX + 1, retY + 2));
				}
			}

			if (x < map.Num() - 2 && y > 0 && map[y - 1][x + 2] != 1) {	// Up right right
				if (map[y][x + 1] == 0 && map[y - 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(retX + 2, retY - 1));
				}
			}
			if (x < map.Num() - 2 && y < map.Num() - 1 && map[y + 1][x + 2] != 1) {	// Down right right
				if (map[y][x + 1] == 0 && map[y + 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(retX + 2, retY + 1));
				}
			}
		}
	}
}




/*
Kinematic point model & diff. drive model
*/

#include "VisibilityGraph.h"

TArray<FVector> AAStar::generate_path2()
{
	getMap();
	getPositions();

	FVector2D start = FVector2D(positions[0][0], positions[0][1]);
	FVector2D goal = FVector2D(positions[1][0], positions[1][1]);

	vertices.Add(start);
	vertices.Add(goal);

	getVisibilityGraph(edges, vertices);

	TArray<FVector2D> closedSet;	// The set of nodes already evaluated.
	TArray<FVector2D> openSet;		// The set of tentative nodes to be evaluated, initially containing the start node
	openSet.Add(start);

	TMap<FVector2D, FVector2D> cameFrom;	// The map of navigated nodes.

	TMap<FVector2D, float> gScore = infMap();	// map with default value of Infinity

	gScore[start] = 0;	// Cost from start along best known path.
																	// Estimated total cost from start to goal through y.

	TMap<FVector2D, float> fScore = infMap();	// map with default value of Infinity

	fScore[start] = heuristic_cost_estimate(start, goal);

	while (openSet.Num() > 0) {
		int32 index = findLowestFScore(openSet, fScore);
		FVector2D current = openSet[index]; // openSet.Pop();	// the node in OpenSet having the lowest f_score[] value
		if (current == goal) {
			TArray<FVector> path = reconstruct_path(cameFrom, goal);
			return path;
		}

		openSet.RemoveAt(index);
		closedSet.Add(current);

		TQueue<FVector2D> neighbours;
		getNeighbours(neighbours, current);

		FVector2D neighbour;
		while (neighbours.Dequeue(neighbour)) {
			if (closedSet.Contains(neighbour)) {
				continue;	// Ignore the neighbor which is already evaluated.
			}

			float tentative_g_score = gScore[current] + dist_between(current, neighbour); // length of this path.

			if (!openSet.Contains(neighbour)) {
				openSet.Add(neighbour);
			} else if (tentative_g_score >= gScore[neighbour]) {
				continue;		// This is not a better path.
			}

			// This path is the best until now. Record it!
			cameFrom.Add(neighbour, current);

			gScore[neighbour] = tentative_g_score;
			fScore[neighbour] = gScore[neighbour] + heuristic_cost_estimate(neighbour, goal);
		}
	}

	throw std::exception("No path found!");
}

TMap<FVector2D, float> AAStar::infMap()
{
	TMap<FVector2D, float> map;
	for (int32 c = 0; c < vertices.Num(); c++) {
		map.Add(vertices[c], std::numeric_limits<float>::infinity());
	}

	return map;
}

int32 AAStar::findLowestFScore(TArray<FVector2D> openSet, TMap<FVector2D, float> fScore)
{
	int32 index = 0;
	float score = std::numeric_limits<float>::infinity();
	for (int32 c = 0; c < openSet.Num(); c++) {
		if (fScore[openSet[c]] < score) {
			index = c;
			score = fScore[openSet[c]];
		}
	}

	return index;
}

void AAStar::getNeighbours(TQueue<FVector2D> & queue, FVector2D current)
{
	TArray<FVector2D> neighbours;
	visibilityGraph.MultiFind(current, neighbours);

	for (int32 c = 0; c < neighbours.Num(); c++) {
		queue.Enqueue(neighbours[c]);
	}
}
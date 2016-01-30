// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment1.h"
#include "RRT.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "CoreMisc.h"
#include <limits>
#include <stdlib.h>
#include <time.h>

static FString fileMap = "discObst.txt";
static FString filePositions = "positions.txt";
static FString fileOutName = "discData.txt";

TArray<FVector> ARRT::generate_path(int32 n)
{
	srand(time(NULL));

	TArray<TArray<int32>> map = readData(fileMap);
	TArray<TArray<int32>> positions = readData(filePositions);

	FVector2D start = FVector2D(positions[0][0] - 1, positions[0][1] - 1);

	FVector2D goal = FVector2D(positions[1][0] - 1, positions[1][1] - 1);

	TArray<FVector2D> freeSpace = getFreeSpace(map);

	TArray<Node> pathNode = findPath(start, goal, freeSpace, map);

	TArray<FVector> path = makePath(pathNode);

	writePathToFile(path, "Test");

	return path;
	/*
	Algorithm BuildRRT
	  Input: Initial configuration qinit, number of vertices in RRT K, incremental distance q)
	  Output: RRT graph G

	  G.init(qinit)
	  for k = 1 to K
		qrand = RAND_CONF()
		qnear = NEAREST_VERTEX(qrand, G)
		qnew = NEW_CONF(qnear, qrand, q)
		G.add_vertex(qnew)
		G.add_edge(qnear, qnew)
	  return G

	  In the algorithm above, "RAND_CONF" grabs a random configuration qrand in C. 
	  This may be replaced with a function "RAND_FREE_CONF" that uses samples in Cfree, 
	  while rejecting those in Cobs using some collision detection algorithm.

	  "NEAREST_VERTEX" is a straightforward function that runs through all vertexes v in graph G, 
	  calculates the distance between qrand and v using some measurement function thereby returning the nearest vertex.

	  "NEW_CONF" selects a new configuration qnew by moving an incremental distance q from qnear in the direction of qrand. 
	  (According to [4] in holonomic problems, this should be omitted and qrand used instead of qnew.)
	*/
}

TArray<FVector> ARRT::makePath(TArray<Node> nodes)
{
	TArray<FVector> path;

	for (int c = 0; c < nodes.Num(); c++) {
		path.Add(FVector(nodes[c].pos, 0));
	}

	return path;
}

TArray<ARRT::Node> ARRT::findPath(FVector2D start, FVector2D goal, TArray<FVector2D> freeSpace, TArray<TArray<int32>> map)
{
	TArray<Node> rrt;

	Node current;
	current.pos = start;

	rrt.Add(current);

	freeSpace.RemoveSingle(start);

	FVector2D randomPoint;

	Node closest;

	while (current.pos != goal && freeSpace.Num() > 0) {
		randomPoint = getRandomPoint(freeSpace);

		closest = getClosest(rrt, randomPoint);

		// TODO: get input?!

		Node newCurrent;
		if (goTowards(newCurrent, closest, randomPoint, freeSpace)) {
			current = newCurrent;
			freeSpace.RemoveSingle(current.pos);
		}
	}

	TArray<Node> path;
	path.Add(current);

	while (current.cameFrom != nullptr) {
		current = *current.cameFrom;
		path.Insert(current, 0);
	}
	
	return path;
}

bool ARRT::goTowards(Node & node, Node from, FVector2D to, TArray<FVector2D> freeSpace)
{
	FVector2D test = from.pos;

	float distance = std::numeric_limits<float>::infinity();
	FVector2D best;

	bool success = false;

	test = FVector2D(test[0] - 1, test[1]);
	if (freeSpace.Contains(test)) {
		if (getDistance(test, to) < distance) {
			distance = getDistance(test, to);
			best = test;
			success = true;
		}
	}
	test = FVector2D(test[0] + 1, test[1]);
	if (freeSpace.Contains(test)) {
		if (getDistance(test, to) < distance) {
			distance = getDistance(test, to);
			best = test;
			success = true;
		}
	}
	test = FVector2D(test[0], test[1] - 1);
	if (freeSpace.Contains(test)) {
		if (getDistance(test, to) < distance) {
			distance = getDistance(test, to);
			best = test;
			success = true;
		}
	}
	test = FVector2D(test[0], test[1] + 1);
	if (freeSpace.Contains(test)) {
		if (getDistance(test, to) < distance) {
			distance = getDistance(test, to);
			best = test;
			success = true;
		}
	}

	node.pos = best;
	node.cameFrom = &from;

	return success;
}

ARRT::Node ARRT::getClosest(TArray<Node> rrt, FVector2D point)
{
	Node closestNode;
	float distance = std::numeric_limits<float>::infinity();
	
	float tempDistance;
	for (int32 c = 0; c < rrt.Num(); c++) {
		tempDistance = getDistance(rrt[c].pos, point);
		if (tempDistance < distance) {
			distance = tempDistance;
			closestNode = rrt[c];
		}
	}


	return closestNode;
}

float ARRT::getDistance(FVector2D start, FVector2D goal)
{
	float distance = (goal[0] - start[0]) * (goal[0] - start[0]) + (goal[1] - start[1]) * (goal[1] - start[1]);

	return sqrt(distance);
}

FVector2D ARRT::getRandomPoint(TArray<FVector2D> points)
{
	int32 index = rand() % points.Num();
	FVector2D point = points[index];
	return point;
}

TArray<FVector2D> ARRT::getFreeSpace(TArray<TArray<int32>> map)
{
	TArray<FVector2D> freeSpace;

	for (int c = 0; c < map.Num(); c++) {
		for (int g = 0; g < map[c].Num(); g++) {
			if (map[c][g] == 0) {
				freeSpace.Add(FVector2D(c, g));
			}
		}
	}

	return freeSpace;
}

TArray<TArray<int>> ARRT::readData(FString fileName)
{
	TArray<FString> strArray;
	FString projectDir = FPaths::GameDir();
	projectDir += "Input Data/" + fileName;
	FFileHelper::LoadANSITextFileToStrings(*projectDir, NULL, strArray);

	TArray<TArray<int32>> data;
	for (FString line : strArray) {
		if (line.Trim().IsEmpty()) continue;

		TArray<int32> row;

		FString solid;

		while (line.Split(FString("\t"), &solid, &line)) {
			row.Add(FCString::Atoi(*solid));
		}

		row.Add(FCString::Atoi(*line));

		data.Add(row);
	}

	return data;
}

void ARRT::writePathToFile(TArray<FVector> path, FString fileName)
{
	FString str;

	for (int32 c = path.Num() - 1; c >= 0; c--) {
		str += FString::SanitizeFloat(path[c][0] + 1) + "\t" + FString::SanitizeFloat(path[c][1] + 1) + "\n";
	}

	FString projectDir = FPaths::GameDir();
	projectDir += "Output Data/" + fileName;
	FFileHelper::
		FFileHelper::SaveStringToFile(str, *projectDir);

}

TArray<TArray<float>> ARRT::infMap(int32 rows, int32 columns)
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

float ARRT::dist_between(FVector2D start, FVector2D goal)
{
	float x = (goal[0] - start[0]) * (goal[0] - start[0]) + (goal[1] - start[1]) * (goal[1] - start[1]);

	return sqrtf(x);
}

float ARRT::heuristic_cost_estimate(FVector2D start, FVector2D goal)
{
	return dist_between(start, goal);
}

TArray<FVector> ARRT::reconstruct_path(TMap<FVector2D, FVector2D> cameFrom, FVector2D current)
{
	TArray<FVector> totalPath;
	totalPath.Add(FVector(current, 0));
	while (cameFrom.Contains(current)) {
		UE_LOG(LogTemp, Warning, TEXT("%f %f"), current[0], current[1]);
		current = cameFrom[current];
		totalPath.Add(FVector(current, 0));
	}
	UE_LOG(LogTemp, Warning, TEXT("%f %f"), current[0], current[1]);

	return totalPath;
}

void ARRT::getNeighbours(TQueue<FVector2D> & neighbours, TArray<TArray<int32>> map, FVector2D node, int32 numNeighbours)
{
	float x = node[0];
	float y = node[1];

	if (x != 0 && map[y][x - 1] != 1) {
		neighbours.Enqueue(FVector2D(x - 1, y));
	}
	if (x != map[0].Num() - 1 && map[y][x + 1] != 1) {
		neighbours.Enqueue(FVector2D(x + 1, y));
	}
	if (y != 0 && map[y - 1][x] != 1) {
		neighbours.Enqueue(FVector2D(x, y - 1));
	}
	if (y != map.Num() - 1 && map[y + 1][x] != 1) {
		neighbours.Enqueue(FVector2D(x, y + 1));
	}

	if (numNeighbours != 4) {
		if (x != 0 && y != 0 && map[y - 1][x - 1] != 1) {		// Up left
			if (map[y][x - 1] == 0 || map[y - 1][x] == 0) {
				neighbours.Enqueue(FVector2D(x - 1, y - 1));
			}
		}
		if (x != map[0].Num() - 1 && y != 0 && map[y - 1][x + 1] != 1) {	// Up right
			if (map[y][x + 1] == 0 || map[y - 1][x] == 0) {
				neighbours.Enqueue(FVector2D(x + 1, y - 1));
			}
		}
		if (x != 0 && y != map.Num() - 1 && map[y + 1][x - 1] != 1) {	// Down left
			if (map[y][x - 1] == 0 || map[y + 1][x] == 0) {
				neighbours.Enqueue(FVector2D(x - 1, y + 1));
			}
		}
		if (x != map[0].Num() - 1 && y != map.Num() - 1 && map[y + 1][x + 1] != 1) {	// Down left
			if (map[y][x + 1] == 0 || map[y + 1][x] == 0) {
				neighbours.Enqueue(FVector2D(x + 1, y + 1));
			}
		}

		if (numNeighbours != 8) {
			if (x != 0 && y > 1 && map[y - 2][x - 1] != 1) {	// Up up left
				if (map[y - 1][x] == 0 && map[y - 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(x - 1, y - 2));
				}
			}
			if (x != 0 && y < map.Num() - 2 && map[y + 2][x - 1] != 1) {	// Down down left
				if (map[y + 1][x] == 0 && map[y + 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(x - 1, y + 2));
				}
			}

			if (x > 1 && y != 0 && map[y - 1][x - 2] != 1) {	// Up left left
				if (map[y][x - 1] == 0 && map[y - 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(x - 2, y - 1));
				}
			}
			if (x > 1 && y < map.Num() - 1 && map[y + 1][x - 2] != 1) {	// Down left left
				if (map[y][x - 1] == 0 && map[y + 1][x - 1] == 0) {
					neighbours.Enqueue(FVector2D(x - 2, y + 1));
				}
			}

			if (x < map.Num() - 1 && y > 1 && map[y - 2][x + 1] != 1) {	// Up up right
				if (map[y - 1][x] == 0 && map[y - 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(x + 1, y - 2));
				}
			}
			if (x < map.Num() - 1 && y < map.Num() - 2 && map[y + 2][x + 1] != 1) {	// Down down right
				if (map[y + 1][x] == 0 && map[y + 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(x + 1, y + 2));
				}
			}

			if (x < map.Num() - 2 && y > 0 && map[y - 1][x + 2] != 1) {	// Up right right
				if (map[y][x + 1] == 0 && map[y - 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(x + 2, y - 1));
				}
			}
			if (x < map.Num() - 2 && y < map.Num() - 1 && map[y + 1][x + 2] != 1) {	// Down right right
				if (map[y][x + 1] == 0 && map[y + 1][x + 1] == 0) {
					neighbours.Enqueue(FVector2D(x + 2, y + 1));
				}
			}
		}
	}
}
#pragma once
#include "pch.h"
#include "Types.h"
#include "ResourceManager.h"
#include "ADPhysics.h"
#include <unordered_map>
#include <set>


namespace ADAI
{

	struct PathingNode
	{
		XMFLOAT3 position;
		int column;
		int row;

		bool walkable;
		long displayState;
		unsigned char terrainWeight;
		bool threeSided;
		bool directions[8];
	};


	class ADPathfinding
	{
		private:
		struct PathingPoint
		{
			XMFLOAT3 position;
			bool walkable;
		};

		struct PathingGrid
		{
			XMFLOAT2 planeLows;
			XMFLOAT2 mapSize;
			XMFLOAT2 cellSize;
			UINT columns;
			UINT rows;
			UINT xDivisions;
			UINT yDivisions;
			std::vector<PathingNode*> nodeGrid;

			void Initializing(std::vector<SimpleVertex>* _planeVertices, XMFLOAT2 _mapSize, float _agentSize, float _agentToWallGap)
			{
				float xhigh = 0;
				float zhigh = 0;

				for (int i = 0; i < _planeVertices->size(); i++)
				{
					float x = (*_planeVertices)[i].Position.x;
					float z = (*_planeVertices)[i].Position.z;
					if (x < planeLows.x)
					{
						planeLows.x = x;
					}
					if (x > xhigh)
					{
						xhigh = x;
					}
					if (z < planeLows.y)
					{
						planeLows.y = z;
					}
					if (z > zhigh)
					{
						zhigh = z;
					}
				}

				float xdelta = xhigh - planeLows.x;
				float zdelta = zhigh - planeLows.y;

				mapSize.x = xdelta * _mapSize.x;
				mapSize.y = zdelta * _mapSize.y;

				cellSize.x = (_agentSize + _agentToWallGap);
				cellSize.y = (_agentSize + _agentToWallGap);
				xDivisions = (mapSize.x / cellSize.x)+1;
				yDivisions = (mapSize.y / cellSize.y)+1;
				columns = (mapSize.x / cellSize.x);
				rows = (mapSize.y / cellSize.y);
			}

			void GetColumnRowFromPosition(XMFLOAT2 _position, UINT& _out_column, UINT& _out_row)
			{
				float x = _position.x - (planeLows.x * mapSize.x);
				float z = _position.y - (planeLows.y * mapSize.y);
				_out_column = int(x / cellSize.x);
				_out_row = int(z / cellSize.y);
			};
		};

		struct SearchNode
		{
			SearchNode(PathingNode* _tile) : tile(_tile) {};
			PathingNode* tile;
			std::vector<SearchNode*> neighbors;
			std::vector<float> neighborDist;
		};

		struct PlannerNode
		{
			SearchNode* searchNode;
			PlannerNode* parent;

			float heuristicCost;
			float givenCost;
			float finalCost;
		};

		class CompareCost
		{
		public:
			bool operator() (PlannerNode* _a, PlannerNode* _b)
			{
				return (_a->finalCost > _b->finalCost);
			};
		};

		std::vector<PathingPoint> pointGrid;

		std::unordered_map<PathingNode*, SearchNode*> searching_map;
		std::unordered_map<SearchNode*, PlannerNode*> visited_map;
		std::vector<PathingNode const*> solution;
		std::vector<PathingNode*> previousTrace;
		PlannerNode* current;
		PlannerNode* retracer;

		std::vector<PlannerNode*> pHeap;

		PathingNode* target;
		UINT hWeight = 1.0f;

		void CreatePointGrid(std::vector<SimpleVertex>* _planeVertices);
		void CleanUpPointGrid();
		void SetNeighbors(SearchNode* searchNode);
		float DistanceCalculation(PathingNode* _a);
		float DistanceCalculation(PathingNode* _a, PathingNode* _b);
		bool done = false;

	public:
		PathingGrid tileMap;
		ADPathfinding();
		~ADPathfinding();
		void Initialize(std::vector<SimpleVertex>* _planeVertices, XMFLOAT2 _mapSize, float _agentSize, float _agentToWallGap);
		void enter(int startColumn, int startRow, int goalColumn, int goalRow);
		bool isDone() const;
		void update(long timeslice);
		std::vector<PathingNode const*> const getSolution() const;
		void exit();
		void shutdown();
		void UpdatePlayerNode(float x, float z, float mapWidth, float mapHeight);
		std::vector<PathingNode*>* GetPlaneNodes();

		void ClearDebug()
		{
			for (auto it = searching_map.begin(), itEnd = searching_map.end(); it != itEnd; ++it)
			{
				if(it->first->walkable)
					it->first->displayState = 0;
				else
					it->first->displayState = 1;
			}
		}

		SearchNode* GetTile(int _column = 0, int _row = 0)
		{
			if (_column < 0 || _column >= tileMap.columns || _row < 0 || _row >= tileMap.rows)
			{
				return nullptr;
			}
			return searching_map[tileMap.nodeGrid[(_row * tileMap.columns) + _column]];
		};
	};
}


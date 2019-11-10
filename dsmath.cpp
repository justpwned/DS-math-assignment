/*
	Discrete mathematics homework.

	Objective: Build shortest path tree for a given graph from specified vertex using Floyd–Warshall algorithm.
*/

#ifdef _WIN32

#define _WIN32_WINNT 0x0500
#include <Windows.h>

#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
#include <cstdint>

int INF = (int)1e6;

std::string normalize(int w)
{
	if (w < 10)
		return "-" + std::to_string(w) + "-";
	else if (w < 100)
		return "-" + std::to_string(w);
	else
		return std::to_string(w);
}

void FillGraph(int n, std::vector<std::vector<int>>& g, std::vector<std::vector<int>>& s, std::ifstream& sinput)
{
	int a, b, w;

	while (sinput >> a >> b >> w)
	{
		g[a - 1][b - 1] = w;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i != j && g[i][j] == 0)
			{
				g[i][j] = INF;
			}
			s[i][j] = j + 1;
		}
	}
}

void PrintMatrices(std::vector<std::vector<int>>& d, std::vector<std::vector<int>>& s)
{
	for (int i = 0; i < (int)d.size(); i++)
	{
		for (int j = 0; j < (int)d.size(); j++)
		{
			if (i == j)
				std::cout << std::setw(4) << "-";
			else if (d[i][j] >= INF)
				std::cout << std::setw(4) << static_cast<unsigned char>(236);
			else
				std::cout << std::setw(4) << d[i][j];
		}

		std::cout << std::string(6, ' ');

		for (int j = 0; j < (int)s.size(); j++)
		{
			std::cout << std::setw(4) << s[i][j];
		}

		std::cout << "\n";
	}
	std::cout << "\n";
}

void dfs(int v, int w, std::vector<std::vector<std::pair<int, int>>>& tree, std::vector<bool>& used, int depth, std::vector<bool>& d, uint8_t ch)
{
	if (depth == 0)
	{
		std::cout << v + 1 << "\n";
	}
	else
	{
		for (int i = 0; i < depth - 1; i++)
		{
			if (d[i])
				std::cout << "|       ";
			else
				std::cout << "        ";
		}
		std::cout << static_cast<unsigned char>(ch) << "--" << normalize(w) << "--" << v + 1 << "\n";
	}

	used[v] = true;
	d[depth] = true;
	ch = 195;

	for (size_t i = 0; i < tree[v].size(); i++)
	{
		std::pair<int, int> vertex = tree[v][i];

		if (i == tree[v].size() - 1)
		{
			d[depth] = false;
			ch = 192;
		}

		if (!used[vertex.first])
		{
			dfs(vertex.first, vertex.second, tree, used, depth + 1, d, ch);
		}
	}

	if (depth == 1 && std::any_of(used.begin(), used.end(), [](bool v) { return !v; }))
		std::cout << "|\n";
}

void VisualizeTree(int root, std::vector<std::vector<std::pair<int, int>>>& tree)
{
	std::vector<bool> used(tree.size(), false);
	std::vector<bool> d(50, false);
	d[0] = true;

	dfs(root, 0, tree, used, 0, d, 195);
}

void BuildTree(int start, std::vector<std::vector<std::pair<int, int>>>& tree,
	std::vector<std::vector<int>>& g, std::vector<std::vector<int>>& s)
{
	std::queue<int> q;
	std::vector<bool> used(tree.size(), false);
	used[start] = true;
	q.push(start);

	while (!q.empty())
	{
		int v = q.front();
		q.pop();

		for (size_t i = 0; i < s[v].size(); i++)
		{
			if (s[v][i] == i + 1 && !used[s[v][i] - 1] && g[v][i] != INF)
			{
				tree[v].push_back(std::make_pair(i, g[v][i]));
				q.push(i);
				used[i] = true;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	/*
		Input file - file that stores representation of a graph (see graph.txt):

		n		(number of vertices)
		a b w   (edge from vertex 'a' to 'b' thas weight equal to 'w')

		Start vertex - vertex from which you have to build shortest path tree.
	*/


	std::string in_file, out_file;

	std::cout << "Input file: ";
	std::cin >> in_file;

	int start;
	std::cout << "Start vertex: ";
	std::cin >> start;
	start--;

	std::ifstream input(in_file);

	int n;
	input >> n;

#ifdef _WIN32

	HWND Console = GetConsoleWindow();
	RECT WindowRect;
	GetWindowRect(Console, &WindowRect);

	int Width = WindowRect.right - WindowRect.left;
	int Height = WindowRect.bottom - WindowRect.top;
	int RequiredWidth = (n * 8 + 14) * 8;

	if (Width < RequiredWidth)
	{
		MoveWindow(Console, WindowRect.left, WindowRect.top, RequiredWidth, Height, TRUE);
	}

#endif

	std::vector<std::vector<int>> g(n, std::vector<int>(n));
	std::vector<std::vector<int>> s(n, std::vector<int>(n));
	std::vector<std::vector<std::pair<int, int>>> tree(n, std::vector<std::pair<int, int>>());

	FillGraph(n, g, s, input);
	input.close();

	std::string sep(n * 8 + 7, '-');

	std::cout << sep << "\n";

	std::cout << "\nD0" << std::string((n - 1) * 4 + 3, ' ') << "    S0\n";
	PrintMatrices(g, s);

	std::cout << sep << "\n";

	// Floyd algorithm
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
			{
				if (g[i][k] + g[k][j] < g[i][j])
				{
					g[i][j] = g[i][k] + g[k][j];
					s[i][j] = k + 1;
				}
			}

		std::cout << "\n";

		std::cout << "\nD" << k + 1 << std::string(k + 1 < 10 ? (n - 1) * 4 + 3 : (n - 1) * 4 + 2, ' ')
			<< "    S" << k + 1 << "\n";

		PrintMatrices(g, s);

		std::cout << sep << "\n";
	}
	std::cout << "\n";

	BuildTree(start, tree, g, s);
	VisualizeTree(start, tree);

	return 0;
}
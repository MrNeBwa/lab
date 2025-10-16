#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> 
using namespace std;

const int N = 6; 
const int dx[] = {1, -1, 0, 0}; 
const int dy[] = {0, 0, 1, -1}; 

struct Point {
    int x, y;
};


bool isValid(int x, int y, const vector<vector<int>>& maze, vector<vector<bool>>& visited) {
    // maze[x][y] == 0 означает, что это проход (0)
    return x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0 && !visited[x][y];
}

int main() {
    
    vector<vector<int>> maze(N, vector<int>(N));
    //cout << "Введите матрицу " << N << "x" << N << " (0 - проход, 1 - стена):\n";
    
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> maze[i][j];

    vector<vector<bool>> visited(N, vector<bool>(N, false));
    queue<Point> q;

        cout << "Начальная точка заблокирована.\n";
        return 0;
    }
    
    // Начало BFS
    q.push({0, 0});
    visited[0][0] = true;
    // Родитель начальной точки остается {-1, -1} по умолчанию.

    bool found = false;
    Point end_point = {N - 1, N - 1}; // Конечная точка (5, 5)

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();

        if (cur.x == end_point.x && cur.y == end_point.y) {
            found = true;
            break; // Путь найден
        }

        // 4 направления (вверх, вниз, влево, вправо)
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];

            if (isValid(nx, ny, maze, visited)) {
                visited[nx][ny] = true;
                parent[nx][ny] = cur; // Запоминаем текущую точку как родителя
                q.push({nx, ny});
            }
        }
    }
    if (!found) {
        cout << "Путь не найден.\n";
    } else {
        vector<Point> path;
        Point cur = end_point;
        
            path.push_back(cur);
            cur = parent[cur.x][cur.y];
        }
        
        reverse(path.begin(), path.end());

        cout << "Путь найден. Длина: " << path.size() << " клеток.\n";
        cout << "Координаты пути:\n";
        for (const auto& p : path) {
            cout << "(" << p.x << ", " << p.y << ")\n";
        }
    }

    return 0;
}

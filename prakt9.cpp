#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstring>
using namespace std;

const int row = 4, column = 4;
int maxFlow = 0;
int threadNetwork[row][column] = {
    {2, 2, 0, 2},
    {1, 2, 2, 0},
    {0, 1, 2, 2},
    {2, 0, 2, 2}
};

int residualThreadNetwork[row][column];
int arrayVertex[row];
bool visited[row];
int parentVertex[row];

bool findIncreasingPath(int source, int sink)
{
    memset(visited, false, sizeof(visited));
    memset(parentVertex, -1, sizeof(parentVertex));

    vector<int> queue;
    queue.push_back(source);
    visited[source] = true;

    while (!queue.empty())
    {
        int currentVertex = queue.front();
        queue.erase(queue.begin());

        for (int i = 0; i < row; i++)
        {
            if (!visited[i] && residualThreadNetwork[currentVertex][i] > 0)
            {
                queue.push_back(i);
                parentVertex[i] = currentVertex;
                visited[i] = true;
            }
        }
    }

    return (visited[sink]);
}

struct ThreadData
{
    int start; //исток
    int end; //сток
};

void FindMax(LPVOID param)
{
    ThreadData* data = (ThreadData*)param;
    int source = data->start;
    int sink = data->end;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            residualThreadNetwork[i][j] = threadNetwork[i][j];
        }
    }

    int localMaxFlow = 0;

    while (findIncreasingPath(source, sink))
    {
        int pathFlow = INT_MAX;

        for (int i = sink; i != source; i = parentVertex[i])
        {
            int parent = parentVertex[i];
            pathFlow = min(pathFlow, residualThreadNetwork[parent][i]);
        }

        for (int i = sink; i != source; i = parentVertex[i])
        {
            int parent = parentVertex[i];
            residualThreadNetwork[parent][i] -= pathFlow;
            residualThreadNetwork[i][parent] += pathFlow;
        }

        maxFlow += pathFlow;
    }
}

int main()
{
    ThreadData data1 = { 0, 1 };
    ThreadData data2 = { 1, 0 };

    HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FindMax, &data1, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FindMax, &data2, 0, NULL);

    if (hThread1 == NULL || hThread2 == NULL)
    {
        return GetLastError();
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    cout << "Max Flow: " << maxFlow << endl;

    return 0;
}


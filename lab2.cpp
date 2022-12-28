#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <iomanip>
#include <string>
#include <queue>
#include <algorithm>

void readFromFile(std::vector<std::vector<std::vector<int>>>& startMatrix, int& Q, int& M, int T, std::vector<int>& F)
{
    std::ifstream fin;

    fin.open("input.txt");

    if (!fin.is_open())
    {
        std::cout << "Ошибка открытия файла!" << std::endl;
        system("pause");
        system("exit");
    }
    else
    {
        std::cout << "Файл успешно открыт!\n" << std::endl;
        fin >> Q;
        startMatrix.assign(Q, std::vector<std::vector<int>>(T));

        for (int i = 0; i < Q; i++)
        {
            for (int j = 0; j < T; j++)
            {
                fin >> M;
                startMatrix[i][j].assign(M, 0);
            }
        }

        std::cout << std::setw(7) << "0" << std::setw(7) << "1\n";
        for (int i = 0; i < Q; i++)
        {
            std::cout << "Q" << i;
            for (int j = 0; j < T; j++)
            {
                if (j == 0)
                {
                    std::cout << std::setw(3) << "(";
                }
                else if(startMatrix[i][j-1].size() == 1)
                {
                    std::cout << std::setw(3) << "(";
                }
                else if (startMatrix[i][j - 1].size() == 2)
                {
                    std::cout << std::setw(1) << "(";
                }
                for (size_t k = 0; k < startMatrix[i][j].size(); k++)
                {
                    fin >> startMatrix[i][j][k];
                    if (k == startMatrix[i][j].size() - 1)
                    {
                        std::cout << startMatrix[i][j][k];
                    }
                    else
                    {
                        std::cout << startMatrix[i][j][k] << ",";
                    }
                }
                std::cout << ") ";
            }
            std::cout << "\n";
        }
        while (!fin.eof())
        {
            int fromFile;
            fin >> fromFile;
            F.push_back(fromFile);
        }
    }
    fin.close();
}

void testFSM(std::vector<std::vector<std::vector<int>>>& startMatrix, std::vector<int> F)
{
    system("cls");
    std::queue<int> testWord;
    std::cout << "Введите тестовое слово для проверки конечного автомата(слитно): ";
    std::string p;
    std::cin >> p;

    for (int i = 0; i < p.length(); i++)
    {
        testWord.push(p[i] - 48);
    }

    std::queue<std::pair<int, std::queue<int>>> i;
    std::vector<int> finalStates;
    i.push({ 0, testWord });
    bool ready = false;
    while (!ready)
    {
        int n = i.front().second.front();
        i.front().second.pop();
        for (size_t j = 0; j < startMatrix[i.front().first][n].size(); j++)
        {
            i.push({ startMatrix[i.front().first][n][j], i.front().second });
            if (i.front().second.empty())
            {
                finalStates.push_back(startMatrix[i.front().first][n][j]);
            }
        }

        i.pop();

        if (finalStates.size() == i.size())
        {
            ready = true;
        }
    }

    system("pause");
    system("cls");
    ready = false;
    for (size_t i = 0; i < finalStates.size(); i++)
    {
        for (size_t j = 0; j < F.size(); j++)
        {
            if (finalStates[i] == F[j])
            {
                std::cout << "Введенное слово принадлежит конечному автомату!\n\n";
                ready = true;
                break;
            }
        }
        if (ready)
        {
            break;

        }
    }

    for (size_t i = 0; i < finalStates.size(); i++)
    {
        std::cout << finalStates[i] << " ";
    }
    std::cout << "\n\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    std::vector<std::vector<std::vector<int>>> startMatrix;
    std::vector<std::vector<std::vector<int>>> finishMatrix;

    int Tstart = 2;
    int Tfinish = 3;
    int Q;
    int M;
    std::vector<int> F;

    readFromFile(startMatrix, Q, M, Tstart, F);
    system("pause");

    testFSM(startMatrix, F);
    system("pause");
    

    system("cls");
    finishMatrix.assign(1, std::vector<std::vector<int>>(Tfinish));

    int size = 3;

    finishMatrix[0][0].push_back(0);
    finishMatrix[0][1].push_back(1);
    finishMatrix[0][2].push_back(2);

    int count = 0;
    bool finish = false;
    while (!finish)
    {
        count++;
        

        std::vector<int> a1;
        std::vector<std::vector<int>> a2;
        for (int w = 0; w < Tstart; w++)
        {
            a1.clear();
            for (size_t i = 0; i < finishMatrix[count - 1][0].size(); i++)
            {
                int k = finishMatrix[count - 1][0][i];

                
                for (size_t j = 0; j < startMatrix[k][w].size(); j++)
                {
                    a1.push_back(startMatrix[k][w][j]);
                    std::sort(a1.begin(), a1.end());
                }
                
            }

            for (size_t z = 1; z < a1.size(); z++)
            {
                if (a1[z - 1] == a1[z])
                {
                    a1.erase(a1.begin() + z);
                    z--;
                }
            }
            a2.push_back(a1);

            std::vector<bool> ready;
            for (size_t j = 0; j < finishMatrix.size(); j++)
            {
                if (finishMatrix[j][0].size() == a1.size())
                {
                    for (size_t k = 0; k < finishMatrix[j][0].size(); k++)
                    {
                        if (finishMatrix[j][0][k] == a1[k])
                        {
                            ready.push_back(true);
                            if(ready.size() == a1.size())
                            {
                                break;
                            }
                        }
                        else
                        {
                            ready.clear();
                        }
                    }
                }
                if (ready.size() == a1.size())
                {
                    break;
                }
                else
                {
                    ready.clear();
                }
            }

            if (ready.empty())
            {
                finishMatrix.push_back(a2);
            }
            a1.clear();
            a2.clear();
        }

        std::vector<bool> ready;
        std::vector<int> a3;
        for (size_t p = 0; p < Tstart; p++)
        {
            int interValue = -1;
            a3.clear();
            for (size_t i = 0; i < finishMatrix[count][0].size(); i++)
            {
                for (size_t k = 0; k < startMatrix[finishMatrix[count][0][i]][p].size(); k++)
                {
                    a3.push_back(startMatrix[finishMatrix[count][0][i]][p][k]);
                }
            }

            std::sort(a3.begin(), a3.end());
            for (size_t z = 1; z < a3.size(); z++)
            {
                if (a3[z - 1] == a3[z])
                {
                    a3.erase(a3.begin() + z);
                }
            }

            for (size_t j = 0; j < finishMatrix.size(); j++)
            {
                for (size_t i = 0; i < a3.size(); i++)
                {
                    if (a3.size() == finishMatrix[j][0].size())
                    {
                        if (a3[i] == finishMatrix[j][0][i])
                        {
                            ready.push_back(true);
                        }
                        else
                        {
                            ready.clear();
                        }
                    }
                }

                if (ready.size() == a3.size())
                {
                    interValue = j;
                    ready.clear();
                    break;
                }
            }

            if (interValue >= 0)
            {
                a1.push_back(interValue);
                finishMatrix[count].push_back(a1);
                a1.clear();
            }
            else
            {
                a1.push_back(size);
                finishMatrix[count].push_back(a1);
                a1.clear();
                size++;
            }
        }

        if (count == finishMatrix.size() - 1)
        {
            finish = true;
        }
    }

    std::cout << std::setw(18) << "0" << std::setw(9) << "1\n";
    for (size_t i = 0; i < finishMatrix.size(); i++)
    {
        std::cout << "Q" << i << " =";
        for (size_t j = 0; j < finishMatrix[i].size(); j++)
        {
            std::cout <<std::setw(5) << " { ";
            if (j == 0)
            {
                for (size_t k = 0; k < finishMatrix[i][j].size(); k++)
                {
                    std::cout << finishMatrix[i][j][k] << " ";
                }
                std::cout << "}";
            }
            else if (j == 1)
            {
                for (size_t k = 0; k < finishMatrix[i][j].size(); k++)
                {
                    std::cout << finishMatrix[i][j][k] << " ";
                }
                std::cout << "}";
            }
            else if (j == 2)
            {
                for (size_t k = 0; k < finishMatrix[i][j].size(); k++)
                {
                    std::cout << finishMatrix[i][j][k] << " ";
                }
                std::cout << "}";
            }
        }
        std::cout << "\n";
    }

    system("pause");
}

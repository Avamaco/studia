#include <cstdio>
#include <vector>

int main()
{
    std::vector<int> v;
    for (int i = 0; i < 13; i++)
        v.push_back(i * 20 + 10);

    for (int i = 0; i < 10; i++)
        printf("%d\n" ,v[i]);

    int czy_to_liczba = v.end() - v.begin();
    printf("\n%d\n", czy_to_liczba);
    return 0;
}

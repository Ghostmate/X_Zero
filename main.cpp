#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>

#include "lang.h"

//const float fSkipCheck[]{.35,.15,.1};

const int iPriority[]{1,4,1,3};

int iComp{0};

typedef std::vector<std::vector<int>> field_x;

struct Sum {
    void operator()(int n) { sum +=n;}
    int sum{0};
};

enum eLocale {
    eng = 0,
    ru = 1,
    fr = 2
};

enum eField {
    basic = 3
};

enum eCon {
    x_o = 3
};

int iCheckWinningMove(field_x,int,int);

void vCalcMove(const field_x &, field_x &);

inline int iMax(int a, int b) {
    return (a>b)?a:b;
};

//int iCheckLosingMove();

int iCheckCoordinates(field_x);

int main(void) {
    srand(time(0));
    //int iLocale = eng;
    field_x iField{{2,1,1},{1,2,1},{1,1,1}};
    /*do {
  int iA;
  std::cout << sIntroduction[iLocale];
  std::cin >> iA;
  switch(iA)
  {
    case 0:
    break;
    case 1:
    break;
  }

  } while(1);*/
    field_x vChoices;
    vCalcMove(iField, vChoices);

    std::cout << vChoices.at(0).at(0) << std::endl << vChoices.at(0).at(1);

}

void vReturnWeight(const field_x &lField,int x,int y, int *iWeight) {
    //field_x field[3][3]{(field_x)lField};
    iWeight[0] = 0;
    iWeight[1] = 0;
    iWeight[2] = 0;
    iWeight[3] = 0;
    iWeight[4] = 0;
    iWeight[5] = 0;
    if(x==y) {
        int k[3]{0,0,0};
        for(int i = 0; i < basic; ++i) {
            k[0] += (lField[i][i] == 1);
            k[1] += (lField[i][i] == 2);
            k[2] += (lField[i][i] == 4);
        }
        iWeight[0] = iMax(iWeight[0],k[2]);
        iWeight[1] = iMax(iWeight[1],k[1]);
        iWeight[2] = iMax(iWeight[2],k[0]);
        iWeight[3] += k[2];
        iWeight[4] += k[1];
        iWeight[5] += k[0];
    }
    if(x == basic - y - 1) {
        int k[3]{0,0,0};
        for(int i = 0; i < basic; ++i) {
            k[0] += (lField.at(basic - i - 1).at(i) == 1);
            k[1] += (lField.at(basic - i - 1).at(i) == 2);
            k[2] += (lField.at(basic - i - 1).at(i) == 4);
        }
        iWeight[0] = iMax(iWeight[0],k[2]);
        iWeight[1] = iMax(iWeight[1],k[1]);
        iWeight[2] = iMax(iWeight[2],k[0]);
        iWeight[3] += k[2];
        iWeight[4] += k[1];
        iWeight[5] += k[0];
    }
    {
        int k[3]{0,0,0};
        for(int i = 0; i < basic; ++i) {
            k[0] += lField[x][i] == 1;
            k[1] += lField[x][i] == 2;
            k[2] += lField[x][i] == 4;
        }
        iWeight[0] = iMax(iWeight[0],k[2]);
        iWeight[1] = iMax(iWeight[1],k[1]);
        iWeight[2] = iMax(iWeight[2],k[0]);
        iWeight[3] += k[2];
        iWeight[4] += k[1];
        iWeight[5] += k[0];
    }
    {
        int k[3]{0,0,0};
        for(int i = 0; i < basic; ++i) {
            k[0] += lField[i][y] == 1;
            k[1] += lField[i][y] == 2;
            k[2] += lField[i][y] == 4;
        }
        iWeight[0] = iMax(iWeight[0],k[2]);
        iWeight[1] = iMax(iWeight[1],k[1]);
        iWeight[2] = iMax(iWeight[2],k[0]);
        iWeight[3] += k[2];
        iWeight[4] += k[1];
        iWeight[5] += k[0];
    }

};

void vCalcMove(const field_x &lField, field_x &vChoices) {
    vChoices.clear();
    std::vector<std::vector<int>> vWeighted;
    for(int x = 0; x < basic; ++x) {
        for(int y = 0; y < basic; ++y) {
            if(lField[x][y] == 1) {
                int iWeight[6]{0,0,0,0,0,0};
                vReturnWeight(lField, x, y, iWeight);
                std::vector<int> vWeight{x,y,iWeight[0],iWeight[1],iWeight[2],iWeight[3],iWeight[4],iWeight[5]};
                vWeighted.push_back(vWeight);
            }
        }
    }

    for(int i = 0; (i < 2); ++i)  {
        int maxVal{0};
        for(auto k:vWeighted) {
            maxVal = iMax(maxVal,k[i+2]);
        }
        if(maxVal==2 && i < 2) {

            auto coord = vWeighted.begin();

            while(coord != vWeighted.end()) {
                if((*coord).at(i+2)==2) vChoices.push_back({(*coord).at(0),(*coord).at(1), iPriority[iComp + i]});
                coord = std::find_if(coord + 1,vWeighted.end(),[i](std::vector<int> coor){return (coor.at(i+2)==2);});

            } ;
        }
    }

    if (vChoices.size() == 0) {
        for(auto k:vWeighted) {
            vChoices.push_back({k[0],k[1], iPriority[3]*k[7]*k[7] + iPriority[iComp]*k[5]*k[5] + iPriority[iComp + 1]*k[6]*k[6]});
        }
    }

    if(vChoices.size() != 0) {
        int iPMax{0};
        int iPFull{0};
        for(auto k:vChoices) {
            iPMax = iMax(k[2],iPMax);
        }

        for_each(vChoices.begin(),vChoices.end(),[&iPFull, iPMax](std::vector<int> k ){ iPFull += (iPMax <= 1.5 * k[2])?k[2]:0;});

        int iPk{rand()%(iPFull+1)};

        for(auto k:vChoices) {
            if(iPk > 0 && iPMax <= 1.5 * k[2]) {
                iPk -= k[2];
                if(iPk <= 0) {
                    vChoices.at(0)[0]=k[0];
                    vChoices.at(0)[1]=k[1];
                }
            }
        }
    }

    if(vChoices.size() > 1)
        vChoices.erase(vChoices.begin() + 1,vChoices.end());

};



/*
int iCheckWinningMove(int lField[basic][basic],int x,int y) {
  uint iResult{0};

  for(int i = 0; i < basic; ++i) {
    if(x==y || x == basic - y) {
      iResult += lField[i][i];
      iResult += lField[basic - i][i] << 4;
      }
    iResult += lField[x][i]<<(4 * i + 8);
    iResult += lField[i][y]<<(4 * i + 20);
  }

  for(int i = 0; i < 8; ++i) {
    if((iResult >> (4 * i)) & 10)
    return ((iResult >> (4 * i)) & 15);
  }
  return 0;
};

int iCheckCoordinates(int lField[basic][basic]) {
  int iResult{0};
  for(int i = 0; i < basic; ++i) {
    iResult = iCheckWinningMove(lField, i, i);
    if((iResult & 3) == 3)
      return 1;
    if((iResult & 12) == 12)
      return 2;
    }
  return 0;
};

*/

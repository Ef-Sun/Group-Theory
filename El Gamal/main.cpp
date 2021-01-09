#include <iostream>
#include <vector>
#include <climits>

using namespace std;
const int INFN = INT_MAX;
const int MOD = 11;
const int s = 5;
const int r = 7;
const int m = 3;
const int GENERARTOR = 11;
const pair<int, int> P(2, 7);
vector<pair<int, int>> points;

/*
 定义域内计算：+ - * ^{-1} /
 */
int modPlus(int a, int b) {
    return (a + b) % MOD;
}

int modMinus(int a, int b) {
    int result = (a % MOD - b % MOD + MOD) % MOD;
    return result;
}

int modMul(int a, int b) {
    return (a * b) % MOD;
}

int calInv(int a) {
    for (int i = 0; i < MOD; i++){
        if (modMul(a, i) == 1) {
            return i;
        }
    }
    return -1;
}

int modDiv(int a, int b) {
    int invB = calInv(b);
    if (invB != -1) {
        return modMul(a, invB);
    }
    return -1;
}


/*
 获得所有椭圆曲线上的点并打印
 */
void getAllPoints(int generator){
    for (int i = 0; i < generator; i++) {
        int x = i;
        for (int j = 0; j < generator; j++) {
            int y = j*j % generator;
            if (y == (x*x*x + x +6) % generator) {
                y = j;
                pair<int, int> t(x, y);
                points.push_back(t);
                cout << "(" << x << "," << y << ")" << endl;
            }
        }
    }
    cout << "O(无穷远点)" << endl;
}

/*
 计算斜率：要求p1和p2横坐标不相等；作为切点则 y!= 0
*/
int calSlope(pair<int, int>p1, pair<int, int>p2){
    int slope;
    if (p1 != p2) {
        if (p1.first == p2.first)
            slope = INFN;
        else
            slope = modDiv(modMinus(p2.second, p1.second), modMinus(p2.first, p1.first));
    }
    else {
        if (p1.second != 0)
            slope = modDiv(modPlus(modMul(modMul(p1.first, p1.first), 3), 1), modMul(2, p1.second));
        else slope = INFN;
    }
    return slope;
}

/*
 切线求第三个交点
 */
pair<int, int> tangenetLine(pair<int, int> p) {
    int k = calSlope(p, p);
        for (auto pp: points) {
            if (pp == p)
                continue;
            else if (calSlope(pp, p) == k) {
                return {pp.first, MOD - pp.second};
            }
        }

    return {INFN, INFN};
}


/*
 割线求第三个交点
 */
pair<int, int> SecantLine(pair<int, int> p1, pair<int, int> p2){
    int k = calSlope(p1, p2);
    for (auto pp: points) {
        if (pp == p1 || pp == p2)
            continue;
        else if(calSlope(p1, pp) == k){
            return {pp.first, MOD - pp.second};
        }
    }
    if (calSlope(p1, p1) == k)
        return p1;
    else
        return p2;
}


/*
 计算p1+p2
 */
pair<int, int> calPlus(pair<int, int>p1, pair<int, int>p2){
    pair<int, int> resultPoint;
    // 无穷远点
    if (p1.first == INFN)
        return p2;
    else if (p2.first  == INFN)
        return p1;
    // p1、p2相等，计算切线
    else if (p1 == p2){
        resultPoint = tangenetLine(p1);
    }
    // p1、2不等，计算割线
    else {
        resultPoint = SecantLine(p1, p2);
    }
    return resultPoint;
}

/*
 计算公钥pub = nP
 */
pair<int, int> calNp(int x, pair<int, int> pk){
    pair<int, int> pub = pair<int, int>(INFN, INFN);
    for (int i = 0; i < x; i ++) {
        pub = calPlus(pub, pk);
    }
    return pub;
}

class c{
public:
    int x;
    int y;
    int ct;
    c(int x1, int y1, int c1) {
        this->x = x1;
        this->y = y1;
        this->ct = c1;
    }
};

/*
 加密信息m
*/
c encryption(pair<int, int> pkP, pair<int, int>pkQ, int m_, int r_){
    pair<int, int> p1 = calNp(r_, pkP);
    int x1 = p1.first;
    int y1 = p1.second;

    pair<int, int> p2 = calNp(r_, pkQ);
    int x2 = p2.first;
    int ct = modMul(m_, x2);
    return {x1, y1, ct};
}

/*
  解密密文c
 */
int decryption(int x, int y, int c){
    pair<int, int> p(x, y);
    p = calNp(s, p);
    return modMul(c, calInv(p.first));
}

int main(int argc, const char * argv[]) {
    cout << "E上的所有点为：" << endl;
    getAllPoints(GENERARTOR);
    pair<int, int> pkQ = calNp(s, P);

    cout << endl << "公钥：y^2 = x^3 + x +6, ";
    cout << "P(" << P.first << ", " << P.second << "), Q(" << pkQ.first << ", " << pkQ.second << ")" << endl;

    c cipher = encryption(P, pkQ, m, r);
    cout << endl << "密文:（" << cipher.x << ", " << cipher.y << ", "<< cipher.ct << ")" << endl;

    cout << endl << "明文：" << decryption(cipher.x, cipher.y, cipher.ct) << endl;
    return 0;
}


import cmath
import random
from math import gcd

M1 = int(15)
M2 = int(20)
MAXRANGE = int(1023)
MINRANGE = int(513)


# 利用素数筛获得小于根号x的所有素数
def getPrimeLessThan(x):
    sqrt_x = int((cmath.sqrt(x).real + 1))
    isPrime = []

    # 设定0
    isPrime.append(False)

    # 将所有偶数设定为非素数，奇数设定为素数
    for num in range(1, sqrt_x):

        if num == 1:
            isPrime.append(False)
        elif num == 2:
            isPrime.append(True)
        elif num % 2 == 0:
            isPrime.append(False)
        else:
            isPrime.append(True)

    # 将已是素数的倍数设定为非素数 如3是素数，则9，15……均为非素数
    for num in range(3, sqrt_x):
        if isPrime[num]:
            for i in range(2 * num, sqrt_x, num):
                isPrime[i] = False

    return isPrime


# 判断p是否为素数
def isPrime(p):
    if p <= 0:
        return False
    if p == 1:
        return False
    if p == 2:
        return True

    isPrime = True
    primesLessThanSqrt_p = getPrimeLessThan(p)

    for num in range(2, len(primesLessThanSqrt_p)):
        if primesLessThanSqrt_p[num] == True and gcd(p, num) > 1:
            isPrime = False
            break

    return isPrime


# 获取(min, max)内的随机素数
def getPrime(min: int, max: int) -> int:
    notPrime = True
    while notPrime:
        x = random.randint(min, max)
        if isPrime(x):
            notPrime = False
    return x


# 计算base^index (mod module)
def calnTimesMod(base: int, index: int, module: int) -> int:
    ans = int(base) % module
    for i in range(1, index):
        ans = ans * base % module
    return ans


# 利用扩展的欧几里得算法求逆元
def getInv(a: int, mod: int) -> int:
    if a == 1:
        return a
    return getInv(mod % a, mod) * (mod - int(mod / a)) % mod


class Paillier:
    def __init__(self):
        self.p = 0
        self.q = 0
        while self.p == self.q or gcd(self.p, self.q) != 1:
            self.p = getPrime(MINRANGE, MAXRANGE)  # 私钥p
            self.q = getPrime(MINRANGE, MAXRANGE)  # 私钥q

        self.r = int((self.p - 1) * (self.q - 1))  # 私钥r = [p-1, q-1]

        self.n = int(self.p * self.q)  # 公钥n
        self.g = self.n + 1  # 公钥g
        self.square = self.n * self.n

    def printPrivateKey(self):
        print("确认的私钥为：")
        print("p = %d, q = %d, r = %d" % (self.p, self.q, self.r))

    def printPublicKey(self):
        print("确认的公钥为：")
        print("n = %d, g = %d" % (self.n, self.g))

    def encryption(self, m):
        r = random.randint(1, self.n)
        while gcd(r, self.n) != 1:
            r = random.randint(1, self.n)
        c = calnTimesMod(r, self.n, self.square) * calnTimesMod(self.g, m, self.square) % self.square
        return c

    def decryption(self, c):
        up = int((calnTimesMod(c, self.r, self.square) - 1) / self.n)
        bottom = int((calnTimesMod(self.g, self.r, self.square) - 1) / self.n)
        bottom = getInv(bottom, self.n) % self.n
        return up * bottom % self.n


if __name__ == '__main__':
    paillier = Paillier()
    paillier.printPublicKey()
    paillier.printPrivateKey()
    c1 = paillier.encryption(M1)
    c2 = paillier.encryption(M2)
    m1 = paillier.decryption(c1)
    m2 = paillier.decryption(c2)
    m3 = paillier.decryption(c1 * c2)
    print("m1的加密结果为：%d" % c1)
    print("m2的加密结果为：%d" % c2)
    print("c1的解密结果为：%d" % m1)
    print("c2的解密结果为：%d" % m2)
    print("c1*c2的解密结果为：%d" % m3)

# get the script path
import os
import configparser
from math import factorial

# read file in ../simulations/cloudSystem.ini
config = configparser.ConfigParser()
config.read(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../simulations/cloudSystem.ini'))

# get all [Config **] sections
print(config.sections())

def f(Nvm, u):
    s = 0
    for n in range(Nvm, 9999999):
        s += (1-u)*u**n * (n-Nvm)
    return s

for c in config.sections():
    if not c.startswith('Config'):
        continue

    if config[c]['*.*_type'] != '1':
        continue


    # get parameter *.T
    T = float(config[c]['*.T'].replace('s', ''))
    Nvm = int(config[c]['*.N'])
    I = float(config[c]['*.I'])
    R_ = float(config[c]['*.R'])
    p = float(config[c]['*.p'])
    S = float(config[c]['*.S'].replace('s', ''))

    print('T:', T)
    print('Nvm:', Nvm)
    print('I:', I)
    print('R:', R_)

    for mode in ["fairshare", "segregation", "backed"]:
        if mode == "fairshare":
            lamda = 1 / T
            mu = R_ / I
            u = lamda / mu
            p0 = 1-u
            rho = 0
 
            part1 =  u * (1 - Nvm*u**(Nvm-1) + (Nvm-1)*u**Nvm) / (1 - u)
            part2 =  Nvm*u**Nvm
            N = u / (1 - u)
            Nq = N - part1 - part2
            W = Nq / lamda
            R = N / lamda
            c = part1 + part2
        elif mode == "segregation":
            # Segregation
            lamda = 1 / T
            mu = R_ / (I * Nvm)
            rho = lamda * I / R_
            u = lamda / mu

            p0 = 1 / (sum([u**k / factorial(k) for k in range(Nvm)]) + u**Nvm / factorial(Nvm) / (1 - rho))
            
            Nq = u**Nvm / factorial(Nvm) * p0 * rho / ((1 - rho)**2)
            N = Nq + u
            W = Nq / lamda
            R = W + 1 / mu
            c = u
        elif mode == "backed":
            lamda = 1 / T * p
            mu = S
            rho = lamda / mu
            u = rho
            p0 = 1 - rho

            N = rho / (1 - rho)
            Nq = N - rho
            W = Nq / lamda
            R = N / lamda
            c = rho

        print(f'\n\n{mode}:\n')
        print('lamda:', lamda)
        print('rho:', rho)
        print('u:', u)
        print('p0:', p0)
        print()
        print('Nq:', Nq)
        print('N:', N)
        print('W:', W)
        print('R:', R)
        print('c:', c)

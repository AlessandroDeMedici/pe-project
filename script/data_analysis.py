import sys
import matplotlib
import matplotlib.pyplot as plt
import json
import random
from scipy.stats import norm

random.seed(42)

if len(sys.argv) != 3:
    print('Usage: python autocorrelation.py <json_file> <stat>')
    sys.exit(1)

json_file = sys.argv[1]
stat = sys.argv[2]

f = open(json_file, 'r')
data = json.load(f)

print("data loaded")

repetitions = {}

for repetition in data:
    for vector in data[repetition]['vectors']:
        if vector['name'] != stat:
            continue
        
        if data[repetition]['itervars']['distr'] != '1' or data[repetition]['itervars']['fairSharing'] != 'true':
            continue

        repetitions[int(data[repetition]['itervars']['N'])] = vector['value']
        break

print("repetitions loaded")

p = [2 for i in range(len(repetitions))]
autocorrelated = [True for i in range(len(repetitions))]

alpha = 0.05

for rep in range(100, 0, -1):
    rep_0 = rep-1

    while autocorrelated[rep_0]:
        p[rep_0] /= 1.5

        if p[rep_0] < 2**(-15):
            exit(f"error at {rep}")

        samples = []
        for s in repetitions[rep]:
            if random.random() < p[rep_0]:
                samples.append(s)

        print(rep, 'p:', p[rep_0])
        print('Original samples:', len(repetitions[rep]))
        print('Samples:', len(samples))

        autocorrelation = []

        X_avg = sum(samples) / len(samples)
        S2 = sum([(x - X_avg) ** 2 for x in samples]) / (len(samples) - 1)

        for i in range(1, min(100, len(samples)//5)):
            sum_ = 0
            for j in range(len(samples) - i):
                sum_ += (samples[j] - X_avg) * (samples[j + i] - X_avg)
            autocorrelation.append(sum_ / ((len(samples) - i) * S2))

        z = norm.ppf(1 - alpha/2)
        confidence = z / (len(samples) ** 0.5)

        out_of_bounds = 0
        outliers = 0
        
        for i in range(len(autocorrelation)):
            if abs(autocorrelation[i]) > confidence:
                out_of_bounds += 1
                outliers += abs(autocorrelation[i])-confidence

        if out_of_bounds < len(autocorrelation) * alpha * 1.3 and outliers < confidence:
            autocorrelated[rep_0] = False

        # plot autocorrelation and confidence interval
        #plt.plot(autocorrelation)
        #plt.axhline(confidence, color='r', linestyle='dashed')
        #plt.axhline(-confidence, color='r', linestyle='dashed')
        #plt.show()

print(autocorrelated)
print(p)
[print(1/p[i]) for i in range(len(p))]

N = [i for i in range(1, len(repetitions)+1)]
average = []
confidence = []

alpha = 0.05
z = norm.ppf(1 - alpha/2)

for n in N:
    X = sum(repetitions[n]) / len(repetitions[n])
    S = (sum([(x - X) ** 2 for x in repetitions[n]]) / (len(repetitions[n]) - 1)) ** 0.5

    average.append(X)
    confidence.append(z * S / (len(repetitions[n]) ** 0.5))

plt.errorbar(N, average, yerr=confidence, fmt='o')
plt.xlabel('N')
plt.ylabel(stat)

#set y to start at 0
plt.ylim(bottom=0)
plt.show()




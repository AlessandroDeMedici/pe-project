import sys
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

original_samples = []

# dovrebbe essere una sola ripetizione
for repetition in data:
    for vector in data[repetition]['vectors']:
        if vector['name'] != stat:
            continue

        original_samples = vector['value']
        break

p = 1/256

samples = []
for s in original_samples:
    if random.random() < p:
        samples.append(s)

print('Original samples:', len(original_samples))
print('Samples:', len(samples))

autocorrelation = []

X_avg = sum(samples) / len(samples)
S2 = sum([(x - X_avg) ** 2 for x in samples]) / (len(samples) - 1)

for i in range(0, min(100, len(samples)//3)):
    sum_ = 0
    for j in range(len(samples) - i):
        sum_ += (samples[j] - X_avg) * (samples[j + i] - X_avg)
    autocorrelation.append(sum_ / ((len(samples) - i) * S2))

# confidence
alpha = 0.05
# get z_alpha/2 value for 95% confidence on a gaussian distribution, based on alpha
z = norm.ppf(alpha / 2)

# confidence interval
confidence_interval1 = [z / (len(samples) ** 0.5) for i in range(len(autocorrelation)+1)]
confidence_interval2 = [-z / (len(samples) ** 0.5) for i in range(len(autocorrelation)+1)]

# plot with dots small
plt.plot(autocorrelation, 'bo', markersize=2)
plt.plot(confidence_interval1, 'r')
plt.plot(confidence_interval2, 'r')
plt.xlabel('Lag')
plt.ylabel('Autocorrelation')
plt.title('Autocorrelation of ' + stat)
plt.grid()
plt.minorticks_on()
plt.grid(which='major', linestyle='-', linewidth='0.5', color='black')
plt.grid(which='minor', linestyle=':', linewidth='0.2', color='black')
#set x line from 0 to len(autocorrelation)
plt.xlim(0, len(autocorrelation))
plt.show()



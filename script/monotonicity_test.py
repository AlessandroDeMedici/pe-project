import sys
import json
import matplotlib.pyplot as plt

stat = sys.argv[2]

parameter_names = ['T (s)', 'N', 'I', 'R (1/s)', 'S (s)', 'p']

x_functions = [lambda x: x/100 + 1.01, lambda x: x, lambda x: x*14, lambda x: x*15+1980, lambda x: x/34.5, lambda x: x/100]

json_file = sys.argv[1]
with open(json_file) as f:
    data = json.load(f)

result = {}

for test in data:
    sim = data[test]

    sliding_parameter = int(sim['itervars']['variable'])
    x = int(sim['itervars']['range'])

    for scalar in sim['scalars']:
        if scalar['name'] == stat: # 'cloudNodeStatNq:timeavg':#'taskTimeStat:mean':
            y = scalar['value']
            if not sliding_parameter in result:
                result[sliding_parameter] = {}
            result[sliding_parameter][x] = y
            break

for sliding_parameter in result:
    # sort result[sliding_parameter] by x
    result[sliding_parameter] = dict(sorted(result[sliding_parameter].items()))

#plot



# plot each sliding parameter in a different plot (but in the same figure)
# plor 3 x 2

# add spacing between plots

fig, ax = plt.subplots(3, 2, figsize=(15, 5))
# add spacing between plots horizontally
for sliding_parameter in result:
    x = list(result[sliding_parameter].keys())
    y = list(result[sliding_parameter].values())
    x = [x_functions[sliding_parameter-1](i) for i in x]
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].plot(x, y)
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].set_xlabel(parameter_names[sliding_parameter-1])
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].set_ylabel('taskTimeStat (s)')
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].grid()
#    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].set_ylim(bottom=0)
#    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].set_xlim(left=0)

    # add minor ticks
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].minorticks_on()

    # add minor ticks grid
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].grid(which='minor', linestyle=':', linewidth='0.2', color='black')

plt.grid(True)
plt.tight_layout()
plt.subplots_adjust(wspace=0.2)
plt.show()



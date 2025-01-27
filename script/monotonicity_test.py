import sys
import json
import matplotlib.pyplot as plt

parameter_names = ['T', 'N', 'I', 'R', 'S', 'p']

json_file = sys.argv[1]
with open(json_file) as f:
    data = json.load(f)

result = {}

for test in data:
    sim = data[test]

    sliding_parameter = int(sim['itervars']['variable'])
    x = int(sim['itervars']['range'])

    for scalar in sim['scalars']:
        if scalar['name'] == 'taskTimeStat:mean':
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
fig.tight_layout(pad=2.0)
for sliding_parameter in result:
    x = list(result[sliding_parameter].keys())
    y = list(result[sliding_parameter].values())
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].plot(x, y)
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].set_xlabel(parameter_names[sliding_parameter-1])
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].set_ylabel('taskTimeStat (s)')
    ax[(sliding_parameter-1)//2, (sliding_parameter-1)%2].grid()

plt.show()



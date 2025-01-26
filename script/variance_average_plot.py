import sys
import matplotlib.pyplot as plt
import json

if len(sys.argv) != 3:
    print('Usage: python variance_average_plot.py <json_file> <stats_name>')
    sys.exit(1)

stat = sys.argv[2]
json_file = sys.argv[1]

f = open(json_file, 'r')
data = json.load(f)

print(f"Found {len(data)} repetitions")

time = []
stat_data = []

for repetition in data:
    for vector in data[repetition]['vectors']:
        if vector['name'] != stat:
            continue

        time.append(vector['time'])
        stat_data.append(vector['value'])

resampled_time = []

for i in range(len(time)):
    resampled_time += time[i]

resampled_time.sort()

resampled_stat_data = [[] for i in range(len(stat_data))]
variance = []
average = []
moving_average = []

# resample data as a step function
for i in range(len(stat_data)):
    j = 0
    for t in resampled_time:
        while j < len(stat_data[i]) - 1 and t >= time[i][j]:
            j += 1
        resampled_stat_data[i].append(stat_data[i][j])

print(f"Resampled data with {len(resampled_time)} points")

for i in range(len(resampled_time)):
    average.append(sum([resampled_stat_data[j][i] for j in range(len(resampled_stat_data))]) / len(resampled_stat_data))

print(f"Computed average")

for i in range(len(resampled_time)):
    variance.append(sum([(resampled_stat_data[j][i] - average[i])**2 for j in range(len(resampled_stat_data))]) / len(resampled_stat_data))

print(f"Computed variance")

WINDOW_SIZE = 100 # in seconds
right_index = 0
for left_index in range(len(resampled_time) - WINDOW_SIZE):
    continue   
    # set right index to the last time <= time_window
    while right_index < len(resampled_time)-1 and resampled_time[right_index] - resampled_time[left_index] < WINDOW_SIZE:
        right_index += 1
    actual_window_size = resampled_time[right_index] - resampled_time[left_index]
    moving_average.append(sum(average[left_index:right_index]) / actual_window_size)
    
    #print(left_index, right_index, actual_window_size, end='\r')

print(f"Computed moving average with window size {WINDOW_SIZE} seconds")

# plot average with variance confidence interval at 95%
plt.plot(resampled_time, average, label='Average')
#plt.plot(resampled_time[:len(moving_average)], moving_average, label=f'Moving Average with window size {WINDOW_SIZE} seconds')
plt.fill_between(resampled_time, [average[i] - 1.96 * variance[i] for i in range(len(variance))], [average[i] + 1.96 * variance[i] for i in range(len(variance))], alpha=0.5, label='95% Confidence Interval')

# final expected value line
stabline = [average[-1] for i in range(len(resampled_time))]
plt.plot(resampled_time, stabline, label='Expected value', linestyle='--')

# set log x scale with lines
plt.xscale('log')
plt.grid(True, which="both", ls="--")

plt.xlabel('Time (s)')
plt.ylabel(stat)

#remove white space and make plot start to the left and end to the right
plt.xlim(left=min(resampled_time), right=max(resampled_time)+1)
plt.ylim(bottom=0)


# add a note about the number of repetitions (outisde the plot) like a title
plt.title(f"Simulation with {len(stat_data)} repetitions")

plt.legend()
plt.show()




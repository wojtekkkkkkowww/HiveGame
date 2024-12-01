import matplotlib.pyplot as plt

def process_file(file_path):
    result = []
    with open(file_path, 'r') as file:
        for line in file:
            numbers = list(map(int, line.split()))
            result.append(numbers)
    return result

data = process_file('branching_factor.txt')

epochs = len(data[0])
iterations = len(data)
branchingFactorSum = [sum(data[i][j] for j in range(epochs)) for i in range(iterations)]
print(len(branchingFactorSum))

average_branching_factors = [branchingFactorSum[i] / epochs for i in range(iterations)]
print(average_branching_factors)


global_average = sum(average_branching_factors) / iterations
print('Global average:', global_average)

global_max = max(data[i][j] for i in range(iterations) for j in range(epochs))
print('Global max:', global_max)

x = list(range(1, iterations + 1))
plt.figure(figsize=(10, 6))
plt.plot(x, average_branching_factors, label='Average Branching Factor', color='blue')


for i in x:
    for j in range(epochs):
     plt.scatter(i,data[i-1][j], color='red', s=10)

plt.xlabel('Move Number')
plt.ylabel('Branching Factor')
plt.title('Branching Factor per Move')
plt.legend()
plt.grid(True)
plt.savefig('branching_factor.pdf')
plt.show()


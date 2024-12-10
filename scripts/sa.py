import matplotlib.pyplot as plt

file_path = "cache.txt"

values = []

with open(file_path, 'r') as file:
    for line in file:
        try:
            last_value = float(line.split()[-1])
            values.append(last_value)
        except (ValueError, IndexError):
            continue

iterations = list(range(1, len(values) + 1))

plt.figure(figsize=(10, 6))
plt.scatter(iterations, values, color='b', label='SA Values')
plt.title("SA Values Over Iterations")
plt.xlabel("Iterations")
plt.ylabel("SA Value")
plt.grid(True)
plt.legend()
plt.savefig("sa_scatter_plot.pdf")  # Save the plot
plt.show()
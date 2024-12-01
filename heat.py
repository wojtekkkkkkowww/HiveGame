import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import Normalize
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.cm import ScalarMappable
from matplotlib.colors import LinearSegmentedColormap


def plot_3d_scatter(df, output_image="3d_scatter.png"):

    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')
    
    
    value_min, value_max = df['value'].min(), df['value'].max()
    value_normalized = Normalize(vmin=value_min, vmax=value_max)

    
    cmap = LinearSegmentedColormap.from_list("black_yellow", ["black", "red", "orange", "yellow"])

    
    sc = ax.scatter(df['a1'], df['a2'], df['a3'], c=df['value'], cmap=cmap, s=10, alpha=0.7)

    
    cbar = plt.colorbar(sc, ax=ax, label='Value')

    
    ax.set_xlabel('a1')
    ax.set_ylabel('a2')
    ax.set_zlabel('a3')
    ax.set_title('3D Heatmap')

    
    plt.savefig(output_image)
    print(f"3D plot saved as {output_image}")
    plt.show()


def plot_parallel_coordinates(df, arguments, output_image="parallel_coordinates.png"):
    
    all_min = df.iloc[:, :-1].min().min()  
    all_max = df.iloc[:, :-1].max().max()  
    normalized_df = (df.iloc[:, :-1] - all_min) / (all_max - all_min)  
    normalized_df['value'] = df['value']  

    
    value_min, value_max = df['value'].min(), df['value'].max()
    value_normalized = Normalize(vmin=value_min, vmax=value_max)

    
    fig, ax = plt.subplots(figsize=(12, 8))  

    
    cmap = LinearSegmentedColormap.from_list("black_yellow", ["black", "red", "orange", "yellow"])

    
    for _, row in normalized_df.iterrows():
        color = cmap(value_normalized(row['value']))  
        ax.plot(range(len(arguments)), row[:-1], color=color, alpha=0.7)  

    
    sm = ScalarMappable(cmap=cmap, norm=value_normalized)
    sm.set_array([])  
    fig.colorbar(sm, ax=ax, label='Value')  

    
    ax.set_xticks(range(len(arguments)))
    ax.set_xticklabels([f'a{i+1}' for i in range(len(arguments))], rotation=45)
    ax.set_xlabel('Arguments')
    ax.set_ylabel('Normalized Values')
    ax.set_title('Parallel Coordinates Plot')
    ax.grid(alpha=0.3)

    
    plt.savefig(output_image)
    print(f"Plot saved as {output_image}")
    plt.show()


def plot_data(file_path, output_image="plot.png"):
    
    try:
        df = pd.read_csv(file_path, sep=r'\s+', header=None)  
    except Exception as e:
        print(f"Error reading file: {e}")
        return

    
    *arguments, value = df.columns
    df.columns = [f'a{i+1}' for i in range(len(arguments))] + ['value']

    
    if len(arguments) == 3:
        
        plot_3d_scatter(df, output_image)
    else:
        
        plot_parallel_coordinates(df, arguments, output_image)



file_path = "cache.txt"  
plot_data(file_path, output_image="output_plot.pdf")

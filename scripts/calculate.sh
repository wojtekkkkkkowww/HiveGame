#!/bin/bash

# Initialize total time and total moves
total_time=0
total_moves=0

# File to save results for plotting
results_file="game_results.csv"

# Header for results file
echo "Game,Time,Moves,TimePerMove" > "$results_file"

# Number of games to play
num_games=10

# Play the games
for ((i=1; i<=num_games; i++))
do
    echo "Running game $i..."

    # Use a temporary file to capture the output
    temp_file=$(mktemp)
    
    # Measure execution time and redirect output to temp file
    exec_time=$( { /usr/bin/time -f "%e" ./build/ai_tests/AiTester alphabeta alphabeta > "$temp_file"; } 2>&1 )

    # Extract the number of moves from the temp file using regex
    num_moves=$(grep -oP "movesCounter:\s*\K\d+" "$temp_file")
    if [[ -z "$num_moves" ]]; then
        echo "Error: Unable to extract the number of moves for game $i"
        num_moves=0
    fi
    
    # Extract the game time from the temp file using regex
    game_time=$(grep -oP "Game Time\s*=\s*\K\d+" "$temp_file")
    if [[ -z "$game_time" ]]; then
        echo "Error: Unable to extract the game time for game $i"
        game_time=0
    fi

    echo "Number of moves: $num_moves"
    echo "Game time (raw): $game_time"

    # Convert game time from nanoseconds to seconds
    game_time=$(echo "scale=6; $game_time / 1000000000" | bc)

    # Add to totals
    total_time=$(echo "$total_time + $game_time" | bc)
    total_moves=$(echo "$total_moves + $num_moves" | bc)

    # Calculate time per move
    time_per_move=$(echo "scale=6; $game_time / $num_moves" | bc)

    # Save results for this game
    echo "$i,$game_time,$num_moves,$time_per_move" >> "$results_file"

    echo "Game $i: Time = $game_time seconds, Moves = $num_moves, Time/Move = $time_per_move seconds"

    # Clean up the temporary file
    rm "$temp_file"
done

avg_time_per_move=$(echo "scale=6; $total_time / $total_moves" | bc)
echo "Average Time Per Move: $avg_time_per_move seconds"

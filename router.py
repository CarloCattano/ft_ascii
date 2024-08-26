import os
import time
import random as Random

fifo_in = "/tmp/pong_in"
fifo_out = "/tmp/pong_out"

# Open the named pipes
with open(fifo_in, 'w') as pipe_out, open(fifo_out, 'r') as pipe_in:
    try:
        while True:
            # Simulate sending ball and player2 positions to C program
            ball_x, ball_y, player2_y = 10, 10, 10


            positions = f"{ball_x} {ball_y} {player2_y}\n"
            pipe_out.write(positions)
            pipe_out.flush()  # Ensure the data is sent immediately

            # Receive player1's position from C program
            player1_pos = pipe_in.readline().strip()
            if player1_pos:
                print(f"Received: {player1_pos}")

    except KeyboardInterrupt:
        print("Terminated.")


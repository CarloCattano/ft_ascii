import os
import time, random as Random

fifo_in = "/tmp/pong_in"
fifo_out = "/tmp/pong_out"

# Open the named pipes
with open(fifo_in, 'w') as pipe_out, open(fifo_out, 'r') as pipe_in:
    try:
        while True:
            # # Simulate sending ball and player2 positions to C program
            # ball_x, ball_y, player2_x, player2_y = 10, 10, 70, 10
            #
            # # randomize ball position
            # ball_x += Random.randint(3, 50)
            # ball_y += Random.randint(3, 50)
            #
            # positions = f"{ball_x} {ball_y} {player2_x} {player2_y}\n"
            # pipe_out.write(positions)
            # pipe_out.flush()

            # Receive player1's position from C program
            print("pipe_in.readline()", pipe_in.readline())
            player1_pos = pipe_in.readline().strip()
            if player1_pos:
                print(f"Received: {player1_pos}")
            # Sleep for a short while before the next update
            time.sleep(1)

    except KeyboardInterrupt:
        print("Terminated.")


import os
import time
import random as Random

fifo_in = "/tmp/pong_in"
fifo_out = "/tmp/pong_out"

# simulate the ball bouncing around for testing 
ball_x, ball_y = 10, 10
ball_dx, ball_dy = 1, 1

# on each frame update the ball position
def update_ball_position():
    global ball_x, ball_y, ball_dx, ball_dy
    ball_x += ball_dx
    ball_y += ball_dy

    # Bounce the ball off the walls
    if ball_x == 0 or ball_x == 20:
        ball_dx *= -1
    if ball_y == 0 or ball_y == 20:
        ball_dy *= -1

    # Check if the ball went out of bounds
    if ball_x == 0 or ball_x == 20:
        ball_x, ball_y = 10, 10
        ball_dx = -ball_dx
        ball_dy = Random.choice([-1, 1])

def movePlayer2():
    global player2_y
    
    # move up and down with sin
    player2_y = int(10 + 5 * (time.time() % 1.0)) 
    

# Open the named pipes
with open(fifo_in, 'w') as pipe_out, open(fifo_out, 'r') as pipe_in:
    try:
        while True:
            # Simulate sending ball and player2 positions to C program
            # ball_x, ball_y, player2_y = 10, 10, 10
            
            player2_y = 10
            update_ball_position()
            movePlayer2()

            positions = f"{ball_x} {ball_y} {player2_y}\n"
            pipe_out.write(positions)
            pipe_out.flush()  # Ensure the data is sent immediately

            # Receive player1's position from C program
            player1_pos = pipe_in.readline().strip()
            if player1_pos:
                print(f"Received: {player1_pos}")

    except KeyboardInterrupt:
        print("Terminated.")


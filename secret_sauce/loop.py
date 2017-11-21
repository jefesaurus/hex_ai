import random

import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from secret_sauce.hex_env import HexEnv
from secret_sauce.learner import Learner

init = tf.initialize_all_variables()

# Set learning parameters
y = .99
e = 0.1
num_episodes = 2000
BOARD_SIZE = 11

# create lists to contain total rewards and steps per episode
game_lengths = []

with tf.Session() as sess:
    sess.run(init)
    env = HexEnv(BOARD_SIZE)
    agent = Learner()

    for i in range(num_episodes):
        # Reset environment and get first new observation
        game_length = 0

        while game_state.Winner() is not None:
            to_play = env.to_play()
            game_length += 1

            # Choose an action by greedily (with e chance of random action)
            # from the Q-network
            current_state = Learner.convert_state(env)
            action, prob_dist = agent.exploration_policy(sess, current_state)

            # Get new state and reward from environment
            env.make_move(action)
            next_state = Learner.convert_state(env)
            reward = 0
            if env.winner() is to_play:
                reward = 1
            elif env.winner() is HexEnv.flip_player(to_play):
                reward = -1

            # TODO flip board
            agent.update_memory(current_state, action, next_state, reward)

            # If there are enough moves in the history. update the network
            if (agent.has_history(batch_size)):
                # TODO - implement this, please (:O
                # batch_states := batch_size number of states of shape
                #   (batch_size, board_size, board_size, 3)
                # transformed_rewards := weirdness of modifying the rewards
                #   to not always be binary values, see here:
                #   https://github.com/kenjyoung/Hexplorer/blob/master/learner.py#L333-L341
                batch_states, transformed_rewards = agent.draw_memory(
                        batch_size)

                agent.learn(sess, batch_states, transformed_rewards)

        game_lengths.append(game_length)


print "Percent of succesful episodes: " + str(sum(rList)/num_episodes) + "%"`

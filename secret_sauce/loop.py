import numpy as np
import random
import tensorflow as tf
import matplotlib.pyplot as plt
from secret_sauce.learner import Learner
from secret_sauce.hex_env import HexEnv

init = tf.initialize_all_variables()

# Set learning parameters
y = .99
e = 0.1
num_episodes = 2000
BOARD_SIZE = 11

#create lists to contain total rewards and steps per episode
game_lengths = []

with tf.Session() as sess:
    sess.run(init)
    env = HexEnv(BOARD_SIZE)
    agent = Learner()
    
    for i in range(num_episodes):
        #Reset environment and get first new observation
        game_length = 0
        
        while game_state.Winner() != None:
            game_length += 1
            #Choose an action by greedily (with e chance of random action) from the Q-network
            action, prob_dist = agent.exploration_policy(sess, game_state)
                
            #Get new state and reward from environment
            next_game_state = env.make_move(action)
            terminal = 0
            if game_state.winner() is not None:
                terminal = 1
                
            # TODO flip board
            agent.update_memory(game_state, action, next_game_state, terminal)
            prob_dist_costs = agent.learn(sess, batch_size)

            
            #Obtain maxQ' and set our target value for chosen action.
            maxQ1 = np.max(Q1)
            targetQ = allQ
            targetQ[0,a[0]] = reward + y*maxQ1
            
            #Train our network using target and predicted Q values
            _,W1 = sess.run([updateModel,W],feed_dict={inputs1:np.identity(16)[game_state:game_state+1],nextQ:targetQ})
            game_state = next_game_state
                        if(costs is not None):
                Pw_cost, Qsigma_cost = costs
            else:
                Pw_cost = 0
                Qsigma_cost = 0

            #update running sums for this episode
            num_step += 1
            Pw_var_sum += np.mean(((1-Pw)*Pw)[np.logical_not(played)])
            Qsigma_sum += np.mean(Qsigma[np.logical_not(played)])
            Qsigma_cost_sum += Qsigma_cost
            Pw_cost_sum += Pw_cost
            
        game_lengths.append(game_length)

print "Percent of succesful episodes: " + str(sum(rList)/num_episodes) + "%"`

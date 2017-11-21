"""Let's train the model to play Hex!
"""
import random

import numpy as np
import tensorflow as tf
from secret_sauce.hex_env import HexEnv


class Learner(object):
    def __init__(self, board_size):
        self.create_layers(board_size)
        self.create_loss_function()

    def learn(self, sess, batch_state, transformed_rewards):
        sess.run([self.update_model],
                 feed_dict={
                     'input_layer': batch_state,
                     'train_labels': transformed_rewards
                 })

    def create_loss_function(self):
        """Defines the logic to update the neural network based on
        a gradient descent optimizer (RMS prop) applied to a set of
        predictions (self.final_layer) and the training reward labels
        (self.train_labels).
        """
        # Define loss function
        loss = tf.reduce_sum(tf.square(self.train_labels - self.final_layer))

        # Update the model
        trainer = tf.train.RMSPropOptimizer(learning_rate=1.0)
        self.update_model = trainer.minimize(loss)

        # TODO - print out loss for debugging

    def create_layers(self, image_size):
        """Initializes the layers in the network.
        """

        ###############################
        # Initializing parameters for the network

        # batch_size := # examples to bundle during training for efficiency
        self.batch_size = tf.placeholder(
                tf.int32,
                None,
                name='batch_size')

        # channels := [horizontal, vertical, to_play]
        num_channels = 3

        # num_filters :=
        num_filters = 128

        # num_layers := # layers in the neural network
        num_layers = 9

        # num_labels := # possible columns of labels, in our case just 1
        num_labels = 1

        # This is where training samples and labels are fed to the graph.
        # These placeholder nodes will be fed a batch of training data at each
        # training step, which we'll write once we define the graph structure.
        self.input_layer = tf.placeholder(
            tf.float32,
            shape=(self.batch_size, image_size, image_size, num_channels),
            name='input_layer')
        self.train_labels = tf.placeholder(tf.float32,
                                           shape=(self.batch_size, num_labels),
                                           name='train_labels')

        # Create initial convolutional layer + ReLU activation function
        conv1_weights = tf.layers.conv2d(
                inputs=self.input_layer,
                filters=num_filters,
                kernel_size=[5, 5],
                padding="same",
                activation=tf.nn.relu)  # TODO - switch to leaky ReLU

        self.layers = [
            self.input_layer,
            conv1_weights
        ]

        # Add (num_layers - 2) convolutional hidden layers + ReLU
        for layer_index in xrange(num_layers - 3):
            conv_weights = tf.layers.conv2d(
                    inputs=self.layers[-1],
                    filters=num_filters,
                    kernel_size=[3, 3],
                    padding="same",
                    activation=tf.nn.relu)  # TODO - switch to leaky ReLU
            self.layers.append(conv_weights)

        # NOTE: Example Hex paper seems to change the radius and padding
        # on the penultimate layer. Might be worth investigating?
        # https://github.com/kenjyoung/Hexplorer/blob/master/Q_learner/Q_learner.py#L134-L143
        self.layers.append(tf.layers.conv2d(
                inputs=self.layers[-1],
                filters=num_filters,
                kernel_size=[3, 3],
                padding="same",
                activation=tf.nn.relu))

        # Add final layer consisting of a sigmoid function
        final_layer = tf.layers.conv2d(
                inputs=self.layers[-1],
                filters=1,
                kernel_size=[1, 1],
                padding="same",
                activation=tf.sigmoid)
        self.layers.append(final_layer)
        self.final_layer = self.layers[-1]

    @staticmethod
    def rargmax(vector):
        """Argmax that chooses randomly among eligible maximum indices.

        @params: vector - 1-D array
        @return: index of vector of one of the maximum values in the vector
        """
        m = np.amax(vector)
        indices = np.nonzero(vector == m)[0]
        return random.choice(indices)

    @staticmethod
    def convert_state(env):
        """Converts the current game state in the HexEnv instance into a
        single game state that can be added to a batch.

        1. Reshapes from (3, 11, 11) --> (11, 11, 3)
        2. Sets all cells in the third channel to the player about to
           play (env.to_play)

        @params: env - HexEnv instance
        @return: np.array of shape (11, 11, 3)
        """
        state = env.game_state()
        out = np.zeros((env.board_size, env.board_size, 3), dtype=bool)
        out[:, :, 0] = state[0, :, :]
        out[:, :, 1] = state[1, :, :]
        out[:, :, 2] = np.full(out[:, :, 2].shape, env.to_play)
        return out

    def evaluate_prob_dist(self, sess, state):
        """Computes the probability distribution of winning (or possibly
        the estimated reward value) over all possible actions given a
        board game state. Computes using the convolutional neural network
        defined in create_layers()

        @params: sess - tensorflow session instance
                 state - batch of states, of shape
                         (batch_size, board_size, board_size, 3)
        @return: floats of shape (batch_size, board_size, board_size, 1)
        """
        feed_dict = {
            'input_layer': state,
            'batch_size': state.shape[0]
        }
        prob_output = sess.run([self.final_layer], feed_dict=feed_dict)
        return prob_output

    def exploration_policy(self, sess, env, epsilon=0.1):
        """Search for the next action given the current board state using an
        episilon greedy policy.

        The majority of the time, we will perform the greedy policy and select
        the action with the highest probability. For episilon percent of the
        time, we will explore and randomly select an unplayed action.

        @return: Tuple of next_action and probability distribution over all
            possible actions, where the probability distribution is
            np.array of shape (1, 11, 11, 1)
        """
        # Getting the board state
        state = Learner.convert_state(env)

        # Where someone decideds to jankily convert a single board state into
        # a batch_state, with batch_size = 1
        batch_shape = [1]
        batch_shape.extend(state.shape)
        batch_states = np.zeros(batch_shape)
        batch_states[0] = state

        prob_dist = self.evaluate_prob_dist(sess, batch_states)[0]
        played = np.logical_or(state[:, :, HexEnv.VERTICAL],
                               state[:, :, HexEnv.HORIZONTAL]).flatten()

        # Randomly select for epsilon amount
        if np.random.rand() < epsilon:
            action = np.random.choice(np.where(played == 0)[0])
            return action, prob_dist

        # Reshaping the probability distribution into a 1-D array for rargmax
        values = np.copy(prob_dist[0, :, :, :]).flatten()

        # Ensure that the agent never selects played values
        values[played] = -2

        # Select the action that has the highest probability
        action = Learner.rargmax(values)
        return action, prob_dist

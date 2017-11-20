"""Let's train the model to play Hex!
"""
import random

import numpy as np
import tensorflow as tf
from secret_sauce.hex_env import HexEnv


class Learner(object):
    def __init__(self):
        # TODO set up network
        """
        # @geoff - where did this come from...?
        self.inputs = tf.placeholder(shape=[1,16], dtype=tf.float32)
        W = tf.Variable(tf.random_uniform([16,4], 0, 0.01))
        self.prob_output_layer = tf.matmul(inputs1, W)
        self.max_action_layer = tf.argmax(prob_output_layer, 1)
        """

    def learn(self):
        """Updates the model given one set of predictions compared to the
        training labels.
        """
        # Predictions are the final layer's output?
        predictions = self.layers[-1]

        # Define loss function
        loss = tf.reduce_sum(tf.square(self.train_labels - predictions))

        # Update the model
        trainer = tf.train.RMSPropOptimizer(learning_rate=1.0)
        self.update_model = trainer.minimize(loss)

        # TODO - Evaluation metrics (for EVAL mode)

    def create_layers(self,
                      image_size=11):
        """Initializes the layers in the network.
        """

        ###############################
        # Initializing parameters for the network

        # batch_size := # examples to bundle during training for efficiency
        batch_size = 1

        # channels := [horizontal, vertical]
        num_channels = 2

        # num_filters :=
        num_filters = 128

        # num_layers := # layers in the neural network
        num_layers = 9

        # num_labels := # possible columns of labels, in our case just 1
        num_labels = 1

        # This is where training samples and labels are fed to the graph.
        # These placeholder nodes will be fed a batch of training data at each
        # training step, which we'll write once we define the graph structure.

        # TODO - where do we populate the placeholders..?
        input_layer = tf.placeholder(
            tf.float32,
            shape=(batch_size, image_size, image_size, num_channels))
        self.train_labels = tf.placeholder(tf.float32,
                                           shape=(batch_size, num_labels))

        # Create initial convolutional layer + ReLU activation function
        conv1_weights = tf.layers.conv2d(
                inputs=input_layer,
                filters=num_filters,
                kernel_size=[5, 5],
                padding="valid",
                activation=tf.nn.relu)  # TODO - switch to leaky ReLU

        self.layers = [
            input_layer,
            conv1_weights
        ]

        # Add (num_layers - 2) convolutional hidden layers + ReLU
        for layer_index in xrange(num_layers - 2):
            conv_weights = tf.layers.conv2d(
                    inputs=self.layers[-1],
                    filters=num_filters,
                    kernel_size=[3, 3],
                    padding="valid",
                    activation=tf.nn.relu)  # TODO - switch to leaky ReLU
            self.layers.append(conv_weights)

        # Add final layer consisting of a sigmoid function
        final_layer = tf.layers.conv2d(
                inputs=self.layers[-1],
                filters=1,
                kernel_size=[1, 1],
                padding="valid",
                activation=tf.sigmoid)
        self.layers.append(final_layer)

    @staticmethod
    def rargmax(vector):
        """ Argmax that chooses randomly among eligible maximum indices. """
        m = np.amax(vector)
        indices = np.nonzero(vector == m)[0]
        return random.choice(indices)

    @staticmethod
    def convert_state(env):
        """ Argmax that chooses randomly among eligible maximum indices. """
        state = env.game_state()
        out = np.zeros((3, board_size*board_size), dtype=bool)
        out[0, :] = state.(0, :, :).flatten()
        out[1, :] = state.(1, :, :).flatten()
        out[2, :] = np.full(out[2,:].shape(), state.to_play())
        return out

    def evaluate_prob_dist(self, sess, state):
        """Computes the probability distribution over all possible actions
        given a board game state.

        @return: list of floats
        """
        prob_output = sess.run([self.prob_output_layer],
                               feed_dict={self.inputs: state})
        return prob_output

    def exploration_policy(self, sess, state, epsilon=0.1):
        """Search for the next action given the current board state using an
        episilon greedy policy.

        The majority of the time, we will perform the greedy policy and select
        the action with the highest probability. For episilon percent of the
        time, we will explore and randomly select an unplayed action.

        @return: (next_action, probability distribution over all
            possible actions)
        """
        played = np.logical_or(state[HexEnv.VERTICAL],
                               state[HexEnv.HORIZONTAL]).flatten()

        # TODO - do we still need this?
        state = np.asarray(state, dtype=float)

        prob_dist = self.evaluate_prob_dist(sess, state)

        # Randomly select for epsilon amount
        if np.random.rand() < epsilon:
            action = np.random.choice(np.where(played == 0)[0])
            return action, prob_dist

        values = np.copy(prob_dist)
        # Ensure that the agent never selects played values
        values[played] = -2

        # Select the action that has the highest probability
        action = Learner.rargmax(values)
        return action, prob_dist

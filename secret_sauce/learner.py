"""Let's train the model to play Hex!
"""
import random
import copy
import os
import sys

import numpy as np
import tensorflow as tf
from secret_sauce.hex_env import HexEnv
from secret_sauce.memory import Memory


# State action pair into state, action, next_state, reward
# This should be done in the most easily accessible form because each stored
# observation is used many times.
class Observation:
    def __init__(self, env, action):
        env_copy = copy.deepcopy(env)
        # Store state before the action is applied
        self.start_state = Learner.convert_state(env_copy)

        # Store action
        self.action = action
        env_copy.make_move(action)
        self.end_state = Learner.convert_state(env_copy)

        # The reward yielded from this action applied to this state
        if env_copy.winner is not None:
          self.reward = 1
        else:
          self.reward = 0

def leaky_relu(x, alpha=.01):
    return tf.maximum(x, alpha * x) 

class Learner(object):
    def __init__(self, model_name, board_size, checkpoint_period=np.timedelta64(10,'s')):
        self.board_size = board_size
        self.graph = tf.Graph()
        with self.graph.as_default():
            self._create_layers()
            self._create_loss_function()
        self.sess = tf.Session(graph=self.graph)
        with self.graph.as_default():
            self.sess.run(tf.global_variables_initializer())
            self.saver = tf.train.Saver()
        self.memory = Memory(10000)
        self.model_name = model_name
        self.checkpoint_period = checkpoint_period
        self.last_checkpoint = np.datetime64('now')
        self.learn_count = 0

    def store_observation(self, start_env, action):
        self.memory.store(Observation(start_env, action))

    def _sample_memory(self, batch_size):
        board_batch_shape = (batch_size, self.board_size, self.board_size, 3)

        start_states = np.zeros(board_batch_shape, dtype='bool')
        actions = np.zeros((batch_size, 3), dtype='uint8')
        end_states = np.zeros(board_batch_shape, dtype='bool')
        rewards = np.zeros(batch_size, dtype='float32')
        for i, obs in enumerate(self.memory.sample(batch_size)):
            start_states[i] = obs.start_state
            actions[i, 0] = i 
            actions[i, 1] = obs.action // self.board_size
            actions[i, 2] = obs.action % self.board_size
            end_states[i] = obs.end_state
            rewards[i] = obs.reward

        return start_states, actions, end_states, rewards

    def learn(self, batch_size):
        if not self.memory.full():
          return
        
        start_states, actions, end_states, rewards = self._sample_memory(batch_size)

        # Q(start_state) <= reward(start_state, action) - max_a(Q(end_state))

        # Current network's estimate for previous states
        estimated_end_values = self.run_estimator(end_states)

        # Maximum from the next player's perspective, then negate to get the
        # estimated value from the current player's perspective
        max_next_state_values = np.max(estimated_end_values) * -1

        # Any rewards pulled from memory will be 1, and that is ground truth
        estimated_rewards = rewards 

        # For everything else, use the negation of the estimate for the next
        # state
        estimated_rewards[rewards == 0] = max_next_state_values

        _, train_loss, reg_loss  = self.sess.run([self.update_model, self.training_loss, self.regularization_loss],
                 feed_dict={
                     'input_layer:0': start_states,
                     'action_coords:0': actions,
                     'target_values:0': estimated_rewards 
                 })
        self.learn_count = self.learn_count + 1
        print train_loss, reg_loss 
        if np.datetime64('now') - self.last_checkpoint > self.checkpoint_period:
              self.checkpoint_model('/tmp/hex_models/')

    def model_id(self):
        return '%s_%s' % (self.learn_count, self.model_name)

    def checkpoint_model(self, checkpoint_dir):
        checkpoint_name = '%s.ckpt' % self.model_id() 
        full_path = os.path.join(checkpoint_dir, checkpoint_name)
        save_path = self.saver.save(self.sess, full_path)
        print('Model saved in file: %s' % full_path)
        self.last_checkpoint = np.datetime64('now')

    def load_model(self, full_path):
        # Unpack the model
        self.saver.restore(self.sess, full_path)

        # Set the learn count using the filename
        filename = os.path.splitext(os.path.basename(full_path))[0]
        learn_count = int(filename.split('_')[0])
        self.learn_count = learn_count

    def visualize_graph(self):
        writer = tf.summary.FileWriter("/tmp/tensorboard/", self.sess.graph)
        writer.close()

    def _create_loss_function(self):
        """Defines the logic to update the neural network based on
        a gradient descent optimizer (RMS prop) applied to a set of
        predictions (self.final_layer) and the training reward labels
        (self.train_labels).
        """

        # Only train on the delta in previous actions taken.
        # (batch_index, row, col)
        self.action_coords = tf.placeholder(tf.int32, shape=(None, 3), name='action_coords')

        # Apply actions_layer indices to get the values from the current network output

        # The idealized value of the positions in the batch
        self.target_values = tf.placeholder(tf.float32, shape=(None), name='target_values')

        # Current value of previously chosen action assessed by current network
        self.chosen_action_values = tf.gather_nd(tf.squeeze(self.final_layer, axis=3), self.action_coords)
        print self.final_layer.shape
        print self.action_coords.shape
        print self.chosen_action_values.shape

        self.regularization_loss = .01 * tf.reduce_sum(tf.get_collection(tf.GraphKeys.REGULARIZATION_LOSSES))
        self.training_loss = tf.losses.mean_squared_error(self.target_values, self.chosen_action_values)

        # Define loss function
        self.loss = self.training_loss + self.regularization_loss

        # Update the model
        trainer = tf.train.RMSPropOptimizer(learning_rate=.001, decay=.9)
        self.update_model = trainer.minimize(self.loss)

        # TODO - print out loss for debugging

    def _create_layers(self):
        """Initializes the layers in the network.
        """

        # batch_size := # examples to bundle during training for efficiency
        # None means dynamically sized
        batch_size = None

        # [horizontal, vertical, to_play]
        num_channels = 3

        # Number of kernels to learn
        num_filters = 128

        num_layers = 9

        num_labels = 1

        # This is where training samples and labels are fed to the graph.
        # These placeholder nodes will be fed a batch of training data at each
        # training step, which we'll write once we define the graph structure.
        self.input_layer = tf.placeholder(
            tf.float32,
            shape=(batch_size, self.board_size, self.board_size, num_channels),
            name='input_layer')

        # Create initial convolutional layer + ReLU activation function
        conv1_weights = tf.layers.conv2d(
                inputs=self.input_layer,
                filters=num_filters,
                kernel_size=[5, 5],
                padding="same",
                kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-7),
                activation=leaky_relu)  # TODO - switch to leaky ReLU

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
                    kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-7),
                    activation=leaky_relu)  # TODO - switch to leaky ReLU
            self.layers.append(conv_weights)

        # NOTE: Example Hex paper seems to change the radius and padding
        # on the penultimate layer. Might be worth investigating?
        # https://github.com/kenjyoung/Hexplorer/blob/master/Q_learner/Q_learner.py#L134-L143
        self.layers.append(tf.layers.conv2d(
                inputs=self.layers[-1],
                filters=num_filters,
                kernel_size=[3, 3],
                kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-7),
                padding="same",
                activation=leaky_relu))

        # Add final layer consisting of a sigmoid function
        final_layer = tf.layers.conv2d(
                inputs=self.layers[-1],
                filters=1,
                kernel_size=[1, 1],
                kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-7),
                padding="same",
                activation=tf.tanh)
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

        1. Reshapes from (2, 11, 11) --> (11, 11, 3)
        2. Sets all cells in the third channel to the player about to
           play (env.to_play)

        @params: env - HexEnv instance
        @return: np.array of shape (11, 11, 3)
        """
        # Create output array shape
        out = np.zeros((env.board_size, env.board_size, 3), dtype='bool')
        # Fill the final channel with the "to_play" value
        out[:,:,2].fill(env.to_play)
        # Copy the board states into the first two channels
        out[:, :, 0:2] = np.transpose(env.game_state(), [1,2,0])
        return out

    def run_estimator(self, state):
        """Computes the value distribution of over all possible actions given a
        board game state. Computes using the convolutional neural network
        defined in create_layers()

        @params: sess - tensorflow session instance
                 state - batch of states, of shape
                         (batch_size, board_size, board_size, 3)
        @return: floats of shape (batch_size, board_size, board_size, 1)
        """
        feed_dict = {
            'input_layer:0': state,
        }
        output = self.sess.run([self.final_layer], feed_dict=feed_dict)
        return output

    def exploration_policy(self, env, epsilon=0.1):
        """Search for the next action given the current board state using an
        episilon greedy policy.

        The majority of the time, we will perform the greedy policy and select
        the action with the highest value. For episilon percent of the
        time, we will explore and randomly select an unplayed action.

        @return: Tuple of next_action and value distribution over all
            possible actions, where the value distribution is
            np.array of shape (1, 11, 11, 1)
        """
        # Getting the board state
        state = Learner.convert_state(env)

        # Turn this one state into a batch by adding a dimension
        state_batch = np.expand_dims(state, 0)

        estimated_value = self.run_estimator(state_batch)[0]

        # Get the "played" mask, which represents all cells that are occupied.
        # This ignores the pie rule
        played = np.logical_or(state[:, :, HexEnv.VERTICAL],
                               state[:, :, HexEnv.HORIZONTAL]).flatten()

        # Randomly select for epsilon amount
        if np.random.rand() < epsilon:
            action = np.random.choice(np.where(played == 0)[0])
            return action, estimated_value

        # Reshaping the value distribution into a 1-D array for rargmax
        values = np.copy(estimated_value[0, :, :, :]).flatten()

        # Ensure that the agent never selects played values
        values[played] = -2

        # Select the action that has the highest estimated value
        action = Learner.rargmax(values)
        return action, estimated_value

    def optimization_policy(self, env):
        # Getting the board state
        state = Learner.convert_state(env)

        # Turn this one state into a batch by adding a dimension
        state_batch = np.expand_dims(state, 0)

        estimated_value = self.run_estimator(state_batch)[0]

        # Get the "played" mask, which represents all cells that are occupied.
        # This ignores the pie rule
        played = np.logical_or(state[:, :, HexEnv.VERTICAL],
                               state[:, :, HexEnv.HORIZONTAL]).flatten()

        # Reshaping the value distribution into a 1-D array for rargmax
        values = np.copy(estimated_value[0, :, :, :]).flatten()

        # Ensure that the agent never selects played values
        values[played] = -2

        # Select the action that has the highest estimated value
        action = Learner.rargmax(values)
        return action, estimated_value

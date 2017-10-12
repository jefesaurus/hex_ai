from google.protobuf.text_format import MessageToString, Merge

def WriteBinaryProto(message, filename):
	bytes = message.SerializeToString()
	with open(filename, 'wb') as f:
		f.write(bytes)

def ReadBinaryProto(message_class, filename):
	# Copy the message class so we can parse into it.
	message = type(message_class)()
	with open(filename, 'rb') as f:
		message.ParseFromString(f.read())
	return message

def WriteTextProto(message, filename):
	with open(filename, 'w') as f:
		f.write(MessageToString(message))

def ReadTextProto(message_class, filename):
	# Copy the message class so we can parse into it.
	message = type(message_class)()
	with open(filename, 'r') as f:
		Merge(f.read(), message)
	return message

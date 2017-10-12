def WriteBinaryProto(message, filename):
	bytes = message.SerializeToString()
	with open(filename, 'wb') as f:
		f.write(bytes)

def ReadBinaryProto(message_class, filename):
	# Copy the message class so we can parse into it.
	message = message_class()
	with open(filename, 'rb') as f:
		message.ParseFromString(f.read())
	return message

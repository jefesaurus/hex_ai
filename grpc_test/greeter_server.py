from __future__ import print_function

import grpc
import time

from concurrent import futures
from grpc_test import helloworld_pb2
from grpc_test import helloworld_pb2_grpc

_ONE_DAY_IN_SECONDS = 60 * 60 * 24


class _GreeterServer(object):

    def __init__(self, greeter_service, server_port):
        self.server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
        helloworld_pb2_grpc.add_GreeterServicer_to_server(greeter_service, self.server)
        self.server.add_insecure_port('[::]:{server_port}'.format(server_port=server_port))

    def start(self):
        self.server.start()

    def stop(self):
        self.server.stop(0)

    def await_termination(self):
        """
        server.start() doesn't block so we explicitly block here unless someone keyboard-exits us.
        :return:
        """
        try:
            while True:
                time.sleep(_ONE_DAY_IN_SECONDS)
        except KeyboardInterrupt:
            self.server.stop(0)
        pass


class _GreeterService(helloworld_pb2_grpc.GreeterServicer):

    def SayHello(self, hello_request, context):
        print("Greeter server received: " + hello_request.name)
        hello_reply = helloworld_pb2.HelloReply()
        hello_reply.message = 'Hello {name}'.format(name=hello_request.name)
        return hello_reply


def main():
    greeter_server = _GreeterServer(_GreeterService(), 50051)
    greeter_server.start()
    greeter_server.await_termination()

if __name__ == '__main__':
    main()

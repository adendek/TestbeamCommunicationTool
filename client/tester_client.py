#!/usr/bin/python
from __future__ import print_function

import sys
from socket import *
import struct
from Messages_pb2 import TestbeamMessage, InitEventReq


def make_socket(port=4050):
    """ Create a socket on localhost and return it.
    """
    sockobj = socket(AF_INET, SOCK_STREAM)
    sockobj.connect(('localhost', port))
    return sockobj


def send_message(sock, message):
    """ Send a serialized message (protobuf Message interface)
        to a socket, prepended by its length packed in 4 bytes.
    """
    s = message.SerializeToString()
    packed_len = struct.pack('>L', len(s))
    packed_message = packed_len + s
    sock.send(packed_message)


def socket_read_n(sock, n):
    """ Read exactly n bytes from the socket.
        Raise RuntimeError if the connection closed before n bytes were read.
    """
    buf = ''
    while n > 0:
        data = sock.recv(n)
        if data == '':
            raise RuntimeError('unexpected connection close')
        buf += data
        n -= len(data)
    return buf
    

def get_response(sock):
    """ Read a serialized response message from a socket.
    """
    msg = TestbeamMessage()
    len_buf = socket_read_n(sock, 4)
    msg_len = struct.unpack('>L', len_buf)[0]
    msg_buf = socket_read_n(sock, msg_len)
    msg.ParseFromString(msg_buf)
    return msg




def send_INIT_MAMBA_REQ(sock):
    rq = TestbeamMessage()
    rq.msgType = TestbeamMessage.INIT_MAMBA_REQ
    rq.seqNo = 1213
    send_message(sock, rq)
    # return get_response(sock)

def send_INIT_MAMBA_REQ(sock):
    rq = TestbeamMessage()
    rq.msgType = TestbeamMessage.START_EVENT_MAMBA_REQ
    rq.seqNo = 1214
    #rq.initEventReq = InitEventReq()
    rq.initEventReq.runType= InitEventReq.BIAS
    rq.initEventReq.dutRunNumber=100
    rq.initEventReq.biasVoltage= -222 
    rq.initEventReq.angle= -10
    rq.initEventReq.stageX= 30
    rq.initEventReq.stageY= 20
    rq.initEventReq.sector= 2
    rq.initEventReq.temperature= -6
    rq.initEventReq.sensorName= "B1"
    rq.initEventReq.operatorName= "ADendek"
    send_message(sock, rq)
    # return get_response(sock)




if __name__ == '__main__':
    port = 4050
    if len(sys.argv) >= 2:
        port = int(sys.argv[1])

    sockobj = make_socket(port)
    send_INIT_MAMBA_REQ(sockobj)
    send_INIT_MAMBA_REQ(sockobj)


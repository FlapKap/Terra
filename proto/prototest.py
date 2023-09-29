from EndDeviceProtocol import *
from EndDeviceProtocol import ExpressionInstructions as Einstr
import base64
import shlex
# This file is used to generate the binary protobuf messages that are used for testing
# the protocol implementation in ../src/protocol.py

def byte_string_as_c_char_array(bs: bytes) -> str:
    output = "["
    for b in bs:
        output += str(hex(b))
        output +=","
    output += "]"
    return output

def print_protobuf_message(message: Message, name: str) -> None:
    print(f"Message:{name}")
    print(f"Serialized String:{message.SerializeToString()}")
    print(f"Serialized As C Array:{byte_string_as_c_char_array(message.SerializeToString())}")
    print(f"Serialized as dict:{message.to_dict()}")
    print(f"Serialized as base64:{base64.b64encode(message.SerializeToString())}")
if __name__ == "__main__":
    # generate messages
    messages = {}

    messages['empty_msg'] = Message(queries=[])

    messages['map_msg'] = Message(queries=[
        Query(operations=[Operation(MapOperation(
            [
                Data(Einstr.CONST),
                Data(_uint8=8),
                Data(Einstr.MUL)
            ],
            attribute=1))])
    ])

    messages['filter_msg'] = Message(queries=[Query([Operation(filter=FilterOperation(
        predicate=[Data(Einstr.CONST),
                              Data(_uint8=8),
                              Data(Einstr.LT)]
                             ))])])

    messages['map_filter_msg'] = Message(queries=[Query([
        Operation(map=MapOperation(
            [Data(Einstr.CONST),
                        Data(_uint8=8), Data(Einstr.MUL)])),
        Operation(filter=FilterOperation(predicate=[Data(Einstr.CONST), Data(_int8=50), Data(Einstr.GT)]))])])

    messages['window_msg'] = Message(queries=[Query([
        Operation(window=WindowOperation(3, WindowSizeType.COUNTBASED, WindowAggregationType.COUNT, 0, 1, 2, 3))])])

    messages['multiple_msg'] = Message(
        queries=[
            Query(
                operations=[Operation(
                    map=MapOperation(
                        function=[Data(Einstr.CONST),
                                             Data(_uint8=8)], attribute=0))
                            ]
            ),
            Query([
                Operation(window=WindowOperation(
                    3, WindowSizeType.COUNTBASED, WindowAggregationType.COUNT, 0, 1, 2, 3)),
                Operation(filter=FilterOperation(
                    predicate=[Data(Einstr.CONST), Data(_int8=50), Data(Einstr.LT)]))])
        ]
    )

    messages['output_single_msg'] = Output([OutputQueryResponse(
        1, [Data(_uint8=ord('H')), Data(_uint8=ord('E')), Data(_uint8=ord('L')), Data(_uint8=ord('L')), Data(_uint8=ord('O'))])])

    messages['output_multiple_msg'] = Output([
        OutputQueryResponse(1, [Data(_uint8=ord('H'))]),
        OutputQueryResponse(2, [Data(_uint8=ord('E'))]),
        OutputQueryResponse(3, [Data(_uint8=ord('L'))]),
        OutputQueryResponse(4, [Data(_uint8=ord('L'))]),
        OutputQueryResponse(5, [Data(_uint8=ord('O'))]),
    ])

    for name, message in messages.items():
        print_protobuf_message(message, name)
        print()

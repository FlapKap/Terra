from TerraProtocol import *
from TerraProtocol import ExpressionInstructions as Einstr
import base64
import shlex

# This file is used to generate the binary protobuf messages that are used for testing
# the protocol implementation in ../src/protocol.py


def byte_string_as_c_char_array(bs: bytes) -> str:
    output = "["
    for b in bs:
        output += str(hex(b))
        output += ","
    output += "]"
    return output


def print_protobuf_message(message: Message, name: str) -> None:
    print(f"Message:{name} with length:{len(message.SerializeToString())}")
    print(f"Serialized String:{message.SerializeToString()}")
    print(
        f"Serialized As C Array:{byte_string_as_c_char_array(message.SerializeToString())}"
    )

    print(
        f"Serialized as 2 digit hex: {' '.join([f'{int(b):02x}' for b in message.SerializeToString()])}"
    )
    print(f"Serialized as dict:{message.to_dict()}")
    print(f"Serialized as base64:{base64.b64encode(message.SerializeToString())}")


if __name__ == "__main__":
    # generate messages
    messages = {}

    messages["empty_msg"] = Message(queries=[])
    messages["map_short_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        MapOperation(
                            Expression([Data(instruction=Einstr.VAR), Data(_uint8=0)]),
                            attribute=0,
                        )
                    )
                ]
            )
        ]
    )
    messages["map_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        MapOperation(
                            Expression(
                                [
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                ]
                            ),
                            attribute=1,
                        )
                    )
                ]
            )
        ]
    )

    messages["medium_map_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        MapOperation(
                            Expression(
                                [
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=0),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=1),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.ABS),
                                ]
                            ),
                            attribute=2,
                        )
                    )
                ]
            )
        ]
    )
    messages["semi_long_map_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        MapOperation(
                            Expression(
                                [
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=0),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=1),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ABS),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=50),
                                    Data(instruction=Einstr.SUB),
                                    Data(instruction=Einstr.FLOOR),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.MOD),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.EXP),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CEIL),
                                ]
                            ),
                            attribute=2,
                        )
                    )
                ]
            )
        ]
    )
    messages["long_map_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        MapOperation(
                            Expression(
                                [
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=0),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=1),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ABS),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=50),
                                    Data(instruction=Einstr.SUB),
                                    Data(instruction=Einstr.FLOOR),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.MOD),
                                    Data(instruction=Einstr.CEIL),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.EXP),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=1),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.SUB),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.ADD),
                                ]
                            ),
                            attribute=2,
                        )
                    ),
                ]
            )
        ]
    )

    messages["long_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        MapOperation(
                            Expression(
                                [
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=0),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                    Data(instruction=Einstr.VAR),
                                    Data(_uint8=1),
                                    Data(instruction=Einstr.DIV),
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=2),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ABS),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=50),
                                    Data(instruction=Einstr.SUB),
                                    Data(instruction=Einstr.FLOOR),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.MOD),
                                    Data(instruction=Einstr.CEIL),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.EXP),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=2),
                                    Data(instruction=Einstr.DIV),
                                ]
                            ),
                            attribute=2,
                        )
                    ),
                    Operation(
                        filter=FilterOperation(
                            predicate=Expression(
                                [
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.ADD),
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=1),
                                    Data(instruction=Einstr.LT),
                                ]
                            )
                        )
                    ),
                ]
            )
        ]
    )

    messages["filter_msg"] = Message(
        queries=[
            Query(
                [
                    Operation(
                        filter=FilterOperation(
                            predicate=Expression(
                                [
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.LT),
                                ]
                            )
                        )
                    )
                ]
            )
        ]
    )

    messages["map_filter_msg"] = Message(
        queries=[
            Query(
                [
                    Operation(
                        map=MapOperation(
                            Expression(
                                [
                                    Data(instruction=Einstr.CONST),
                                    Data(_uint8=8),
                                    Data(instruction=Einstr.MUL),
                                ]
                            )
                        )
                    ),
                    Operation(
                        filter=FilterOperation(
                            predicate=Expression(
                                [
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=50),
                                    Data(instruction=Einstr.GT),
                                ]
                            )
                        )
                    ),
                ]
            )
        ]
    )

    messages["window_msg"] = Message(
        queries=[
            Query(
                [
                    Operation(
                        window=WindowOperation(
                            aggregation=Aggregation(
                                aggregation_type=WindowAggregationType.COUNT,
                                on_attribute=0,
                                as_attribute=1,
                            ),
                            start_attribute=2,
                            end_attribute=3,
                            tumbling=TumblingWindowOperation(size_ms=60000),
                        )
                    )
                ]
            )
        ]
    )

    messages["window_20_min_msg"] = Message(
        queries=[
            Query(
                [
                    Operation(
                        window=WindowOperation(
                            aggregation=Aggregation(
                                aggregation_type=WindowAggregationType.COUNT,
                                on_attribute=0,
                                as_attribute=1,
                            ),
                            start_attribute=2,
                            end_attribute=3,
                            tumbling=TumblingWindowOperation(size_ms=1200000),
                        )
                    )
                ]
            )
        ]
)

    messages["window_10_min_msg"] = Message(
        queries=[
            Query(
                [
                    Operation(
                        window=WindowOperation(
                            aggregation=Aggregation(
                                aggregation_type=WindowAggregationType.COUNT,
                                on_attribute=0,
                                as_attribute=1,
                            ),
                            start_attribute=2,
                            end_attribute=3,
                            tumbling=TumblingWindowOperation(size_ms=600000),
                        )
                    )
                ]
            )
        ]
    )
    messages["window_2_min_msg"] = Message(
        queries=[
            Query(
                [
                    Operation(
                        window=WindowOperation(
                            aggregation=Aggregation(
                                aggregation_type=WindowAggregationType.COUNT,
                                on_attribute=0,
                                as_attribute=1,
                            ),
                            start_attribute=2,
                            end_attribute=3,
                            tumbling=TumblingWindowOperation(size_ms=120000),
                        )
                    )
                ]
            )
        ]
    )

    messages["multiple_msg"] = Message(
        queries=[
            Query(
                operations=[
                    Operation(
                        map=MapOperation(
                            function=Expression(
                                [Data(instruction=Einstr.CONST), Data(_uint8=8)]
                            ),
                            attribute=0,
                        )
                    )
                ]
            ),
            Query(
                [
                    Operation(
                        map=MapOperation(
                            function=Expression(
                                [Data(instruction=Einstr.CONST), Data(_uint8=8)]
                            ),
                            attribute=0,
                        )
                    ),
                    Operation(
                        filter=FilterOperation(
                            predicate=Expression(
                                [
                                    Data(instruction=Einstr.CONST),
                                    Data(_int8=50),
                                    Data(instruction=Einstr.LT),
                                ]
                            )
                        )
                    ),
                ]
            ),
        ]
    )

    messages["output_single_msg"] = Output(
        [
            OutputQueryResponse(
                1,
                [
                    Data(_uint8=ord("H")),
                    Data(_uint8=ord("E")),
                    Data(_uint8=ord("L")),
                    Data(_uint8=ord("L")),
                    Data(_uint8=ord("O")),
                ],
            )
        ]
    )

    messages["output_multiple_msg"] = Output(
        [
            OutputQueryResponse(1, [Data(_uint8=ord("H"))]),
            OutputQueryResponse(2, [Data(_uint8=ord("E"))]),
            OutputQueryResponse(3, [Data(_uint8=ord("L"))]),
            OutputQueryResponse(4, [Data(_uint8=ord("L"))]),
            OutputQueryResponse(5, [Data(_uint8=ord("O"))]),
        ]
    )

    for name, message in messages.items():
        print_protobuf_message(message, name)
        print()

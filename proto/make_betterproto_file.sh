protoc -I ../app/terra/ -I ../RIOT/build/pkg/nanopb/generator/proto/ --python_betterproto_out=. ../app/terra/terraprotocol.proto
rm __init__.py
rm nanopb.py
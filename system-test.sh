#!/usr/bin/env bash
set -xe
./build/mqtt pub -t hello/topic -v -a broker.emqx.io:1883 -c client '{"hello": "world"}'
./build/mqtt sub -t hello/topic -v -a broker.emqx.io:1883 -c client

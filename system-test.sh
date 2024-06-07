#!/usr/bin/env bash
set -e

# given a subscription
./build/mqtt sub -v -t hello/topic -a broker.emqx.io:1883 -c mqttclient1 &> system-test-output &
PID=$!
sleep 1

# publish a message
./build/mqtt pub -v -t hello/topic -a broker.emqx.io:1883 -c mqttclient2 '{"hello": "world"}'
sleep 1

# then verify the subscription received the message
if grep -q '{"hello": "world"}' system-test-output; then
    echo "Message received successfully"
    kill -9 $PID &> /dev/null # TODO SIGINT, after making recv play nicer with interrupts
    exit 0
else
    echo "Message not received"
    kill -9 $PID
    exit 1
fi

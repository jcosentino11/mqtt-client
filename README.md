# MQTT Client

Bare-bones MQTT CLI for Mac and Linux. 

This project is a personal learning exercise, for diving into MQTT internals and network programming. You can read more about my experience [on my website](https://josephcosentino.me/posts/mqtt-client/).

## Publish to a topic
```
mqtt pub -t hello/topic -a localhost:1883 -c clientId '{"hello": "world"}'
```

```
Usage: mqtt pub [-v] -t <topic> -a <address> -c <clientId> <message>
Options:
  -t, --topic <topic>           Specify the topic to publish to (required)
  -a, --address <address>       <host>[:<port>] address to connect to (required)
  -c, --client-id <client id>   Client identifier
  -v, --verbose                 Enable verbose output
  -h, --help                    Show this help message
```

## Subscribe to a topic

```
mqtt sub -t hello/topic -a localhost:1883 -c clientId
```

```
Usage: mqtt sub [-v] -t <topic> -a <address> -c <clientId>
Options:
  -t, --topic <topic>           Specify the topic to publish to (required)
  -a, --address <address>       <host>[:<port>] address to connect to (required)
  -c, --client-id <client id>   Client identifier
  -v, --verbose                 Enable verbose output
  -h, --help                    Show this help message
```

## Features

|                         | Supported | Planned |
|-------------------------|-----------|---------|
| MQTT3                   | x         |         |
| MQTT5                   |           | x       |
| QOS0                    | x         |         |
| QOS1                    | x         |         |
| QOS2                    |           | x       |
| PUBLISH                 | x         |         |
| SUBSCRIBE               | x         |         |
| Various CONNECT options |           | x       |
| SSL                     |           | x       |

## Related Projects

Here are some real-world MQTT clients to check out:

* [MQTTX](https://mqttx.app/cli)
* [mosquitto_pub/sub](https://mosquitto.org/man/)

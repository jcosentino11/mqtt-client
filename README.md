# MQTT Client

Bare-bones MQTT CLI for Mac and Linux.

## Publish to a topic
```
mqtt pub -t hello/topic -a localhost:1883 -c clientId '{"hello": "world"}'
```

## Subscribe to a topic
```
mqtt sub -t hello/topic -a localhost:1883 -c clientId 
```

## Related Projects

Here are some real-world MQTT clients to check out:

* [MQTTX](https://mqttx.app/cli)

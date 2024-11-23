#include <Arduino.h>
#include <SimpleCLI.h>

SimpleCLI cli;

// Список команд
Command cmdPing;
Command cmdPong;

void errorCallback(cmd_error *errorPtr)
{
    CommandError e(errorPtr);

    Serial.println("ERROR: " + e.toString());

    if (e.hasCommand())
    {
        Serial2.println("Did you mean? " + e.getCommand().toString());
    }
    else
    {
        Serial2.println(cli.toString());
    }
}

void pongCallback(cmd *cmdPtr)
{
    Command cmd(cmdPtr);

    int argNum = cmd.countArgs();

    for (int i = 0; i < argNum; i++)
    {
        Serial2.println(cmd.getArgument(i).getValue());
    }
}

void pingCallback(cmd *cmdPtr)
{
    Command cmd(cmdPtr);

    Argument argN = cmd.getArgument("num");
    String argVal = argN.getValue();
    int n = argVal.toInt();

    Argument argStr = cmd.getArgument("str");
    String strVal = argStr.getValue();

    Argument argC = cmd.getArgument("c");
    bool c = argC.isSet();

    if (c)
        strVal.toUpperCase();

    for (int i = 0; i < n; i++)
    {
        Serial2.println(strVal);
    }
}

void cliInit(void)
{

    cmdPing = cli.addCommand("ping", pingCallback);
    
    cmdPing.addPositionalArgument("str", "pong");
    cmdPing.addArgument("n/um/ber,anzahl", "1");
    cmdPing.addFlagArgument("c");

    cmdPong = cli.addBoundlessCommand("pong,hello", pongCallback);

    cli.setOnError(errorCallback);
}

#include <Arduino.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"

extern void update();

SimpleCLI cli;

// Список команд
Command cmdReset;  // Перезагрузка reset
Command cmdEnable; // Включение отключение драйвера enable [on/off]

void resetCallback(cmd *c)
{   
    Command cmd(c); 
    Serial2.println("Перезагрузка");
    esp_restart();
}

void enableCallback(cmd *c)
{
    Command cmd(c);
    Argument arg = cmd.getArgument(0);
    String argVal = arg.getValue();
    if(argVal == "on"){
        tmcDriverEnable = 1;
        Serial2.println("Включить мотор");
        update();
    }
    else
    {
       tmcDriverEnable = 0; 
       Serial2.println("Выключить мотор");
       update();
    }
}


// if (value == "motor_on")
// {
//     Serial2.println("motor_on");
//     tmcDriverEnable = 1;
// }

// if (value == "motor_off")
// {
//     Serial2.println("motor_off");
//     tmcDriverEnable = 0;
// }

void errorCallback(cmd_error *errorPtr)
{
    CommandError e(errorPtr);
    Serial2.println("ERROR: " + e.toString());
    if (e.hasCommand())
    {
        Serial2.println("Did you mean? " + e.getCommand().toString());
    }
    else
    {
        Serial2.println(cli.toString());
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

    //Сброс
    cmdReset = cli.addCommand("reset", resetCallback);
    cmdEnable = cli.addSingleArgCmd("enable", enableCallback);

    // cmdPing.addPositionalArgument("str", "pong");
    // cmdPing.addArgument("n/um/ber,anzahl", "1");
    // cmdPing.addFlagArgument("c");

    // cmdPong = cli.addBoundlessCommand("pong,hello", pongCallback);

    cli.setOnError(errorCallback);
}

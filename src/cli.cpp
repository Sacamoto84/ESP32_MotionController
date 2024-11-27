#include <Arduino.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"
#include "global.h"

extern void update();

SimpleCLI cli;

// Список команд
Command cmdReset;  // Перезагрузка reset
Command cmdEnable; // Включение отключение драйвера >>enable [on/off]
Command cmdReadGCONF;

void printBits(uint32_t value)
{
    for (int i = 31; i >= 0; --i)
    { // 31-й бит - старший
        Serial2.printf("%c", (value & (1U << i)) ? '1' : '0');
    }
    Serial2.printf("\n");
}

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
    if (argVal == "on")
    {
        tmcDriverEnable.set(1);
        Serial2.println("Включить мотор");
        update();
    }
    else
    {
        tmcDriverEnable.set(0);
        Serial2.println("Выключить мотор");
        update();
    }
}

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

void readGSONCallback(cmd *c)
{
    Command cmd(c);
    Serial2.print("Прочесть GCONF:");
    // uint32_t a = driver.GCONF();
    printBits(driver.GCONF());

    Serial2.print("Прочесть MSCNT:");
    printBits(driver.MSCNT());

      Serial2.print("Прочесть CHOPCONF:");
    printBits(driver.CHOPCONF());

    Serial2.print("Прочесть DRV_STATUS:");
    printBits(driver.DRV_STATUS());
    
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

void cliInit()
{

    // Сброс
    cmdReset = cli.addCommand("reset", resetCallback);
    cmdEnable = cli.addSingleArgCmd("enable", enableCallback);
    cmdReadGCONF = cli.addCommand("readGCONF", readGSONCallback);

    // cmdPing.addPositionalArgument("str", "pong");
    // cmdPing.addArgument("n/um/ber,anzahl", "1");
    // cmdPing.addFlagArgument("c");

    // cmdPong = cli.addBoundlessCommand("pong,hello", pongCallback);

    cli.setOnError(errorCallback);
}
